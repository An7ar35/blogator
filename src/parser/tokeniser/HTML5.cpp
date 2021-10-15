#include "HTML5.h"

#include <functional>
#include <utility>
#include <algorithm>

#include "../../logger/Logger.h"
#include "../logging/ParserLog.h"

using namespace blogator::parser;
using           blogator::parser::logging::ParserLog;
using           blogator::parser::specs::html5::ErrorCode;
using           blogator::parser::specs::html5::TokeniserState;

//==========================================[ PUBLIC ]==============================================

/**
 * Constructor
 * @param tree_builder TreeBuilder to use during tokenization
 */
tokeniser::HTML5::HTML5( dom::TreeBuilder &tree_builder ) :
    _eof( false ),
    _src_path( "" ),
    _error_count( 0 ),
    _tree_builder( tree_builder ),
    _current_state( State_e::DATA ),
    _return_state( State_e::DATA )
{
    tree_builder.setChangeHTML5TokeniserStateCallback( [this]( State_e s ) { setState( s ); } );
}

/**
 * Constructor (for unit tests)
 * @param tree_builder TreeBuilder to use during tokenization
 * @param init_state Initial state
 * @param last_start_tag Last start tag name (optional)
 */
tokeniser::HTML5::HTML5( dom::TreeBuilder & tree_builder, specs::html5::TokeniserState init_state, std::u32string last_start_tag ) :
    _eof( false ),
    _src_path( "" ),
    _error_count( 0 ),
    _tree_builder( tree_builder ),
    _current_state( init_state ),
    _return_state( State_e::DATA ),
    _last_start_tag( std::move( last_start_tag ) )
{}

/**
 * Parse HTML5 content into tokens
 * @param text UTF32 text
 * @param starting_ctx Context from which the parsing originally began (default=HTML5)
 * @return Ending/break Context
 * @throws parsing_failure if parsing fails
 */
