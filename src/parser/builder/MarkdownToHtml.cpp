#include "MarkdownToHtml.h"

#include <set>

#include "../specs/markdown/ListType.h"
#include "../specs/markdown/ErrorCode.h"
#include "../token/markdown/tokens.h"
#include "../logging/ParserLog.h"
#include "../../logger/Logger.h"
#include "../../exception/failed_expectation.h"
#include "../../exception/parsing_failure.h"

using namespace blogator::parser::builder;
using namespace blogator::parser::token::markdown;
using           blogator::parser::specs::markdown::ErrorCode;
using           blogator::parser::logging::ParserLog;

//template pre-declarations
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::INITIAL>( std::unique_ptr<token::markdown::MarkdownTk> token );
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::CONTENT>( std::unique_ptr<token::markdown::MarkdownTk> token );
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::IMAGE>( std::unique_ptr<token::markdown::MarkdownTk> token );
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::DEFINITION_LIST>( std::unique_ptr<token::markdown::MarkdownTk> token );
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::END_OF_FILE>( std::unique_ptr<token::markdown::MarkdownTk> token );

/**
 * Constructor
 */
MarkdownToHtml::MarkdownToHtml() :
    _error_count( 0 ),
    _insertion_mode( std::stack<InsertionMode_e>( { InsertionMode_e::INITIAL } ) )
{}

/**
 * Initialises the builder to an initial state
 * @param path Source file path
 */
void MarkdownToHtml::init( std::filesystem::path path ) {
    _error_count    = 0;
    _src_path       = std::move( path );
    _output         = std::make_unique<std::vector<char32_t>>();
    _insertion_mode = std::stack<InsertionMode_e>( { InsertionMode_e::INITIAL } );
}

/**
 * Process token
 * @param token Markdown token
 * @throw blogator::exception::failed_expectation when builder has not been initialised
 * @throw blogator::exception::parsing_failure when builder is in a EOF state
 */
void MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    if( !_output ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::builder::MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> )] "
            "Output is not set - builder require initialisation."
        );
    }

    if( currentInsertionMode() == InsertionMode_e::END_OF_FILE ) {
        throw exception::parsing_failure(
            "[parser::builder::MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> )] "
            "EOF had already been reached - content cannot be appended to current output in this state."
        );
    }

    if( !token ) {
        LOG_ERROR(
            "[parser::builder::MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> )] "
            "NULL token passed - skipped."
        );

        return; //EARLY RETURN
    }

    processContent( std::move( token ) );
}

/**
 * Extracts the html output
 * @return HTML text output
 */
std::unique_ptr<blogator::parser::U32Text> MarkdownToHtml::reset() {
    if( _output ) {
        if( currentInsertionMode() != InsertionMode_e::END_OF_FILE ) {
            LOG_WARNING(
                "[parser::builder::MarkdownToHtml::reset()] "
                "Incomplete state detected - current output never received an EOF token."
            );
        }

        auto u32text = std::make_unique<U32Text>( _src_path, *_output );

        _error_count = 0;
        _src_path.clear();
        _output.reset();
        _open_elements.clear();
        _footnotes.clear();

        return std::move( u32text );

    } else {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::builder::MarkdownToHtml::reset()] Builder had not been initiated."
        );
    }
}

/**
 * Gets the number of parsing error raised
 * @return Parsing error count
 */
