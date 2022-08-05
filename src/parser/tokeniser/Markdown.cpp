#include "Markdown.h"

#include <set>
#include <type_traits>

#include "../../logger/Logger.h"
#include "../../exception/parsing_failure.h"
#include "../logging/ParserLog.h"
#include "../../unicode/unicode.h"
#include "../token/markdown/tokens.h"

using namespace blogator::parser;
using namespace blogator::parser::token::markdown;
using           blogator::parser::specs::markdown::ErrorCode;
using           blogator::parser::logging::ParserLog;

/**
 * Constructor
 * @param html_builder MarkdownToHtml instance
 */
blogator::parser::tokeniser::Markdown::Markdown( parser::builder::MarkdownToHtml & html_builder ) :
    _html_builder( html_builder ),
    _current_state( State_e::BEFORE_BLOCK ),
    _empty_lines( 0 ),
    _actual_blockquote_lvl( 0 ),
    _curr_blockquote_lvl( 0 ),
    _curr_open_container_i( 0 ),
    _list_spacing( ListSpacing_e::DEFAULT_TIGHT ),
    _error_count( 0 ),
    _eof( false )
{}

/**
 * Parse Markdown content into tokens
 * @param text UTF32 text
 * @param starting_ctx Context from which the parsing originally began (default=MARKDOWN)
 * @return Ending/break Context
 * @throws exception::parsing_failure when an unrecoverable error occurs during parsing
 */