specs::Context tokeniser::HTML5::parse( U32Text &text, specs::Context starting_ctx ) { //TODO
//    if( starting_ctx != specs::Context::HTML5 ) { //that would be good in the tree builder, not here really
//        return parseSnippet( text, tokens, starting_ctx );
//    }

    //break condition HTML => "{{" when not in verbatim mode or other places where BLOGATOR::* wouldn't work

    _src_path = text.path(); //internal caching for error calls
    _eof      = false;

    bool reconsume_flag = true;
    bool next_line_flag = false;

    /**
     * [LAMBDA] Sets the state to reconsume the current code point under the caret
     */
    const auto reconsume = [&]( State_e new_state ) {
        reconsume_flag = true;
        setState( new_state );
    };

    /**
     * [LAMBDA] Moves the caret and gets the code point under it
     */
    const auto getNextChar = [&]( U32Text &text ) {
        if( reconsume_flag ) {
            reconsume_flag = false;
        } else {
            text.advanceCol();

            if( next_line_flag ) {
                text.advanceLineTracker();
                next_line_flag = false;
            }

            next_line_flag = unicode::ascii::isnewline( text.character() );
        }

        return text.character();
    };

    // ============================== [ Tokenizer state machine ] ==================================

    auto character = getNextChar( text );

    next_line_flag = unicode::ascii::isnewline( text.character() ); //edge case: when the first character is a newline.

    while( !_eof ) {
        switch( currentState() ) {
            case State_e::DATA: {
                if( text.reachedEnd() ) {
                    emitEndOfFileToken( text.position() );
                } else if( character == AMPERSAND ) {
                    setReturnState( State_e::DATA );
                    setState( State_e::CHARACTER_REFERENCE );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::TAG_OPEN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    emitCharacterToken( text.position(), character );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::RCDATA: {
                if( text.reachedEnd() ) {
                    emitEndOfFileToken( text.position() );
                } else if( character == AMPERSAND ) {
                    setReturnState( State_e::RCDATA );
                    setState( State_e::CHARACTER_REFERENCE );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::RCDATA_LESS_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::RAWTEXT: {
                if( text.reachedEnd() ) {
                    emitEndOfFileToken( text.position() );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::RAWTEXT_LESS_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::SCRIPT_DATA: {
                if( text.reachedEnd() ) {
                    emitEndOfFileToken( text.position() );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::PLAIN_TEXT: {
                if( text.reachedEnd() ) {
                    emitEndOfFileToken( text.position() );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::TAG_OPEN: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_BEFORE_TAG_NAME );
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitEndOfFileToken( text.position() );
                } else if( character == EXCLAMATION_MARK ) {
                    setState( State_e::MARKUP_DECLARATION_OPEN );
                } else if( character == SOLIDUS ) {
                    setState( State_e::END_TAG_OPEN );
                } else if( unicode::ascii::isalpha( character ) ) {
                    createStartTagToken( text.position() );
                    reconsume( State_e::TAG_NAME );
                } else if( character == QUESTION_MARK ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME );
                    createCommentToken( text.position() );
                    reconsume( State_e::BOGUS_COMMENT );
                } else {
                    logError( text.position(), ErrorCode::INVALID_FIRST_CHARACTER_OF_TAG_NAME );
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    reconsume( State_e::DATA );
                }
            } break;

            case State_e::END_TAG_OPEN: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_BEFORE_TAG_NAME );
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );
                    emitEndOfFileToken( text.position() );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::MISSING_END_TAG_NAME );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( unicode::ascii::isalpha( character ) ) {
                    createEndTagToken( text.position() );
                    reconsume( State_e::TAG_NAME );
                } else {
                    logError( text.position(), ErrorCode::INVALID_FIRST_CHARACTER_OF_TAG_NAME );
                    createCommentToken( text.position() );
                    reconsume( State_e::BOGUS_COMMENT );
                }
            } break;

            case State_e::TAG_NAME: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_TAG );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::BEFORE_ATTRIBUTE_NAME );
                } else if( character == SOLIDUS ) {
                    setState( State_e::SELF_CLOSING_START_TAG );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingTokenText( unicode::ascii::tolower( character ) );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenText( REPLACEMENT_CHAR );
                } else {
                    appendToPendingTokenText( character );
                }
            } break;

            case State_e::RCDATA_LESS_THAN_SIGN: {
                if( character == SOLIDUS ) {
                    clearTempBuffer();
                    setState( State_e::RCDATA_END_TAG_OPEN );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    reconsume( State_e::RCDATA );
                }
            } break;

            case State_e::RCDATA_END_TAG_OPEN: {
                if( unicode::ascii::isalpha( character ) ) {
                    createEndTagToken( text.position() );
                    reconsume( State_e::RCDATA_END_TAG_NAME );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );
                    reconsume( State_e::RCDATA );
                }
            } break;

            case State_e::RCDATA_END_TAG_NAME: {
                if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::BEFORE_ATTRIBUTE_NAME );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::RCDATA );
                    }

                } else if( character == SOLIDUS ) {

                    if( appropriateEndTagToken() ) {
                        setState( State_e::SELF_CLOSING_START_TAG );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::RCDATA );
                    }
                } else if( character == GREATER_THAN_SIGN ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::DATA );
                        emitPendingToken( text.position() );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::RCDATA );
                    }
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingTokenText( unicode::ascii::tolower( character ) );
                    appendToTempBuffer( character );
                } else if( unicode::ascii::islower( character ) ) {
                    appendToPendingTokenText( character );
                    appendToTempBuffer( character );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );

                    for( const auto & c : tempBuffer() ) {
                        emitCharacterToken( text.position(), c );
                    }

                    reconsume( State_e::RCDATA );
                }
            } break;

            case State_e::RAWTEXT_LESS_THAN_SIGN: {
                if( character == SOLIDUS ) {
                    clearTempBuffer();
                    setState( State_e::RAWTEXT_END_TAG_OPEN );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    reconsume( State_e::RAWTEXT );
                }
            } break;

            case State_e::RAWTEXT_END_TAG_OPEN: {
                if( unicode::ascii::isalpha( character ) ) {
                    createEndTagToken( text.position() );
                    reconsume( State_e::RAWTEXT_END_TAG_NAME );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );
                    reconsume( State_e::RAWTEXT );
                }
            } break;

            case State_e::RAWTEXT_END_TAG_NAME: {
                if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::BEFORE_ATTRIBUTE_NAME );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::RAWTEXT );
                    }
                } else if( character == SOLIDUS ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::SELF_CLOSING_START_TAG );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::RAWTEXT );
                    }
                } else if( character == GREATER_THAN_SIGN ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::DATA );
                        emitPendingToken( text.position() );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::RAWTEXT );
                    }
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingTokenText( unicode::ascii::tolower( character ) );
                    appendToTempBuffer( character );
                } else if( unicode::ascii::islower( character ) ) {
                    appendToPendingTokenText( character );
                    appendToTempBuffer( character );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );

                    for( const auto & c : tempBuffer() ) {
                        emitCharacterToken( text.position(), c );
                    }

                    reconsume( State_e::RAWTEXT );
                }
            } break;

            case State_e::SCRIPT_DATA_LESS_THAN_SIGN: {
                if( character == SOLIDUS ) {
                    clearTempBuffer();
                    setState( State_e::SCRIPT_DATA_END_TAG_OPEN );
                } else if( character == EXCLAMATION_MARK ) {
                    setState( State_e::SCRIPT_DATA_ESCAPE_START );
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), EXCLAMATION_MARK );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    reconsume( State_e::SCRIPT_DATA );
                }
            } break;

            case State_e::SCRIPT_DATA_END_TAG_OPEN: {
                if( unicode::ascii::isalpha( character ) ) {
                    createEndTagToken( text.position() );
                    reconsume( State_e::SCRIPT_DATA_END_TAG_NAME );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );
                    reconsume( State_e::SCRIPT_DATA );
                }
            } break;

            case State_e::SCRIPT_DATA_END_TAG_NAME: {
                if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::BEFORE_ATTRIBUTE_NAME );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::DATA );
                    }
                } else if( character == SOLIDUS ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::SELF_CLOSING_START_TAG );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::DATA );
                    }
                } else if( character == GREATER_THAN_SIGN ) {
                    if( appropriateEndTagToken() ) {
                        setState( State_e::DATA );
                        emitPendingToken( text.position() );
                    } else {
                        emitCharacterToken( text.position(), LESS_THAN_SIGN );
                        emitCharacterToken( text.position(), SOLIDUS );

                        for( const auto & c : tempBuffer() ) {
                            emitCharacterToken( text.position(), c );
                        }

                        reconsume( State_e::DATA );
                    }
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingTokenText( unicode::ascii::tolower( character ) );
                    appendToTempBuffer( character );
                } else if( unicode::ascii::islower( character ) ) {
                    appendToPendingTokenText( character );
                    appendToTempBuffer( character );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );

                    for( const auto & c : tempBuffer() ) {
                        emitCharacterToken( text.position(), c );
                    }

                    reconsume( State_e::DATA );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPE_START: {
                if( character == HYPHEN_MINUS ) {
                    setState( State_e::SCRIPT_DATA_ESCAPE_START_DASH );
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else {
                    reconsume( State_e::SCRIPT_DATA );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPE_START_DASH: {
                if( character == HYPHEN_MINUS ) {
                    setState( State_e::SCRIPT_DATA_ESCAPED_DASH_DASH );
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else {
                    reconsume( State_e::SCRIPT_DATA );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    setState( State_e::SCRIPT_DATA_ESCAPED_DASH );
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPED_DASH: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    setState( State_e::SCRIPT_DATA_ESCAPED_DASH_DASH );
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    setState( State_e::SCRIPT_DATA_ESCAPED );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    setState( State_e::SCRIPT_DATA_ESCAPED );
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPED_DASH_DASH: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA );
                    emitCharacterToken( text.position(), GREATER_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    setState( State_e::SCRIPT_DATA_ESCAPED );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    setState( State_e::SCRIPT_DATA_ESCAPED );
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN: {
                if( character == SOLIDUS ) {
                    clearTempBuffer();
                    setState( State_e::SCRIPT_DATA_ESCAPED_END_TAG_OPEN );
                } else if( unicode::ascii::isalpha( character ) ) {
                    clearTempBuffer();
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    reconsume( State_e::SCRIPT_DATA_DOUBLE_ESCAPE_START );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    reconsume( State_e::SCRIPT_DATA_ESCAPED );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPED_END_TAG_OPEN: {
                if( unicode::ascii::isalpha( character ) ) {
                    createEndTagToken( text.position() );
                    reconsume( State_e::SCRIPT_DATA_ESCAPED_END_TAG_NAME );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );
                    reconsume( State_e::SCRIPT_DATA_ESCAPED );
                }
            } break;

            case State_e::SCRIPT_DATA_ESCAPED_END_TAG_NAME: {
                const bool appropriate = appropriateEndTagToken();

                if( ( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) && appropriate ) {
                    setState( State_e::BEFORE_ATTRIBUTE_NAME );
                } else if( character == SOLIDUS && appropriate ) {
                    setState( State_e::SELF_CLOSING_START_TAG );
                } else if( character == GREATER_THAN_SIGN && appropriate ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingTokenText( unicode::ascii::tolower( character ) );
                    appendToTempBuffer( character );
                } else if( unicode::ascii::islower( character ) ) {
                    appendToPendingTokenText( character );
                    appendToTempBuffer( character );
                } else {
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), SOLIDUS );

                    for( const auto & c : tempBuffer() ) {
                        emitCharacterToken( text.position(), c );
                    }

                    reconsume( State_e::SCRIPT_DATA_ESCAPED );
                }
            } break;

            case State_e::SCRIPT_DATA_DOUBLE_ESCAPE_START: {
                if( unicode::ascii::istab( character )   ||
                    unicode::ascii::isfeed( character )  ||
                    unicode::ascii::isspace( character ) ||
                    character == SOLIDUS                 ||
                    character == GREATER_THAN_SIGN )
                {
                    if( isEqualToTempBuffer( U"script" ) ) {
                        setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                    } else {
                        setState( State_e::SCRIPT_DATA_ESCAPED );
                    }
                    emitCharacterToken( text.position(), character );
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToTempBuffer( unicode::ascii::tolower( character ) );
                    emitCharacterToken( text.position(), character );
                } else if( unicode::ascii::islower( character ) ) {
                    appendToTempBuffer( character );
                    emitCharacterToken( text.position(), character );
                } else {
                    reconsume( State_e::SCRIPT_DATA_ESCAPED );
                }
            } break;

            case State_e::SCRIPT_DATA_DOUBLE_ESCAPED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED_DASH );
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::SCRIPT_DATA_DOUBLE_ESCAPED_DASH: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH );
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    emitCharacterToken( text.position(), HYPHEN_MINUS );
                } else if( character == LESS_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN );
                    emitCharacterToken( text.position(), LESS_THAN_SIGN );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::SCRIPT_DATA );
                    emitCharacterToken( text.position(), GREATER_THAN_SIGN );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                    emitCharacterToken( text.position(), REPLACEMENT_CHAR );
                } else {
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN: {
                if( character == SOLIDUS ) {
                    clearTempBuffer();
                    setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPE_END );
                    emitCharacterToken( text.position(), SOLIDUS );
                } else {
                    reconsume( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                }
            } break;

            case State_e::SCRIPT_DATA_DOUBLE_ESCAPE_END: {
                if( unicode::ascii::istab( character )   ||
                    unicode::ascii::isfeed( character )  ||
                    unicode::ascii::isspace( character ) ||
                    character == SOLIDUS                 ||
                    character == GREATER_THAN_SIGN )
                {
                    if( isEqualToTempBuffer( U"script" ) ) {
                        setState( State_e::SCRIPT_DATA_ESCAPED );
                    } else {
                        setState( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                    }
                    emitCharacterToken( text.position(), character );
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToTempBuffer( unicode::ascii::tolower( character ) );
                    emitCharacterToken( text.position(), character );
                } else if( unicode::ascii::islower( character ) ) {
                    appendToTempBuffer( character );
                    emitCharacterToken( text.position(), character );
                } else {
                    reconsume( State_e::SCRIPT_DATA_DOUBLE_ESCAPED );
                }
            } break;

            case State_e::BEFORE_ATTRIBUTE_NAME: {
                if( text.reachedEnd() || character == SOLIDUS || character == GREATER_THAN_SIGN ) {
                    reconsume( State_e::AFTER_ATTRIBUTE_NAME );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == EQUALS_SIGN ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME );
                    clearPendingTokenAttribute();
                    appendToPendingTokenAttrNameBuffer( character );
                    setState( State_e::ATTRIBUTE_NAME );
                } else {
                    if( !addPendingTokenAttribute() ) {
                        logError( text.position(), ErrorCode::DUPLICATE_ATTRIBUTE );
                    }
                    reconsume( State_e::ATTRIBUTE_NAME );
                }
            } break;

            case State_e::ATTRIBUTE_NAME: {
                if( text.reachedEnd()                    ||
                    unicode::ascii::istab( character )   ||
                    unicode::ascii::isfeed( character )  ||
                    unicode::ascii::isspace( character ) ||
                    character == SOLIDUS                 ||
                    character == GREATER_THAN_SIGN )
                {
                    reconsume( State_e::AFTER_ATTRIBUTE_NAME );
                } else if( character == EQUALS_SIGN ) {
                    setState( State_e::BEFORE_ATTRIBUTE_VALUE );
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingTokenAttrNameBuffer( unicode::ascii::tolower( character ) );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenAttrNameBuffer( REPLACEMENT_CHAR );
                } else if( character == QUOTATION_MARK || character == APOSTROPHE || character == LESS_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME );
                    appendToPendingTokenAttrNameBuffer( character );
                } else {
                    appendToPendingTokenAttrNameBuffer( character );
                }
            } break;

            case State_e::AFTER_ATTRIBUTE_NAME: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_TAG );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == SOLIDUS ) {
                    setState( State_e::SELF_CLOSING_START_TAG );
                } else if( character == EQUALS_SIGN ) {
                    setState( State_e::BEFORE_ATTRIBUTE_VALUE );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    if( !addPendingTokenAttribute() ) {
                        logError( text.position(), ErrorCode::DUPLICATE_ATTRIBUTE );
                    }
                    emitPendingToken( text.position() );
                } else {
                    if( !addPendingTokenAttribute() ) {
                        logError( text.position(), ErrorCode::DUPLICATE_ATTRIBUTE );
                    }
                    reconsume( State_e::ATTRIBUTE_NAME );
                }
            } break;

            case State_e::BEFORE_ATTRIBUTE_VALUE: {
                if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == QUOTATION_MARK ) {
                    setState( State_e::ATTRIBUTE_VALUE_DOUBLE_QUOTED );
                } else if( character == APOSTROPHE ) {
                    setState( State_e::ATTRIBUTE_VALUE_SINGLE_QUOTED );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::MISSING_ATTRIBUTE_VALUE );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    reconsume( State_e::ATTRIBUTE_VALUE_UNQUOTED );
                }
            } break;

            case State_e::ATTRIBUTE_VALUE_DOUBLE_QUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_TAG );
                    emitEndOfFileToken( text.position() );
                } else if( character == QUOTATION_MARK ) {
                    setState( State_e::AFTER_ATTRIBUTE_VALUE_QUOTED );
                    if( !addPendingTokenAttribute() ) {
                        logError( text.position(), ErrorCode::DUPLICATE_ATTRIBUTE );
                    }
                } else if( character == AMPERSAND ) {
                    setReturnState( State_e::ATTRIBUTE_VALUE_DOUBLE_QUOTED );
                    setState( State_e::CHARACTER_REFERENCE );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenAttrValueBuffer( REPLACEMENT_CHAR );
                } else {
                    appendToPendingTokenAttrValueBuffer( character );
                }
            } break;

            case State_e::ATTRIBUTE_VALUE_SINGLE_QUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_TAG );
                    emitEndOfFileToken( text.position() );
                } else if( character == APOSTROPHE ) {
                    setState( State_e::AFTER_ATTRIBUTE_VALUE_QUOTED );
                    if( !addPendingTokenAttribute() ) {
                        logError( text.position(), ErrorCode::DUPLICATE_ATTRIBUTE );
                    }
                } else if( character == AMPERSAND ) {
                    setReturnState( State_e::ATTRIBUTE_VALUE_SINGLE_QUOTED );
                    setState( State_e::CHARACTER_REFERENCE );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenAttrValueBuffer( REPLACEMENT_CHAR );
                } else {
                    appendToPendingTokenAttrValueBuffer( character );
                }
            } break;

            case State_e::ATTRIBUTE_VALUE_UNQUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_TAG );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::BEFORE_ATTRIBUTE_NAME );
                } else if( character == AMPERSAND ) {
                    setReturnState( State_e::ATTRIBUTE_VALUE_UNQUOTED );
                    setState( State_e::CHARACTER_REFERENCE );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenAttrValueBuffer( REPLACEMENT_CHAR );
                } else if( character == QUOTATION_MARK ||
                           character == APOSTROPHE     ||
                           character == LESS_THAN_SIGN ||
                           character == EQUALS_SIGN    ||
                           character == GRAVE_ACCENT )
                {
                    logError( text.position(), ErrorCode::UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE );
                    appendToPendingTokenAttrValueBuffer( character );
                } else {
                    appendToPendingTokenAttrValueBuffer( character );
                }
            } break;

            case State_e::AFTER_ATTRIBUTE_VALUE_QUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_TAG );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::BEFORE_ATTRIBUTE_NAME );
                } else if( character == SOLIDUS ) {
                    setState( State_e::SELF_CLOSING_START_TAG );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_BETWEEN_ATTRIBUTES );
                    reconsume( State_e::BEFORE_ATTRIBUTE_NAME );
                }
            } break;

            case State_e::SELF_CLOSING_START_TAG: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_TAG );
                    emitEndOfFileToken( text.position() );
                } else if( character == GREATER_THAN_SIGN ) {
                    setPendingTokenSelfCloseFlag();
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    logError( text.position(), ErrorCode::UNEXPECTED_SOLIDUS_IN_TAG );
                    reconsume( State_e::BEFORE_ATTRIBUTE_NAME );
                }
            } break;

            case State_e::BOGUS_COMMENT: {
                if( text.reachedEnd() ) {
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenText( REPLACEMENT_CHAR );
                } else {
                    appendToPendingTokenText( character );
                }
            } break;

            case State_e::MARKUP_DECLARATION_OPEN: {
                const auto [ next_char, end_reached ] = text.character( 1 );
                const auto next7                      = text.characters( 7 );

                if( character == HYPHEN_MINUS && !end_reached && next_char == HYPHEN_MINUS ) {
                    text.advanceCol( 1 );
                    createCommentToken( text.position() );
                    setState( State_e::COMMENT_START );
                } else if( unicode::ascii::toupper( next7 ) == U"DOCTYPE" ) {
                    text.advanceCol( 6 );
                    setState( State_e::DOCTYPE );
                } else if( next7 == U"[CDATA[" ) {
                    text.advanceCol( 6 );
                    if( _tree_builder.hasAdjustedCurrentNode() && _tree_builder.adjustedCurrentNode().second != specs::html5::Namespace::HTML5 ) {
                        setState( State_e::CDATA_SECTION );
                    } else {
                        logError( text.position(), ErrorCode::CDATA_IN_HTML_CONTENT );
                    }
                    createCommentToken( text.position() );
                    appendToPendingTokenText( U"[CDATA[" );
                    setState( State_e::BOGUS_COMMENT );
                } else {
                    logError( text.position(), ErrorCode::INCORRECTLY_OPENED_COMMENT );
                    createCommentToken( text.position() );
                    reconsume( State_e::BOGUS_COMMENT );
                }
            } break;

            case State_e::COMMENT_START: {
                if( character == HYPHEN_MINUS ) {
                    setState( State_e::COMMENT_START_DASH );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::ABRUPT_CLOSING_OF_EMPTY_COMMENT );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    reconsume( State_e::COMMENT );
                }
            } break;

            case State_e::COMMENT_START_DASH: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_COMMENT );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    setState( State_e::COMMENT_END );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::ABRUPT_CLOSING_OF_EMPTY_COMMENT );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    appendToPendingTokenText( HYPHEN_MINUS );
                    reconsume( State_e::COMMENT );
                }
            } break;

            case State_e::COMMENT: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_COMMENT );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == LESS_THAN_SIGN ) {
                    appendToPendingTokenText( character );
                    setState( State_e::COMMENT_LESS_THAN_SIGN );
                } else if( character == HYPHEN_MINUS ) {
                    setState( State_e::COMMENT_END_DASH );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenText( REPLACEMENT_CHAR );
                } else {
                    appendToPendingTokenText( character );
                }
            } break;

            case State_e::COMMENT_LESS_THAN_SIGN: {
                if( character == EXCLAMATION_MARK ) {
                    appendToPendingTokenText( character );
                    setState( State_e::COMMENT_LESS_THAN_SIGN_BANG );
                } else if( character == LESS_THAN_SIGN ) {
                    appendToPendingTokenText( character );
                } else {
                    reconsume( State_e::COMMENT );
                }
            } break;

            case State_e::COMMENT_LESS_THAN_SIGN_BANG: {
                if( character == HYPHEN_MINUS ) {
                    setState( State_e::COMMENT_LESS_THAN_SIGN_BANG_DASH );
                } else {
                    reconsume( State_e::COMMENT );
                }
            } break;

            case State_e::COMMENT_LESS_THAN_SIGN_BANG_DASH: {
                if( character == HYPHEN_MINUS ) {
                    setState( State_e::COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH );
                } else {
                    reconsume( State_e::COMMENT_END_DASH );
                }
            } break;

            case State_e::COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH: {
                if( text.reachedEnd() || character == GREATER_THAN_SIGN ) {
                    reconsume( State_e::COMMENT_END );
                } else {
                    logError( text.position(), ErrorCode::NESTED_COMMENT );
                    reconsume( State_e::COMMENT_END );
                }
            } break;

            case State_e::COMMENT_END_DASH: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_COMMENT );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    setState( State_e::COMMENT_END );
                } else {
                    appendToPendingTokenText( HYPHEN_MINUS );
                    reconsume( State_e::COMMENT );
                }
            } break;

            case State_e::COMMENT_END: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_COMMENT );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( character == EXCLAMATION_MARK ) {
                    setState( State_e::COMMENT_END_BANG );
                } else if( character == HYPHEN_MINUS ) {
                    appendToPendingTokenText( HYPHEN_MINUS );
                } else {
                    appendToPendingTokenText( HYPHEN_MINUS );
                    appendToPendingTokenText( HYPHEN_MINUS );
                    reconsume( State_e::COMMENT );
                }
            } break;

            case State_e::COMMENT_END_BANG: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_COMMENT );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == HYPHEN_MINUS ) {
                    appendToPendingTokenText( HYPHEN_MINUS );
                    appendToPendingTokenText( HYPHEN_MINUS );
                    appendToPendingTokenText( EXCLAMATION_MARK );
                    setState( State_e::COMMENT_END_DASH );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::INCORRECTLY_CLOSED_COMMENT );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    appendToPendingTokenText( HYPHEN_MINUS );
                    appendToPendingTokenText( HYPHEN_MINUS );
                    appendToPendingTokenText( EXCLAMATION_MARK );
                    reconsume( State_e::COMMENT );
                }
            } break;

            case State_e::DOCTYPE: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    createDoctypeToken( text.position(), true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::BEFORE_DOCTYPE_NAME );
                } else if( character == GREATER_THAN_SIGN ) {
                    reconsume( State_e::BEFORE_DOCTYPE_NAME );
                } else {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME );
                    reconsume( State_e::BEFORE_DOCTYPE_NAME );
                }
            } break;

            case State_e::BEFORE_DOCTYPE_NAME: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    createDoctypeToken( text.position(), true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( unicode::ascii::isupper( character ) ) {
                    createDoctypeToken( text.position() );
                    appendToPendingTokenText( unicode::ascii::tolower( character ) );
                    setState( State_e::DOCTYPE_NAME );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    createDoctypeToken( text.position() );
                    appendToPendingTokenText( REPLACEMENT_CHAR );
                    setState( State_e::DOCTYPE_NAME );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::MISSING_DOCTYPE_NAME );
                    createDoctypeToken( text.position(), true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    createDoctypeToken( text.position() );
                    appendToPendingTokenText( character );
                    setState( State_e::DOCTYPE_NAME );
                }
            } break;

            case State_e::DOCTYPE_NAME: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::AFTER_DOCTYPE_NAME );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingTokenText( unicode::ascii::tolower( character ) );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenText( REPLACEMENT_CHAR );
                } else {
                    appendToPendingTokenText( character );
                }
            } break;

            case State_e::AFTER_DOCTYPE_NAME: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    auto next6 = unicode::ascii::toupper( text.characters( 6 ) );
                    if( next6 == U"PUBLIC" ) {
                        text.advanceCol( 5 );
                        setState( State_e::AFTER_DOCTYPE_PUBLIC_KEYWORD );
                    } else if( next6 ==U"SYSTEM" ) {
                        text.advanceCol( 5 );
                        setState( State_e::AFTER_DOCTYPE_SYSTEM_KEYWORD );
                    } else {
                        logError( text.position(), ErrorCode::INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME );
                        setPendingDoctypeTokenQuirksFlag( true );
                        reconsume( State_e::BOGUS_DOCTYPE );
                    }
                }
            } break;

            case State_e::AFTER_DOCTYPE_PUBLIC_KEYWORD: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER );
                } else if( character == QUOTATION_MARK ) {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD );
                    setPendingDoctypeTokenPIDFlag( true );
                    setState( State_e::DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED );
                } else if( character == APOSTROPHE ) {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD );
                    setPendingDoctypeTokenPIDFlag( true );
                    setState( State_e::DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::MISSING_DOCTYPE_PUBLIC_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    logError( text.position(), ErrorCode::MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    reconsume( State_e::BOGUS_DOCTYPE );
                }
            } break;

            case State_e::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == QUOTATION_MARK ) {
                    setPendingDoctypeTokenPIDFlag( true );
                    setState( State_e::DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED );
                } else if( character == APOSTROPHE ) {
                    setPendingDoctypeTokenPIDFlag( true );
                    setState( State_e::DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::MISSING_DOCTYPE_PUBLIC_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    logError( text.position(), ErrorCode::MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    reconsume( State_e::BOGUS_DOCTYPE );
                }
            } break;

            case State_e::DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == QUOTATION_MARK ) {
                    setState( State_e::AFTER_DOCTYPE_PUBLIC_IDENTIFIER );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenPIDBuffer( REPLACEMENT_CHAR );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    appendToPendingTokenPIDBuffer( character );
                }
            } break;

            case State_e::DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == APOSTROPHE ) {
                    setState( State_e::AFTER_DOCTYPE_PUBLIC_IDENTIFIER );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenPIDBuffer( REPLACEMENT_CHAR );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    appendToPendingTokenPIDBuffer( character );
                }
            } break;

            case State_e::AFTER_DOCTYPE_PUBLIC_IDENTIFIER: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIER );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( character == QUOTATION_MARK ) {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS );
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED );
                } else if( character == APOSTROPHE ) {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS );
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED );
                } else {
                    logError( text.position(), ErrorCode::MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    reconsume( State_e::BOGUS_DOCTYPE );
                }
            } break;

            case State_e::BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIER: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( character == QUOTATION_MARK ) {
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED );
                } else if( character == APOSTROPHE ) {
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED );
                } else {
                    logError( text.position(), ErrorCode::MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    reconsume( State_e::BOGUS_DOCTYPE );
                }
            } break;

            case State_e::AFTER_DOCTYPE_SYSTEM_KEYWORD: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    setState( State_e::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER );
                } else if( character == QUOTATION_MARK ) {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD );
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED );
                } else if( character == APOSTROPHE ) {
                    logError( text.position(), ErrorCode::MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD );
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::MISSING_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    logError( text.position(), ErrorCode::MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    reconsume( State_e::BOGUS_DOCTYPE );
                }
            } break;

            case State_e::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == QUOTATION_MARK ) {
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED );
                } else if( character == APOSTROPHE ) {
                    setPendingDoctypeTokenSIDFlag( true );
                    setState( State_e::DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::MISSING_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    logError( text.position(), ErrorCode::MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    reconsume( State_e::BOGUS_DOCTYPE );
                }
            } break;

            case State_e::DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == QUOTATION_MARK ) {
                    setState( State_e::AFTER_DOCTYPE_SYSTEM_IDENTIFIER );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenSIDBuffer( REPLACEMENT_CHAR );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    appendToPendingTokenSIDBuffer( character );
                }
            } break;

            case State_e::DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == APOSTROPHE ) {
                    setState( State_e::AFTER_DOCTYPE_SYSTEM_IDENTIFIER );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    appendToPendingTokenSIDBuffer( REPLACEMENT_CHAR );
                } else if( character == GREATER_THAN_SIGN ) {
                    logError( text.position(), ErrorCode::ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER );
                    setPendingDoctypeTokenQuirksFlag( true );
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    appendToPendingTokenSIDBuffer( character );
                }
            } break;

            case State_e::AFTER_DOCTYPE_SYSTEM_IDENTIFIER: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_DOCTYPE );
                    setPendingDoctypeTokenQuirksFlag( true );
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( unicode::ascii::istab( character ) || unicode::ascii::isfeed( character ) || unicode::ascii::isspace( character ) ) {
                    //ignore character
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else {
                    logError( text.position(), ErrorCode::UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER );
                    reconsume( State_e::BOGUS_DOCTYPE );
                }
            } break;

            case State_e::BOGUS_DOCTYPE: {
                if( text.reachedEnd() ) {
                    emitPendingToken( text.position() );
                    emitEndOfFileToken( text.position() );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                    emitPendingToken( text.position() );
                } else if( character == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::UNEXPECTED_NULL_CHARACTER );
                    //ignore character
                }
            } break;

            case State_e::CDATA_SECTION: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_CDATA );
                    emitEndOfFileToken( text.position() );
                } else if( character == RIGHT_SQUARE_BRACKET ) {
                    setState( State_e::CDATA_SECTION_BRACKET );
                } else {
                    emitCharacterToken( text.position(), character );
                }
            } break;

            case State_e::CDATA_SECTION_BRACKET: {
                if( character == RIGHT_SQUARE_BRACKET ) {
                    setState( State_e::CDATA_SECTION_END );
                } else {
                    emitCharacterToken( text.position(), RIGHT_SQUARE_BRACKET );
                    reconsume( State_e::CDATA_SECTION );
                }
            } break;

            case State_e::CDATA_SECTION_END: {
                if( character == RIGHT_SQUARE_BRACKET ) {
                    emitCharacterToken( text.position(), RIGHT_SQUARE_BRACKET );
                } else if( character == GREATER_THAN_SIGN ) {
                    setState( State_e::DATA );
                } else {
                    emitCharacterToken( text.position(), RIGHT_SQUARE_BRACKET );
                    emitCharacterToken( text.position(), RIGHT_SQUARE_BRACKET );
                    reconsume( State_e::CDATA_SECTION );
                }
            } break;

            case State_e::CHARACTER_REFERENCE: {
                clearTempBuffer();
                appendToTempBuffer( AMPERSAND );

                if( unicode::ascii::isalnum( character ) ) {
                    reconsume( State_e::NAMED_CHARACTER_REFERENCE );
                } else if( character == NUMBER_SIGN ) {
                    appendToTempBuffer( character );
                    setState( State_e::NUMERIC_CHARACTER_REFERENCE );
                } else {
                    flushCodePoints( text.position() );
                    clearTempBuffer(); //??
                    reconsume( returnState() );
                }
            } break;

            case State_e::NAMED_CHARACTER_REFERENCE: {
                auto match_tracker  = TrieTracker<uint32_t>();
                auto start_position = text.position();

                specs::html5::NamedCharRef::match( match_tracker, AMPERSAND );

                while( specs::html5::NamedCharRef::match( match_tracker, character ) ) {
                    appendToTempBuffer( character );
                    character = getNextChar( text );
                }

                const auto next_input_char = ( match_tracker.complete() && match_tracker.atEnd() )
                                             ? character
                                             : tempBuffer().back();

                if( match_tracker.matched() && match_tracker.complete() ) {
                    if( ( returnState() == State_e::ATTRIBUTE_VALUE_DOUBLE_QUOTED ||
                          returnState() == State_e::ATTRIBUTE_VALUE_SINGLE_QUOTED ||
                          returnState() == State_e::ATTRIBUTE_VALUE_UNQUOTED )
                        && ( match_tracker.lastMatchedElement() != SEMICOLON )
                        && ( next_input_char == EQUALS_SIGN || unicode::ascii::isalnum( next_input_char ) ) )
                    {
                        flushCodePoints( start_position );
                        reconsume( returnState() ); //since next character is already current because of the NamedCharRef::match(..) while loop

                    } else {
                        using specs::html5::NamedCharRef;

                        const auto match_buffer   = match_tracker.lastCompleteMatch();
                        const auto remainders     = match_tracker.remainder();
                        const auto [ found, ncr ] = NamedCharRef::fetch( std::u32string( match_buffer.begin(), match_buffer.end() ) );

                        if( match_tracker.lastMatchedElement() != SEMICOLON ) {
                            const auto err_pos = ( start_position + TextPos { 0, match_tracker.lastCompleteMatchSize() - 1 } );
                            logError(err_pos, ErrorCode::MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE );
                        }

                        clearTempBuffer();

                        if( found ) {
                            appendToTempBuffer( ncr.codepoint1 );

                            if( ncr.codepoint2 ) {
                                appendToTempBuffer( ncr.codepoint2 );
                            }

                        } else {
                            appendToTempBuffer( match_buffer.begin(), match_buffer.end() );
                            //TODO log error (tempbuff will be used as-is)
                        }

                        appendToTempBuffer( remainders.begin(), remainders.end() );
                        flushCodePoints( start_position );
                        reconsume( returnState() ); //since next character is already current because of the NamedCharRef::match(..) while loop
                    }

                } else {
                    flushCodePoints( start_position );
                    reconsume( State_e::AMBIGUOUS_AMPERSAND ); //since next character is already current because of the NamedCharRef::match(..) while loop
                }
            } break;

            case State_e::AMBIGUOUS_AMPERSAND: {
                if( unicode::ascii::isalnum( character ) ) {
                    if( returnState() == State_e::ATTRIBUTE_VALUE_DOUBLE_QUOTED ||
                        returnState() == State_e::ATTRIBUTE_VALUE_SINGLE_QUOTED ||
                        returnState() == State_e::ATTRIBUTE_VALUE_UNQUOTED )
                    {
                        appendToPendingTokenAttrValueBuffer( character );

                    } else {
                        emitCharacterToken( text.position(), character );
                    }
                } else if( character == SEMICOLON ) {
                    logError( text.position(), ErrorCode::UNKNOWN_NAMED_CHARACTER_REFERENCE );
                    reconsume( returnState() );
                } else {
                    reconsume( returnState() );
                }
            } break;

            case State_e::NUMERIC_CHARACTER_REFERENCE: {
                resetCharRefCode();

                if( character == LATIN_SMALL_LETTER_X || character == LATIN_CAPITAL_LETTER_X ) {
                    appendToTempBuffer( character );
                    setState( State_e::HEXADECIMAL_CHARACTER_REFERENCE_START );
                } else {
                    reconsume( State_e::DECIMAL_CHARACTER_REFERENCE_START );
                }
            } break;

            case State_e::HEXADECIMAL_CHARACTER_REFERENCE_START: {
                if( unicode::ascii::isxdigit( character ) ) {
                    reconsume( State_e::HEXADECIMAL_CHARACTER_REFERENCE );
                } else {
                    logError( text.position(), ErrorCode::ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE );
                    flushCodePoints( text.position() );
                    reconsume( returnState() );
                }
            } break;

            case State_e::DECIMAL_CHARACTER_REFERENCE_START: {
                if( unicode::ascii::isdigit( character ) ) {
                    reconsume( State_e::DECIMAL_CHARACTER_REFERENCE );
                } else {
                    logError( text.position(), ErrorCode::ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE );
                    flushCodePoints( text.position() );
                    reconsume( returnState() );
                }
            } break;

            case State_e::HEXADECIMAL_CHARACTER_REFERENCE: {
                if( unicode::ascii::isdigit( character ) ) {
                    addToCharRefCode( 16, ( character - DIGIT_ZERO ) );
                } else if( unicode::ascii::isuxdigit( character ) ) {
                    addToCharRefCode( 16, ( character - 0x0037 ) );
                } else if( unicode::ascii::islxdigit( character ) ) {
                    addToCharRefCode( 16, ( character - 0x0057 ) );
                } else if( character == SEMICOLON ) {
                    setState( State_e::NUMERIC_CHARACTER_REFERENCE_END );
                } else {
                    logError( text.position(), ErrorCode::MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE );
                    reconsume( State_e::NUMERIC_CHARACTER_REFERENCE_END );
                }
            } break;

            case State_e::DECIMAL_CHARACTER_REFERENCE: {
                if( unicode::ascii::isdigit( character ) ) {
                    addToCharRefCode( 10, ( character - DIGIT_ZERO ) );
                } else if( character == SEMICOLON ) {
                    setState( State_e::NUMERIC_CHARACTER_REFERENCE_END );
                } else {
                    logError( text.position(), ErrorCode::MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE );
                    reconsume( State_e::NUMERIC_CHARACTER_REFERENCE_END );
                }
            } break;

            case State_e::NUMERIC_CHARACTER_REFERENCE_END: {
                auto crc = charRefCode();

                if( crc == NULL_CHAR ) {
                    logError( text.position(), ErrorCode::NULL_CHARACTER_REFERENCE );
                    resetCharRefCode( REPLACEMENT_CHAR );
                } else if( !validCharRefCode() || crc > UNDEFINED_CHAR_10FFFF ) {
                    logError( text.position(), ErrorCode::CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE );
                    resetCharRefCode( REPLACEMENT_CHAR );
                } else if( unicode::utf32::issurrogate( crc ) ) {
                    logError( text.position(), ErrorCode::SURROGATE_CHARACTER_REFERENCE );
                    resetCharRefCode( REPLACEMENT_CHAR );
                } else if( unicode::utf32::isnonchar( crc ) ) {
                    logError( text.position(), ErrorCode::NONCHARACTER_CHARACTER_REFERENCE );
                } else {
                    if( crc == CARRIAGE_RETURN || ( unicode::utf32::iscntrl( crc ) && !unicode::ascii::iswspace( crc ) ) ) {
                        logError( text.position(), ErrorCode::CONTROL_CHARACTER_REFERENCE );
                    }

                    auto [found, codepoint] = specs::html5::NumericCharRef::fetch( crc );

                    if( found ) {
                        resetCharRefCode( codepoint );
                    }
                }

                clearTempBuffer();
                appendToTempBuffer( charRefCode() );
                flushCodePoints( text.position() );
                reconsume( returnState() ); //since char is already after the char ref code
            } break;
        }

        character = getNextChar( text );
    }

    return specs::Context::HTML5; //TODO
}