size_t MarkdownToHtml::errors() const {
    return _error_count;
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Markdown error code
 */
void MarkdownToHtml::logError( TextPos position, int err_code ) {
    ++_error_count;
    logging::ParserLog::log( _src_path, specs::Context::MARKDOWN, err_code, position );
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Markdown error code
 * @param str Description string to append
 */
void MarkdownToHtml::logError( blogator::parser::TextPos position, int err_code, std::string str ) {
    ++_error_count;
    logging::ParserLog::log( _src_path, specs::Context::MARKDOWN, err_code, std::move( str ), position );
}

/**
 * Processes Markdown tokens
 * @param token Token to process
 * @throws blogator::exception::parsing_failure when unknown/invalid state is reached
 */
void MarkdownToHtml::processContent( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    try {
//        { //TODO remove
//            std::stringstream ss;
//            ss << *token;
//            LOG_CRITICAL( "Insertion mode: ", currentInsertionMode(), ": ",  ss.str() ); //TODO remove/temp
//        }
        switch( currentInsertionMode() ) {
            case InsertionMode_e::INITIAL:         { this->processContent<InsertionMode_e::INITIAL>( std::move( token ) );         } break;
            case InsertionMode_e::CONTENT:         { this->processContent<InsertionMode_e::CONTENT>( std::move( token ) );         } break;
            case InsertionMode_e::IMAGE:           { this->processContent<InsertionMode_e::IMAGE>( std::move( token ) );           } break;
            case InsertionMode_e::DEFINITION_LIST: { this->processContent<InsertionMode_e::DEFINITION_LIST>( std::move( token ) ); } break;
            case InsertionMode_e::END_OF_FILE:     { this->processContent<InsertionMode_e::END_OF_FILE>( std::move( token ) );     } break;

            default: {
                throw exception::parsing_failure(
                    "[parser::builder::MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> )] "
                    "Unknown insertion mode: " + blogator::to_string( currentInsertionMode() )
                );
            }
        }

    } catch( blogator::exception::failed_expectation &e ) {
        LOG_CRITICAL(
            "[parser::builders::TreeBuilder::processAsHTMLContent( std::unique_ptr<token::html5::HTML5Tk> )] "
            "Failed to process token."
        );
    }
}

/**
 * Sets up the initial document up to where tokens can be converted to HTML proper
 * @param token First token to process
 */
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::INITIAL>( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    //TODO add basic CSS markdown template?
    static const std::u32string DOCTYPE = U"<!DOCTYPE html>";
    _output->insert( _output->end(), DOCTYPE.cbegin(), DOCTYPE.cend() );
    openElement( HtmlElement_e::HTML5_HTML );
    openElement( HtmlElement_e::HTML5_BODY );
    pushInsertionMode( InsertionMode_e::CONTENT );
    processContent<InsertionMode_e::CONTENT>( std::move( token ) );
}

/**
 * Processes tokens as content
 * @param token Token to process
 * @throws blogator::exception::parsing_failure when there is an unexpected state during parsing
 */
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::CONTENT>( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    switch( token->type() ) {
        case TkType_e::HEADING: {
            const auto * tk = dynamic_cast<const HeadingTk *>( token.get() );

            if( tk->hasID() ) {
                switch( tk->level() ) {
                    case 1:  { openElement( HtmlElement_e::HTML5_H1, UR"(id=")" + tk->id() + UR"(")" ); } break;
                    case 2:  { openElement( HtmlElement_e::HTML5_H2, UR"(id=")" + tk->id() + UR"(")" ); } break;
                    case 3:  { openElement( HtmlElement_e::HTML5_H3, UR"(id=")" + tk->id() + UR"(")" ); } break;
                    case 4:  { openElement( HtmlElement_e::HTML5_H4, UR"(id=")" + tk->id() + UR"(")" ); } break;
                    case 5:  { openElement( HtmlElement_e::HTML5_H5, UR"(id=")" + tk->id() + UR"(")" ); } break;
                    case 6:  { openElement( HtmlElement_e::HTML5_H6, UR"(id=")" + tk->id() + UR"(")" ); } break;
                    default: {
                        logError( tk->position(), ErrorCode::INVALID_HEADING_LEVEL );
                        openElement( HtmlElement_e::HTML5_H6, UR"(id=")" + tk->id() + UR"(")" );
                    } break;
                }

            } else {
                switch( tk->level() ) {
                    case 1:  { openElement( HtmlElement_e::HTML5_H1 ); } break;
                    case 2:  { openElement( HtmlElement_e::HTML5_H2 ); } break;
                    case 3:  { openElement( HtmlElement_e::HTML5_H3 ); } break;
                    case 4:  { openElement( HtmlElement_e::HTML5_H4 ); } break;
                    case 5:  { openElement( HtmlElement_e::HTML5_H5 ); } break;
                    case 6:  { openElement( HtmlElement_e::HTML5_H6 ); } break;
                    default: {
                        logError( tk->position(), ErrorCode::INVALID_HEADING_LEVEL );
                        openElement( HtmlElement_e::HTML5_H6 );
                    } break;
                }
            }
        } break;

        case TkType_e::PARAGRAPH: {
            if( peekOpenElements() != HtmlElement_e::HTML5_LI || !peekOpenListTightness() ) {
                const auto * tk = dynamic_cast<const ParagraphTk *>( token.get() );

                if( tk->hasID() ) {
                    openElement( HtmlElement_e::HTML5_P, UR"(id=")" + tk->id() + UR"(")" );
                } else {
                    openElement( HtmlElement_e::HTML5_P, tk->id() );
                }
            } //else: tight list so not paragraphed
        } break;

        case TkType_e::HORIZONTAL_RULE: {
            sendToOutput( UR"(<hr>)" );
        } break;

        case TkType_e::CODE_BLOCK: {
            const auto * tk = dynamic_cast<const CodeBlockTk *>( token.get() );

            openElement( HtmlElement_e::HTML5_PRE );

            if( tk->lang().empty() ) {
                openElement( HtmlElement_e::HTML5_CODE );
            } else {
                openElement( HtmlElement_e::HTML5_CODE, UR"(class="code-block-)" + tk->lang() + UR"(")" );
            }
        } break;

        case TkType_e::HTML: { /* ignore token */ } break;

        case TkType_e::BLOCKQUOTE: {
            openElement( HtmlElement_e::HTML5_BLOCKQUOTE );
        } break;

        case TkType_e::LIST: {
            const auto * tk = dynamic_cast<const ListTk *>( token.get() );

            _open_list_tightness.push( tk->tight() );

            switch( tk->listType() ) {
                case specs::markdown::ListType::UL_HYPHEN:
                case specs::markdown::ListType::UL_ASTERISK:    [[fallthrough]];
                case specs::markdown::ListType::UL_PLUS_SIGN:   { openElement( HtmlElement_e::HTML5_UL );                              } break;
                case specs::markdown::ListType::UL_TASK:        { openElement( HtmlElement_e::HTML5_UL, UR"(class="task-list")" );     } break;
                case specs::markdown::ListType::OL_ALPHA_UPPER: { openElement( HtmlElement_e::HTML5_OL, UR"(type="A")" );              } break;
                case specs::markdown::ListType::OL_ALPHA_LOWER: { openElement( HtmlElement_e::HTML5_OL, UR"(type="a")" );              } break;
                case specs::markdown::ListType::OL_NUMERIC:     { openElement( HtmlElement_e::HTML5_OL, UR"(type="1")" );              } break;
                case specs::markdown::ListType::FOOTNOTE_DEFS:  { openElement( HtmlElement_e::HTML5_OL, UR"(class="footnote-defs")" ); } break;

                default: {
                    throw exception::parsing_failure(
                        "[parser::builder::MarkdownToHtml::processContent<InsertionMode_e::" + blogator::to_string( InsertionMode_e::CONTENT ) + ">( std::unique_ptr<token::markdown::MarkdownTk> )] "
                        "Unrecognised list type: " + blogator::to_string( tk->listType() )
                    );
                }
            }
        } break;

        case TkType_e::LIST_ITEM: {
            static const std::u32string CHECKBOX_UNCHECKED = UR"(<input class="task-list-item-checkbox" type="checkbox" disabled="">)";
            static const std::u32string CHECKBOX_CHECKED   = UR"(<input class="task-list-item-checkbox" type="checkbox" disabled="" checked>)";

            const auto * tk = dynamic_cast<const ListItemTk *>( token.get() );

            if( !tk->refID().empty() ) {
                auto it = _footnotes.find( tk->refID() );

                if( it == _footnotes.end() ) {
                    logError( tk->position(), ErrorCode::ORPHANED_FOOTNOTE_DEFINITION, ( "\"" + unicode::utf8::convert( tk->refID() ) + "\"" ) );
                    openElement( HtmlElement_e::HTML5_LI );

                } else {
                    openElement( HtmlElement_e::HTML5_LI, UR"(id="footnote-ref:)" + it->second.id + UR"(")" );
                    it->second.has_def = true;
                }

            } else {
                openElement( HtmlElement_e::HTML5_LI );
            }

            switch( tk->modality() ) {
                case ListItemTk::Modality::CHECKED:   { sendToOutput( CHECKBOX_CHECKED );   } break;
                case ListItemTk::Modality::UNCHECKED: { sendToOutput( CHECKBOX_UNCHECKED ); } break;
                default: break;
            }
        } break;

        case TkType_e::TABLE: {
            const auto * tk = dynamic_cast<const TableTk *>( token.get() );

            if( tk->hasID() ) {
                openElement( HtmlElement_e::HTML5_TABLE, UR"(id=")" + tk->id() + UR"(")" );
            } else {
                openElement( HtmlElement_e::HTML5_TABLE );
            }

            openElement( HtmlElement_e::HTML5_THEAD );
        } break;

        case TkType_e::TABLE_ROW: {
            openElement( HtmlElement_e::HTML5_TR );
        } break;

        case TkType_e::TABLE_HEADING: {
            openElement( HtmlElement_e::HTML5_TH );
        } break;

        case TkType_e::TABLE_CELL: {
            openElement( HtmlElement_e::HTML5_TD );
        } break;

        case TkType_e::DEFINITION_LIST_DT: {
            openElement( HtmlElement_e::HTML5_DL ); //<dl>
            pushInsertionMode( InsertionMode_e::DEFINITION_LIST );
            processContent<InsertionMode_e::DEFINITION_LIST>( std::move( token ) );
        } break;

        case TkType_e::DEFINITION_LIST_DD: {
            openElement( HtmlElement_e::HTML5_DL );  //<dl>
            openElement( HtmlElement_e::HTML5_DT );  //  <dt>
            closeElement( HtmlElement_e::HTML5_DT ); //  </dt>
            pushInsertionMode( InsertionMode_e::DEFINITION_LIST );
            processContent<InsertionMode_e::DEFINITION_LIST>( std::move( token ) );
        } break;

        case TkType_e::BLOCK_END: {
            closeBlock( dynamic_cast<const BlockEndTk *>( token.get() ) );
        } break;

        case TkType_e::CHARACTER: {
            sendToOutput( token->text() );
        } break;

        case TkType_e::LINE_BREAK: {
            sendToOutput( UR"(<br />)" );
        } break;

        case TkType_e::FORMAT_BEGIN: {
            const auto * tk = dynamic_cast<const FormatBeginTk *>( token.get() );

            switch( tk->formatType() ) {
                case FormattingTk::Type::BOLD:          { openElement( HtmlElement_e::HTML5_STRONG ); } break;
                case FormattingTk::Type::ITALIC:        { openElement( HtmlElement_e::HTML5_EM );     } break;
                case FormattingTk::Type::STRIKETHROUGH: { openElement( HtmlElement_e::HTML5_DEL );    } break;
                case FormattingTk::Type::HIGHLIGHT:     { openElement( HtmlElement_e::HTML5_MARK );   } break;
                case FormattingTk::Type::SUBSCRIPT:     { openElement( HtmlElement_e::HTML5_SUB );    } break;
                case FormattingTk::Type::SUPERSCRIPT:   { openElement( HtmlElement_e::HTML5_SUP );    } break;
                case FormattingTk::Type::INLINE_CODE_1: [[fallthrough]];
                case FormattingTk::Type::INLINE_CODE_2: { openElement( HtmlElement_e::HTML5_CODE );   } break;
                default:                                break;
            }
        } break;

        case TkType_e::FORMAT_END: {
            const auto * tk = dynamic_cast<const FormatEndTk *>( token.get() );

            switch( tk->formatType() ) {
                case FormattingTk::Type::BOLD:          { closeElement( HtmlElement_e::HTML5_STRONG ); } break;
                case FormattingTk::Type::ITALIC:        { closeElement( HtmlElement_e::HTML5_EM );     } break;
                case FormattingTk::Type::STRIKETHROUGH: { closeElement( HtmlElement_e::HTML5_DEL );    } break;
                case FormattingTk::Type::HIGHLIGHT:     { closeElement( HtmlElement_e::HTML5_MARK );   } break;
                case FormattingTk::Type::SUBSCRIPT:     { closeElement( HtmlElement_e::HTML5_SUB );    } break;
                case FormattingTk::Type::SUPERSCRIPT:   { closeElement( HtmlElement_e::HTML5_SUP );    } break;
                case FormattingTk::Type::INLINE_CODE_1: [[fallthrough]];
                case FormattingTk::Type::INLINE_CODE_2: { closeElement( HtmlElement_e::HTML5_CODE );   } break;
                default:                                break;
            }
        } break;

        case TkType_e::IMAGE: {
            initImageCache( std::move( token ) );
            pushInsertionMode( InsertionMode_e::IMAGE );
        } break;

        case TkType_e::HYPERLINK: {
            const auto * tk   = dynamic_cast<const HyperlinkTk *>( token.get() );
            openElement( HtmlElement_e::HTML5_A, UR"(href=")" + tk->url() + UR"(")" );
        } break;

        case TkType_e::FOOTNOTE_REF: {
            const auto * tk      = dynamic_cast<const FootnoteRefTk *>( token.get() );
            const auto   ref_id  = std::to_string( _footnotes.size() + 1 );

            auto [it, inserted] = _footnotes.try_emplace( tk->ref(), FootnoteRef( tk->position(), std::u32string( ref_id.cbegin(), ref_id.cend() ) ) );

            const auto html = UR"(<sup class="footnote-ref"><a href="#footnote-ref:)"
                            + it->second.id + UR"(">)"
                            + it->second.id
                            + UR"(</a></sup>)";

            sendToOutput( html );
        } break;

        case TkType_e::END_OF_FILE: {
            processContent<InsertionMode_e::END_OF_FILE>( std::move( token ) );
        } break;

        default: {
            throw exception::parsing_failure(
                "[parser::builder::MarkdownToHtml::processContent<InsertionMode_e::" + blogator::to_string( InsertionMode_e::CONTENT ) + ">( std::unique_ptr<token::markdown::MarkdownTk> )] "
                "Unrecognised token: " + blogator::to_string( token->type() )
            );
        }
    }
}

/**
 * Processes tokens within the context of an image declaration
 * @param token Token to process
 * @throws blogator::exception::parsing_failure when an unexpected state is encountered
 */
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::IMAGE>( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    switch( token->type() ) {
        case TkType_e::CHARACTER: {
            appendToImageCacheBuffer( token->text() );
        } break;

        case TkType_e::BLOCK_END: {
            const auto * tk  = dynamic_cast<const BlockEndTk *>( token.get() );

            if( tk->blockType() == TkType_e::IMAGE ) {
                const auto * img_tk = dynamic_cast<const ImageTk *>( _image_cache.token.get() );
                const auto   html   = UR"(<img src=")" + img_tk->src()
                                    + UR"(" alt=")" + std::u32string( _image_cache.buffer.cbegin(), _image_cache.buffer.cend() )
                                    + UR"(">)";

                sendToOutput( html );
                clearImageCache();
                popInsertionMode();
            } else {
                throw exception::parsing_failure(
                    "[parser::builder::MarkdownToHtml::processContent<InsertionMode_e::" + blogator::to_string( InsertionMode_e::IMAGE ) + ">( std::unique_ptr<token::markdown::MarkdownTk> )] "
                    "Unexpected block type '" + blogator::to_string( token->type() ) + "' in token '" + blogator::to_string( tk->blockType() ) + "'."
                );
            }
        } break;

        default: {
            throw exception::parsing_failure(
                "[parser::builder::MarkdownToHtml::processContent<InsertionMode_e::" + blogator::to_string( InsertionMode_e::IMAGE ) + ">( std::unique_ptr<token::markdown::MarkdownTk> )] "
                "Unexpected token for '" + blogator::to_string( InsertionMode_e::IMAGE ) + "."
            );
        }
    }
}