specs::Context tokeniser::Markdown::parse( U32Text & text, specs::Context starting_ctx ) {
    _src_path = text.path(); //internal caching for error calls
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
            case State_e::BEFORE_BLOCK: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else {
                    clearPendingCacheData();
                    setPendingBufferStartPosition( text.position() );
                    incrementBlockLineCount();
                    reconsume( State_e::BLOCK_BEGIN );
                }
            } break;

            case State_e::AFTER_BLOCK: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( peekLastOpenContainerBlockType() != Type_e::NONE ) {
                    closeFormattingMarkers( text.position() );

                    if( hasOpenBlock() && specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeLastOpenedBlock( text.position() );
                    }

                    reconsume( State_e::BEFORE_BLOCK );
                } else {
                    if( !unicode::ascii::isfeed( character ) ) {
                        logError( text.position(), ErrorCode::BLOCK_LINEFEED_SEPARATOR_MISSING, character );
                        reconsume( State_e::AFTER_BLOCK_DOUBLE_NEWLINE );
                    } else {
                        setState( State_e::AFTER_BLOCK_DOUBLE_NEWLINE );
                    }
                }
            } break;

            case State_e::AFTER_BLOCK_DOUBLE_NEWLINE: {
                closeFormattingMarkers( text.position() );

                if( hasOpenBlock() ) {
                    closeLastOpenedBlock( text.position() );
                }

                if( unicode::ascii::isfeed( character ) ) {
                    ++_empty_lines;
                    setListSpacingFlag( ListSpacing_e::LOOSE );
                    setState( State_e::BEFORE_BLOCK );
                } else {
                    reconsume( State_e::BEFORE_BLOCK );
                }
            } break;

            case State_e::BLOCK_BEGIN: {
                if( hasOpenContainerBlock() )  {
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_BEGIN );
                } else {
                    reconsume( State_e::BLOCK_BEGIN_WHITESPACE );
                }
            } break;

            case State_e::BLOCK_BEGIN_WHITESPACE: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::SPACE ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::TAB ) {
                    appendToPendingBuffer( FOUR_SPACE_CHARS );
                } else if( unicode::ascii::isfeed( character ) ) {
                    setState( State_e::NEWLINE_AFTER_EMPTY_LINE );
                } else {
                    reconsume( State_e::BLOCK_BEGIN_AFTER_WHITESPACE );
                }
            } break;

            case State_e::BLOCK_BEGIN_AFTER_WHITESPACE: {
                _empty_lines = 0; //reset
                resetPendingBuffer( text.position() );

                if( character == unicode::NUMBER_SIGN ) { //'atx' styled heading
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    reconsume( State_e::HEADING_BLOCK_ATX_DEFINITION );
                } else if( character == unicode::LESS_THAN_SIGN ) { //html block
                    clearHtmlCache( text.position() );
                    _html_cache.fallback_state = std::make_unique<U32Text::State>( text.createMarker() );
                    pushSectionMarker(
                        queueToken( std::make_unique<HtmlBlockTk>( text.position() ) )
                    );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::HTML_BLOCK_BEGIN );
                } else if( character == unicode::GREATER_THAN_SIGN ) { //blockquote
                    reconsume( State_e::BLOCKQUOTE_NEW_BLOCKQUOTE_BLOCK );
                } else if( character == unicode::HYPHEN_MINUS ) { //could be either unordered list, task list, or paragraph text
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN );
                } else if( character == unicode::ASTERISK ) { //could be either unordered list or paragraph text formatting
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_ASTERISK );
                } else if( character == unicode::UNDERSCORE ) {
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_UNDERSCORE );
                } else if( character == unicode::PLUS_SIGN ) {
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_PLUS_SIGN );
                } else if( character == unicode::LEFT_SQUARE_BRACKET ) {
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_LEFT_SQUARE_BRACKET );
                } else if( character == unicode::GRAVE_ACCENT && text.characters( 3 ) == UR"(```)" ) { //code block
                    text.advanceCaret( 2 );
                    setCurrentBlockFence( Fence_e::TRIPLE_GRAVE_ACCENT );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::CODE_BLOCK_DECLARATION );
                } else if( character == unicode::TILDE && text.characters( 3 ) == UR"(~~~)" ) { //code block
                    text.advanceCaret( 2 );
                    setCurrentBlockFence( Fence_e::TRIPLE_TILDE );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::CODE_BLOCK_DECLARATION );
                } else if( character == unicode::VERTICAL_LINE ) { //could be a table
                    if( _table_cache.is_fake_table ) {
                        _table_cache.is_fake_table = false; //reset flag for next block
                        pushSectionMarker(
                            queueParagraphToken( pendingBufferPosition() )
                        );
                        reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    } else {
                        _table_cache = TableCache();
                        _table_cache.fallback_state = std::make_unique<U32Text::State>( text.createMarker() );
                        appendToPendingBuffer( character );
                        modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                        setState( State_e::TABLE_BEGIN );
                    }
                } else if( unicode::ascii::isdigit( character ) ) { //could be an ordered list or paragraph text
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_NUMERAL );
                } else if( unicode::ascii::islower( character ) ) { //could be an ordered list or paragraph text
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_LOWER_ALPHA );
                } else if( unicode::ascii::isupper( character ) ) { //could be an ordered list or paragraph text
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_UPPER_ALPHA );
                } else {
                    pushSectionMarker(
                        queueParagraphToken( pendingBufferPosition() )
                    );
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                }
            } break;

            case State_e::BLOCK_BEGIN_UNDERSCORE: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::SPACE || character == unicode::TAB ) { //"_ "
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_UNDERSCORE );
                } else if( character == unicode::UNDERSCORE ) { //"__"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_UNDERSCORE_UNDERSCORE );
                } else { //"_?" i.e.: inline paragraph formatting
                    pushSectionMarker(
                        queueParagraphToken( pendingBufferPosition() )
                    );
                    queueFormattingToken( pendingBufferPosition(), unicode::UNDERSCORE );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                }
            } break;

            case State_e::BLOCK_BEGIN_UNDERSCORE_UNDERSCORE: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::UNDERSCORE ) { //"___"
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_UNDERSCORE );
                } else { //"__?" i.e.: inline paragraph formatting
                    pushSectionMarker(
                        queueParagraphToken( pendingBufferPosition() )
                    );
                    queueFormattingToken( pendingBufferPosition(), unicode::UNDERSCORE, unicode::UNDERSCORE );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                }
            } break;

            case State_e::BLOCK_BEGIN_ASTERISK: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::SPACE ) { //"* "
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_ASTERISK_SPACE );
                } else if( character == unicode::TAB ) {
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_ASTERISK_TAB );
                } else if( character == unicode::ASTERISK ) { //"**"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_ASTERISK_ASTERISK );
                } else { //"*?" i.e.: inline paragraph formatting
                    pushSectionMarker(
                        queueParagraphToken( pendingBufferPosition() )
                    );
                    queueFormattingToken( pendingBufferPosition(), unicode::ASTERISK );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                }
            } break;

            case State_e::BLOCK_BEGIN_ASTERISK_ASTERISK: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::ASTERISK ) { //"***"
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_ASTERISK );
                } else { //"**?" i.e.: inline paragraph formatting
                    pushSectionMarker(
                        queueParagraphToken( pendingBufferPosition() )
                    );
                    queueFormattingToken( pendingBufferPosition(), unicode::ASTERISK, unicode::ASTERISK );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                }
            } break;

            case State_e::BLOCK_BEGIN_ASTERISK_SPACE: { //paragraph formatting, horizontal rule or unordered list
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::SPACE ) { //"*  "
                    appendToPendingBuffer( character );
                } else if( character == unicode::ASTERISK ) { //"* *"
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_ASTERISK );
                } else { //"* ?" i.e.: unordered list
                    if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                    }
                    queueListToken( ListType_e::UL_ASTERISK, pendingBufferPosition() );
                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::BLOCK_BEGIN_ASTERISK_TAB: { //paragraph formatting or horizontal rule
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::SPACE || character == unicode::TAB ) { //"*\t "
                    appendToPendingBuffer( character );
                } else if( character == unicode::ASTERISK ) { //"* ... *"
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_ASTERISK );
                } else {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN: { //"-"
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::SPACE ) { //"- "
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE );
                } else if( character == unicode::TAB ) { //"-\t"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_TAB );
                } else if( character == unicode::HYPHEN_MINUS ) { //"--"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_HYPHEN );
                } else { //"-?"
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_HYPHEN: { //"--"
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::HYPHEN_MINUS ) { //"---"
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_HYPHEN );
                } else { //"--?"
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE: { //"- "
                if( text.reachedEnd() ) {
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                } else if( character == unicode::HYPHEN_MINUS ) { //"- -"
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_HYPHEN );
                } else if( character == unicode::LEFT_SQUARE_BRACKET ) { //"- [" could be a task list or ul with a footnote def
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET );
                } else { //"- ?" i.e.: unordered list
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_TAB: { //paragraph formatting or horizontal rule
                if( text.reachedEnd() ) {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                } else if( character == unicode::SPACE || character == unicode::TAB ) { //"-\t "
                    appendToPendingBuffer( character );
                } else if( character == unicode::HYPHEN_MINUS ) { //"- ... -"
                    appendToPendingBuffer( character );
                    setState( State_e::HORIZONTAL_RULE_ASTERISK );
                } else { //paragraph content
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET: { //"- ["
                if( text.reachedEnd() ) {
                    text.reverseCaret( 1 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET ) { //"- []"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET );
                } else if( character == unicode::SPACE ) { //"- [ "
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE );
                } else if( character == unicode::LATIN_SMALL_LETTER_X ) { //"- [x"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X );
                } else if( character == unicode::LATIN_CAPITAL_LETTER_X ) { //"- [X"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X );
                } else { //"- [?"
                    text.reverseCaret( 1 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }

            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET: { //"- []"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [] "
                    setState( State_e::BLOCK_BEGIN_TASK_LIST_UNCHECKED_ITEM );
                } else { //"- []?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE: { //"- [ "
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [ ]"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET );
                } else { //"- [ ?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET: { //"- [ ]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [ ] "
                    setState( State_e::BLOCK_BEGIN_TASK_LIST_UNCHECKED_ITEM );
                } else { //"- [ ]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X: { //"- [x"
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [x]"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET );
                } else { //"- [x?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET: { //"- [x]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [x] "
                    setState( State_e::BLOCK_BEGIN_TASK_LIST_CHECKED_ITEM );
                } else { //"- [x]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X: { //"- [X"
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [X]"
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET );
                } else { //"- [x?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET: { //"- [X]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [X] "
                    setState( State_e::BLOCK_BEGIN_TASK_LIST_CHECKED_ITEM );
                } else { //"- [X]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::BLOCK_BEGIN_TASK_LIST_CHECKED_ITEM: {
                if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                }
                queueListToken( ListType_e::UL_TASK, pendingBufferPosition() );
                queueToken( std::make_unique<ListItemTk>( true, pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                resetPendingBuffer( text.position() );
                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( consumeReturnState() );
            } break;

            case State_e::BLOCK_BEGIN_TASK_LIST_UNCHECKED_ITEM: {
                if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                }
                queueListToken( ListType_e::UL_TASK, pendingBufferPosition() );
                queueToken( std::make_unique<ListItemTk>( false, pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                resetPendingBuffer( text.position() );
                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( consumeReturnState() );
            } break;

            case State_e::BLOCK_BEGIN_UNORDERED_LIST_ITEM_HYPHEN: {
                if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                }
                queueListToken( ListType_e::UL_HYPHEN, pendingBufferPosition() );
                queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                resetPendingBuffer( text.position() );
                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( consumeReturnState() );
            } break;

            case State_e::LINE_BEGIN: {
                if( character == unicode::GREATER_THAN_SIGN ) { //blockquote
                    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeFormattingMarkers( text.position() );
                        closeLastOpenedBlock( text.position() );
                    }
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::BLOCKQUOTE_NEW_BLOCKQUOTE_BLOCK );
                } else if( character == unicode::LESS_THAN_SIGN ) { //HTML block?
                    resetPendingBuffer( text.position() );
                    clearHtmlCache( text.position() );
                    _html_cache.fallback_state = std::make_unique<U32Text::State>( text.createMarker() );
                    pushSectionMarker(
                        queueToken( std::make_unique<HtmlBlockTk>( text.position() ) )
                    );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                    setState( State_e::HTML_BLOCK_BEGIN );
                } else if( character == unicode::ASTERISK ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_ASTERISK );
                } else if( character == unicode::PLUS_SIGN ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_PLUS_SIGN );
                } else if( unicode::ascii::isfeed( character ) ) { //i.e.: block separation "\n\n"
                    closeFormattingMarkers( text.position() );
                    closeLastOpenedBlock( text.position() );
                    setState( State_e::NEWLINE_AFTER_EMPTY_LINE );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    if( incrementBlockLineCount() == 3 ) {
                        setSETextFlag( false );
                    };

                    if( character == unicode::EQUALS_SIGN ) {
                        resetPendingBuffer( text.position() );
                        appendToPendingBuffer( character );
                        pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                        setState( State_e::LINE_BEGIN_EQUAL_SIGN );
                    } else if( character == unicode::HYPHEN_MINUS ) {
                        resetPendingBuffer( text.position() );
                        appendToPendingBuffer( character );
                        pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                        setState( State_e::LINE_BEGIN_HYPHEN );
                    } else if( character == unicode::LEFT_SQUARE_BRACKET ) {
                        resetPendingBuffer( text.position() );
                        appendToPendingBuffer( character );
                        pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                        setState( State_e::LINE_BEGIN_LEFT_SQUARE_BRACKET );
                    } else if( character == unicode::COLON ) {
                        resetPendingBuffer( text.position() );
                        appendToPendingBuffer( character );
                        pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                        setState( State_e::LINE_BEGIN_COLON );
                    } else if( unicode::ascii::islower( character ) ) {
                        resetPendingBuffer( text.position() );
                        appendToPendingBuffer( character );
                        pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                        setState( State_e::LINE_BEGIN_LOWER_ALPHA );
                    } else if( unicode::ascii::isupper( character ) ) {
                        resetPendingBuffer( text.position() );
                        appendToPendingBuffer( character );
                        pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                        setState( State_e::LINE_BEGIN_UPPER_ALPHA );
                    } else if( unicode::ascii::isdigit( character ) ) {
                        resetPendingBuffer( text.position() );
                        appendToPendingBuffer( character );
                        pushReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                        setState( State_e::LINE_BEGIN_NUMERAL );
                    } else {
                        reconsume( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                    }
                } //else: ignore prefixing space & tab characters
            } break;

            case State_e::LINE_BEGIN_ASTERISK: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::SPACE ) { //i.e.: list item "* "
                    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeLastOpenedBlock( pendingBufferPosition() );
                    }
                    if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                    }
                    queueListToken( ListType_e::UL_ASTERISK, pendingBufferPosition() );
                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( consumeReturnState() );
                } else if( character == unicode::ASTERISK ) { //i.e.: formatting "**"
                    appendToPendingBuffer( character );
                    setState( consumeReturnState() );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_EQUAL_SIGN: {
                if( !text.reachedEnd() && character == unicode::EQUALS_SIGN ) { //"=="
                    appendToPendingBuffer( character );
                    setState( State_e::LINE_BEGIN_EQUAL_SIGN_EQUAL_SIGN );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_EQUAL_SIGN_EQUAL_SIGN: {
                if( !text.reachedEnd() && character == unicode::EQUALS_SIGN && isPossibleStateForSETextHeading() ) { //"==="
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::HEADING_BLOCK_SETEXT_FMT_EQUAL_SIGN );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_PLUS_SIGN: {
                if( !text.reachedEnd() && character == unicode::SPACE ) { //i.e.: list item "+ "
                    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeLastOpenedBlock( pendingBufferPosition() );
                    }
                    if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                    }
                    queueListToken( ListType_e::UL_PLUS_SIGN, pendingBufferPosition() );
                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( consumeReturnState() );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::HYPHEN_MINUS ) { //"--"
                    appendToPendingBuffer( character );
                    setState( State_e::LINE_BEGIN_HYPHEN_HYPHEN );
                } else if( character == unicode::SPACE ) { //i.e.: list item "- "
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_HYPHEN: {
                if( !text.reachedEnd() && character == unicode::HYPHEN_MINUS && isPossibleStateForSETextHeading() ) { //"---"
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::HEADING_BLOCK_SETEXT_FMT_HYPHEN );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE: { //"- "
                if( !text.reachedEnd() && character == unicode::LEFT_SQUARE_BRACKET ) {
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET );
                } else {
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET: { //"- ["
                if( text.reachedEnd() ) {
                    text.reverseCaret( 1 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                } else if( character == unicode::SPACE ) { //"- [ "
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET ) { //"- []"
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET );
                } else if( character == unicode::LATIN_CAPITAL_LETTER_X ) { //"- [X"
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X );
                } else if( character == unicode::LATIN_SMALL_LETTER_X ) { //"- [x"
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X );
                } else { //"- [?"
                    text.reverseCaret( 1 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET: { //"- []"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [] "
                    setState( State_e::LINE_BEGIN_TASK_LIST_UNCHECKED_ITEM );
                } else { //"- []?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE: { //"- [ "
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [ ]"
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET );
                } else { //"- [ ?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET: { //"- [ ]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [ ] "
                    setState( State_e::LINE_BEGIN_TASK_LIST_UNCHECKED_ITEM );
                } else { //"- [ ]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X: { //"- [x"
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [x]"
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET );
                } else { //"- [x?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET: { //"- [x]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [x] "
                    setState( State_e::LINE_BEGIN_TASK_LIST_CHECKED_ITEM );
                } else { //"- [x]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X: { //"- [X"
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [x]"
                    setState( State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET );
                } else { //"- [x?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET: { //"- [X]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [X] "
                    setState( State_e::LINE_BEGIN_TASK_LIST_CHECKED_ITEM );
                } else { //"- [X]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::LINE_BEGIN_TASK_LIST_CHECKED_ITEM: {
                if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                    closeLastOpenedBlock( pendingBufferPosition() );
                }
                if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                }
                queueListToken( ListType_e::UL_TASK, pendingBufferPosition() );
                queueToken( std::make_unique<ListItemTk>( true, pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                resetPendingBuffer( text.position() );
                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( consumeReturnState() );
            } break;

            case State_e::LINE_BEGIN_TASK_LIST_UNCHECKED_ITEM: {
                if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                    closeLastOpenedBlock( pendingBufferPosition() );
                }
                if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                }
                queueListToken( ListType_e::UL_TASK, pendingBufferPosition() );
                queueToken( std::make_unique<ListItemTk>( false, pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                resetPendingBuffer( text.position() );
                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( consumeReturnState() );
            } break;

            case State_e::LINE_BEGIN_UNORDERED_LIST_ITEM_HYPHEN: {
                if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                    closeLastOpenedBlock( pendingBufferPosition() );
                }
                if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                }
                queueListToken( ListType_e::UL_HYPHEN, pendingBufferPosition() );
                queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                resetPendingBuffer( text.position() );
                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( consumeReturnState() );
            } break;

            case State_e::LINE_BEGIN_LOWER_ALPHA: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( unicode::ascii::islower( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::FULL_STOP ) { //"[a-z]*\."
                    appendToPendingBuffer( character );
                    setState( State_e::LINE_BEGIN_LOWER_ALPHA_FULL_STOP );
                } else { //not a list
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_LOWER_ALPHA_FULL_STOP: {
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"[a-z]*\. "
                    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeLastOpenedBlock( pendingBufferPosition() );
                    }
                    if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                    }
                    queueListToken( ListType_e::OL_ALPHA_LOWER, pendingBufferPosition() );
                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( consumeReturnState() );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_UPPER_ALPHA: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( unicode::ascii::isupper( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::FULL_STOP ) { //"[A-Z]*\."
                    appendToPendingBuffer( character );
                    setState( State_e::LINE_BEGIN_UPPER_ALPHA_FULL_STOP );
                } else { //not a list
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_UPPER_ALPHA_FULL_STOP: {
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"[A-Z]*\. "
                    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeLastOpenedBlock( pendingBufferPosition() );
                    }
                    if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                    }
                    queueListToken( ListType_e::OL_ALPHA_UPPER, pendingBufferPosition() );
                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    pushSectionMarker();
                    resetPendingBuffer( text.position() );
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( consumeReturnState() );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_NUMERAL: { //decide if it's an ordered list or normal content
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( unicode::ascii::isdigit( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::FULL_STOP ) { //"[\d]*\."
                    appendToPendingBuffer( character );
                    setState( State_e::LINE_BEGIN_NUMERAL_FULL_STOP );
                } else { //not a list
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_NUMERAL_FULL_STOP: {
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"[\d]*\. "
                    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeLastOpenedBlock( pendingBufferPosition() );
                    }
                    if( peekLastOpenContainerBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                    }
                    queueListToken( ListType_e::OL_NUMERIC, pendingBufferPosition() );
                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( consumeReturnState() );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_LEFT_SQUARE_BRACKET: {
                if( !text.reachedEnd() && character == unicode::CIRCUMFLEX_ACCENT ) { //"[^"
                    resetPendingBuffer( pendingBufferPosition() );
                    setState( State_e::LINE_BEGIN_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT );
                } else {
                    text.reverseCaret( 1 );
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT: { //"[^"
                if( !text.reachedEnd() && !unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::LINE_BEGIN_FOOTNOTE_ID );
                } else { //try as hyperlink
                    text.reverseCaret( 2 ); //go back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_FOOTNOTE_ID: { //"[^..."
                if( text.reachedEnd() ) {
                    text.reverseCaret( _pending.buffer.size() + 2 ); //back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET && !_pending.buffer.empty() ) { //"[^...]"
                    setState( State_e::LINE_BEGIN_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET );
                } else if( !unicode::ascii::isfeed( character ) ) {
                    appendToPendingBuffer( character );
                } else { //"[^...\n"
                    text.reverseCaret( _pending.buffer.size() + 2 ); //back to '['
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( unicode::LEFT_SQUARE_BRACKET );
                    setState( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET: { //"[^...]"
                if( !text.reachedEnd() && character == unicode::COLON ) { //"[^...]:"
                    setState( State_e::LINE_BEGIN_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET_COLON );
                } else { //"[^...]?"
                    text.reverseCaret( _pending.buffer.size() + 3 ); //back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET_COLON: { //"[^...]:"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"[^...]: "
                    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                        closeLastOpenedBlock( pendingBufferPosition() );
                    }
                    if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
                    }
                    queueListToken( ListType_e::FOOTNOTE_DEFS, pendingBufferPosition() );
                    queueToken( std::make_unique<ListItemTk>( pendingBufferToStr(), pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    pushSectionMarker();
                    resetPendingBuffer( text.position() );
                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( consumeReturnState() );
                } else { //"[^...]:?"
                    text.reverseCaret( _pending.buffer.size() + 4 ); //back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_COLON: {
                if( !text.reachedEnd() && _pending.block_lines > 1 && ( character == unicode::SPACE || character == unicode::TAB ) ) { //": "
                    appendToPendingBuffer( character );
                    setState( State_e::LINE_BEGIN_COLON_SPACE );
                } else { //not a definition (":?")
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::LINE_BEGIN_COLON_SPACE: { //": " only on line 2+ of a block
               if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                   logError( pendingBufferPosition(), ErrorCode::DEFINITION_LIST_EMPTY_DEFINITION );
                   reconsume( consumeReturnState() );
               } else if( character == unicode::SPACE || character == unicode::TAB ) {
                   appendToPendingBuffer( character );
               } else {
                   auto & tk_ptr = getQueuedToken( peekLastSectionMarker() );

                   if( tk_ptr->type() == Type_e::PARAGRAPH ) {
                       tk_ptr = std::make_unique<DLTitleTk>( tk_ptr->position() );
                       queueToken( std::make_unique<BlockEndTk>( Type_e::DEFINITION_LIST_DT, pendingBufferPosition() ) );
                       popOpenBlockStack();
                       queueToken( std::make_unique<DLDefinitionTk>( pendingBufferPosition() ) );
                       pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                   } else if( peekLastOpenBlockType() == Type_e::DEFINITION_LIST_DD ) {
                       closeLastOpenedBlock( pendingBufferPosition() );
                       queueToken( std::make_unique<DLDefinitionTk>( pendingBufferPosition() ) );
                       pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                   }

                   resetPendingBuffer( text.position() );
                   reconsume( consumeReturnState() );
               }
            } break;

            case State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK: {
                if( text.reachedEnd() ) {
                    if( !_pending.buffer.empty() && requiresSpaceSeparatorToken() ) {
                        queueToken( std::make_unique<CharacterTk>( unicode::SPACE, text.position() ) );
                    }
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                } else if( !_pending.buffer.empty() || ( character != unicode::SPACE && character != unicode::TAB ) ) {
                    if( requiresSpaceSeparatorToken() ) {
                        queueToken( std::make_unique<CharacterTk>( unicode::SPACE, text.position() ) );
                    }
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                } //else: ignore prefixing whitespace
            } break;

            case State_e::NEWLINE_CONSUME_PREFIXES_BEGIN: {
                if( hasOpenContainerBlock() ) {
                    _curr_open_container_i = 0; //readying for iteration of open container blocks
                    _curr_blockquote_lvl   = 0;
                    pushReturnState( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );
                } else {
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::NEWLINE_CONSUME_WHITESPACE );
                }
            } break;

            case State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE: {
                resetPendingBuffer( text.position() );

                if( unicode::ascii::isfeed( character ) ) { //i.e.: solely 0..n container block prefixes but no line content
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_END );

                } else {
                    if( _curr_open_container_i < openContainerBlockCount() ) {
                        switch( _open_container[ _curr_open_container_i ].type ) {
                            case Type_e::BLOCKQUOTE: { reconsume( State_e::NEWLINE_CONSUME_BLOCKQUOTE_PREFIX ); } break;
                            case Type_e::LIST      : { reconsume( State_e::NEWLINE_CONSUME_LIST_CONTAINER );    } break;
                            case Type_e::LIST_ITEM : { reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX );       } break;
                            default                : { reconsume( State_e::NEWLINE_CONSUME_WHITESPACE );        } break;
                        }

                    } else {
                        if( peekLastOpenContainerBlockType() == Type_e::LIST_ITEM &&
                            _list_spacing                    != ListSpacing_e::DEFAULT_TIGHT )
                        {
                            loosenLastListBlockSpacing();
                        }

                        reconsume( consumeReturnState() );
                    }
                }
            } break;

            case State_e::NEWLINE_CONSUME_PREFIXES_END: {
                closeFormattingMarkers( text.position() );

                while( hasOpenBlock() && specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
                    closeLastOpenedBlock( text.position() );
                }

                if( _curr_blockquote_lvl < _actual_blockquote_lvl ) {
                    while( hasOpenContainerBlock() && _actual_blockquote_lvl > _curr_blockquote_lvl ) {
                        if( peekLastOpenBlockType() == Type_e::BLOCKQUOTE ) {
                            --_actual_blockquote_lvl;
                        }

                        queueToken( std::make_unique<BlockEndTk>( peekLastOpenBlockType(), text.position() ) );
                        popOpenBlockStack();
                    }

                    modifyReturnState( State_e::BLOCK_BEGIN );
                    setState( consumeReturnState() );

                } else if( peekLastOpenBlockType() == Type_e::TABLE && _pending.block_lines < 3 ) { //edge-case
                    setState( State_e::TABLE_ABORT );

                } else {
                    if( peekLastOpenBlockType() == Type_e::TABLE ) { //edge-cases
                        closeLastOpenedBlock( text.position() );
                    }

                    if( _empty_lines > 1 ) {
                        closeFormattingMarkers( text.position() );

                        while( hasOpenContainerBlock() && peekLastOpenBlockType() != Type_e::BLOCKQUOTE ) {
                            closeLastOpenedBlock( text.position() );
                        }
                    }

                    setState( State_e::NEWLINE_AFTER_EMPTY_LINE );
                }
            } break;

            case State_e::NEWLINE_CONSUME_WHITESPACE: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_END );
                } else if( character == unicode::SPACE ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::TAB ) {
                    appendToPendingBuffer( FOUR_SPACE_CHARS );
                } else {
                    ++_curr_open_container_i;
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );
                }
            } break;

            case State_e::NEWLINE_CONSUME_BLOCKQUOTE_PREFIX: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_END );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    const auto prefix_col = peekOpenContainerBlock( _curr_open_container_i ).column();

                    if( text.position().col != prefix_col ) {
                        logError( text.position(), ErrorCode::BLOCK_MISALIGNMENT );
                    }

                    ++_curr_blockquote_lvl;
                    ++_curr_open_container_i;
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                } else if( character != unicode::TAB && character != unicode::SPACE ) {
                    //trailing newline content continues (like a paragraph)
                    if( _empty_lines > 0 && _curr_blockquote_lvl < _actual_blockquote_lvl ) {
                        closeFormattingMarkers( text.position() );

                        while( hasOpenBlock() && ( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) ) {
                            closeLastOpenedBlock( text.position() );
                        }

                        while( hasOpenContainerBlock() && _actual_blockquote_lvl > _curr_blockquote_lvl ) {
                            if( peekLastOpenBlockType() == Type_e::BLOCKQUOTE ) {
                                --_actual_blockquote_lvl;
                            }

                            queueToken( std::make_unique<BlockEndTk>( peekLastOpenBlockType(), text.position() ) );
                            popOpenBlockStack();
                        }

                        modifyReturnState( State_e::BLOCKQUOTE_BLOCK_BEGIN );
                    } else {
                        ++_curr_blockquote_lvl;
                        ++_curr_open_container_i;
                    }

                    reconsume( consumeReturnState() );
                } //else: ignore space and tab character
            } break;

            case State_e::NEWLINE_CONSUME_LIST_CONTAINER: {
                //ensures that "NEWLINE_CONSUME_LIST_PREFIX" never starts beyond the expected col position
                if( text.position().col >= peekOpenContainerBlock( _curr_open_container_i ).column() || character != unicode::SPACE ) {
                    ++_curr_open_container_i;
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );
                } //else: ignore until container position is reached
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX: {
                if( unicode::ascii::isdigit( character ) ) {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_NUMERIC );
                } else if( unicode::ascii::islower( character ) ) {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER );
                } else if( unicode::ascii::isupper( character ) ) {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER );
                } else if( character == unicode::HYPHEN_MINUS ) {
                    appendToPendingBuffer( character );
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN );
                } else if( character == unicode::PLUS_SIGN ) {
                    appendToPendingBuffer( character );
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_PLUS_SIGN );
                } else if( character == unicode::ASTERISK ) {
                    appendToPendingBuffer( character );
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_ASTERISK );
                } else if( character == unicode::LEFT_SQUARE_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_LEFT_SQUARE_BRACKET );
                } else {
                    ++_curr_open_container_i;
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_NUMERIC: {
                if( unicode::ascii::isdigit( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::FULL_STOP ) {
                    appendToPendingBuffer( character );
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_NUMERIC_FULL_STOP );
                } else { //note: pending buffer size always >= 1
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_NUMERIC_FULL_STOP: {
                if( character == unicode::SPACE ) {
                    if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                        closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                    }

                    if( peekLastOpenListType() != ListType_e::OL_NUMERIC ) {
                        std::stringstream ss;
                        ss << ListType_e::OL_NUMERIC << " in " << peekLastOpenListType();
                        logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                    }

                    closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                    if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                    }

                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                    ++_curr_open_container_i;

                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );

                } else {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER: {
                if( unicode::ascii::islower( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::FULL_STOP ) {
                    appendToPendingBuffer( character );
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER_FULL_STOP );
                } else { //note: pending buffer size always >= 1
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_LOWER_FULL_STOP: {
                if( character == unicode::SPACE ) {
                    if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                        closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                    }

                    if( peekLastOpenListType() != ListType_e::OL_ALPHA_LOWER ) {
                        std::stringstream ss;
                        ss << ListType_e::OL_ALPHA_LOWER << " in " << peekLastOpenListType();
                        logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                    }

                    closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                    if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                    }

                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                    ++_curr_open_container_i;

                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );

                } else {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER: {
                if( unicode::ascii::isupper( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::FULL_STOP ) {
                    appendToPendingBuffer( character );
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER_FULL_STOP );
                } else {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_ALPHA_UPPER_FULL_STOP: {
                if( character == unicode::SPACE ) {
                    if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                        closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                    }

                    if( peekLastOpenListType() != ListType_e::OL_ALPHA_UPPER ) {
                        std::stringstream ss;
                        ss << ListType_e::OL_ALPHA_UPPER << " in " << peekLastOpenListType();
                        logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                    }

                    closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                    if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                    }

                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                    ++_curr_open_container_i;

                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );

                } else {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_ASTERISK: {
                if( character == unicode::SPACE ) {
                    if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                        closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                    }

                    if( peekLastOpenListType() != ListType_e::UL_ASTERISK ) {
                        std::stringstream ss;
                        ss << ListType_e::UL_ASTERISK << " in " << peekLastOpenListType();
                        logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                    }

                    closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                    if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                    }

                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                    ++_curr_open_container_i;

                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );

                } else {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_PLUS_SIGN: {
                if( character == unicode::SPACE ) {
                    if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                        closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                    }

                    if( peekLastOpenListType() != ListType_e::UL_PLUS_SIGN ) {
                        std::stringstream ss;
                        ss << ListType_e::UL_PLUS_SIGN << " in " << peekLastOpenListType();
                        logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                    }

                    closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                    if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                        loosenLastListBlockSpacing();
                    }

                    queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                    ++_curr_open_container_i;

                    modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                    setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );

                } else {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN: {
                if( !text.reachedEnd() && character == unicode::SPACE ) {
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE );
                } else {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE: {
                if( !text.reachedEnd() && character == unicode::LEFT_SQUARE_BRACKET ) { //"- ["
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET );
                } else { //"- ?"
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET: { //"- ["
                if( text.reachedEnd() ) {
                    text.reverseCaret( 1 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET ) { //"- []"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET );
                } else if( character == unicode::LATIN_CAPITAL_LETTER_X ) { //"- [X"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X );
                } else if( character == unicode::LATIN_SMALL_LETTER_X ) { //"- [x"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X );
                } else if( character == unicode::SPACE ) { //"- [ "
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE );
                } else { //"- [?"
                    text.reverseCaret( 1 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET: { //"- []"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [] "
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_UNCHECKED_ITEM );
                } else { //"- []?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE: { //"- [ "
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [ ]"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET );
                } else { //"- [ ]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_SPACE_RIGHT_SQUARE_BRACKET: { //"- [ ]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [ ] "
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_UNCHECKED_ITEM );
                } else { //"- [ ]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X: { //"- [x"
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [x]"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET );
                } else { //"- [x?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_LOWERCASE_X_RIGHT_SQUARE_BRACKET: { //"- [x]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [ ] "
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_CHECKED_ITEM );
                } else { //"- [ ]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X: { //"- [X"
                if( !text.reachedEnd() && character == unicode::RIGHT_SQUARE_BRACKET ) { //"- [X]"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET );
                } else { //"- [X?"
                    text.reverseCaret( 2 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_HYPHEN_SPACE_LEFT_SQUARE_BRACKET_UPPERCASE_X_RIGHT_SQUARE_BRACKET: { //"- [X]"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"- [ ] "
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_CHECKED_ITEM );
                } else { //"- [ ]?"
                    text.reverseCaret( 3 ); //back to '['
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_LEFT_SQUARE_BRACKET: { //"["
                if( !text.reachedEnd() && character == unicode::CIRCUMFLEX_ACCENT ) { //"[^"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT );
                } else { //"[?"
                    text.reverseCaret( 1 );
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT: { //"[^"
                if( !text.reachedEnd() && !unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID );
                } else {
                    text.reverseCaret( 2 ); //go back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID: { //"[^..."
                if( text.reachedEnd() ) {
                    text.reverseCaret( _pending.buffer.size() + 2 ); //back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET && !_pending.buffer.empty() ) { //"[^...]"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET );
                } else if( !unicode::ascii::isfeed( character ) ) {
                    appendToPendingBuffer( character );
                } else { //"[^...\n"
                    text.reverseCaret( _pending.buffer.size() + 2 ); //back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET: { //"[^...]"
                if( !text.reachedEnd() && character == unicode::COLON ) { //"[^...]:"
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET_COLON );
                } else { //"[^...]?"
                    text.reverseCaret( _pending.buffer.size() + 3 ); //back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_ID_RIGHT_SQUARE_BRACKET_COLON: { //"[^...]:"
                if( !text.reachedEnd() && character == unicode::SPACE ) { //"[^...]: "
                    setState( State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_DEFINITION_ITEM );
                } else { //"[^...]:?"
                    text.reverseCaret( _pending.buffer.size() + 4 ); //back to '['
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_CHECKED_ITEM: { //"- [x]" or "- [X]"
                if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                    closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                }

                if( peekLastOpenListType() != ListType_e::UL_TASK ) {
                    std::stringstream ss;
                    ss << ListType_e::UL_TASK << " in " << peekLastOpenListType();
                    logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                }

                closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                }

                queueToken( std::make_unique<ListItemTk>( true, pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                ++_curr_open_container_i;

                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_TASK_LIST_UNCHECKED_ITEM: { //"- []" or "- [ ]"
                if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                    closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                }

                if( peekLastOpenListType() != ListType_e::UL_TASK ) {
                    std::stringstream ss;
                    ss << ListType_e::UL_TASK << " in " << peekLastOpenListType();
                    logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                }

                closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                }

                queueToken( std::make_unique<ListItemTk>( false, pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                ++_curr_open_container_i;

                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_UNORDERED_LIST_ITEM_HYPHEN: { //"- ?"
                if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                    closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                }

                if( peekLastOpenListType() != ListType_e::UL_HYPHEN ) {
                    std::stringstream ss;
                    ss << ListType_e::UL_HYPHEN << " in " << peekLastOpenListType();
                    logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                }

                closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                }

                queueToken( std::make_unique<ListItemTk>( pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                ++_curr_open_container_i;

                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_FOOTNOTE_DEFINITION_ITEM: {
                if( pendingBufferPosition().col < peekLastOpenContainerBlock().column() ) { //back to a parent list
                    closeOpenContainerBlocks( pendingBufferPosition(), false, pendingBufferPosition() );
                }

                if( peekLastOpenListType() != ListType_e::FOOTNOTE_DEFS ) {
                    std::stringstream ss;
                    ss << ListType_e::FOOTNOTE_DEFS << " in " << peekLastOpenListType();
                    logError( pendingBufferPosition(), ErrorCode::MISMATCHED_LIST_PREFIX, ss.str() );
                }

                closeOpenContainerBlocks( _curr_open_container_i, pendingBufferPosition() );

                if( _list_spacing != ListSpacing_e::DEFAULT_TIGHT ) {
                    loosenLastListBlockSpacing();
                }

                queueToken( std::make_unique<ListItemTk>( pendingBufferToStr(), pendingBufferPosition() ) );
                pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );

                ++_curr_open_container_i;

                modifyReturnState( State_e::BLOCK_BEGIN_WHITESPACE );
                reconsume( State_e::NEWLINE_CONSUME_PREFIXES_CONTINUE );

                setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT );
            } break;

            case State_e::NEWLINE_CONSUME_LIST_PREFIX_ABORT: {
                if( _empty_lines == 0 ) {
                    modifyReturnState( State_e::LINE_BEGIN_INSIDE_PARAGRAPH_BLOCK );
                } else { //_empty_lines > 0
                    // |    | <container>
                    // |    |   <content>
                    // |
                    // | | <content>
                    // ^<-->^ : anything starting in that range and does not have the same prefix type as
                    //          the previous container block (e.g: list item) is treated as a separate
                    //          non-nested block and, thus, the previous container + its content gets closed.
                    if( pendingBufferPosition().col <= peekLastOpenContainerBlock().column() ) {
                        closeOpenContainerBlocks( pendingBufferPosition(), true, pendingBufferPosition() );
                    }
                    text.reverseCaret( _pending.buffer.size() );
                }
                reconsume( consumeReturnState() );
            } break;

            case State_e::NEWLINE_AFTER_EMPTY_LINE: {
                if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing == ListSpacing_e::DEFAULT_TIGHT ) {
                    setListSpacingFlag( ListSpacing_e::MAYBE_LOOSE );
                }

                ++_empty_lines;
                modifyReturnState( State_e::BLOCK_BEGIN );
                reconsume( consumeReturnState() );
            } break;

            case State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK: {
                pushSectionMarker(
                    queueParagraphToken( pendingBufferPosition() )
                );
                reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
            } break;

            case State_e::PARAGRAPH_BLOCK_LINE_CONTENT: {
                flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY );

                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::SPACE ) {
                    pushSectionMarker(
                        queueToken( std::make_unique<CharacterTk>( character, text.position() ) )
                    );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::INLINE_CONTENT_SINGLE_SPACE );
                } else if( character == unicode::LEFT_CURLY_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::INLINE_CONTENT_LEFT_CURLY_BRACKET );
                } else if( character == unicode::EXCLAMATION_MARK ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::INLINE_CONTENT_BANG );
                } else if( character == unicode::LEFT_SQUARE_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::INLINE_CONTENT_LEFT_SQUARE_BRACKET );
                } else if( isFormattingChar( character ) ) {
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    reconsume( State_e::INLINE_CONTENT_FORMATTING );
                } else if( unicode::ascii::isfeed( character ) ) {
                    modifyReturnState( State_e::LINE_BEGIN );
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_BEGIN );
                } else if( character == unicode::REVERSE_SOLIDUS ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::ESCAPED_CHARACTER_TO_TOKEN_QUEUE );
                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::INLINE_CONTENT_SINGLE_SPACE: {
                if( character == unicode::SPACE ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    setState( State_e::INLINE_CONTENT_LINE_BREAK );
                } else { //other characters
                    popSectionMarker();
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::INLINE_CONTENT_LINE_BREAK: {
                if( unicode::ascii::isfeed( character ) ) {
                    clearPendingTokensFromLastSectionMarkerPosition();
                    queueToken( std::make_unique<LineBreakTk>( text.position() - TextPos( 0, 2 ) ) ); //(- "  ")
                    setState( consumeReturnState() );
                } else if( character == unicode::SPACE ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                } else { //other characters
                    popSectionMarker();
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::INLINE_CONTENT_BANG: {
                if( character == unicode::LEFT_SQUARE_BRACKET ) {
                    resetPendingBuffer( pendingBufferPosition() );
                    setState( State_e::IMAGE_ALT_TEXT );
                } else {
                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY ); //"!"
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::INLINE_CONTENT_LEFT_SQUARE_BRACKET: {
                if( text.reachedEnd() ) {
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_SQUARE_BRACKET, ( text.position() - TextPos( 0, 1 ) ) ) );
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET ) {
                    logError( pendingBufferPosition(), ErrorCode::VOID_SQUARE_BRACKET_DEFINITION );
                    appendToPendingBuffer( character );
                    flushPendingBufferToQueue( CharacterProcessing::NONE ); //"[]"
                    setState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                } else if( character == unicode::CIRCUMFLEX_ACCENT ) { //"[^"
                    resetPendingBuffer( pendingBufferPosition() );
                    setState( State_e::INLINE_CONTENT_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT );
                } else {
                    resetPendingBuffer( pendingBufferPosition() );  //keeps the position on '[' of the "[..."
                    reconsume( State_e::HYPERLINK_TEXT );
                }
            } break;

            case State_e::INLINE_CONTENT_LEFT_SQUARE_BRACKET_CIRCUMFLEX_ACCENT: { //"[^"
                if( !text.reachedEnd() && ( unicode::ascii::isalnum( character ) || character == unicode::UNDERSCORE ) ) {
                    reconsume( State_e::FOOTNOTE_REF_ID );
                } else { //try as hyperlink
                    text.reverseCaret( 1 );
                    resetPendingBuffer( pendingBufferPosition() );  //keeps the position on '[' of the "[..."
                    reconsume( State_e::HYPERLINK_TEXT );
                }
            } break;

            case State_e::INLINE_CONTENT_LEFT_CURLY_BRACKET: {
                if( text.reachedEnd() ) {
                    flushPendingBufferToQueue( CharacterProcessing::NONE ); //i.e.: "{"
                    reconsume( consumeReturnState() );
                } else if( character == unicode::LEFT_CURLY_BRACKET ) {
                    setState( State_e::INLINE_CONTENT_DOUBLE_LEFT_CURLY_BRACKET );
                } else if( character == unicode::NUMBER_SIGN ) { //"{#"
                    appendToPendingBuffer( character );
                    setState( State_e::INLINE_CONTENT_LEFT_CURLY_BRACKET_NUMBER_SIGN );
                } else {
                    popSectionMarker();
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::INLINE_CONTENT_LEFT_CURLY_BRACKET_NUMBER_SIGN: { //"{#"
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_BLOCK_ID );
                    text.reverseCaret( 2 ); //back to '{'
                    resetPendingBuffer( text.position() );
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_CURLY_BRACKET, text.position() ) );
                    setState( consumeReturnState() );
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    reconsume( consumeReturnState() );
                } else if( unicode::ascii::isalnum( character ) ||
                           character == unicode::HYPHEN_MINUS   ||
                           character == unicode::UNDERSCORE )
                {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_ID_NAME );
                } else {
                    logError( text.position(), ErrorCode::INVALID_CHARACTER_IN_BLOCK_ID_NAME );
                    text.reverseCaret( 2 ); //back to '{'
                    resetPendingBuffer( text.position() );
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_CURLY_BRACKET, text.position() ) );
                    setState( consumeReturnState() );
                }
            } break;

            case State_e::INLINE_CONTENT_DOUBLE_LEFT_CURLY_BRACKET: {
                clearPendingTokensFromLastSectionMarkerPosition(); //i.e.: "{{"
                return specs::Context::NATIVE; //EARLY RETURN
            } break;

            case State_e::INLINE_CONTENT_FORMATTING: {
                const auto [ next_character, reached_end ] = text.character( 1 );
                const auto is_double_fmt                   = ( !reached_end && next_character == character );

                if( is_double_fmt ) { //possible double formatting character
                    const auto n = queueFormattingToken( text.position(), character, next_character );

                    if( n > 1 ) {
                        text.advanceCaret( 1 );
                    }

                    if( character == unicode::GRAVE_ACCENT ) {
                        setState( State_e::INLINE_CONTENT_INLINE_CODE_DOUBLE_GRAVE_ACCENT );
                    } else {
                        setState( consumeReturnState() );
                    }

                } else {
                    const auto n = queueFormattingToken( text.position(), character );

                    if( n == 0 ) { //i.e.: not a formatting character when as a lone single character
                        queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    }

                    if( character == unicode::GRAVE_ACCENT ) {
                        setState( State_e::INLINE_CONTENT_INLINE_CODE_SINGLE_GRAVE_ACCENT );
                    } else {
                        setState( consumeReturnState() );
                    }
                }
            } break;

            case State_e::INLINE_CONTENT_INLINE_CODE_SINGLE_GRAVE_ACCENT: {
                if( text.reachedEnd() ) {
                    queueFormattingToken( text.position(), unicode::GRAVE_ACCENT );
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::GRAVE_ACCENT ) {
                    queueFormattingToken( text.position(), unicode::GRAVE_ACCENT );
                    setState( consumeReturnState() );
                } else if( character == unicode::AMPERSAND ) {
                    queueToken( std::make_unique<CharacterTk>( U"&amp;", text.position() ) );
                } else if( character == unicode::LESS_THAN_SIGN ) {
                    queueToken( std::make_unique<CharacterTk>( U"&lt;", text.position() ) );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    queueToken( std::make_unique<CharacterTk>( U"&gt;", text.position() ) );
                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::INLINE_CONTENT_INLINE_CODE_DOUBLE_GRAVE_ACCENT: {
                if( text.reachedEnd() ) {
                    queueFormattingToken( text.position(), unicode::GRAVE_ACCENT, unicode::GRAVE_ACCENT );
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::GRAVE_ACCENT && text.characters( 2 ) == UR"(``)" ) {
                    queueFormattingToken( text.position(), unicode::GRAVE_ACCENT, unicode::GRAVE_ACCENT );
                    text.advanceCaret( 1 ); //skip the second '`'
                    setState( consumeReturnState() );
                } else if( character == unicode::AMPERSAND ) {
                    queueToken( std::make_unique<CharacterTk>( U"&amp;", text.position() ) );
                } else if( character == unicode::LESS_THAN_SIGN ) {
                    queueToken( std::make_unique<CharacterTk>( U"&lt;", text.position() ) );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    queueToken( std::make_unique<CharacterTk>( U"&gt;", text.position() ) );
                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::ESCAPED_CHARACTER_TO_TOKEN_QUEUE: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else {
                    if( Markdown::isEscapable( character ) ) {
                        popLastQueuedToken(); //'\'
                        queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    } else if( character == unicode::AMPERSAND ) {
                        popLastQueuedToken(); //'\'
                        queueToken( std::make_unique<CharacterTk>( U"&amp;", text.position() ) );
                    } else if( character == unicode::LESS_THAN_SIGN ) {
                        popLastQueuedToken(); //'\'
                        queueToken( std::make_unique<CharacterTk>( U"&lt;", text.position() ) );
                    } else if( character == unicode::GREATER_THAN_SIGN ) {
                        popLastQueuedToken(); //'\'
                        queueToken( std::make_unique<CharacterTk>( U"&gt;", text.position() ) );
                    } else {
                        queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    }

                    setState( consumeReturnState() );
                }
            } break;

            case State_e::ESCAPED_CHARACTER_TO_PENDING_CHAR_BUFFER: {
                //Note: escaped characters are processed when `flushPendingBufferToQueue(..)` is called
                appendToPendingBuffer( character );
                setState( consumeReturnState() );
            } break;

            case State_e::HORIZONTAL_RULE_ASTERISK: {
                if( text.reachedEnd() ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::ASTERISK ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    reconsume( State_e::HORIZONTAL_RULE_WHITESPACE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    setState( State_e::AFTER_BLOCK );
                } else {
                    text.reverseCaret( _pending.buffer.size() );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::HORIZONTAL_RULE_HYPHEN: {
                if( text.reachedEnd() ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::HYPHEN_MINUS ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    reconsume( State_e::HORIZONTAL_RULE_WHITESPACE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    setState( State_e::AFTER_BLOCK );
                } else {
                    text.reverseCaret( _pending.buffer.size() );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::HORIZONTAL_RULE_UNDERSCORE: {
                if( text.reachedEnd() ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::UNDERSCORE ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    reconsume( State_e::HORIZONTAL_RULE_WHITESPACE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    setState( State_e::AFTER_BLOCK );
                } else {
                    text.reverseCaret( _pending.buffer.size() );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::HORIZONTAL_RULE_WHITESPACE: {
                if( text.reachedEnd() ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    appendToPendingBuffer( character );
                } else if( unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<HorizontalRuleTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    setState( State_e::AFTER_BLOCK );
                } else if( character == unicode::ASTERISK && _pending.buffer.front() == unicode::ASTERISK ) {
                    reconsume( State_e::HORIZONTAL_RULE_ASTERISK );
                } else if( character == unicode::HYPHEN_MINUS && _pending.buffer.front() == unicode::HYPHEN_MINUS ) {
                    reconsume( State_e::HORIZONTAL_RULE_HYPHEN );
                } else if( character == unicode::UNDERSCORE && _pending.buffer.front() == unicode::UNDERSCORE ) {
                    reconsume( State_e::HORIZONTAL_RULE_UNDERSCORE );
                } else {
                    pushSectionMarker(
                        queueParagraphToken( pendingBufferPosition() )
                    );
                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY );
                    reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                }

            } break;

            case State_e::IMAGE_ALT_TEXT: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<CharacterTk>( unicode::EXCLAMATION_MARK, pendingBufferPosition() ) );
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_SQUARE_BRACKET, pendingBufferPosition() + TextPos( 0, 1 ) ) );
                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET ) {
                    setState( State_e::IMAGE_AFTER_ALT_TEXT );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::IMAGE_AFTER_ALT_TEXT: {
                if( !text.reachedEnd() && character == unicode::LEFT_PARENTHESIS ) {
                    pushSectionMarker(
                        queueToken( std::make_unique<ImageTk>( pendingBufferPosition() ) )
                    );

                    setPendingBufferStartPosition( pendingBufferPosition() + TextPos( 0, 2 ) ); //adjust position to after '!['

                    if( _pending.buffer.empty() ) {
                        logError( pendingBufferPosition(), ErrorCode::UNDEFINED_IMAGE_ALT_TEXT );
                    }

                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY );
                    setState( State_e::IMAGE_SOURCE );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_SQUARE_BRACKET_DECLARATION_FORMAT );
                    queueToken( std::make_unique<CharacterTk>( unicode::EXCLAMATION_MARK, pendingBufferPosition() ) );
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_SQUARE_BRACKET, pendingBufferPosition() + TextPos( 0, 1 ) ) );
                    setPendingBufferStartPosition( pendingBufferPosition() + TextPos( 0, 2 ) ); //adjust position to after '!['
                    appendToPendingBuffer( unicode::RIGHT_SQUARE_BRACKET );
                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::IMAGE_SOURCE: {
                if( text.reachedEnd() ) {
                    auto * link_tk = dynamic_cast<ImageTk *>( getQueuedToken( peekLastSectionMarker() ).get() );

                    if( _pending.buffer.empty() ) {
                        logError( text.position(), ErrorCode::UNDEFINED_IMAGE_SOURCE );
                    } else {
                        link_tk->setSource( pendingBufferToStr() );
                    }

                    logError( link_tk->position(), ErrorCode::EOF_IN_IMAGE );
                    queueToken( std::make_unique<BlockEndTk>( Type_e::IMAGE, text.position() ) );
                    clearPendingCacheData();
                    popSectionMarker();
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_PARENTHESIS ) {
                    if( _pending.buffer.empty() ) {
                        logError( text.position(), ErrorCode::UNDEFINED_IMAGE_SOURCE );
                    } else {
                        auto * link_tk = dynamic_cast<ImageTk *>( getQueuedToken( peekLastSectionMarker() ).get() );
                        link_tk->setSource( pendingBufferToStr() );
                    }

                    queueToken( std::make_unique<BlockEndTk>( Type_e::IMAGE, text.position() ) );
                    popSectionMarker();
                    resetPendingBuffer( text.position() + TextPos( 0, 1 ) );
                    setState( consumeReturnState() );
                } else if( unicode::ascii::isfeed( character ) ) {
                    logError( text.position(), ErrorCode::NEWLINE_IN_IMAGE_SOURCE );

                    if( _pending.buffer.empty() ) {
                        logError( text.position(), ErrorCode::UNDEFINED_IMAGE_SOURCE );
                    } else {
                        auto * link_tk = dynamic_cast<ImageTk *>( getQueuedToken( peekLastSectionMarker() ).get() );
                        link_tk->setSource( pendingBufferToStr() );
                    }

                    queueToken( std::make_unique<BlockEndTk>( Type_e::IMAGE, text.position() ) );
                    popSectionMarker();
                    resetPendingBuffer( text.position() + TextPos( 0, 1 ) );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::AMPERSAND ) {
                    appendToPendingBuffer( U"&amp;" );
                } else if( character == unicode::LESS_THAN_SIGN ) {
                    appendToPendingBuffer( U"&lt;" );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    appendToPendingBuffer( U"&gt;" );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HYPERLINK_TEXT: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_SQUARE_BRACKET, pendingBufferPosition() ) );
                    text.reverseCaret( _pending.buffer.size() );
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET ) {
                    setState( State_e::HYPERLINK_AFTER_TEXT );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HYPERLINK_AFTER_TEXT: {
                if( !text.reachedEnd() && character == unicode::LEFT_PARENTHESIS ) {
                    pushSectionMarker(
                        queueToken( std::make_unique<HyperlinkTk>( pendingBufferPosition() ) )
                    );

                    setPendingBufferStartPosition( pendingBufferPosition() + TextPos( 0, 1 ) ); //adjust position to after '['

                    if( _pending.buffer.empty() ) {
                        logError( pendingBufferPosition(), ErrorCode::UNDEFINED_HYPERLINK_TEXT );
                    }

                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY );
                    setState( State_e::HYPERLINK_URL );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_SQUARE_BRACKET_DECLARATION_FORMAT );
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_SQUARE_BRACKET, pendingBufferPosition() ) );
                    setPendingBufferStartPosition( pendingBufferPosition() + TextPos( 0, 1 ) ); //adjust position to after '['
                    appendToPendingBuffer( unicode::RIGHT_SQUARE_BRACKET );
                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY );
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::HYPERLINK_URL: {
                if( text.reachedEnd() ) {
                    auto * link_tk = dynamic_cast<HyperlinkTk *>( getQueuedToken( peekLastSectionMarker() ).get() );

                    if( _pending.buffer.empty() ) {
                        logError( text.position(), ErrorCode::UNDEFINED_HYPERLINK_URL );
                    } else {
                        link_tk->setURL( pendingBufferToStr() );
                    }

                    logError( link_tk->position(), ErrorCode::EOF_IN_HYPERLINK );
                    resetPendingBuffer( text.position() );
                    queueToken( std::make_unique<BlockEndTk>( Type_e::HYPERLINK, text.position() ) );
                    popSectionMarker();
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_PARENTHESIS ) {
                    if( _pending.buffer.empty() ) {
                        logError( text.position(), ErrorCode::UNDEFINED_HYPERLINK_URL );
                    } else {
                        auto * link_tk = dynamic_cast<HyperlinkTk *>( getQueuedToken( peekLastSectionMarker() ).get() );
                        link_tk->setURL( pendingBufferToStr() );
                    }

                    queueToken( std::make_unique<BlockEndTk>( Type_e::HYPERLINK, text.position() ) );
                    popSectionMarker();
                    resetPendingBuffer( text.position() + TextPos( 0, 1 ) );
                    setState( consumeReturnState() );
                } else if( unicode::ascii::isfeed( character ) ) {
                    logError( text.position(), ErrorCode::NEWLINE_IN_HYPERLINK_URL );

                    if( _pending.buffer.empty() ) {
                        logError( text.position(), ErrorCode::UNDEFINED_HYPERLINK_URL );
                    } else {
                        auto * link_tk = dynamic_cast<HyperlinkTk *>( getQueuedToken( peekLastSectionMarker() ).get() );
                        link_tk->setURL( pendingBufferToStr() );
                    }

                    queueToken( std::make_unique<BlockEndTk>( Type_e::HYPERLINK, text.position() ) );
                    popSectionMarker();
                    resetPendingBuffer( text.position() + TextPos( 0, 1 ) );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::AMPERSAND ) {
                    appendToPendingBuffer( U"&amp;" );
                } else if( character == unicode::LESS_THAN_SIGN ) {
                    appendToPendingBuffer( U"&lt;" );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    appendToPendingBuffer( U"&gt;" );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HEADING_BLOCK_ATX_DEFINITION: {
                if( character == unicode::NUMBER_SIGN ) {
                    appendToPendingBuffer( character );
                } else {
                    queueToken( std::make_unique<HeadingTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    reconsume( State_e::HEADING_BLOCK_ATX_BEFORE_TEXT );
                }
            } break;

            case State_e::HEADING_BLOCK_ATX_BEFORE_TEXT: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    reconsume( State_e::HEADING_BLOCK_ATX_TEXT );
                } //else: ignore
            } break;

            case State_e::HEADING_BLOCK_ATX_TEXT: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::LEFT_CURLY_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    pushReturnState( State_e::HEADING_BLOCK_ATX_TEXT );
                    setState( State_e::INLINE_CONTENT_LEFT_CURLY_BRACKET );
                } else if( unicode::ascii::isfeed( character ) ) {
                    setState( State_e::AFTER_BLOCK );
                } else if( character == unicode::NUMBER_SIGN ) {
                    resetPendingBuffer( pendingBufferPosition() );
                    appendToPendingBuffer( character );
                    setState( State_e::HEADING_BLOCK_ATX_TEXT_CLOSING );
                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::HEADING_BLOCK_ATX_TEXT_CLOSING: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::HEADING_BLOCK_ATX_TEXT_AFTER_CLOSING );
                } else if( unicode::ascii::isfeed( character ) ) { //"..#\n"
                    setState( State_e::AFTER_BLOCK );
                } else if( character == unicode::SPACE ) { //"..# "
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::HEADING_BLOCK_ATX_TEXT_AFTER_CLOSING );
                } else if( character != unicode::NUMBER_SIGN ) { //"..#?"
                    logError( text.position(), ErrorCode::INLINE_CONTENT_AFTER_HEADING );
                    reconsume( State_e::AFTER_BLOCK );
                } //else: ignore '#' characters
            } break;

            case State_e::HEADING_BLOCK_ATX_TEXT_AFTER_CLOSING: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    setState( State_e::AFTER_BLOCK );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    logError( text.position(), ErrorCode::INLINE_CONTENT_AFTER_HEADING );
                    reconsume( State_e::AFTER_BLOCK );
                } else { //i.e.: unicode::SPACE character
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HEADING_BLOCK_SETEXT_FMT_EQUAL_SIGN: {
                if( character == unicode::EQUALS_SIGN ) {
                    appendToPendingBuffer( character );
                } else if( unicode::ascii::isfeed( character ) || text.reachedEnd() ) {
                    if( hasQueuedTokens() ) {
                        auto &     old_section_tk    = getQueuedToken( peekLastSectionMarker() );
                        const auto old_section_id    = dynamic_cast<BlockBeginTk *>( old_section_tk.get() )->id();
                        auto       new_section_block = std::make_unique<HeadingTk>( 1, std::move( pendingBufferToStr() ), pendingBufferPosition() );

                        dynamic_cast<BlockBeginTk *>( new_section_block.get() )->setID( old_section_id );
                        old_section_tk = std::move( new_section_block );

                        popSectionMarker();
                        popOpenBlockStack(); //'PARAGRAPH'
                        trimQueuedSpaceTokens();
                        queueToken( std::make_unique<BlockEndTk>( Type_e::HEADING, text.position() ) );
                        reconsume( State_e::AFTER_BLOCK );
                    } else {
                        pushSectionMarker(
                            queueParagraphToken( pendingBufferPosition() )
                        );
                        flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY ); //since just '=' characters
                        reconsume( consumeReturnState() );
                    }
                } else {
                    if( requiresSpaceSeparatorToken() && !isPendingBufferLastChar( unicode::SPACE ) ) {
                        queueToken( std::make_unique<CharacterTk>( unicode::SPACE, pendingBufferPosition() ) );
                    }
                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY ); //since just '=' characters
                    popSectionMarker();
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::HEADING_BLOCK_SETEXT_FMT_HYPHEN: {
                if( character == unicode::HYPHEN_MINUS ) {
                    appendToPendingBuffer( character );
                } else if( unicode::ascii::isfeed( character ) || text.reachedEnd() ) {
                    if( hasQueuedTokens() ) {
                        auto &     old_section_tk    = getQueuedToken( peekLastSectionMarker() );
                        const auto old_section_id    = dynamic_cast<BlockBeginTk *>( old_section_tk.get() )->id();
                        auto       new_section_block = std::make_unique<HeadingTk>( 2, std::move( pendingBufferToStr() ), pendingBufferPosition() );

                        dynamic_cast<BlockBeginTk *>( new_section_block.get() )->setID( old_section_id );
                        old_section_tk = std::move( new_section_block );

                        popSectionMarker();
                        popOpenBlockStack(); //'PARAGRAPH'
                        trimQueuedSpaceTokens();
                        queueToken( std::make_unique<BlockEndTk>( Type_e::HEADING, text.position() ) );
                        reconsume( State_e::AFTER_BLOCK );
                    } else {
                        pushSectionMarker(
                            queueParagraphToken( pendingBufferPosition() )
                        );
                        flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY ); //since just '-' characters
                        reconsume( consumeReturnState() );
                    }
                } else {
                    if( requiresSpaceSeparatorToken() && !isPendingBufferLastChar( unicode::SPACE ) ) {
                        queueToken( std::make_unique<CharacterTk>( unicode::SPACE, pendingBufferPosition() ) );
                    }
                    flushPendingBufferToQueue( CharacterProcessing::FORMAT_ONLY ); //since just '-' characters
                    popSectionMarker();
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::CODE_BLOCK_DECLARATION: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_CODE_BLOCK );
                    queueToken( std::make_unique<CodeBlockTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( _pending.block_fence == Fence_e::TRIPLE_TILDE && character == unicode::TILDE ) { //"~~~~"
                    setCurrentBlockFence( Fence_e::QUAD_TILDE );
                    setState( State_e::CODE_BLOCK_BEFORE_LANGUAGE_TAG );
                } else if( _pending.block_fence == Fence_e::TRIPLE_GRAVE_ACCENT && character == unicode::GRAVE_ACCENT ) { //"````"
                    setCurrentBlockFence( Fence_e::QUAD_GRAVE_ACCENT );
                    setState( State_e::CODE_BLOCK_BEFORE_LANGUAGE_TAG );
                } else {
                    reconsume( State_e::CODE_BLOCK_BEFORE_LANGUAGE_TAG );
                }
            } break;

            case State_e::CODE_BLOCK_BEFORE_LANGUAGE_TAG: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_CODE_BLOCK );
                    queueToken( std::make_unique<CodeBlockTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<CodeBlockTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    setState( State_e::CODE_BLOCK_CONTENT_NEWLINE );
                } else if( unicode::ascii::isalpha( character ) ) {
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::CODE_BLOCK_LANGUAGE_TAG );
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    logError( text.position(), ErrorCode::INVALID_WHITESPACE_IN_CODE_BLOCK_DECLARATION );
                    queueToken( std::make_unique<CodeBlockTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::CODE_BLOCK_INLINED_CONTENT );
                } else if( ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::TRIPLE_GRAVE_ACCENT && text.characters( 3 ) == UR"(```)"  ) ||
                           ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::QUAD_GRAVE_ACCENT   && text.characters( 4 ) == UR"(````)" ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::TRIPLE_TILDE        && text.characters( 3 ) == UR"(~~~)"  ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::QUAD_TILDE          && text.characters( 4 ) == UR"(~~~~)" ) )
                {
                    queueToken( std::make_unique<CodeBlockTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::CODE_BLOCK_INLINED_CONTENT );
                } else { //any other characters
                    logError( text.position(), ErrorCode::INVALID_LANGUAGE_TAG_IN_CODE_BLOCK );
                    queueToken( std::make_unique<CodeBlockTk>( pendingBufferPosition() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    resetPendingBuffer( text.position() );
                    reconsume( State_e::CODE_BLOCK_INLINED_CONTENT );
                }
            } break;

            case State_e::CODE_BLOCK_INLINED_CONTENT: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::INLINED_CODE_BLOCK );
                    logError( text.position(), ErrorCode::EOF_IN_CODE_BLOCK );
                    flushPendingBufferToQueue( CharacterProcessing::ENCODING_ONLY );
                    reconsume( State_e::END_OF_FILE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    flushPendingBufferToQueue( CharacterProcessing::ENCODING_ONLY );
                    setState( State_e::CODE_BLOCK_CONTENT_NEWLINE );
                } else if( ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::TRIPLE_GRAVE_ACCENT && text.characters( 3 ) == UR"(```)"  ) ||
                           ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::QUAD_GRAVE_ACCENT   && text.characters( 4 ) == UR"(````)" ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::TRIPLE_TILDE        && text.characters( 3 ) == UR"(~~~)"  ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::QUAD_TILDE          && text.characters( 4 ) == UR"(~~~~)" ) )
                {
                    logError( text.position(), ErrorCode::INLINED_CODE_BLOCK );
                    flushPendingBufferToQueue( CharacterProcessing::ENCODING_ONLY );
                    reconsume( State_e::CODE_BLOCK_END );
                } else if( character == unicode::REVERSE_SOLIDUS && text.characters( 2 ) == UR"(\`)" ) {
                    appendToPendingBuffer( character );
                    modifyReturnState( currentState() );
                    setState( State_e::ESCAPED_CHARACTER_TO_PENDING_CHAR_BUFFER );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::CODE_BLOCK_LANGUAGE_TAG: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_CODE_BLOCK );
                    queueToken( std::make_unique<CodeBlockTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() - currentFenceSize() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    reconsume( State_e::END_OF_FILE );
                } else if( unicode::ascii::isalpha( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    queueToken( std::make_unique<CodeBlockTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() - currentFenceSize() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    setState( State_e::CODE_BLOCK_WHITESPACE_AFTER_LANGUAGE_TAG );
                } else if( unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<CodeBlockTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() - currentFenceSize() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    setState( State_e::CODE_BLOCK_CONTENT_NEWLINE );
                } else if( ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::TRIPLE_GRAVE_ACCENT && text.characters( 3 ) == UR"(```)"  ) ||
                           ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::QUAD_GRAVE_ACCENT   && text.characters( 4 ) == UR"(````)" ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::TRIPLE_TILDE        && text.characters( 3 ) == UR"(~~~)"  ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::QUAD_TILDE          && text.characters( 4 ) == UR"(~~~~)" ) )
                {
                    logError( text.position(), ErrorCode::INLINED_CODE_BLOCK );
                    queueToken( std::make_unique<CodeBlockTk>( std::move( pendingBufferToStr() ), pendingBufferPosition() - currentFenceSize() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    reconsume( State_e::CODE_BLOCK_END );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_LANGUAGE_TAG_IN_CODE_BLOCK );
                    queueToken( std::make_unique<CodeBlockTk>( pendingBufferPosition() - currentFenceSize() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    reconsume( State_e::CODE_BLOCK_INLINED_CONTENT );
                }
            } break;

            case State_e::CODE_BLOCK_WHITESPACE_AFTER_LANGUAGE_TAG: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_CODE_BLOCK );
                    reconsume( State_e::END_OF_FILE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    setState( State_e::CODE_BLOCK_CONTENT_NEWLINE );
                } else if( ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::TRIPLE_GRAVE_ACCENT && text.characters( 3 ) == UR"(```)"  ) ||
                           ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::QUAD_GRAVE_ACCENT   && text.characters( 4 ) == UR"(````)" ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::TRIPLE_TILDE        && text.characters( 3 ) == UR"(~~~)"  ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::QUAD_TILDE          && text.characters( 4 ) == UR"(~~~~)" ) )
                {
                    logError( text.position(), ErrorCode::INLINED_CODE_BLOCK );
                    reconsume( State_e::CODE_BLOCK_END );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    logError( pendingBufferPosition(), ErrorCode::INVALID_FORMAT_IN_CODE_BLOCK_DECLARATION );
                    reconsume( State_e::CODE_BLOCK_CONTENT );
                } //else: ignore space & tab
            } break;

            case State_e::CODE_BLOCK_CONTENT: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_CODE_BLOCK );
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::GRAVE_ACCENT ) {
                    reconsume( State_e::CODE_BLOCK_END );
                } else if( character == unicode::TILDE ) {
                    reconsume( State_e::CODE_BLOCK_END );
                } else if( character == unicode::REVERSE_SOLIDUS && text.characters( 2 ) == UR"(\`)" ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    modifyReturnState( currentState() );
                    setState( State_e::ESCAPED_CHARACTER_TO_TOKEN_QUEUE );
                } else if( character == unicode::AMPERSAND ) {
                    queueToken( std::make_unique<CharacterTk>( U"&amp;", text.position() ) );
                } else if( character == unicode::LESS_THAN_SIGN ) {
                    queueToken( std::make_unique<CharacterTk>( U"&lt;", text.position() ) );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    queueToken( std::make_unique<CharacterTk>( U"&gt;", text.position() ) );
                } else if( unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    setState( State_e::CODE_BLOCK_CONTENT_NEWLINE );
                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::CODE_BLOCK_CONTENT_NEWLINE: {
                if( hasOpenContainerBlock() ) {
                    pushReturnState( State_e::CODE_BLOCK_CONSUME_WHITESPACE );
                    reconsume( State_e::NEWLINE_CONSUME_PREFIXES_BEGIN );
                } else {
                    reconsume( State_e::CODE_BLOCK_CONSUME_WHITESPACE );
                }
            } break;

            case State_e::CODE_BLOCK_CONSUME_WHITESPACE: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::EOF_IN_CODE_BLOCK );
                    reconsume( State_e::END_OF_FILE );
                } else if( text.position().col == peekLastOpenBlock().position.col ) {
                    reconsume( State_e::CODE_BLOCK_CONTENT );
                } else if( unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::CODE_BLOCK_CONTENT );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    logError( text.position(), ErrorCode::MISALIGNED_BLOCK_CONTENT, blogator::to_string( peekLastOpenBlockType() ) );
                    reconsume( State_e::CODE_BLOCK_CONTENT );
                } //else: ignore
            } break;

            case State_e::CODE_BLOCK_END: {
                if( ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::TRIPLE_GRAVE_ACCENT && text.characters( 3 ) == UR"(```)" ) ||
                    ( character == unicode::TILDE        && _pending.block_fence == Fence_e::TRIPLE_TILDE        && text.characters( 3 ) == UR"(~~~)" ) )
                {
                    if( unicode::ascii::isfeed( peekLastQueuedToken()->text().back() ) ) {
                        popLastQueuedToken();
                    }

                    text.advanceCaret( 3 );
                    closeLastOpenedBlock( text.position() );
                    reconsume( State_e::CODE_BLOCK_AFTER_END );

                } else if( ( character == unicode::GRAVE_ACCENT && _pending.block_fence == Fence_e::QUAD_GRAVE_ACCENT && text.characters( 4 ) == UR"(````)" ) ||
                           ( character == unicode::TILDE        && _pending.block_fence == Fence_e::QUAD_TILDE        && text.characters( 4 ) == UR"(~~~~)" ) )
                {
                    if( unicode::ascii::isfeed( peekLastQueuedToken()->text().back() ) ) {
                        popLastQueuedToken();
                    }

                    text.advanceCaret( 4 );
                    closeLastOpenedBlock( text.position() );
                    reconsume( State_e::CODE_BLOCK_AFTER_END );

                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    setState( State_e::CODE_BLOCK_CONTENT );
                }
            } break;

            case State_e::CODE_BLOCK_AFTER_END: {
                if( text.reachedEnd() ) {
                    modifyReturnState( State_e::AFTER_BLOCK );
                    reconsume( consumeReturnState() );
                } else if( unicode::ascii::isfeed( character ) ) {
                    modifyReturnState( State_e::BEFORE_BLOCK );
                    setState( consumeReturnState() );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    logError( text.position(), ErrorCode::INLINED_CONTENT_AFTER_CODE_BLOCK );
                    logError( text.position(), ErrorCode::BLOCK_LINEFEED_SEPARATOR_MISSING, character );
                    modifyReturnState( State_e::BEFORE_BLOCK );
                    reconsume( consumeReturnState() );
                } //else: ignore
            } break;

            case State_e::BLOCKQUOTE_NEW_BLOCKQUOTE_BLOCK: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    queueToken( std::make_unique<BlockQuoteTk>( _actual_blockquote_lvl, text.position() ) );
                    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
                    appendToPendingBuffer( character );
                    ++_curr_blockquote_lvl;
                    ++_actual_blockquote_lvl;
                } else if( character != unicode::TAB && character != unicode::SPACE ) {
                    reconsume( State_e::BLOCKQUOTE_BLOCK_BEGIN );
                } //else: ignore
            } break;

            case State_e::BLOCKQUOTE_BLOCK_BEGIN: {
                resetPendingBuffer( text.position() );

                if( text.reachedEnd() ) {
                    reconsume( State_e::END_OF_FILE );
                } else if( character == unicode::NUMBER_SIGN ) { //'atx' styled heading
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    reconsume( State_e::HEADING_BLOCK_ATX_DEFINITION );
                } else if( character == unicode::HYPHEN_MINUS ) { //could be either unordered list, task list, or paragraph text
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_HYPHEN );
                } else if( character == unicode::ASTERISK ) { //could be either unordered list or paragraph text formatting
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_ASTERISK );
                } else if( character == unicode::UNDERSCORE ) {
                    appendToPendingBuffer( character );
                    setState( State_e::BLOCK_BEGIN_UNDERSCORE );
                } else if( character == unicode::GRAVE_ACCENT && text.characters( 3 ) == UR"(```)" ) { //code block
                    text.advanceCaret( 2 );
                    setCurrentBlockFence( Fence_e::TRIPLE_GRAVE_ACCENT );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::CODE_BLOCK_DECLARATION );
                } else if( character == unicode::TILDE && text.characters( 3 ) == UR"(~~~)" ) { //code block
                    text.advanceCaret( 2 );
                    setCurrentBlockFence( Fence_e::TRIPLE_TILDE );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    setState( State_e::CODE_BLOCK_DECLARATION );
                } else if( character == unicode::VERTICAL_LINE ) { //could be a table
                    if( _table_cache.is_fake_table ) {
                        _table_cache.is_fake_table = false; //reset flag for next block
                        pushSectionMarker(
                            queueParagraphToken( pendingBufferPosition() )
                        );
                        reconsume( State_e::PARAGRAPH_BLOCK_LINE_CONTENT );
                    } else {
                        _table_cache = TableCache();
                        _table_cache.fallback_state = std::make_unique<U32Text::State>( text.createMarker() );
                        appendToPendingBuffer( character );
                        modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                        setState( State_e::TABLE_BEGIN );
                    }
                } else if( unicode::ascii::isfeed( character ) ) {
                    setState( State_e::BEFORE_BLOCK );
                } else if( unicode::ascii::isdigit( character ) ) { //could be an ordered list or paragraph text
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_NUMERAL );
                } else if( unicode::ascii::islower( character ) ) { //could be an ordered list or paragraph text
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_LOWER_ALPHA );
                } else if( unicode::ascii::isupper( character ) ) { //could be an ordered list or paragraph text
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                    setState( State_e::LINE_BEGIN_UPPER_ALPHA );
                } else {
                    reconsume( State_e::PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK );
                }
            } break;

            case State_e::BLOCK_ID_NAME: {
                if( text.reachedEnd() ) { //"{#..."
                    logError( text.position(), ErrorCode::EOF_IN_BLOCK_ID );
                    text.reverseCaret( _pending.buffer.size() + 2 ); //back to '{'
                    resetPendingBuffer( text.position() );
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_CURLY_BRACKET, text.position() ) );
                    setState( consumeReturnState() );
                } else if( character == unicode::RIGHT_CURLY_BRACKET ) { //"{#...}"
                    auto name = pendingBufferToStr();

                    if( Markdown::isValidIDName( name ) ) {
                        setLastOpenBlockID( name, text.position() );
                        setState( State_e::AFTER_BLOCK_ID );
                    } else {
                        logError( text.position(), ErrorCode::INVALID_BLOCK_ID_NAME, "\"" + unicode::utf8::convert( name ) + "\"" );
                        text.reverseCaret( _pending.buffer.size() + 2 ); //back to '{'
                        resetPendingBuffer( text.position() );
                        queueToken( std::make_unique<CharacterTk>( unicode::LEFT_CURLY_BRACKET, text.position() ) );
                        setState( consumeReturnState() );
                    }
                } else if( unicode::ascii::isalnum( character ) ||
                           character == unicode::HYPHEN_MINUS ||
                           character == unicode::UNDERSCORE )
                {
                    appendToPendingBuffer( character );
                } else {
                    text.reverseCaret( _pending.buffer.size() + 2 ); //back to '{'
                    resetPendingBuffer( text.position() );
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_CURLY_BRACKET, text.position() ) );
                    setState( consumeReturnState() );
                }
            } break;

            case State_e::AFTER_BLOCK_ID: {
                if( character != unicode::SPACE ) {
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                } //else: ignore
            } break;

            case State_e::FOOTNOTE_REF_ID: {
                if( text.reachedEnd() ) {
                    queueToken( std::make_unique<CharacterTk>( unicode::LEFT_SQUARE_BRACKET, pendingBufferPosition() ) );
                    text.reverseCaret( _pending.buffer.size() + 1 ); //back to '^'
                    resetPendingBuffer( text.position() );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::RIGHT_SQUARE_BRACKET && !_pending.buffer.empty() ) { //"[^0]"
                    setState( State_e::AFTER_FOOTNOTE_REF );
                } else if( !unicode::ascii::isfeed( character ) ) {
                    appendToPendingBuffer( character );
                } else { //"[^...\n"
                    text.reverseCaret( _pending.buffer.size() + 2 ); //back to '^'
                    resetPendingBuffer( pendingBufferPosition() );  //keeps the position on '[' of the "[..."
                    appendToPendingBuffer( unicode::LEFT_SQUARE_BRACKET );
                    setState( consumeReturnState() );
                }
            } break;

            case State_e::AFTER_FOOTNOTE_REF: {
                queueToken( std::make_unique<FootnoteRefTk>( pendingBufferToStr(), pendingBufferPosition() ) );
                resetPendingBuffer( text.position() );
                reconsume( consumeReturnState() );
            } break;

            case State_e::TABLE_BEGIN: { //"|"
                if( !text.reachedEnd() && !unicode::ascii::isfeed( character ) ) {
                    pushSectionMarker(
                        queueTableToken( pendingBufferPosition() )
                    );
                    queueTableRowToken( pendingBufferPosition() );
                    reconsume( State_e::TABLE_COL_HEADING_NEW );
                } else { //not a table
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::TABLE_COL_HEADING_NEW: { //"|"
                if( text.reachedEnd() ) {
                    reconsume( State_e::TABLE_ABORT );
                } else if( unicode::ascii::isfeed( character ) ) {
                    if( _table_cache.heading_tokens.empty() ) { //"|[\s]*\n"
                        reconsume( State_e::TABLE_ABORT );
                    } else {
                        closeLastOpenedBlock( text.position() ); //'TABLE_ROW'
                        modifyReturnState( State_e::TABLE_DEFINITION_ROW_BEGIN );
                        setState( State_e::NEWLINE_CONSUME_PREFIXES_BEGIN );
                    }
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    queueTableHeadingToken( pendingBufferPosition() );
                    reconsume( State_e::TABLE_COL_HEADING_CONTENT );
                } //else: ignore character (space/tab)
            } break;

            case State_e::TABLE_COL_HEADING_CONTENT: { //"|?"
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::TABLE_ABORT );
                } else if( character == unicode::VERTICAL_LINE ) {
                    trimQueuedSpaceTokens();
                    closeFormattingMarkers( text.position() );
                    closeLastOpenedBlock( text.position() ); //'TABLE_HEADING'
                    resetPendingBuffer( text.position() );
                    setState( State_e::TABLE_COL_HEADING_NEW );
                } else if( character == unicode::LEFT_CURLY_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::TABLE_COL_HEADING_CONTENT );
                    setState( State_e::INLINE_CONTENT_LEFT_CURLY_BRACKET );
                } else if( character == unicode::EXCLAMATION_MARK ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::TABLE_COL_HEADING_CONTENT );
                    setState( State_e::INLINE_CONTENT_BANG );
                } else if( character == unicode::LEFT_SQUARE_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::TABLE_COL_HEADING_CONTENT );
                    setState( State_e::INLINE_CONTENT_LEFT_SQUARE_BRACKET );
                } else if( isFormattingChar( character ) ) {
                    modifyReturnState( State_e::TABLE_COL_HEADING_CONTENT );
                    reconsume( State_e::INLINE_CONTENT_FORMATTING );
                } else if( character == unicode::REVERSE_SOLIDUS ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    modifyReturnState( State_e::TABLE_COL_HEADING_CONTENT );
                    setState( State_e::ESCAPED_CHARACTER_TO_TOKEN_QUEUE );
                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::TABLE_DEFINITION_ROW_BEGIN: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::TABLE_ABORT );
                } else if( character == unicode::VERTICAL_LINE ) {
                    incrementBlockLineCount();
                    resetPendingBuffer( text.position() );
                    setState( State_e::TABLE_COL_DEFINITION_NEW );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    reconsume( State_e::TABLE_ABORT );
                } //else: ignore character (space/tab)
            } break;

            case State_e::TABLE_COL_DEFINITION_NEW: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::TABLE_DEFINITION_ROW_END );
                } else if( character == unicode::VERTICAL_LINE ) {
                    reconsume( State_e::TABLE_COL_DEFINITION_END );
                } else if( character == unicode::COLON || character == unicode::HYPHEN_MINUS ) {
                    appendToPendingBuffer( character );
                    setState( State_e::TABLE_COL_DEFINITION );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    reconsume( State_e::TABLE_ABORT );
                } //else: ignore character (space/tab)
            } break;

            case State_e::TABLE_COL_DEFINITION: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::TABLE_DEFINITION_ROW_END );
                } else if( character == unicode::VERTICAL_LINE ) {
                    reconsume( State_e::TABLE_COL_DEFINITION_END );
                } else if( character == unicode::COLON ) {
                    appendToPendingBuffer( character );
                    setState( State_e::TABLE_COL_DEFINITION_END );
                } else if( character == unicode::HYPHEN_MINUS ) {
                    appendToPendingBuffer( character );
                } else {
                    logError( pendingBufferPosition(), ErrorCode::TABLE_COLUMN_DEFINITION_INVALID, true );
                    reconsume( State_e::TABLE_ABORT );
                }
            } break;

            case State_e::TABLE_COL_DEFINITION_END: {
                if( text.reachedEnd() ) {
                    logError( text.position(), ErrorCode::TABLE_COLUMN_MISMATCH, true );
                    reconsume( State_e::TABLE_ABORT );
                } else if( character == unicode::VERTICAL_LINE ) {
                    if( _table_cache.col_i < _table_cache.heading_tokens.size() && _pending.buffer.size() >= 3 ) { //"---"/":--"/"--:"/":-:"
                        _table_cache.heading_tokens[ _table_cache.col_i ]->setAlignment( TableHeadingTk::resolveAlignment( _pending.buffer ) );
                        ++_table_cache.col_i;
                        resetPendingBuffer( text.position() );
                        setState( State_e::TABLE_COL_DEFINITION_NEW );
                    } else {
                        if( _table_cache.col_i < _table_cache.heading_tokens.size() ) {
                            logError( pendingBufferPosition(), ErrorCode::TABLE_COLUMN_DEFINITION_INVALID, true );
                        } else {
                            logError( text.position(), ErrorCode::TABLE_COLUMN_MISMATCH, true );
                        }

                        reconsume( State_e::TABLE_ABORT );
                    }
                } else if( unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::TABLE_DEFINITION_ROW_END );
                } else if( character == unicode::HYPHEN_MINUS || character == unicode::COLON ) {
                    logError( pendingBufferPosition(), ErrorCode::TABLE_COLUMN_DEFINITION_INVALID, true );
                    reconsume( State_e::TABLE_ABORT );
                } else { //"...:?"
                    logError( pendingBufferPosition(), ErrorCode::TABLE_COLUMN_DEFINITION_INVALID, true );
                    reconsume( State_e::TABLE_ABORT );
                }
            } break;

            case State_e::TABLE_DEFINITION_ROW_END: {
                if( _table_cache.col_i != _table_cache.heading_tokens.size() ) {
                    logError( text.position(), ErrorCode::TABLE_COLUMN_MISMATCH, true );
                    reconsume( State_e::TABLE_ABORT );
                } else if( text.reachedEnd() ) {
                    reconsume( State_e::TABLE_ABORT );
                } else {
                    modifyReturnState( State_e::TABLE_DATA_ROW_BEGIN );
                    setState( State_e::NEWLINE_CONSUME_PREFIXES_BEGIN );
                }
            } break;

            case State_e::TABLE_DATA_ROW_BEGIN: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    if( _pending.block_lines >= 3 ) {
                        closeOpenContainerBlocks( _table_cache.starting_block_i, text.position() );
                        reconsume( State_e::AFTER_BLOCK );
                    } else { //i.e. no data rows
                        reconsume( State_e::TABLE_ABORT );
                    }
                } else if( character == unicode::VERTICAL_LINE ) {
                    _table_cache.col_i = 0;
                    incrementBlockLineCount();
                    resetPendingBuffer( text.position() );
                    queueTableRowToken( text.position() );
                    setState( State_e::TABLE_DATA_CELL_NEW );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    reconsume( State_e::TABLE_ABORT );
                } //else: ignore character (space/tab)
            } break;

            case State_e::TABLE_DATA_CELL_NEW: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) { //"|\n"
                    reconsume( State_e::TABLE_DATA_ROW_END );
                } else if( character != unicode::SPACE && character != unicode::TAB ) {
                    queueTableCellToken( pendingBufferPosition() );
                    reconsume( State_e::TABLE_DATA_CELL_CONTENT );
                } //else: ignore character (space/tab)
            } break;

            case State_e::TABLE_DATA_CELL_CONTENT: {
                if( text.reachedEnd() || unicode::ascii::isfeed( character ) ) {
                    reconsume( State_e::TABLE_DATA_ROW_END );
                } else if( character == unicode::VERTICAL_LINE ) {
                    trimQueuedSpaceTokens();
                    closeFormattingMarkers( text.position() );
                    closeLastOpenedBlock( text.position() ); //'TABLE_CELL'
                    resetPendingBuffer( text.position() );
                    setState( State_e::TABLE_DATA_CELL_NEW );
                } else if( character == unicode::LEFT_CURLY_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::TABLE_DATA_CELL_CONTENT );
                    setState( State_e::INLINE_CONTENT_LEFT_CURLY_BRACKET );
                } else if( character == unicode::EXCLAMATION_MARK ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::TABLE_DATA_CELL_CONTENT );
                    setState( State_e::INLINE_CONTENT_BANG );
                } else if( character == unicode::LEFT_SQUARE_BRACKET ) {
                    resetPendingBuffer( text.position() );
                    appendToPendingBuffer( character );
                    modifyReturnState( State_e::TABLE_DATA_CELL_CONTENT );
                    setState( State_e::INLINE_CONTENT_LEFT_SQUARE_BRACKET );
                } else if( isFormattingChar( character ) ) {
                    modifyReturnState( State_e::TABLE_DATA_CELL_CONTENT );
                    reconsume( State_e::INLINE_CONTENT_FORMATTING );
                } else if( character == unicode::REVERSE_SOLIDUS ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    modifyReturnState( State_e::TABLE_DATA_CELL_CONTENT );
                    setState( State_e::ESCAPED_CHARACTER_TO_TOKEN_QUEUE );
                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::TABLE_DATA_ROW_END: {
                if( _table_cache.col_i ==_table_cache.heading_tokens.size() && peekLastOpenBlockType() == Type_e::TABLE_ROW ) {
                    _table_cache.col_i = 0;

                    if( text.reachedEnd() ) {
                        reconsume( State_e::TABLE_DATA_ROW_BEGIN );
                    } else if( unicode::ascii::isfeed( character ) ) {
                        closeLastOpenedBlock( text.position() ); //'TABLE_ROW'
                        modifyReturnState( State_e::TABLE_DATA_ROW_BEGIN );
                        setState( State_e::NEWLINE_CONSUME_PREFIXES_BEGIN );
                    }
                } else {
                    logError( text.position(), ErrorCode::TABLE_COLUMN_MISMATCH, true );
                    reconsume( State_e::TABLE_ABORT );
                }
            } break;

            case State_e::TABLE_ABORT: {
                _table_cache.is_fake_table = true;
                text.resetToMarker( *_table_cache.fallback_state );

                while( _open_blocks.size() > _table_cache.starting_block_i ) { //discard all blocks opened in table context + table itself
                    popOpenBlockStack();
                }

                clearPendingTokensFromLastSectionMarkerPosition(); //discard all table tokens and associated buffered errors
                modifyReturnState( State_e::BLOCK_BEGIN_AFTER_WHITESPACE );
                reconsume( consumeReturnState() );
            } break;

            case State_e::HTML_BLOCK_BEGIN: { //"<"
                if( text.reachedEnd() ) {
                    reconsume( State_e::HTML_BLOCK_BEGIN_CONCLUSION );
                } else if( character == unicode::SOLIDUS ) { //"</"
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::HTML_BLOCK_BEGIN_CONCLUSION );
                    setState( State_e::HTML_BLOCK_BEGIN_END_TAG_OPEN ); //condition 6 or 7
                } else if( character == unicode::EXCLAMATION_MARK ) { //"<!"
                    appendToPendingBuffer( character );
                    pushReturnState( State_e::HTML_BLOCK_BEGIN_CONCLUSION );
                    setState( State_e::HTML_BLOCK_BEGIN_MARKUP_DECLARATION ); //condition 2, 4 or 5
                } else if( character == unicode::QUESTION_MARK ) { //"<?"
                    appendToPendingBuffer( character );
                    setHtmlBlockType( HtmlBlockType_e::TYPE_3 );
                    setState( State_e::HTML_BLOCK_BEGIN_CONCLUSION );
                } else if( unicode::ascii::isalpha( character ) ) { //"<[A-Za-z]"
                    pushReturnState( State_e::HTML_BLOCK_BEGIN_CONCLUSION );
                    reconsume( State_e::HTML_BLOCK_BEGIN_TAG_NAME ); //condition 1, 6 or 7
                } else {
                    reconsume( State_e::HTML_BLOCK_BEGIN_CONCLUSION );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_END_TAG_OPEN: { //"</"
                if( text.reachedEnd() ) {
                    setState( consumeReturnState() );
                } else if( character == unicode::GREATER_THAN_SIGN ) { //"</>"
                    reconsume( consumeReturnState() );
                } else if( unicode::ascii::isalpha( character ) ) {
                    pendingHtmlTag().type = markdown::HtmlTag::Type::END;
                    reconsume( State_e::HTML_BLOCK_BEGIN_TAG_NAME );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_MARKUP_DECLARATION: { //"<!"
                if( text.reachedEnd() ) {
                    setState( consumeReturnState() );
                } else if( character == unicode::HYPHEN_MINUS && text.characters( 2 ) == UR"(--)" ) { //"<!--"
                    appendToPendingBuffer( UR"(--)" );
                    text.advanceCaret( 1 );
                    setHtmlBlockType( HtmlBlockType_e::TYPE_2 );
                    setState( consumeReturnState() );
                } else if( unicode::ascii::isalpha( character ) ) { //"<![A-Za-z]"
                    setHtmlBlockType( HtmlBlockType_e::TYPE_4 );
                    reconsume( consumeReturnState() );
                } else if( character == unicode::LEFT_SQUARE_BRACKET && text.characters( 7 ) == UR"([CDATA[)" ) { //"<![CDATA["
                    appendToPendingBuffer( UR"([CDATA[)" );
                    text.advanceCaret( 6 );
                    setHtmlBlockType( HtmlBlockType_e::TYPE_5 );
                    setState( consumeReturnState() );
                } else {
                    setState( consumeReturnState() );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_TAG_NAME: { //cond 1, 6 or 7
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    pendingHtmlTag().name = std::u32string( _html_cache.buffer.cbegin(), _html_cache.buffer.cend() );
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME );
                } else if( character == unicode::SOLIDUS ) {
                    pendingHtmlTag().name = std::u32string( _html_cache.buffer.cbegin(), _html_cache.buffer.cend() );
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_SELF_CLOSING_START_TAG );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    pendingHtmlTag().name = std::u32string( _html_cache.buffer.cbegin(), _html_cache.buffer.cend() );
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_TAG_RESOLVE );
                } else if( unicode::ascii::isupper( character ) ) { //"</[A-Z]"
                    appendToPendingBuffer( character );
                    appendToHtmlBuffer( unicode::ascii::tolower( character ) );
                } else if( unicode::ascii::islower( character ) ) { //"<![a-z]"
                    appendToPendingBuffer( character );
                    appendToHtmlBuffer( character );
                } else {
                    reconsume( consumeReturnState() );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME: {
                if( text.reachedEnd() || character == unicode::SOLIDUS || character == unicode::GREATER_THAN_SIGN ) {
                    reconsume( State_e::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_NAME );
                } else if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::EQUALS_SIGN ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_NAME );
                } else {
                    reconsume( State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_NAME );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_NAME: {
                if( text.reachedEnd()                    ||
                    unicode::ascii::isfeed( character )  ||
                    character == unicode::SPACE          ||
                    character == unicode::TAB            ||
                    character == unicode::SOLIDUS        ||
                    character == unicode::GREATER_THAN_SIGN )
                {
                    reconsume( State_e::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_NAME );
                } else if( character == unicode::EQUALS_SIGN ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_VALUE );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_NAME: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::SOLIDUS ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_SELF_CLOSING_START_TAG );
                } else if( character == unicode::EQUALS_SIGN ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_VALUE );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    appendToPendingBuffer( character );
                    setHtmlBlockType( HtmlBlockType_e::TYPE_7 );
                    setState( State_e::HTML_BLOCK_BEGIN_TAG_RESOLVE );
                } else {
                    appendToPendingBuffer( character );
                    reconsume( State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_NAME );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_VALUE: {
                if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    appendToPendingBuffer( character );
                } else if( character == unicode::QUOTATION_MARK ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_DOUBLE_QUOTED );
                } else if( character == unicode::APOSTROPHE ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_SINGLE_QUOTED );
                } else if( character == unicode::GREATER_THAN_SIGN ) { //bad format
                    appendToPendingBuffer( character );
                    reconsume( consumeReturnState() );
                } else {
                    reconsume( State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_UNQUOTED );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_DOUBLE_QUOTED: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::QUOTATION_MARK ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_VALUE_QUOTED );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_SINGLE_QUOTED: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::APOSTROPHE ) {
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_VALUE_QUOTED );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_ATTRIBUTE_VALUE_UNQUOTED: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    setState( State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    appendToPendingBuffer( character );
                    setHtmlBlockType( HtmlBlockType_e::TYPE_7 );
                    setState( State_e::HTML_BLOCK_BEGIN_TAG_RESOLVE );
                } else {
                    appendToPendingBuffer( character );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_AFTER_ATTRIBUTE_VALUE_QUOTED: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    setState( State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME );
                } else if( character == unicode::SOLIDUS ) {
                    setState( State_e::HTML_BLOCK_BEGIN_SELF_CLOSING_START_TAG );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    appendToPendingBuffer( character );
                    setHtmlBlockType( HtmlBlockType_e::TYPE_7 );
                    setState( consumeReturnState() );
                } else {
                    reconsume( State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_SELF_CLOSING_START_TAG: {
                if( text.reachedEnd() ) {
                    reconsume( consumeReturnState() );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    pendingHtmlTag().type = markdown::HtmlTag::Type::SELF_CLOSE;
                    appendToPendingBuffer( character );
                    setState( State_e::HTML_BLOCK_BEGIN_TAG_RESOLVE );
                } else {
                    reconsume( State_e::HTML_BLOCK_BEGIN_BEFORE_ATTRIBUTE_NAME );
                }
            } break;

            case State_e::HTML_BLOCK_BEGIN_TAG_RESOLVE: { //for tags with no attributes on same line
                if( _html_cache.block_type == HtmlBlockType_e::UNKNOWN ) {
                    if( pendingHtmlTag().type == markdown::HtmlTag::Type::START &&
                        specs::markdown::isHtmlBlockType1( pendingHtmlTag().name ) )
                    {
                        setHtmlBlockType( HtmlBlockType_e::TYPE_1 );

                    } else if( specs::markdown::isHtmlBlockType6( pendingHtmlTag().name ) ) {
                        setHtmlBlockType( HtmlBlockType_e::TYPE_6 );

                    } else {
                        setHtmlBlockType( HtmlBlockType_e::TYPE_7 );
                    }
                }

                reconsume( consumeReturnState() ); //'HTML_BLOCK_BEGIN_CONCLUSION'
            } break;

            case State_e::HTML_BLOCK_BEGIN_CONCLUSION: {
                if( currentHtmlBlockType() == HtmlBlockType_e::UNKNOWN ) {
                    text.resetToMarker( *_html_cache.fallback_state );
                    resetPendingBuffer( text.position() );
                    clearPendingTokensFromLastSectionMarkerPosition(); //'HtmlBlockTk' -> ...
                    popOpenBlockStack(); //'HTML'
                    reconsume( consumeReturnState() ); //'PARAGRAPH_BLOCK_NEW_PARAGRAPH_BLOCK'
                } else { //TYPE 1-7
                    auto * tk = dynamic_cast<HtmlBlockTk *>( peekLastOpenBlock().token );
                    tk->setHtmlBlockType( currentHtmlBlockType() );
                    flushPendingBufferToQueue( CharacterProcessing::NONE ); //as raw HTML
                    pendingHtmlTag()  = {};
                    modifyReturnState( State_e::AFTER_BLOCK );
                    reconsume( State_e::HTML_BLOCK_CONTENT );
                }
            } break;

            case State_e::HTML_BLOCK_CONTENT: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::HTML_BLOCK_CONTENT_NEWLINE );
                } else if( unicode::ascii::isfeed( character ) ) {
                    resetHtmlBuffer( text.position() );
                    appendToHtmlBuffer( character );
                    setState( State_e::HTML_BLOCK_CONTENT_NEWLINE );

                } else if( character              == unicode::LESS_THAN_SIGN &&
                           currentHtmlBlockType() == HtmlBlockType_e::TYPE_1 )
                {
                    queueToken( std::make_unique<CharacterTk>( unicode::LESS_THAN_SIGN, text.position() ) );
                    setState( State_e::HTML_BLOCK_CONTENT_TAG_OPEN );

                } else if( character              == unicode::HYPHEN_MINUS   &&
                           currentHtmlBlockType() == HtmlBlockType_e::TYPE_2 &&
                           text.characters( 3 )   == UR"(-->)" )
                {
                    queueToken( std::make_unique<CharacterTk>( unicode::HYPHEN_MINUS, text.position() ) );
                    text.advanceCaret();
                    queueToken( std::make_unique<CharacterTk>( unicode::HYPHEN_MINUS, text.position() ) );
                    text.advanceCaret();
                    queueToken( std::make_unique<CharacterTk>( unicode::GREATER_THAN_SIGN, text.position() ) );
                    setState( consumeReturnState() );

                } else if( character              == unicode::QUESTION_MARK  &&
                           currentHtmlBlockType() == HtmlBlockType_e::TYPE_3 &&
                           text.characters( 2 )   == UR"(?>)" )
                {
                    queueToken( std::make_unique<CharacterTk>( unicode::QUESTION_MARK, text.position() ) );
                    text.advanceCaret();
                    queueToken( std::make_unique<CharacterTk>( unicode::GREATER_THAN_SIGN, text.position() ) );
                    setState( consumeReturnState() );

                } else if( character              == unicode::GREATER_THAN_SIGN &&
                           currentHtmlBlockType() == HtmlBlockType_e::TYPE_4 )
                {
                    queueToken( std::make_unique<CharacterTk>( unicode::GREATER_THAN_SIGN, text.position() ) );
                    setState( consumeReturnState() );

                } else if( character              == unicode::RIGHT_SQUARE_BRACKET &&
                           currentHtmlBlockType() == HtmlBlockType_e::TYPE_5       &&
                           text.characters( 3 )   == UR"(]]>)" )
                {
                    queueToken( std::make_unique<CharacterTk>( unicode::RIGHT_SQUARE_BRACKET, text.position() ) );
                    text.advanceCaret();
                    queueToken( std::make_unique<CharacterTk>( unicode::RIGHT_SQUARE_BRACKET, text.position() ) );
                    text.advanceCaret();
                    queueToken( std::make_unique<CharacterTk>( unicode::GREATER_THAN_SIGN, text.position() ) );
                    setState( consumeReturnState() );

                } else {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                }
            } break;

            case State_e::HTML_BLOCK_CONTENT_NEWLINE: {
                if( text.reachedEnd() ) {
                    switch( currentHtmlBlockType() ) {
                        case HtmlBlockType_e::TYPE_6: [[fallthrough]];
                        case HtmlBlockType_e::TYPE_7: {
                            reconsume( consumeReturnState() );
                        } break;

                        default: {
                            flushHtmlBufferToQueue();
                            logError( text.position(), ErrorCode::EOF_IN_HTML_BLOCK );
                            reconsume( consumeReturnState() );
                        } break;
                    }
                } else if( character == unicode::SPACE || character == unicode::TAB ) {
                    appendToHtmlBuffer( character );
                } else if( unicode::ascii::isfeed( character ) ) {
                    switch( currentHtmlBlockType() ) {
                        case HtmlBlockType_e::TYPE_6: [[fallthrough]];
                        case HtmlBlockType_e::TYPE_7: {
                            reconsume( consumeReturnState() );
                        } break;

                        default: {
                            appendToHtmlBuffer( character );
                            flushHtmlBufferToQueue();
                            setState( State_e::HTML_BLOCK_CONTENT );
                        } break;
                    }
                } else {
                    flushHtmlBufferToQueue();
                    reconsume( State_e::HTML_BLOCK_CONTENT );
                }
            } break;

            case State_e::HTML_BLOCK_CONTENT_TAG_OPEN: { //'<'
                if( text.reachedEnd() ) {
                    reconsume( State_e::HTML_BLOCK_CONTENT_NEWLINE );
                } else if( character == unicode::SOLIDUS ) { //"</"
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    setState( State_e::HTML_BLOCK_END_END_TAG_OPEN );
                } else {
                    reconsume( State_e::HTML_BLOCK_CONTENT );
                }
            } break;

            case State_e::HTML_BLOCK_END_END_TAG_OPEN: { //"</"
                if( text.reachedEnd() ) {
                    reconsume( State_e::HTML_BLOCK_CONTENT_NEWLINE );
                } else if( unicode::ascii::isalpha( character ) ) {
                    resetHtmlBuffer( text.position() );
                    reconsume( State_e::HTML_BLOCK_END_TAG_NAME );
                } else {
                    reconsume( State_e::HTML_BLOCK_CONTENT );
                }
            } break;

            case State_e::HTML_BLOCK_END_TAG_NAME: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::HTML_BLOCK_CONTENT_NEWLINE );
                } else if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    setState( State_e::HTML_BLOCK_END_TAG_AFTER_TAG_NAME );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    reconsume( State_e::HTML_BLOCK_END_TAG_AFTER_TAG_NAME );
                } else if( unicode::ascii::isupper( character ) ) { //"</[A-Z]"
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    appendToHtmlBuffer( unicode::ascii::tolower( character ) );
                } else if( unicode::ascii::islower( character ) ) { //"<![a-z]"
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                    appendToHtmlBuffer( character );
                } else {
                    reconsume( State_e::HTML_BLOCK_CONTENT );
                }
            } break;

            case State_e::HTML_BLOCK_END_TAG_AFTER_TAG_NAME: {
                if( text.reachedEnd() ) {
                    reconsume( State_e::HTML_BLOCK_CONTENT_NEWLINE );
                } else if( character == unicode::SPACE || character == unicode::TAB || unicode::ascii::isfeed( character ) ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );
                } else if( character == unicode::GREATER_THAN_SIGN ) {
                    queueToken( std::make_unique<CharacterTk>( character, text.position() ) );

                    const auto tag_name = std::u32string( _html_cache.buffer.cbegin(), _html_cache.buffer.cend() );

                    if( specs::markdown::isHtmlBlockType1( tag_name ) ) {
                        setState( consumeReturnState() ); //'AFTER_BLOCK'
                    } else {
                        setState( State_e::HTML_BLOCK_CONTENT );
                    }
                } else {
                    reconsume( State_e::HTML_BLOCK_CONTENT );
                }
            } break;

            case State_e::END_OF_FILE: {
                closeFormattingMarkers( text.position() );

                while( hasOpenBlock() ) {
                    closeLastOpenedBlock( text.position() );
                }

                queueEndOfFileToken( text.position() );
                dispatchQueuedTokens();
            } break;

            default: { //safety net to catch developer stupidity
                std::stringstream ss;
                ss << "[parser::tokeniser::Markdown::parse( U32Text &, '" << starting_ctx << "' )] "
                   << "Unknown tokeniser state (" << currentState() << ") - 'case' implementation missing.";
                throw exception::parsing_failure( ss.str() );
            } break;
        }

        character = text.nextChar();
    }

    return specs::Context::MARKDOWN;
}