/**
 * Resets/Clears the tokeniser (inc. error count)
 */
void tokeniser::HTML5::reset() {
    _error_count = 0;
    setState( State_e::DATA );
    setReturnState( State_e::DATA );
    _last_start_tag.clear();
    _temp_buffer.clear();
    clearPendingToken();
}

/**
 * Gets the number of parsing error raised
 * @return Parsing error count
 */
size_t tokeniser::HTML5::errors() const {
    return _error_count;
}

//==========================================[ PRIVATE ]=============================================

/**
 * Sends a parse error to the parser log
 * @param src Source filepath
 * @param position Position in source text
 * @param err_code HTML5 error code
 */
inline void tokeniser::HTML5::logError( TextPos position, int err_code ) {
    ++_error_count;
    ParserLog::log( _src_path, THIS_CONTEXT, err_code, position );
}

/**
 * Sets the current state
 * @param state State
 */
inline void tokeniser::HTML5::setState( State_e state ) noexcept {
    _current_state = state;
}

/**
 * Sets the return state
 * @param state State
 */
inline void tokeniser::HTML5::setReturnState( tokeniser::HTML5::State_e state ) noexcept {
    _return_state = state;
}

/**
 * Gets the current state
 * @return State
 */
inline tokeniser::HTML5::State_e tokeniser::HTML5::currentState() const noexcept{
    return _current_state;
}