/**
 * Processes tokens withing the context of a definition list
 * @param token Token to process
 */
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::DEFINITION_LIST>( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    switch( token->type() ) {
        case TkType_e::DEFINITION_LIST_DT: { openElement( HtmlElement_e::HTML5_DT );                        } break;
        case TkType_e::DEFINITION_LIST_DD: { openElement( HtmlElement_e::HTML5_DD );                        } break;
        case TkType_e::BLOCK_END:          { closeBlock( dynamic_cast<const BlockEndTk *>( token.get() ) ); } break;

        default: {
            if( peekOpenElements() == HtmlElement_e::HTML5_DL ) {
                closeElement( HtmlElement_e::HTML5_DL );
                popInsertionMode();
            }

            processContent<InsertionMode_e::CONTENT>( std::move( token ) );
        } break;
    }
}

/**
 * Processes an end-of-file state for the current document
 * @param token EndOfFileTk token
 */
template<> void MarkdownToHtml::processContent<MarkdownToHtml::InsertionMode_e::END_OF_FILE>( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    while( !_insertion_mode.empty() ) {
        _insertion_mode.pop();
    }

    pushInsertionMode( InsertionMode_e::END_OF_FILE );

    while( !_open_elements.empty() ) {
        MarkdownToHtml::closeElement( _open_elements.back() );
    }

    for( const auto & footnote : _footnotes ) {
        if( !footnote.second.has_def ) {
            logError( footnote.second.position, ErrorCode::ORPHANED_FOOTNOTE_REFERENCE, ( "\"" + unicode::utf8::convert( footnote.first ) + "\"" ) );
        }
    }
}

