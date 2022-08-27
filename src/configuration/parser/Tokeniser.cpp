#include "Tokeniser.h"

#include "Parser.h"
#include "specs/ErrorCode.h"
#include "token/ConfigTk.h"
#include "../../dto/U32Text.h"
#include "../../unicode/ascii.h"
#include "../../unicode/utf8.h"
#include "../../reporter/ParseReporter.h"
#include "../../exception/parsing_failure.h"
#include "../../logger/Logger.h"

using namespace blogator::configuration::parser;
using           blogator::configuration::parser::specs::ErrorCode;
using           blogator::configuration::parser::token::ConfigTk;

/**
 * Constructor
 * @param parser Configuration parser instance
 */
Tokeniser::Tokeniser( Parser & parser ) :
    _parser( parser ),
    _current_state( State_e::INITIAL ),
    _error_count( 0 ),
    _eof( false ),
    _nesting( 0 )
{}

/**
 * Parse configuration source
 * @param text Configuration source content to parse
 */
void Tokeniser::parse( blogator::U32Text & text ) {
    _src_path = text.path(); //internal caching for error calls
    _nesting  = 0;
    _eof      = false;

    text.reconsume(); //to stay on first char during first call to `text.nextChar()`

    /**
     * [LAMBDA] Sets the state to reconsume the current code point under the caret
     */
    const auto reconsume = [&]( State_e new_state ) {
        text.reconsume();
        setState( new_state );
    };

    // ============================== [ Tokenizer state machine ] ==================================
    auto character = text.nextChar();

    while( !_eof ) {
//        { //TODO remove
//            std::stringstream ss;
//            ss << "State: " << currentState() << " ['";
//            unicode::utf8::convert( ss, text.character() );
//            ss << "'] @ " << text.position();
//            LOG_ERROR( ss.str() );
//        }
        switch( currentState() ) {
            case State_e::INITIAL: {
                resetPendingBuffer( text.position() );

                if( text.reachedEnd() ) {
                    setState( State_e::END_OF_FILE );
                } else if( _nesting > 0 && character == unicode::RIGHT_CURLY_BRACKET ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::BRACKET, character, text.position() ) );
                    setState( consumeReturnState() );
                    --_nesting;
                } else if( _nesting > 0 && character == unicode::LEFT_CURLY_BRACKET ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::BRACKET, character, text.position() ) );
                    ++_nesting;
                } else if( character == unicode::SOLIDUS ) {
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::INITIAL );
                    setState( State_e::COMMENT_SOLIDUS );
                } else if( unicode::ascii::isupper( character ) ||
                           unicode::ascii::islower( character ) )
                {
                    appendToPendingBuffer( character );
                    setState( State_e::KEY );
                } else if( unicode::ascii::isOperator( character ) ) {
                    logError( text.position(), ErrorCode::INVALID_CHARACTER_IN_KEY, character );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::OPERATOR, character, text.position() ) );
                } else if( unicode::ascii::isPunctuator( character ) ) {
                    logError( text.position(), ErrorCode::INVALID_CHARACTER_IN_KEY, character );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::PUNCTUATOR, character, text.position() ) );
                } else if( !unicode::ascii::isfeed( character ) &&
                           character != unicode::SPACE          &&
                           character != unicode::TAB )
                {
                    logError( text.position(), ErrorCode::INVALID_CHARACTER_IN_KEY, character );
                    appendToPendingBuffer( character );
                    setState( State_e::KEY );
                } //else: ignore whitespace
            } break;

            case State_e::KEY: {
                if( text.reachedEnd() ) {
                    dispatchPendingBufferAsToken( Type_e::KEY );
                    reconsume( State_e::END_OF_FILE );
                } else if( unicode::ascii::isfeed( character ) ||
                           character == unicode::SPACE         ||
                           character == unicode::TAB           ||
                           character == unicode::EQUALS_SIGN )
                {
                    dispatchPendingBufferAsToken( Type_e::KEY );
                    reconsume( State_e::AFTER_KEY );
                } else if( unicode::ascii::isalnum( character ) || character == unicode::UNDERSCORE ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::COLON ) {
                    dispatchPendingBufferAsToken( Type_e::NAMESPACE );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::OPERATOR, unicode::COLON, text.position() ) );
                    setState( State_e::NAMESPACE );
                } else if( character == unicode::COMMA || character == unicode::SEMICOLON ) {
                    dispatchPendingBufferAsToken( Type_e::KEY );
                    reconsume( State_e::AFTER_KEY );
                } else if( character == unicode::SOLIDUS ) {
                    dispatchPendingBufferAsToken( Type_e::KEY );
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::AFTER_KEY );
                    setState( State_e::COMMENT_SOLIDUS );
                } else {
                    logError( text.position(), ErrorCode::INVALID_CHARACTER_IN_KEY, character );
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::NAMESPACE: {
                resetPendingBuffer( text.position() );

                if( text.reachedEnd() ) {
                    setState( State_e::END_OF_FILE );
                } else if( character == unicode::COLON ) {
                    logError( text.position(), ErrorCode::INVALID_NAMESPACE_SEPARATOR, character );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::OPERATOR, unicode::COLON, text.position() ) );
                } else if( unicode::ascii::isfeed( character ) ||
                           character == unicode::SPACE         ||
                           character == unicode::TAB           ||
                           character == unicode::EQUALS_SIGN )
                {
                    reconsume( State_e::AFTER_KEY );
                } else {
                    reconsume( State_e::KEY );
                }
            } break;

            case State_e::AFTER_KEY: {
                resetPendingBuffer( text.position() );

                if( text.reachedEnd() ) {
                    setState( State_e::END_OF_FILE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    setState( State_e::INITIAL );
                } else if( character == unicode::EQUALS_SIGN ) {
                    appendToPendingBuffer( character );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::OPERATOR, pendingBufferToStr(), pendingBufferPosition() ) );
                    setState( State_e::BEFORE_VALUE );
                } else if( character == unicode::COMMA || character == unicode::SEMICOLON ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::PUNCTUATOR, character, text.position() ) );
                    setState( State_e::INITIAL );
                } else if( character == unicode::SOLIDUS ) {
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::INITIAL );
                    setState( State_e::COMMENT_SOLIDUS );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    reconsume( State_e::INITIAL );
                } //else: ignore space and tab characters
            } break;

            case State_e::BEFORE_VALUE: {
                if( text.reachedEnd() ) {
                    setState( State_e::END_OF_FILE );
                } else if( character == unicode::SOLIDUS ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::BEFORE_VALUE );
                    setState( State_e::COMMENT_SOLIDUS );
                } else if( character == unicode::LEFT_CURLY_BRACKET ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::BRACKET, character, text.position() ) );
                    pushReturnState( State_e::BEFORE_VALUE );
                    setState( State_e::INITIAL );
                    ++_nesting;
                } else if( character == unicode::RIGHT_CURLY_BRACKET ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::BRACKET, character, text.position() ) );
                    --_nesting;
                } else if( character == unicode::LEFT_PARENTHESIS || character == unicode::RIGHT_PARENTHESIS ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::BRACKET, character, text.position() ) );
                } else if( unicode::ascii::isOperator( character ) ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::OPERATOR, character, text.position() ) );
                } else if( character == unicode::COMMA ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::PUNCTUATOR, character, text.position() ) );
                } else if( character == unicode::SEMICOLON ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::PUNCTUATOR, character, text.position() ) );
                    setState( State_e::INITIAL );
                } else if( character == unicode::FULL_STOP ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    setState( State_e::VALUE_INTEGER_FLOAT );
                } else if( unicode::ascii::isdigit( character ) ) {
                    resetPendingBuffer( text.position() );

                    if( character == unicode::DIGIT_ZERO && text.characters( 2 ) == U"0x" ) {
                        appendToPendingBuffer( character );
                        appendToPendingBuffer( unicode::LATIN_SMALL_LETTER_X );
                        text.advanceCaret( 2 );
                        reconsume( State_e::VALUE_INTEGER_BASE16 );
                    } else {
                        reconsume( State_e::VALUE_INTEGER_BASE10 );
                    }
                } else if( unicode::ascii::isalpha( character ) ) {
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::VALUE_NAME );
                } else if( character == unicode::QUOTATION_MARK ) {
                    resetPendingBuffer( text.position() );
                    setState( State_e::VALUE_STRING );
                } else if( !unicode::ascii::isfeed( character) &&
                           character != unicode::SPACE         &&
                           character != unicode::TAB )
                {
                    logError( text.position(), ErrorCode::INVALID_VALUE_NAME_FORMAT );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::VALUE_RAW );
                } //else: ignore whitespace
            } break;

            case State_e::VALUE_NAME: {
                if( text.reachedEnd()                         ||
                    ( unicode::ascii::isOperator( character ) && character != unicode::COLON ) ||
                    unicode::ascii::isfeed( character )       ||
                    unicode::ascii::isPunctuator( character ) ||
                    character == unicode::SPACE               ||
                    character == unicode::TAB )
                {
                    if( matchPendingBuffer( U"true" ) || matchPendingBuffer( U"false" ) ) {
                        dispatchPendingBufferAsToken( Type_e::BOOLEAN_LITERAL );
                    } else {
                        dispatchPendingBufferAsToken( Type_e::VALUE_NAME );
                    }
                    reconsume( State_e::AFTER_VALUE );

                } else if( character == unicode::COLON ) {
                    dispatchPendingBufferAsToken( Type_e::NAMESPACE );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::OPERATOR, unicode::COLON, text.position() ) );
                    setState( State_e::VALUE_NAMESPACE );
                } else if( unicode::ascii::isalnum( character ) || character == unicode::UNDERSCORE ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::SOLIDUS ) {
                    dispatchPendingBufferAsToken( Type_e::VALUE_NAME );
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::AFTER_VALUE );
                    setState( State_e::COMMENT_SOLIDUS );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_VALUE_NAME_FORMAT );
                    reconsume( State_e::VALUE_RAW );
                }
            } break;

            case State_e::VALUE_NAMESPACE: {
                resetPendingBuffer( text.position() );

                if( text.reachedEnd() ) {
                    setState( State_e::END_OF_FILE );
                } else if( character == unicode::COLON ) {
                    logError( text.position(), ErrorCode::INVALID_NAMESPACE_SEPARATOR, character );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::OPERATOR, unicode::COLON, text.position() ) );
                } else if( unicode::ascii::isfeed( character )     ||
                           unicode::ascii::isOperator( character ) ||
                           character == unicode::SPACE             ||
                           character == unicode::TAB               ||
                           character == unicode::COMMA             ||
                           character == unicode::SEMICOLON )
                {
                    reconsume( State_e::AFTER_VALUE );
                } else {
                    reconsume( State_e::VALUE_NAME );
                }
            } break;

            case State_e::VALUE_RAW: {
                if( text.reachedEnd()                       ||
                    unicode::ascii::isfeed( character )     ||
                    unicode::ascii::isOperator( character ) ||
                    character == unicode::SPACE             ||
                    character == unicode::TAB               ||
                    character == unicode::COMMA             ||
                    character == unicode::SEMICOLON )
                {
                    flushPendingBufferCharactersToDispatch();
                    reconsume( State_e::AFTER_VALUE );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::VALUE_STRING: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_STRING_LITERAL );
                    dispatchPendingBufferAsToken( Type_e::STRING_LITERAL );
                    setState( State_e::END_OF_FILE );
                } else if( character == unicode::QUOTATION_MARK ) {
                    dispatchPendingBufferAsToken( Type_e::STRING_LITERAL );
                    setState( State_e::AFTER_VALUE );
                } else if( character == unicode::REVERSE_SOLIDUS ) {
                    pushReturnState( State_e::VALUE_STRING );
                    setState( State_e::ESCAPED_CHARACTER );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::VALUE_INTEGER_FLOAT: {
                if( text.reachedEnd()                       ||
                    unicode::ascii::isfeed( character )     ||
                    unicode::ascii::isOperator( character ) ||
                    character == unicode::SPACE             ||
                    character == unicode::TAB               ||
                    character == unicode::COMMA             ||
                    character == unicode::SEMICOLON )
                {
                    dispatchPendingBufferAsToken( Type_e::INTEGER_LITERAL_FLT );
                    reconsume( State_e::AFTER_VALUE );
                } else if( unicode::ascii::isdigit( character ) ) {
                    appendToPendingBuffer( character );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_VALUE_NAME_FORMAT );
                    reconsume( State_e::VALUE_RAW );
                }
            } break;

            case State_e::VALUE_INTEGER_BASE10: {
                if( text.reachedEnd()                       ||
                    unicode::ascii::isfeed( character )     ||
                    unicode::ascii::isOperator( character ) ||
                    character == unicode::SPACE             ||
                    character == unicode::TAB               ||
                    character == unicode::COMMA             ||
                    character == unicode::SEMICOLON )
                {
                    dispatchPendingBufferAsToken( Type_e::INTEGER_LITERAL_10 );
                    reconsume( State_e::AFTER_VALUE );
                } else if( character == unicode::FULL_STOP ) {
                    appendToPendingBuffer( character );
                    setState( State_e::VALUE_INTEGER_FLOAT );
                } else if( unicode::ascii::isdigit( character ) ) {
                    appendToPendingBuffer( character );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_VALUE_NAME_FORMAT );
                    reconsume( State_e::VALUE_RAW );
                }
            } break;

            case State_e::VALUE_INTEGER_BASE16: {
                if( text.reachedEnd()                       ||
                    unicode::ascii::isfeed( character )     ||
                    unicode::ascii::isOperator( character ) ||
                    character == unicode::SPACE             ||
                    character == unicode::TAB               ||
                    character == unicode::COMMA             ||
                    character == unicode::SEMICOLON )
                {
                    dispatchPendingBufferAsToken( Type_e::INTEGER_LITERAL_16 );
                    reconsume( State_e::AFTER_VALUE );
                } else if( unicode::ascii::isxdigit( character ) ) {
                    appendToPendingBuffer( character );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_VALUE_NAME_FORMAT );
                    reconsume( State_e::VALUE_RAW );
                }
            } break;

            case State_e::AFTER_VALUE: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_VALUE );
                    setState( State_e::END_OF_FILE );
                } else if( character == unicode::SEMICOLON ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::PUNCTUATOR, character, text.position() ) );
                    setState( State_e::INITIAL );
                } else if( !unicode::ascii::isfeed( character ) &&
                           character != unicode::SPACE          &&
                           character != unicode::TAB )
                {
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::BEFORE_VALUE );
                } //else: ignore whitespace
            } break;

            case State_e::ESCAPED_CHARACTER: {
                if( text.reachedEnd() ) {
                    appendToPendingBuffer( unicode::REVERSE_SOLIDUS );
                    reconsume( consumeReturnState() );
                } else {
                    appendToPendingBuffer( character );
                    setState( consumeReturnState() );
                }
            } break;

            case State_e::COMMENT_SOLIDUS: {
                if( text.reachedEnd() ) {
                    logError( pendingBufferPosition(), ErrorCode::SYNTAX_ERROR, unicode::SOLIDUS );
                    dispatchPendingBufferAsToken( Type_e::OPERATOR );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::SOLIDUS ) {
                    clearPendingBuffer();
                    setState( State_e::COMMENT_INLINED );
                } else if( character == unicode::ASTERISK ) {
                    clearPendingBuffer();
                    setState( State_e::COMMENT_BLOCK );
                } else {
                    if( returnState() == State_e::INITIAL ) {
                        logError( pendingBufferPosition(), ErrorCode::INVALID_CHARACTER_IN_KEY, unicode::SOLIDUS );
                    }
                    dispatchPendingBufferAsToken( Type_e::OPERATOR );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::COMMENT_INLINED: {
                if( text.reachedEnd() ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::COMMENT, pendingBufferToStr(), pendingBufferPosition() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::COMMENT, pendingBufferToStr(), pendingBufferPosition() ) );
                    setState( consumeReturnState() );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::COMMENT_BLOCK: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_COMMENT_BLOCK );
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::COMMENT, pendingBufferToStr(), pendingBufferPosition() ) );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::ASTERISK ) {
                    setState( State_e::COMMENT_ASTERISK );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::COMMENT_ASTERISK: { //"/* .... *?"
                if( text.reachedEnd() ) {
                    appendToPendingBuffer( unicode::ASTERISK );
                    reconsume( State_e::COMMENT_BLOCK );
                } else if( character == unicode::SOLIDUS ) {
                    dispatchToken( std::make_unique<ConfigTk>( Type_e::COMMENT, pendingBufferToStr(), pendingBufferPosition() ) );
                    setState( consumeReturnState() );
                } else {
                    appendToPendingBuffer( unicode::ASTERISK );
                    reconsume( State_e::COMMENT_BLOCK );
                }
            } break;

            case State_e::END_OF_FILE: {
                if( _nesting != 0 ) {
                    logError( text.position(), ErrorCode::ASYMMETRIC_VALUE_NESTING, std::to_string( _nesting ) );
                }

                dispatchToken( std::make_unique<ConfigTk>( Type_e::END_OF_FILE, text.position() ) );
                _eof = true;
            } break;

            default: { //safety net to catch developer stupidity
                std::stringstream ss;
                ss << "[configuration::parser::Tokeniser::parse( U32Text & )] "
                   << "Unknown tokeniser state (" << currentState() << ") - 'case' implementation missing.";
                throw exception::parsing_failure( ss.str() );
            } break;
        }

        character = text.nextChar();
    }
}