/**
 * Gets the set return state
 * @return State
 */
inline tokeniser::HTML5::State_e tokeniser::HTML5::returnState() const noexcept {
    return _return_state;
}

inline void tokeniser::HTML5::clearTempBuffer() {
    _temp_buffer.clear();
}

/**
 * Resets the pending token
 */
inline void tokeniser::HTML5::clearPendingToken() {
    _pending.type = Type_e::UNKNOWN;
    _pending.token_name_buffer.clear();
    _pending.field_buffer_a.clear();
    _pending.field_buffer_b.clear();
    _pending.not_missing_a = false;
    _pending.not_missing_b = false;
    _pending.quirks        = false;
    _pending.char_ref_code = 0;
    _pending.bad_ref_code  = false;
    _pending.token.reset();
}

/**
 * Append a character to the temporary buffer
 * @param c Character
 */
inline void tokeniser::HTML5::appendToTempBuffer( uint32_t c ) {
    _temp_buffer.emplace_back( c );
}

/**
 * Append a range to the temporary buffer
 * @tparam InputIt
 * @param begin
 * @param end
 */
template<typename InputIt> void tokeniser::HTML5::appendToTempBuffer( InputIt begin, InputIt end ) {
    std::copy( begin, end, back_inserter( _temp_buffer ) );
}