/**
 * Push an insertion mode to the stack
 * @param mode Insertion mode
 */
inline void MarkdownToHtml::pushInsertionMode( MarkdownToHtml::InsertionMode_e mode ) {
    _insertion_mode.push( mode );
}

/**
 * Pops the last insertion mode off the stack
 */
inline void MarkdownToHtml::popInsertionMode() {
    if( !_insertion_mode.empty() ) {
        _insertion_mode.pop();
    }
}

/**
 * Gets the last inserted insertion mode on the stack
 * @return Last insertion mode or default ('CONTENT')
 */
inline auto MarkdownToHtml::currentInsertionMode() const -> MarkdownToHtml::InsertionMode_e {
    if( !_insertion_mode.empty() ) {
        return _insertion_mode.top();
    }

    return InsertionMode_e::CONTENT;
}

/**
 * Peeks last open element
 * @return Last open element or 'UNKNOWN' when list is empty
 */
inline auto MarkdownToHtml::peekOpenElements() const -> MarkdownToHtml::HtmlElement_e {
    if( !_open_elements.empty() ) {
        return _open_elements.back();
    }

    return HtmlElement_e::UNKNOWN;
}

/**
 * Pop the last flag off the stack of opened list tightness
 */
inline void MarkdownToHtml::popOpenListTightnessStack() {
    if( !_open_list_tightness.empty() ) {
        _open_list_tightness.pop();
    }
}