/**
 * Resets the tokeniser
 */
void tokeniser::Markdown::reset() {
    _error_count           = 0;
    _current_state         = State_e::BEFORE_BLOCK;
    _empty_lines           = 0;
    _actual_blockquote_lvl = 0;
    _curr_blockquote_lvl   = 0;
    _curr_open_container_i = 0;
    _list_spacing          = ListSpacing_e::DEFAULT_TIGHT;
    _pending               = {};
    _table_cache           = {};
    _html_cache            = {};
    _return_states         = {};
    _open_blocks           = {};
    _queued_tokens.clear();
    _queued_errors.clear();
    _open_container.clear();
    _formatting_markers.clear();
    _section_markers.clear();
}

/**
 * Gets the number of errors raised
 * @return Error count
 */
size_t tokeniser::Markdown::errors() const {
    return _error_count;
}

//==========================================[ PRIVATE ]=============================================
/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Markdown error code
 * @param bypass_buffer Flag to force logging directly to LogParser instead of buffering it locally (default=false)
 */
inline void tokeniser::Markdown::logError( TextPos position, int err_code, bool bypass_buffer ) {
    if( bypass_buffer ) {
        //On occasion there is the need to log a message when inside an 'attempted section' instead
        //of it being buffered and deleted when the section is re-consumed as a different block type.
        //
        //For example: in the case of a table with an inlined hyperlink which happen to be invalid -
        //-----------  errors will be raised during the hyperlink parsing. But if the table is aborted
        //             and everything needs to be parsed again as a paragraph, the invalid hyperlink
        //             will be parsed again and, thus, its errors re-raised. Those are better buffered
        //             to avoid duplicates but if table-specific errors are also raised, they still
        //             need to be published and so, these are sent using the 'bypass' flag.
        ++_error_count;
        logging::ParserLog::log( _src_path, THIS_CONTEXT, err_code, position );
    } else {
        _queued_errors.emplace_back( logging::ErrorObject( _src_path, THIS_CONTEXT, err_code, position ) );
    }
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Markdown error code
 * @param c Current character
 */
inline void tokeniser::Markdown::logError( TextPos position, int err_code, char32_t c ) {
    std::stringstream ss;
    ss << "\'" << unicode::utf8::convert( std::u32string( 1, c ) ) << "\'";
    _queued_errors.emplace_back( logging::ErrorObject( _src_path, THIS_CONTEXT, err_code, position, ss.str() ) );
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Markdown error code
 * @param str Description string
 */
inline void tokeniser::Markdown::logError( TextPos position, int err_code, std::string str ) {
    _queued_errors.emplace_back( logging::ErrorObject( _src_path, THIS_CONTEXT, err_code, position, std::move( str ) ) );
}

/**
 * Sets the current state
 * @param state State
 */
inline void tokeniser::Markdown::setState( State_e state ) noexcept {
    _current_state = state;
}

/**
 * Gets the current state
 * @return State
 */
inline tokeniser::Markdown::State_e tokeniser::Markdown::currentState() const noexcept {
    return _current_state;
}

/**
 * Modifies the last added return state on the stack (if stack is empty the state will be pushed onto it)
 * @param state State
 */
inline void tokeniser::Markdown::modifyReturnState( tokeniser::Markdown::State_e state ) {
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
inline void tokeniser::Markdown::pushReturnState( tokeniser::Markdown::State_e state ) {
    _return_states.push( state );
}

/**
 * Consumes the last return state added to the stack
 * @return Last return state or 'BEFORE_BLOCK' if already consumed
 */
inline tokeniser::Markdown::State_e tokeniser::Markdown::consumeReturnState() {
    auto last = State_e::BEFORE_BLOCK;

    if( !_return_states.empty() ) {
        last = _return_states.top();
        _return_states.pop();
    }

    return last;
}

/**
 * Sets the 'structured-enhanced-text' heading format flag
 * @param state Flag state
 */
inline void tokeniser::Markdown::setSETextFlag( bool state ) {
    _pending.setext_flag = state;
}

/**
 * Sets the cached pending position
 * @param position Position
 */
inline void tokeniser::Markdown::setPendingBufferStartPosition( TextPos position ) {
    _pending.position = position;
}

/*
* Append a character to the temporary text buffer
* @param c Character
*/
inline void tokeniser::Markdown::appendToPendingBuffer( char32_t c ) {
    _pending.buffer.emplace_back( c );
}

/**
 * Append a string to the temporary text buffer
 * @param str String
 */
void tokeniser::Markdown::appendToPendingBuffer( const std::u32string &str ) {
    std::copy( str.cbegin(), str.cend(), std::back_inserter( _pending.buffer ) );
}

/**
 * Clears the content of the temporary pending buffer
 * @param Buffer New start position of buffer relative to the source text
 */
inline void tokeniser::Markdown::resetPendingBuffer( TextPos position ) {
    _pending.buffer.clear();
    _pending.position = position;
}

/**
 * Increments the block line counter
 * @return Counter value
 */
inline size_t tokeniser::Markdown::incrementBlockLineCount() {
    return( ++_pending.block_lines );
}

/**
 * Sets the current block's fence type
 * @param fence Fence type
 */
inline void tokeniser::Markdown::setCurrentBlockFence( tokeniser::Markdown::Fence_e fence ) {
    _pending.block_fence = fence;
}

/**
 * Resets the current block cache data (inc. buffer)
 */
void tokeniser::Markdown::clearPendingCacheData() {
    _pending.buffer.clear();
    _pending.position    = { 0, 0 };
    _pending.setext_flag = { true };
    _pending.block_lines = { 0 };
    _pending.block_fence = { Fence_e::NONE };
}

/**
 * Checks if the current block's state is conducive toward having a heading in the 'structured-enhanced-text' format
 * @return Possibility to have 'setext' heading in the current block
 */
inline bool tokeniser::Markdown::isPossibleStateForSETextHeading() const {
    return _pending.setext_flag;
}

/**
 * Checks if the last character in pending buffer matches the one given
 * @param c Character to check against
 * @return Match state
 */
inline bool tokeniser::Markdown::isPendingBufferLastChar( char32_t c ) {
    return ( !_pending.buffer.empty() && _pending.buffer.back() == c );
}

/**
 * Gets the cached pending position
 * @return Cached position
 */
inline TextPos tokeniser::Markdown::pendingBufferPosition() const {
    return _pending.position;
}

/**
 * Gets the content of the temporary pending buffer as a string
 * @param offset Offset from where to begin string in the buffer (default=0)
 * @return Buffer content from offset
 */
inline std::u32string tokeniser::Markdown::pendingBufferToStr( size_t offset ) const {
    if( offset < _pending.buffer.size() ) {
        return { std::next( _pending.buffer.cbegin(), offset ), _pending.buffer.cend() };
    } else {
        return {};
    }
}

/**
 * Gets the pending block-fence size as a TextPos object { 0, x }
 * @return Number of characters taken by the block fence
 */
inline TextPos tokeniser::Markdown::currentFenceSize() const {
    return { 0, specs::markdown::sizeOf( _pending.block_fence ) };
}

/**
 * Flush all characters in buffer to the queue as character tokens and sets the cached position to the next char after the buffered content
 * @param auto_process Flag to process relevant characters as formatting and/or encoded character tokens
 * @param offset Offset from where to flush the buffer from
 * @return Number of characters flushed
 */
size_t tokeniser::Markdown::flushPendingBufferToQueue( CharacterProcessing auto_process, size_t offset ) {
    const size_t buff_size  = _pending.buffer.size();
    auto         position   = _pending.position;

    /**
     * [LAMBDA] Advance the position tracker by 1 code point
     * @param c character at current position (to decide either a col or line increment)
     */
    auto advancePosition = [&position]( auto c ) {
        if( unicode::ascii::isfeed( c ) ) {
            position = { position.line + 1, 1 };
        } else {
            position += { 0, 1 };
        }
    };

    for( size_t i = 0; i < offset; ++i ) {
        advancePosition( _pending.buffer[i] );
    }

    if( auto_process == CharacterProcessing::NONE ) {
        for( size_t i = offset; i < _pending.buffer.size(); ++i ) {
            queueToken( std::make_unique<CharacterTk>( _pending.buffer[i], position ) );
            advancePosition( _pending.buffer[i] );
        }

    } else {
        size_t i_curr             = offset;
        size_t i_next             = ( offset + 1 );
        bool   double_adv         = false;

        using T = std::underlying_type_t<tokeniser::Markdown::CharacterProcessing>;
        const bool process_format   = ( ( static_cast<T>( auto_process ) & static_cast<T>( CharacterProcessing::FORMAT_ONLY   ) ) == static_cast<T>( CharacterProcessing::FORMAT_ONLY ) );
        const bool process_encoding = ( ( static_cast<T>( auto_process ) & static_cast<T>( CharacterProcessing::ENCODING_ONLY ) ) == static_cast<T>( CharacterProcessing::ENCODING_ONLY ) );

        while( i_curr < buff_size ) {
            const auto curr_char = _pending.buffer[i_curr];

            if( curr_char == unicode::REVERSE_SOLIDUS && i_next < buff_size ) { //i.e.: escaped character
                const auto next_char = _pending.buffer[i_next];
                const auto next_pos  = position + TextPos( 0, 1 );

                switch( next_char ) {
                    case unicode::LESS_THAN_SIGN   : { queueToken( std::make_unique<CharacterTk>( U"&lt;", next_pos ) );   } break;
                    case unicode::GREATER_THAN_SIGN: { queueToken( std::make_unique<CharacterTk>( U"&gt;", next_pos ) );   } break;
                    case unicode::AMPERSAND        : { queueToken( std::make_unique<CharacterTk>( U"&amp;", next_pos ) );  } break;
                    default                        : { queueToken( std::make_unique<CharacterTk>( next_char, next_pos ) ); } break;
                }

                double_adv = true;

            } else if( process_format && isFormattingChar( curr_char ) ) {
                if( i_next < buff_size && curr_char == _pending.buffer[i_next] ) { //i.e.: double formatting character
                    queueFormattingToken( position, curr_char, _pending.buffer[i_next] );
                    double_adv = true;

                } else {
                    if( queueFormattingToken( position, curr_char ) == 0 ) {
                        queueToken( std::make_unique<CharacterTk>( curr_char, position ) );
                    };
                }

            } else if( process_encoding ) {
                switch( curr_char ) {
                    case unicode::LESS_THAN_SIGN   : { queueToken( std::make_unique<CharacterTk>( U"&lt;", position ) );   } break;
                    case unicode::GREATER_THAN_SIGN: { queueToken( std::make_unique<CharacterTk>( U"&gt;", position ) );   } break;
                    case unicode::AMPERSAND        : { queueToken( std::make_unique<CharacterTk>( U"&amp;", position ) );  } break;
                    default                        : { queueToken( std::make_unique<CharacterTk>( curr_char, position ) ); } break;
                }

            } else {
                queueToken( std::make_unique<CharacterTk>( curr_char, position ) );
            }

            if( double_adv ) {
                double_adv    = false;
                position.col += 2;
                i_curr       += 2;
                i_next       += 2;

            } else {
                advancePosition( curr_char );
                ++i_curr;
                ++i_next;
            }
        }
    }

    _pending.buffer.clear();
    _pending.position = position;

    return buff_size;
}

/**
 * Push block type to stack of open blocks
 * @param block Block properties
 */
inline void tokeniser::Markdown::pushToOpenBlockStack( tokeniser::markdown::Block block ) {
    if( specs::markdown::isContainerBlock( block.type ) ) {
        _open_container.push_back( block );
    }
//    LOG_CRITICAL( "> opening block: ", block.type );
    _open_blocks.push_back( block );
}

/**
 * Peek last opened block
 * @return Last opened block
 */
inline const tokeniser::markdown::Block & tokeniser::Markdown::peekLastOpenBlock() const {
    return _open_blocks.back();
}

/**
 * Peek open container block
 * @param i Index
 * @return Open container block at index
 * @throw exception::parsing_failure when index is out of bounds
 */
inline const tokeniser::markdown::Block & tokeniser::Markdown::peekOpenContainerBlock( size_t i ) const {
    try {
        return _open_container.at( i );

    } catch( const std::out_of_range &e ) {
        std::stringstream ss;
        ss << "[tokeniser::Markdown::peekOpenContainerBlock( " << i << " )] "
           << "Index out of range of opened container blocks (size: " << _open_container.size() << ")";
        throw exception::parsing_failure( ss.str() );
    }
}

/**
 * Peek last opened container block
 * @return Last opened container block
 */
inline const tokeniser::markdown::Block & tokeniser::Markdown::peekLastOpenContainerBlock() const {
    return _open_container.back();
}

/**
 * Peek last opened block type
 * @return Last opened block type or NONE if there are none
 */
inline tokeniser::Markdown::Type_e tokeniser::Markdown::peekLastOpenBlockType() const {
    if( !_open_blocks.empty() ) {
        return _open_blocks.back().type;
    }

    return Type_e::NONE;
}

/**
 * Peek last opened container block type
 * @return Last opened block type that is a container or NONE if there are none
 */
inline tokeniser::Markdown::Type_e tokeniser::Markdown::peekLastOpenContainerBlockType() const {
    if( !_open_container.empty() ) {
        return _open_container.back().type;
    }

    return Type_e::NONE;
}

/**
 * Peek last opened list type
 * @return ListType_e enum
 */
tokeniser::Markdown::ListType_e tokeniser::Markdown::peekLastOpenListType() const {
    const auto it = std::find_if( _open_container.crbegin(),
                                  _open_container.crend(),
                                  []( const markdown::Block & block ) { return block.type == Type_e::LIST; } );

    return ( it != _open_container.crend() ? it->listType() : ListType_e::NONE );
}

/**
 * Checks if there are open blocks
 * @return Open block state
 */
inline bool tokeniser::Markdown::hasOpenBlock() const {
    return !_open_blocks.empty();
}

/**
 * Checks if there are open container blocks
 * @return Opened container block state
 */
inline bool tokeniser::Markdown::hasOpenContainerBlock() const {
    return !_open_container.empty();
}

/**
 * Gets the number of currently opened container blocks
 * @return Open container block count
 */
inline size_t tokeniser::Markdown::openContainerBlockCount() const {
    return _open_container.size();
}

/**
 * Pops the last opened block in the stack of opened blocks (and the open container stack if the items are the same)
 */
inline void tokeniser::Markdown::popOpenBlockStack() {
    if( !_open_blocks.empty() ) {
        if( !_open_container.empty() && _open_blocks.back() == _open_container.back() ) {
            _open_container.pop_back();
        }

        _open_blocks.pop_back();
    }
}

/**
 * Sets the "loose list" flag
 * @param flag Value (default= true)
 */
inline void tokeniser::Markdown::setListSpacingFlag( ListSpacing_e flag ) {
    _list_spacing = flag;
}

/**
 * Sets the last current list block's spacing property
 * @param spacing spacing flag
 */
inline void tokeniser::Markdown::loosenLastListBlockSpacing() {
    auto it = std::find_if( _open_container.rbegin(),
                            _open_container.rend(),
                            []( const markdown::Block & block ) { return block.type == Type_e::LIST; } );

    if( it != _open_container.rend() ) {
        dynamic_cast<ListTk *>( it->token )->setTightFlag( false );
        setListSpacingFlag( ListSpacing_e::DEFAULT_TIGHT ); //reset to avoid sequential repeat calls for the same list
    } else {
        LOG_ERROR(
            "[parser::tokeniser::Markdown::loosenLastListBlockSpacing()] "
            "Expected opened 'LIST' block absent from opened container blocks."
        );
    }
}

/**
 * Sets the last opened block's ID
 * @param id ID string
 * @param position Current text position (for errors)
 */
void tokeniser::Markdown::setLastOpenBlockID( const std::u32string & id, TextPos position  ) {
    if( !hasOpenBlock() ) {
        LOG_ERROR(
            "[parser::tokeniser::Markdown::setLastOpenBlockID( \"", unicode::utf8::convert( id ), "\", ", position, " )] "
            "Trying to set ID when there are no opened blocks."
        );

        return; //EARLY RETURN
    }

    if( !specs::markdown::isBlockToken( peekLastOpenBlockType() ) ) {
        LOG_ERROR(
            "[parser::tokeniser::Markdown::setLastOpenBlockID( \"", unicode::utf8::convert( id ), "\", ", position, " )] "
            "Trying to set ID on a non-Block token."
        );

        return; //EARLY RETURN
    }

    auto & block = _open_blocks.back();
    auto * tk    = dynamic_cast<BlockBeginTk *>( block.token );

    if( specs::markdown::isTableBlock( block.type ) && block.type != Type_e::TABLE ) {
        auto it = std::find_if( _open_blocks.rbegin(),
                                _open_blocks.rend(),
                                []( const markdown::Block & blk ) { return blk.type == Type_e::TABLE; } );

        if( it != _open_blocks.rend() ) {
            tk = dynamic_cast<BlockBeginTk *>( it->token );

        } else {
            LOG_ERROR(
                "[parser::tokeniser::Markdown::setLastOpenBlockID( \"", unicode::utf8::convert( id ), "\", ", position, " )] "
                "Failed to find parent TABLE block in open elements."
            );
        }
    }

    if( tk->hasID() ) {
        std::stringstream ss;
        ss << R"(')" << unicode::utf8::convert( tk->id() ) << R"(' <- ')" << unicode::utf8::convert( id ) << R"(')";
        logError( position, ErrorCode::BLOCK_ID_OVERRIDE, ss.str() );

    } else {
        tk->setID( id );
    }
}

/**
 * Queues a token
 * @param token Unique token pointer
 * @return Position in queue
 */
inline size_t tokeniser::Markdown::queueToken( Markdown::TokenPtr_t token ) {
    const auto i = _queued_tokens.size();
    _queued_tokens.emplace_back( std::move( token ) );
    return i;
}

/**
 * Gets a queued token
 * @param queue_position Position in queue
 * @return Reference to unique pointer for the queued token
 * @throws exception::parsing_failure if queue_position >= than the number of queued items
 */
tokeniser::Markdown::TokenPtr_t & tokeniser::Markdown::getQueuedToken( size_t queue_position ) {
    try {
        return _queued_tokens.at( queue_position );

    } catch( const std::out_of_range &e ) {
        std::stringstream ss;
        ss << "[tokeniser::Markdown::getQueuedToken( " << queue_position << " )] "
           << "Index out of range of opened container blocks (size: " << _queued_tokens.size() << ")";
        throw exception::parsing_failure( ss.str() );
    }
}

/**
 * Queues a PARAGRAPH token
 * @param position Position
 * @return Position in queue
 */
size_t tokeniser::Markdown::queueParagraphToken( TextPos position ) {
    const auto i = queueToken( std::make_unique<ParagraphTk>( position ) );
    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
    return i;
}

/**
 * Queues a formatting token
 * @param position Position
 * @param fmt_char Formatting character
 * @return Count of formatting characters used (0 or 1)
 */
size_t tokeniser::Markdown::queueFormattingToken( TextPos position, char32_t fmt_char ) {
    const auto   new_fmt_type            = FormattingTk::resolveFormatType( { fmt_char } );
    const auto * last_opened_marker      = peekLastFormattingMarker();
    const auto   last_opened_marker_type = lastFormattingMarker();

    if( new_fmt_type == FormattingTk::Type::NONE ) {
        return 0; //EARLY RETURN
    }

    auto earliest_marker_it = std::find_if( _formatting_markers.begin(),
                                            _formatting_markers.end(),
                                            [new_fmt_type]( auto * fmt_ptr ) { return fmt_ptr->formatType() == new_fmt_type; } );

    if( earliest_marker_it != _formatting_markers.end() ) {
        _queued_tokens.emplace_back( std::make_unique<FormatEndTk>( fmt_char, position ) );
        _formatting_markers.erase( earliest_marker_it );

    } else {
        pushFormattingMarker(
            dynamic_cast<FormattingTk *> (
                _queued_tokens.emplace_back( std::make_unique<FormatBeginTk>( fmt_char, position ) ).get()
            )
        );
    }

    return 1;
}

/**
 * Queues a formatting token
 * @param position Position
 * @param fmt_char_1 First formatting character
 * @param fmt_char_2 Second formatting character
 * @return Count of formatting characters used (0, 1 or 2)
 */
size_t tokeniser::Markdown::queueFormattingToken( TextPos position, char32_t fmt_char_1, char32_t fmt_char_2 ) {
    auto findEarliestMarker = [this]( FormattingTk::Type fmt_type ) {
        return std::find_if( _formatting_markers.begin(),
                             _formatting_markers.end(),
                             [fmt_type]( auto * fmt_ptr ) { return fmt_ptr->formatType() == fmt_type; } );
    };

    const auto   new_fmt_type_single     = FormattingTk::resolveFormatType( { fmt_char_1 } );
    const auto   new_fmt_type_double     = FormattingTk::resolveFormatType( { fmt_char_1, fmt_char_2 } );
    const auto * last_opened_marker      = peekLastFormattingMarker();
    const auto   last_opened_marker_type = lastFormattingMarker();

    if( new_fmt_type_double == FormattingTk::Type::NONE ) { //only first char is a valid formatting character
        auto earliest_marker_it = findEarliestMarker( new_fmt_type_single );

        if( earliest_marker_it != _formatting_markers.end() && (*earliest_marker_it)->match( fmt_char_1 ) ) {
            _queued_tokens.emplace_back( std::make_unique<FormatEndTk>( fmt_char_1, position ) );
            _formatting_markers.erase( earliest_marker_it );

        } else {
            pushFormattingMarker(
                dynamic_cast<FormattingTk *> (
                    _queued_tokens.emplace_back( std::make_unique<FormatBeginTk>( fmt_char_1, position ) ).get()
                )
            );
        }

        return 1; //EARLY RETURN

    } else { //double char format
        auto earliest_marker_it = findEarliestMarker( new_fmt_type_double );

        if( earliest_marker_it != _formatting_markers.end() ) {
            _queued_tokens.emplace_back( std::make_unique<FormatEndTk>( fmt_char_1, fmt_char_2, position ) );
            _formatting_markers.erase( earliest_marker_it );

        } else {
            pushFormattingMarker(
                dynamic_cast<FormattingTk *> (
                    _queued_tokens.emplace_back( std::make_unique<FormatBeginTk>( fmt_char_1, fmt_char_2, position ) ).get()
                )
            );
        }

        return 2; //EARLY RETURN
    }
}

/**
 * Queues a ListTk token
 * @param type List type
 * @param position Position
 */
inline void tokeniser::Markdown::queueListToken( tokeniser::Markdown::ListType_e type, TextPos position ) {
    queueToken( std::make_unique<ListTk>( type, position ) );
    pushToOpenBlockStack( markdown::Block( _queued_tokens.back().get() ) );
}

/**
 * Queues a TABLE token
 * @param position Position
 * @return Position in queue
 */
inline size_t tokeniser::Markdown::queueTableToken( TextPos position ) {
    const auto i = queueToken( std::make_unique<TableTk>( position ) );
    _table_cache.starting_block_i = _open_container.size();
    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
    return i;
}

/**
 * Queues a TABLE_ROW token
 * @param position Position
 */
inline void tokeniser::Markdown::queueTableRowToken( TextPos position ) {
    queueToken( std::make_unique<TableRowTk>( position ) );
    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
}

/**
 * Queues a TABLE_HEADING token
 * @param position Position
 */
inline void tokeniser::Markdown::queueTableHeadingToken( TextPos position ) {
    const auto i = queueToken( std::make_unique<TableHeadingTk>( position ) );
    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
    _table_cache.heading_tokens.emplace_back( dynamic_cast<TableHeadingTk *>( _queued_tokens.at( i ).get() ) );
}

/**
 * Queues a TABLE_CELL token
 * @param position Position
 */
inline void tokeniser::Markdown::queueTableCellToken( TextPos position ) {
    queueToken( std::make_unique<TableCellTk>( position ) );
    pushToOpenBlockStack( markdown::Block( peekLastQueuedToken() ) );
    ++_table_cache.col_i;
}

/**
 * Queues a EOF token
 * @param position Position
 * @return Position in queue
 */
inline void tokeniser::Markdown::queueEndOfFileToken( TextPos position ) {
    _queued_tokens.emplace_back( std::make_unique<EndOfFileTk>( position ) );
    _eof = true;
}

/**
 * Closes the last opened block (deletes block entirely if leaf and empty) and dispatch queue whenever there are no more opened blocks
 * @param position Closing position
 */
inline void tokeniser::Markdown::closeLastOpenedBlock( TextPos position ) {
    if( !_open_blocks.empty() ) {
        const auto last = peekLastOpenBlockType();

        if( peekLastQueuedToken() != nullptr && ( last == Type_e::PARAGRAPH || last == Type_e::HEADING ) ) {
            trimQueuedSpaceTokens(); //i.e.: remove trailing TAB and SPACE characters if any
        }

        if( peekLastQueuedToken() != nullptr && specs::markdown::isLeafBlock( peekLastQueuedToken()->type() ) && peekLastQueuedToken()->type() == last ) {
            logError( peekLastQueuedToken()->position(),
                      ErrorCode::EMPTY_BLOCK,
                      blogator::to_string( peekLastQueuedToken()->type() )
            );
        }

        if( last == Type_e::BLOCKQUOTE && _actual_blockquote_lvl > 0 ) {
            --_actual_blockquote_lvl;
        } else if( last == Type_e::LIST && _list_spacing == ListSpacing_e::LOOSE ) {
            loosenLastListBlockSpacing();
        }

        queueToken( std::make_unique<BlockEndTk>( last, position ) );
        popOpenBlockStack();

        if( peekLastOpenBlockType() == Type_e::LIST_ITEM && _list_spacing == ListSpacing_e::MAYBE_LOOSE ) {
            setListSpacingFlag( ListSpacing_e::LOOSE );
        }
    }

    if( _open_blocks.empty() ) {
        dispatchQueuedTokens();
    }
}

/**
 * Closes the last opened leaf block and all open containers down to and including the one at given index
 * @param index Index in list of currently opened container blocks (even if out of range the last leaf block will be closed)
 * @param position Closing position
 */
inline void tokeniser::Markdown::closeOpenContainerBlocks( size_t index, TextPos position ) {
    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
        closeFormattingMarkers( position );
        closeLastOpenedBlock( position );
    }

    while( _open_container.size() > index ) {
        closeLastOpenedBlock( position );
    }
}

/**
 * Closes the last opened leaf block and all open containers down to a container positioned at or bellow the target position's column
 * @param target Target position (only column is used)
 * @param inclusive Include any container blocks positioned at target column
 * @param position Closing position
 */
void tokeniser::Markdown::closeOpenContainerBlocks( const TextPos &target, bool inclusive, TextPos position ) {
    if( specs::markdown::isLeafBlock( peekLastOpenBlockType() ) ) {
        closeFormattingMarkers( position );
        closeLastOpenedBlock( position );
    }

    if( inclusive ) {
        while( !_open_container.empty() && _open_container.back().column() >= target.col ) {
            closeLastOpenedBlock( position );
        }
    } else {
        while( !_open_container.empty() && _open_container.back().column() > target.col ) {
            closeLastOpenedBlock( position );
        }
    }
}

/**
 * Dispatches the queued tokens to the Markdown HTML builder with any associated errors and drops all formatting and section markers
 */
inline void tokeniser::Markdown::dispatchQueuedTokens() {
    while( !_queued_tokens.empty() ) {
        _html_builder.dispatch( std::move( _queued_tokens.front() ) );
        _queued_tokens.pop_front();
    }

    _error_count += _queued_errors.size();

    while( !_queued_errors.empty() ) {
        ParserLog::log( std::move( _queued_errors.front() ) );
        _queued_errors.pop_front();
    }

    clearFormattingMarkers();
    clearSectionMarkers();
}

/**
 * Pushes a new section marker for next queued token's position
 */
inline void tokeniser::Markdown::pushSectionMarker() {
    _section_markers.emplace_back(
        SectionMarker{ _queued_tokens.size(), _formatting_markers.size(), _queued_errors.size() }
    );
}

/**
 * Pushes a new section marker for current queued position
 * @param pos Position in token queue
 */
inline void tokeniser::Markdown::pushSectionMarker( size_t pos ) {
    _section_markers.emplace_back( SectionMarker{ pos, _formatting_markers.size(), _queued_errors.size() } );
}

/**
 * Get the value of the last section marker pushed
 * @return Queue position of last section marker or 0
 */
inline size_t tokeniser::Markdown::peekLastSectionMarker() const {
    if( !emptySectionMarkers() ) {
        return _section_markers.back().tk_i;
    }

    return 0;
}

/**
 * Checks if there are no section markers
 * @return Empty state
 */
inline bool tokeniser::Markdown::emptySectionMarkers() const {
    return _section_markers.empty();
}

/**
 * Pops the last added section marker
 */
inline void tokeniser::Markdown::popSectionMarker() {
    if( !_section_markers.empty() ) {
        _section_markers.pop_back();
    }
}

/**
 * Erases any queued tokens, formatting markers and errors from and including the position of the last marker (marker is then removed)
 */
inline void tokeniser::Markdown::clearPendingTokensFromLastSectionMarkerPosition() {
    if( !_section_markers.empty() ) {
        const auto & marker = _section_markers.back();
        _queued_tokens.erase( std::next( _queued_tokens.begin(), marker.tk_i ), _queued_tokens.end() );
        _formatting_markers.erase( std::next( _formatting_markers.begin(), marker.fmt_i ), _formatting_markers.end() );
        _queued_errors.erase( std::next( _queued_errors.begin(), marker.err_i ), _queued_errors.end() );
        _section_markers.pop_back();
    }
}

/**
 * Clears all section position markers
 */
inline void tokeniser::Markdown::clearSectionMarkers() {
    _section_markers.clear();
}

/**
 * Pushes a new formatting marker for current queued position
 * @param pos Pointer to formatting token
 */
inline void tokeniser::Markdown::pushFormattingMarker( FormattingTk * tk ) {
    _formatting_markers.emplace_back( tk );
}

/**
 * Gets the last opened formatting type
 * @return Last format type opened (or NONE)
 */
inline token::markdown::FormattingTk::Type tokeniser::Markdown::lastFormattingMarker() {
    return ( _formatting_markers.empty() ? FormattingTk::Type::NONE : _formatting_markers.back()->formatType() );
}

/**
 * Gets the last formatting marker added
 * @return Pointer to last formatting token (or nullptr)
 */
inline token::markdown::FormattingTk *tokeniser::Markdown::peekLastFormattingMarker() {
    if( !_formatting_markers.empty() ) {
        return _formatting_markers.back();
    }

    return nullptr;
}

/**
 * Close all formatting markers that are still opened
 * @param position Closing position
 */
inline void tokeniser::Markdown::closeFormattingMarkers( TextPos position ) {
    for( auto it = _formatting_markers.rbegin(); it != _formatting_markers.rend(); ++it ) {
        _queued_tokens.emplace_back( std::make_unique<FormatEndTk>( (*it)->text(), position ) );
    }

    _formatting_markers.clear();
}

/**
 * Clears all formatting markers
 */
inline void tokeniser::Markdown::clearFormattingMarkers() {
    _formatting_markers.clear();
}

/**
 * Sets the cached HTML block's type
 */
inline void tokeniser::Markdown::setHtmlBlockType( Markdown::HtmlBlockType_e type ) {
    _html_cache.block_type = type;
}

/**
 * Get the current HTML block's type
 * @return HTML block type enum
 */
inline specs::markdown::HtmlBlockType tokeniser::Markdown::currentHtmlBlockType() const {
    return _html_cache.block_type;
}

/**
 * Gets the current HTML pending tag
 * @return Reference for the current HTML pending tag
 */
inline tokeniser::markdown::HtmlTag &tokeniser::Markdown::pendingHtmlTag() {
    return _html_cache.pending_tag;
}

/**
 * Appends a character to the HTML character buffer
 * @param c Character
 */
inline void tokeniser::Markdown::appendToHtmlBuffer( char32_t c ) {
    _html_cache.buffer.emplace_back( c );
}

/**
 * Resets the buffer
 * @param position Source text position of the first character in the buffer
 */
void tokeniser::Markdown::resetHtmlBuffer( TextPos position ) {
    _html_cache.buffer.clear();
    _html_cache.buffer_pos = position;
}

/**
 * Flushes all characters in HTML buffer to the token queue
 */
inline void tokeniser::Markdown::flushHtmlBufferToQueue() {
    auto position = _html_cache.buffer_pos;

    for( size_t i = 0; i < _html_cache.buffer.size(); ++i ) {
        const auto c = _html_cache.buffer[ i ];

        queueToken( std::make_unique<CharacterTk>( c, position ) );

        if( unicode::ascii::isfeed( c ) ) {
            position = { position.line + 1, 1 };
        } else {
            position += { 0, 1 };
        }
    }

    _html_cache.buffer.clear();
}

/**
 * Clears HTML block cache
 * @param position Source text position of the first character in the buffer
 */
inline void tokeniser::Markdown::clearHtmlCache( TextPos position ) {
    _html_cache.block_type  = specs::markdown::HtmlBlockType::UNKNOWN;
    _html_cache.pending_tag = {};
    _html_cache.buffer.clear();
    _html_cache.buffer_pos = position;
}


/**
 * Pops the last queued token
 */
inline void tokeniser::Markdown::popLastQueuedToken() {
    if( !_queued_tokens.empty() ) {
        _queued_tokens.pop_back();
    }
}

/**
 * Trim any queued SPACE or TAB character tokens from the end of the token queue
 */
inline void tokeniser::Markdown::trimQueuedSpaceTokens() {
    if( peekLastQueuedToken() != nullptr ) {
        while( peekLastQueuedToken()->type() == Type_e::CHARACTER && Markdown::isWhiteSpace( peekLastQueuedToken()->text() ) ) {
            popLastQueuedToken();
        }
    }
}

/**
 * Checks if there are any token(s) in the queue
 * @return Queue not empty state
 */
inline bool tokeniser::Markdown::hasQueuedTokens() const {
    return !_queued_tokens.empty();
}

/**
 * Checks if a space is required after the queued token when more characters are added
 * @return Space separation requirement
 */
inline bool tokeniser::Markdown::requiresSpaceSeparatorToken() const {
    auto it = _queued_tokens.crbegin();

    while( it != _queued_tokens.crend() && !specs::markdown::isBlockToken( (*it)->type() ) ) {
        if( (*it)->type() == Type_e::CHARACTER ) {
            return ( ( *it )->text() != U" " ); //EARLY RETURN
        }

        ++it;
    }

    return false;
}

/**
 * Checks if given text position is an indented code-block start location
 * @return Is indented code block start location
 */
bool tokeniser::Markdown::isIndentedCodeBlock( const TextPos & position ) const {
    const auto & block = peekLastOpenContainerBlock();

    switch( block.type ) {
        case Type_e::BLOCKQUOTE: {
            return ( ( position.col - block.column() ) == 5 ); //EARLY RETURN
        } break;

        case Type_e::LIST:      [[fallthrough]];
        case Type_e::LIST_ITEM: { //indented code-blocks not supported in lists
            return false; //EARLY RETURN
        } break;

        default: break;
    }

    return ( position.col == 4 ); //no container blocks
}

/**
 * Gets a pointer to the last queued token
 * @return Pointer to last queued token or nullptr
 */
inline token::markdown::MarkdownTk * tokeniser::Markdown::peekLastQueuedToken() {
    if( !_queued_tokens.empty() ) {
        return _queued_tokens.back().get();
    }

    return nullptr;
}

/**
 * Checks if a escaped character is a markdown escapable character
 * @param c Escaped Character
 * @return Escapable status
 */
inline bool tokeniser::Markdown::isEscapable( char32_t c ) {
    static const auto MAP = std::set<char32_t>( {
        unicode::REVERSE_SOLIDUS,
        unicode::GRAVE_ACCENT,
        unicode::ASTERISK,
        unicode::UNDERSCORE,
        unicode::LEFT_CURLY_BRACKET,
        unicode::RIGHT_CURLY_BRACKET,
        unicode::LEFT_SQUARE_BRACKET,
        unicode::RIGHT_SQUARE_BRACKET,
        unicode::LEFT_PARENTHESIS,
        unicode::RIGHT_PARENTHESIS,
        unicode::NUMBER_SIGN,
        unicode::PLUS_SIGN,
        unicode::HYPHEN_MINUS,
        unicode::FULL_STOP,
        unicode::EXCLAMATION_MARK,
    } );

    return MAP.contains( c );
}

/**
 * Checks a string is composed uniquely of TAB and SPACE character(s)
 * @param str String to check
 * @return Only composed of either TAB or/and SPACE
 */
bool tokeniser::Markdown::isWhiteSpace( const std::u32string &str ) {
    for( auto c : str ) {
        if( c != unicode::TAB && c != unicode::SPACE ) {
            return false;
        }
    }

    return true;
}

/**
 * Checks if character is a formatting character
 * @param c Character
 * @return Formatting character state
 */
inline bool tokeniser::Markdown::isFormattingChar( char32_t c ) noexcept {
    return c == unicode::ASTERISK
        || c == unicode::UNDERSCORE
        || c == unicode::EQUALS_SIGN
        || c == unicode::TILDE
        || c == unicode::CIRCUMFLEX_ACCENT
        || c == unicode::GRAVE_ACCENT;
}

/**
 * Validate an name ID
 * @param name ID
 * @return Validation state
 */
inline bool tokeniser::Markdown::isValidIDName( const std::u32string &name ) {
    auto it = name.cbegin();

    if( it == name.cend() ) {
        return false; //EARLY RETURN
    }

    if( unicode::ascii::isdigit( *it ) ) {
        return false; //EARLY RETURN
    }

    if( *it++ == unicode::HYPHEN_MINUS && ( it == name.cend() || unicode::ascii::isdigit( *it ) ) ) {
        return false; //EARLY RETURN
    }

    while( it != name.cend() ) {
        if( !( unicode::ascii::isalnum( *it ) || *it == unicode::HYPHEN_MINUS || *it == unicode::UNDERSCORE ) ) {
            return false; //EARLY RETURN
        }

        ++it;
    }

    return true;
}