/**
 * Gets the temporary buffer
 * @return Reference to the temporary buffer
 */
inline const std::vector<uint32_t> &tokeniser::HTML5::tempBuffer() {
    return _temp_buffer;
}

/**
 * Emits the pending token
 * @param position Current caret position at time of call
 */
inline void tokeniser::HTML5::emitPendingToken( TextPos position ) {
    if( _pending.token == nullptr ) {
        return; //EARLY RETURN
    }

    if( _pending.token->type() == Type_e::START_TAG ) {
        auto * tk = dynamic_cast<token::html5::StartTagTk *>( _pending.token.get() );

        tk->setText( std::u32string(
            _pending.token_name_buffer.cbegin(),
            _pending.token_name_buffer.cend()
        ) );

        if( !addPendingTokenAttribute() ) {
            logError( tk->position(), ErrorCode::DUPLICATE_ATTRIBUTE );
        }

        _last_start_tag = tk->name();

    } else if( _pending.token->type() == Type_e::END_TAG ) {
        auto * tk = dynamic_cast<token::html5::EndTagTk *>( _pending.token.get() );

        tk->setText( std::u32string(
            _pending.token_name_buffer.cbegin(),
            _pending.token_name_buffer.cend()
        ) );

        if( !addPendingTokenAttribute() ) {
            logError( tk->position(), ErrorCode::DUPLICATE_ATTRIBUTE );
        }

        if( !tk->attributes().empty() ) //(13.2.5) "...end tag token is emitted with attributes..."
            logError( position, ErrorCode::END_TAG_WITH_ATTRIBUTES );
        if( tk->selfclosing() ) //(13.2.5) "...end tag token is emitted with its self-closing flag set..."
            logError( position, ErrorCode::END_TAG_WITH_TRAILING_SOLIDUS );

        _last_start_tag.clear();

    } else if( _pending.token->type() == Type_e::DOCTYPE ) {
        auto * tk = dynamic_cast<token::html5::DoctypeTk *>( _pending.token.get() );

        if( !_pending.token_name_buffer.empty() ) {
            tk->setName( std::u32string(
                _pending.token_name_buffer.cbegin(),
                _pending.token_name_buffer.cend() )
            );
        }

        if( _pending.quirks )
            tk->setForceQuirks();
        if( _pending.not_missing_a || !_pending.field_buffer_a.empty() )
            tk->setPID( std::u32string( _pending.field_buffer_a.cbegin(), _pending.field_buffer_a.cend() ) );
        if( _pending.not_missing_b || !_pending.field_buffer_b.empty() )
            tk->setSID( std::u32string( _pending.field_buffer_b.cbegin(), _pending.field_buffer_b.cend() ) );

    } else {
        _pending.token->setText( std::u32string(
            _pending.token_name_buffer.cbegin(),
            _pending.token_name_buffer.cend() )
        );
    }

    _tree_builder.addToken( std::move( _pending.token ) );
    clearPendingToken();
}