/**
 * Peeks last opened list's tightness
 * @return Tightness flag (default: true)
 */
inline auto MarkdownToHtml::peekOpenListTightness() const -> bool {
    return ( _open_list_tightness.empty() ? true : _open_list_tightness.top() );
}

/**
 * Prints an HTML input tag to the output
 * @param element HTML element type
 */
inline void MarkdownToHtml::openElement( MarkdownToHtml::HtmlElement_e element ) {
    const std::u32string str = U"<" + blogator::to_u32string( element ) + U">";
    _output->insert( _output->end(), str.cbegin(), str.cend() );
    _open_elements.push_back( element );
}

/**
 * Prints an HTML input tag to the output
 * @param element HTML element type
 * @param property Property string
 */
inline void MarkdownToHtml::openElement( MarkdownToHtml::HtmlElement_e element, const std::u32string & property ) {
    if( property.empty() ) {
        const std::u32string str = UR"(<)" + blogator::to_u32string( element ) + UR"(>)";
        _output->insert( _output->end(), str.cbegin(), str.cend() );
    } else {
        const std::u32string str = UR"(<)" + blogator::to_u32string( element ) + UR"( )" + property + UR"(>)";
        _output->insert( _output->end(), str.cbegin(), str.cend() );
    }

    _open_elements.push_back( element );
}