//==========================================[ PRIVATE ]=============================================
/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Markdown error code
 */
void Tokeniser::logError( blogator::TextPos position, int err_code ) {
    ++_error_count;
    reporter::ParseReporter::log( _src_path, reporter::Context::CONFIGURATION, err_code, position );
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Configuration error code
 * @param c Current character
 */
inline void Tokeniser::logError( blogator::TextPos position, int err_code, char32_t c ) {
    std::stringstream ss;
    ss << "\'" << unicode::utf8::convert( std::u32string( 1, c ) ) << "\'";
    ++_error_count;
    reporter::ParseReporter::log( _src_path, reporter::Context::CONFIGURATION, err_code, ss.str(), position );
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Configuration error code
 * @param str Description string
 */
inline void Tokeniser::logError( blogator::TextPos position, int err_code, std::string str ) {
    ++_error_count;
    reporter::ParseReporter::log( _src_path, reporter::Context::CONFIGURATION, err_code, std::move( str ), position );
}

/**
 * Sets the current state
 * @param state State
 */
inline void Tokeniser::setState( Tokeniser::State_e state ) noexcept {
    _current_state = state;
}

/**
 * Gets the current state
 * @return State
 */
inline Tokeniser::State_e Tokeniser::currentState() const noexcept {
    return _current_state;
}

/**
 * Gets the return state
 * @return State
 */
inline Tokeniser::State_e Tokeniser::returnState() const {
    return (_return_states.empty() ? State_e::INITIAL : _return_states.top() );
}

/**
 * Modifies the last added return state on the stack (if stack is empty the state will be pushed onto it)
 * @param state State
 */
inline void Tokeniser::modifyReturnState( Tokeniser::State_e state ) {
    if( _return_states.empty() ) {
        _return_states.push( state );
    } else {
        _return_states.top() = state;
    }
}

/**
 * Pushes a return state on the stack
 * @param state State
 */
inline void Tokeniser::pushReturnState( Tokeniser::State_e state ) {
    _return_states.push( state );
}

/**
 * Consumes the last return state added to the stack
 * @return Last return state or 'BEFORE_BLOCK' if already consumed
 */
inline Tokeniser::State_e Tokeniser::consumeReturnState() {
    auto last = State_e::INITIAL;

    if( !_return_states.empty() ) {
        last = _return_states.top();
        _return_states.pop();
    }

    return last;
}

/**
 * Append a character to the temporary text buffer
 * @param c Character
 */
inline void Tokeniser::appendToPendingBuffer( char32_t c ) {
    _pending.buffer.emplace_back( c );
}

/**
 * Clears the content of the temporary pending buffer
 * @param Buffer New start position of buffer relative to the source text
 */
inline void Tokeniser::resetPendingBuffer( blogator::TextPos position ) {
    _pending.buffer.clear();
    _pending.position = position;
}

/**
 * Clear the pending buffer content
 */
inline void Tokeniser::clearPendingBuffer() {
    _pending.buffer.clear();
}

/**
 * Check if content of buffer matches a string
 * @param txt String to match
 * @return Content match state
 */
bool Tokeniser::matchPendingBuffer( const std::u32string &txt ) const {
    if( _pending.buffer.size() == txt.size() ) {
        for( auto i = 0; i < txt.size(); ++i ) {
            if( _pending.buffer.at( i ) != txt.at( i ) ) {
                return false;
            }
        }

        return true;
    }

    return false;
}

/**
 * Gets the cached pending position
 * @return Cached position
 */
inline blogator::TextPos Tokeniser::pendingBufferPosition() const {
    return _pending.position;
}

/**
 * Gets the content of the temporary pending buffer as a string
 * @return Buffer content
 */
inline std::u32string Tokeniser::pendingBufferToStr() const {
    return { _pending.buffer.cbegin(), _pending.buffer.cend() };
}

/**
 * Sends the content of the buffer as character tokens to the parser
 */
void Tokeniser::flushPendingBufferCharactersToDispatch() {
    auto position = _pending.position;

    for( const auto c : _pending.buffer ) {
        dispatchToken( std::make_unique<ConfigTk>( Type_e::CHARACTER, std::u32string( 1, c ), position ) );

        if( unicode::ascii::isfeed( c ) ) {
            position = { position.line + 1, 1 };
        } else {
            position += { 0, 1 };
        }
    }

    resetPendingBuffer( position );
}

/**
 * Sends the content of the buffer as a single token to the parser
 * @param tk_type TokenType enum
 */
inline void Tokeniser::dispatchPendingBufferAsToken( Tokeniser::Type_e tk_type ) {
    dispatchToken( std::make_unique<ConfigTk>( tk_type, pendingBufferToStr(), pendingBufferPosition() ) );
}

/**
 * Sends token to the parser
 * @param token Token
 */
inline void Tokeniser::dispatchToken( Tokeniser::TokenPtr_t token ) {
    _parser.dispatch( std::move( token ) );
}