/**
 * Appends a character to the pending token's buffer (will become text()/name() of token)
 * @param c Character
 */
inline void tokeniser::HTML5::appendToPendingTokenText( uint32_t c ) {
    _pending.token_name_buffer.emplace_back( c );
}

/**
 * Appends a string to the pending token's buffer (will become text()/name() of token)
 * @param txt String to append
 */
inline void tokeniser::HTML5::appendToPendingTokenText( const std::u32string &txt ) {
    _pending.token_name_buffer.insert( _pending.token_name_buffer.end(), txt.begin(), txt.end() );
}

/**
 * Emits a token of CHARACTER type
 * @param position Position of character in source text
 * @param c Character
 */
inline void tokeniser::HTML5::emitCharacterToken( TextPos position, uint32_t c ) {
    _tree_builder.addToken( std::make_unique<token::html5::CharacterTk>( std::u32string( 1, c ), position ) );
}

/**
 * Emits a token of CHARACTER type
 * @param position Start position of character(s) in source text
 * @param str String
 */
inline void tokeniser::HTML5::emitCharacterToken( TextPos position, const std::u32string & str ) {
    _tree_builder.addToken( std::make_unique<token::html5::CharacterTk>( str, position ) );
}

/**
 * Emits a token of EOF type
 * @param position Position of EOF in source text
 */