/**
 * Prints an HTML output tag to the output
 * @param element HTML element type
 */
inline void MarkdownToHtml::closeElement( MarkdownToHtml::HtmlElement_e element ) {
    if( _open_elements.empty() ) {
        LOG_ERROR(
            "[parser:builder::MarkdownToHtml::closeElement( '", element, "' )] "
            "Stack of open elements is empty."
        );
    } else if( _open_elements.back() != element ) {
        LOG_ERROR(
            "[parser:builder::MarkdownToHtml::closeElement( '", element, "' )] "
            "Element does not match last opened element in stack: '", _open_elements.back(), "'"
        );
    } else {
        const std::u32string str = U"</" + blogator::to_u32string( element ) + U">";
        _output->insert( _output->end(), str.cbegin(), str.cend() );
        _open_elements.pop_back();
    }
}

/**
 * Closes the last opened element on the stack of open elements
 * @return Closed element or 'UNKNOWN' if stack is empty
 */
inline auto MarkdownToHtml::closeLastElement() -> MarkdownToHtml::HtmlElement_e {
    if( !_open_elements.empty() ) {
        const auto           el  = _open_elements.back();
        const std::u32string str = U"</" + blogator::to_u32string( el ) + U">";
        _output->insert( _output->end(), str.cbegin(), str.cend() );
        _open_elements.pop_back();
        return el;
    }

    return HtmlElement_e::UNKNOWN;
}