inline void tokeniser::HTML5::emitEndOfFileToken( TextPos position ) {
    _tree_builder.addToken( std::make_unique<token::html5::EndOfFileTk>( position ) );
    _eof = true;
}

/**
 * Creates a pending token of COMMENT type
 * @param position Position in source text
 */
inline void tokeniser::HTML5::createCommentToken( TextPos position ) {
    _pending.type = Type_e::COMMENT;
    _pending.token = std::make_unique<token::html5::CommentTk>( position );
}

/**
 * Creates a pending token of DOCTYPE type
 * @param position Position in source text
 * @param force_quirks Force quirks flag (default=false)
 */
inline void tokeniser::HTML5::createDoctypeToken( TextPos position, bool force_quirks ) {
    _pending.type = Type_e::DOCTYPE;
    _pending.token = std::make_unique<token::html5::DoctypeTk>( position );

    if( force_quirks ) {
        auto *ptr = dynamic_cast<token::html5::DoctypeTk *>( _pending.token.get() );
        ptr->setForceQuirks( true );
    }
}

/**
 * Creates a pending token of START_TAG type
 * @param position Position in source text
 */
inline void tokeniser::HTML5::createStartTagToken( TextPos position ) {
    _pending.type = Type_e::START_TAG;
    _pending.token = std::make_unique<token::html5::StartTagTk>( position );
}

/**
 * Creates a pending token of END_TAG type
 * @param position Position in source text
 */
inline void tokeniser::HTML5::createEndTagToken( TextPos position ) {
    if( _pending.token ) {
        if( _pending.token->type() != Type_e::END_TAG ) {
            emitPendingToken( position );
        } else {
            clearPendingToken();
        }
    }

    _pending.type = Type_e::END_TAG;
    _pending.token = std::make_unique<token::html5::EndTagTk>( position );
}



inline bool tokeniser::HTML5::appropriateEndTagToken() {
    if( _pending.token->type() == Type_e::END_TAG ) {
        auto name = std::u32string( _pending.token_name_buffer.cbegin(), _pending.token_name_buffer.cend() );
        
        return ( !_last_start_tag.empty() && name == _last_start_tag );
    }

    return false;
}

/**
 * Checks string is same as temp buffer content
 * @param str String
 * @return Temp buffer equal to string
 */
inline bool tokeniser::HTML5::isEqualToTempBuffer( const std::u32string &str ) const {
    return std::equal( str.cbegin(), str.cend(), _temp_buffer.cbegin(), _temp_buffer.cend() );
}

/**
 * Appends a code point to the pending token's attribute name buffer
 * @param c Code point
 */
inline void tokeniser::HTML5::appendToPendingTokenAttrNameBuffer( uint32_t c ) {
    _pending.field_buffer_a.emplace_back( c );
}

/**
 * Appends a code point to the pending token's attribute value buffer
 * @param c Code point
 */
inline void tokeniser::HTML5::appendToPendingTokenAttrValueBuffer( uint32_t c ) {
    _pending.field_buffer_b.emplace_back( c );
}

/**
 * Appends a code point to the pending token's PUBLIC ID buffer
 * @param c Code point
 */
inline void tokeniser::HTML5::appendToPendingTokenPIDBuffer( uint32_t c ) {
    _pending.field_buffer_a.emplace_back( c );
}

/**
 * Appends a code point to the pending token's SYSTEM ID buffer
 * @param c Code point
 */
inline void tokeniser::HTML5::appendToPendingTokenSIDBuffer( uint32_t c ) {
    _pending.field_buffer_b.emplace_back( c );
}

/**
 * Sets the pending token's PUBLIC ID 'not_missing' flag
 * @param not_missing Flag value
 */
inline void tokeniser::HTML5::setPendingDoctypeTokenPIDFlag( bool not_missing ) {
    _pending.not_missing_a = not_missing;
}

/**
 * Sets the pending token's SYSTEM ID 'not_missing' flag
 * @param not_missing Flag value
 */
inline void tokeniser::HTML5::setPendingDoctypeTokenSIDFlag( bool not_missing ) {
    _pending.not_missing_b = not_missing;
}

/**
 * Sets the pending token's 'force quirks' flag
 * @param force_quirks Flag
 */
inline void tokeniser::HTML5::setPendingDoctypeTokenQuirksFlag( bool force_quirks ) {
    _pending.quirks = force_quirks;
}

/**
 * Adds the pending attribute name:value in the buffers to the token (clears buffers after)
 * @return Success
 */
inline bool tokeniser::HTML5::addPendingTokenAttribute() {
    if( _pending.field_buffer_a.empty() ) //attribute name buffer
        return true; //EARLY RETURN

    if( !_pending.token ) {
        //TODO log error
        return false;  //TODO replace by throw
    }

    if( _pending.token->type() != Type_e::START_TAG && _pending.token->type() != Type_e::END_TAG ) {
        //TODO log error
        return false; //TODO replace by throw
    }

    bool error = false;
    auto * tk  = dynamic_cast<token::html5::GenericTagTk *>( _pending.token.get() );
    auto name  = std::u32string( _pending.field_buffer_a.cbegin(), _pending.field_buffer_a.cend() );

    if( std::ranges::none_of( tk->attributes(), [&name]( auto & attr ) { return attr.name == name; } ) ) {
        tk->addAttribute( std::move( name ),
                          std::u32string( _pending.field_buffer_b.cbegin(), _pending.field_buffer_b.cend() ) );
    } else { //attribute name already exists
        error = true;
    }

    clearPendingTokenAttribute();
    return !( error );
}

/**
 * Clears the pending tokens's attribute buffers
 */
inline void tokeniser::HTML5::clearPendingTokenAttribute() {
    _pending.field_buffer_a.clear();
    _pending.field_buffer_b.clear();
    _pending.not_missing_a = false;
    _pending.not_missing_b = false;
}

/**
 * Sets the 'self-close' flag on the pending token
 */
inline void tokeniser::HTML5::setPendingTokenSelfCloseFlag() {
    if( !_pending.token ) {
        //TODO log error
        //TODO throw
    }

    if( _pending.token->type() != Type_e::START_TAG && _pending.token->type() != Type_e::END_TAG ) {
        //TODO log error
        //TODO throw
    }

    auto * tk = dynamic_cast<token::html5::GenericTagTk *>( _pending.token.get() );
    tk->setSelfClosing( true );
}

/**
 * Sets the cached character reference code
 * @param val Value (default: 0)
 */
inline void tokeniser::HTML5::resetCharRefCode( uint32_t val ) {
    _pending.char_ref_code = val;
    _pending.bad_ref_code  = false;
}

/**
 * Adds a numeric version of a base X character
 * @param base Base
 * @param num Numeric version of the character
 */
inline void tokeniser::HTML5::addToCharRefCode( uint32_t base, uint32_t num ) {
    uint64_t temp = _pending.char_ref_code;

    temp *= base;
    temp += num;

    if( temp <= std::numeric_limits<uint32_t>::max() ) {
        _pending.char_ref_code = static_cast<uint32_t>( temp );
    } else {
        _pending.bad_ref_code = true;
    }
}

/**
 * Gets the character reference code currently cached
 * @return Character reference code
 */
inline uint32_t tokeniser::HTML5::charRefCode() const {
    return _pending.char_ref_code;
}

/**
 * Checks the currently cached character reference code is valid
 * @return Valid state
 */
inline bool tokeniser::HTML5::validCharRefCode() const {
    return !_pending.bad_ref_code;
}

/**
 * Flushes all code points consumed as character reference in the temp buffer
 *  to either the attribute value or as individual character tokens
 * @param position Text position
 */
inline void tokeniser::HTML5::flushCodePoints( TextPos position ) {
    if( returnState() == State_e::ATTRIBUTE_VALUE_DOUBLE_QUOTED ||
        returnState() == State_e::ATTRIBUTE_VALUE_SINGLE_QUOTED ||
        returnState() == State_e::ATTRIBUTE_VALUE_UNQUOTED )
    {
        for( auto c : _temp_buffer ) {
            appendToPendingTokenAttrValueBuffer( c );
        }
    } else {
        for( auto c : _temp_buffer ) {
            emitCharacterToken( position, c );
        }
    }

    clearTempBuffer(); //TODO check if need that
}