/**
 * Prints a string to the output
 * @param str String
 */
inline void MarkdownToHtml::sendToOutput( const std::u32string &str ) {
    _output->insert( _output->end(), str.cbegin(), str.cend() );
}

/**
 * Closes a block
 * @param token BlockEndTk token
 */
inline void MarkdownToHtml::closeBlock( const BlockEndTk * token ) {
    switch( token->blockType() ) {
        case TkType_e::HEADING: {
            const auto last_opened_element = peekOpenElements();

            if( last_opened_element == HtmlElement_e::HTML5_H1 ||
                last_opened_element == HtmlElement_e::HTML5_H2 ||
                last_opened_element == HtmlElement_e::HTML5_H3 ||
                last_opened_element == HtmlElement_e::HTML5_H4 ||
                last_opened_element == HtmlElement_e::HTML5_H5 ||
                last_opened_element == HtmlElement_e::HTML5_H6 )
            {
                closeElement( last_opened_element );

            } else {
                LOG_ERROR(
                    "[parser:builder::MarkdownToHtml::closeBlock( const BlockEndTk * )] "
                    "Last opened element '", last_opened_element, "' is not a heading."
                );
            }
        } break;

        case TkType_e::PARAGRAPH: {
            if( peekOpenElements() != HtmlElement_e::HTML5_LI || !peekOpenListTightness() ) {
                closeLastElement();
            } //else: <li> paragraph close in tight list
        } break;

        case TkType_e::CODE_BLOCK: {
            closeElement( HtmlElement_e::HTML5_CODE );
            closeElement( HtmlElement_e::HTML5_PRE );
        } break;

        case TkType_e::HTML: { /* ignore token */ } break;

        case TkType_e::BLOCKQUOTE: {
            closeElement( HtmlElement_e::HTML5_BLOCKQUOTE );
        } break;

        case TkType_e::LIST: {
            if( isListElement( peekOpenElements() ) ) {
                popOpenListTightnessStack();
                closeLastElement();

            } else {
                LOG_ERROR(
                    "[parser:builder::MarkdownToHtml::closeBlock( const BlockEndTk * )] "
                    "Last opened element '", peekOpenElements(), "' is not a list."
                );
            }
        } break;

        case TkType_e::LIST_ITEM: {
            closeElement( HtmlElement_e::HTML5_LI );
        } break;

        case TkType_e::TABLE: {
            closeElement( HtmlElement_e::HTML5_TBODY );
            closeElement( HtmlElement_e::HTML5_TABLE );
        } break;

        case TkType_e::TABLE_ROW:          {
            closeElement( HtmlElement_e::HTML5_TR );

            if( peekOpenElements() == HtmlElement_e::HTML5_THEAD ) {
                closeElement( HtmlElement_e::HTML5_THEAD );
                openElement( HtmlElement_e::HTML5_TBODY );
            }
        } break;

        case TkType_e::TABLE_HEADING:      { closeElement( HtmlElement_e::HTML5_TH );    } break;
        case TkType_e::TABLE_CELL:         { closeElement( HtmlElement_e::HTML5_TD );    } break;
        case TkType_e::DEFINITION_LIST_DT: { closeElement( HtmlElement_e::HTML5_DT );    } break;
        case TkType_e::DEFINITION_LIST_DD: { closeElement( HtmlElement_e::HTML5_DD );    } break;
        case TkType_e::HYPERLINK:          { closeElement( HtmlElement_e::HTML5_A );     } break;

        default: {
            LOG_ERROR(
                "[parser:builder::MarkdownToHtml::closeBlock( const BlockEndTk * )] "
                "Unrecognised block: '", token->blockType(), "'"
            );
        } break;
    }
}

/**
 * Initialises the image cache
 * @param token Token to set after reset
 */
inline void MarkdownToHtml::initImageCache( std::unique_ptr<token::markdown::MarkdownTk> token ) {
    _image_cache.token = std::move( token );
    _image_cache.buffer.clear();
}

/**
 * Clears the image cache
 */
inline void MarkdownToHtml::clearImageCache() {
    _image_cache.token.reset();
    _image_cache.buffer.clear();
}

/**
 * Appends string to cache character buffer
 * @param str String to append
 */
inline void MarkdownToHtml::appendToImageCacheBuffer( const std::u32string &str ) {
    _image_cache.buffer.insert( _image_cache.buffer.end(), str.cbegin(), str.cend() );
}

/**
 * Checks element is of a list category
 * @param el HTML element
 * @return List category state
 */
bool MarkdownToHtml::isListElement( MarkdownToHtml::HtmlElement_e el ) {
    static const auto LIST_ELEMENTS = std::set<HtmlElement_e>( {
        HtmlElement_e::HTML5_UL, //UL_HYPHEN, UL_ASTERISK, UL_PLUS_SIGN, UL_TASK, FOOTNOTE_DEFS
        HtmlElement_e::HTML5_OL, //OL_ALPHA_UPPER, OL_ALPHA_LOWER, OL_NUMERIC
    } );

    return LIST_ELEMENTS.contains( el );
}

/**
 * FootnoteRef Constructor
 * @param position Text position
 * @param ref_id Reference ID number as a string
 */
MarkdownToHtml::FootnoteRef::FootnoteRef( TextPos position, std::u32string ref_id ) :
    position( position ),
    id( std::move( ref_id ) ),
    has_def( false )
{}
