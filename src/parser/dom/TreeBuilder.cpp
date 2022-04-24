#include "TreeBuilder.h"

#include <unordered_map>
#include <utility>

#include "../../logger/Logger.h"
#include "../specs/infra/Attribute.h"
#include "../logging/ParserLog.h"
#include "../encoding/Format.h"
#include "node/Attr.h"
#include "node/CDATASection.h"
#include "node/Comment.h"
#include "node/Document.h"
#include "node/DocumentFragment.h"
#include "node/DocumentType.h"
#include "node/Element.h"
#include "node/Text.h"

using namespace blogator::parser::dom;

//template pre-declarations
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::INITIAL>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::BEFORE_HTML>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::BEFORE_HEAD>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_HEAD>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_HEAD_NOSCRIPT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_HEAD>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::TEXT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TABLE>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TABLE_TEXT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_CAPTION>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_COLUMN_GROUP>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TABLE_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_ROW>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_CELL>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_SELECT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_SELECT_IN_TABLE>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TEMPLATE>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_FRAMESET>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_FRAMESET>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_AFTER_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_AFTER_FRAMESET>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );


/**
 * Constructor
 * @param scripting Scripting flag (default=false)
 */
TreeBuilder::TreeBuilder( bool scripting ) :
    _error_count( 0 ),
    _strict_validation( ValidationState::ON ),
    _scripting( scripting ),
    _fragment_case( false ),
    _foster_parenting( false ),
    _frameset_ok( true ),
    _parser_cannot_change_the_mode( false ),
    _ignore_next_token_in_body_LF( false ),
    _document( nullptr ),
    _context_element( nullptr ),
    _head_element( nullptr ),
    _form_element( nullptr ),
    _original_insert_mode( InsertionMode_e::INITIAL ),
    _current_insert_mode( InsertionMode_e::INITIAL )
{}

/**
 * Sets the flag for 'strict checking'
 * @param flag Switch element and attribute names validation (default: true)
 */
void TreeBuilder::setStrictChecking( bool flag ) {
    _strict_validation = ( flag ? ValidationState::ON : ValidationState::OFF );
}

/**
 * Sets the tokeniser state change callback function
 * @param cb Callback
 */
void TreeBuilder::setTokeniserStateChangeCallback( std::function<void( specs::infra::TokeniserState )> cb ) {
    _changeHTML5TokeniserState_cb = std::move( cb );
}

/**
 * Sets the self-closing tag acknowledgement callback function
 * @param cb Callback
 */
void TreeBuilder::setSelfClosingTagAckCallback( std::function<void()> cb ) {
    _acknowledgeSelfClosingTag_cb = std::move( cb );
}

/**
 * Initialises the parser into a normal document parsing mode
 * @param document Document node to build from
 * @throws exception::failed_expectation when there is an unexpected state
 */
void TreeBuilder::init( std::unique_ptr<node::Document> document ) {
    if( !document ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( std::unique_ptr<node::Document> )] "
            "Document is NULL."
        );
    }

    if( !_changeHTML5TokeniserState_cb ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( std::unique_ptr<node::Document> )] "
            "Tokeniser state change callback is not set."
        );
    }

    if( !_acknowledgeSelfClosingTag_cb ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( std::unique_ptr<node::Document> )] "
            "Self-closing tag acknowledgment callback is not set."
        );
    }

    LOG(
        "[parser::dom::TreeBuilder::init( std::unique_ptr<node::Document> )] "
        "Building '", document->type(), "' document (src: ", document->filepath() , ")."
    );

    _error_count                   = 0;
    _fragment_case                 = false;
    _foster_parenting              = false;
    _frameset_ok                   = true;
    _parser_cannot_change_the_mode = false;
    _ignore_next_token_in_body_LF  = false;
    _document                      = std::move( document );
    _context_element               = nullptr;
    _head_element                  = nullptr;
    _form_element                  = nullptr;
    _open_elements                 = std::deque<node::Element *>();
    _original_insert_mode          = InsertionMode_e::INITIAL;
    _current_insert_mode           = InsertionMode_e::INITIAL;
    _template_insert_modes         = std::stack<InsertionMode_e>();
    _formatting_elements           = std::deque<node::Element *>();
    _pending_table_char_tokens     = TreeBuilder::CharacterBuffer_t();
}

/**
 * Initialises the parser into a 'fragment case' mode
 * @param context_element Pointer to context element
 * @param position Text position of the context (default={0,0})
 * @throws exception::failed_expectation when there is an unexpected state
 */
void TreeBuilder::init( node::Element * context_element, TextPos position ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-html-fragments
    if( _document ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( node::Element *, " + position.str() + " )] "
            "Document is already set. Builder requires resetting before using it for 'fragment case'."
        );
    }

    if( !_changeHTML5TokeniserState_cb ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( node::Element *, " + position.str() + " )] "
            "Tokeniser state change callback is not set."
        );
    }

    if( !_acknowledgeSelfClosingTag_cb ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( node::Element *, " + position.str() + " )] "
            "Self-closing tag acknowledgment callback is not set."
        );
    }

    if( context_element == nullptr ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( node::Element *, " + position.str() + " )] "
            "Context Element for 'fragment case' is NULL."
        );
    }

    _error_count                   = 0;
    _fragment_case                 = true;
    _foster_parenting              = false;
    _frameset_ok                   = true;
    _parser_cannot_change_the_mode = false;
    _ignore_next_token_in_body_LF  = false;
    _context_element               = context_element;
    _head_element                  = nullptr;
    _form_element                  = nullptr;
    _open_elements                 = std::deque<node::Element *>();
    _original_insert_mode          = InsertionMode_e::INITIAL;
    _current_insert_mode           = InsertionMode_e::INITIAL;
    _template_insert_modes         = std::stack<InsertionMode_e>();
    _original_insert_mode          = InsertionMode_e::INITIAL;
    _current_insert_mode           = InsertionMode_e::INITIAL;
    _formatting_elements           = std::deque<node::Element *>();
    _pending_table_char_tokens     = TreeBuilder::CharacterBuffer_t();

    const auto * context_document = _context_element->ownerDocument();
    const auto   context_enum     = context_element->elementType();

    if( context_document == nullptr ) {  //default fallback
        _document = std::make_unique<node::Document>( blogator::to_u32string( parser::encoding::Format::UTF8 ),
                                                      specs::infra::ContentType::APPLICATION_XHTML_XML,
                                                      _context_element->baseURI(),
                                                      std::filesystem::path(),
                                                      node::Document::Type::HTML,
                                                      QuirksMode::NO_QUIRKS );
    } else {
        _document = std::make_unique<node::Document>( context_document->characterSet(),
                                                      specs::infra::ContentType::APPLICATION_XHTML_XML,
                                                      context_document->URL(),
                                                      context_document->filepath(),
                                                      node::Document::Type::HTML,
                                                      context_document->quirksMode() );
    }

    switch( context_enum ) {
        case Element_e::HTML5_TITLE:    [[fallthrough]];
        case Element_e::HTML5_TEXTAREA: {
            _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::RCDATA );
        } break;

        case Element_e::HTML5_STYLE:        [[fallthrough]];
        case Element_e::DEPR_HTML_XMP:      [[fallthrough]];
        case Element_e::HTML5_IFRAME:       [[fallthrough]];
        case Element_e::DEPR_HTML_NOEMEBED: [[fallthrough]];
        case Element_e::DEPR_HTML_NOFRAMES: {
            _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::RAWTEXT );
        } break;

        case Element_e::HTML5_SCRIPT: {
            _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::SCRIPT_DATA );
        } break;

        case Element_e::HTML5_NOSCRIPT: {
            if( _scripting ) {
                _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::RAWTEXT );
            }
        } break;

        case Element_e::DEPR_HTML_PLAINTEXT: {
            _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::PLAIN_TEXT );
        }

        default: break;
    }

    auto * root = _document->appendChild( std::make_unique<node::Element>( Element_e::HTML5_HTML ) );

    if( root == nullptr ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::init( node::Element *, " + position.str() + " )] "
            "Failed to create root HTML element for 'fragment case'."
        )
    }

    this->pushOpenElements( dynamic_cast<node::Element *>( root ) );

    if( context_enum == Element_e::HTML5_TEMPLATE ) {
        this->pushTemplateInsertionModes( InsertionMode_e::IN_TEMPLATE );
    }

    this->resetInsertionModeAppropriately();

    auto * curr = _context_element;

    while( curr != nullptr ) {
        if( curr->elementType() == Element_e::HTML5_FORM ) {
            this->setFormElementPtr( curr );
            break;
        }

        curr = curr->parentElement();
    }
}

/**
 * Process token
 * @param token HTML5 token
 * @throw blogator::exception::failed_expectation when no Document has been set
 */
void TreeBuilder::dispatch( std::unique_ptr<token::html5::HTML5Tk> token ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#tree-construction
    using blogator::parser::specs::infra::TokenType;

    if( !_document ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::dispatch( std::unique_ptr<token::html5::HTML5Tk> )] "
            "No Document set - builder require initialisation with one."
        );
    }

    if( !token ) {
        LOG_ERROR(
            "[parser::dom::TreeBuilder::dispatch( std::unique_ptr<token::html5::HTML5Tk> )] "
            "NULL token passed - token will be ignored."
        );

        return; //EARLY RETURN
    }

    auto * adj_curr_node = this->adjustedCurrentNode();
    auto   adj_curr_ns   = Namespace_e::UNKNOWN;

    if( _open_elements.empty() ) {
        goto process_as_html;
    }

    if( adj_curr_node != nullptr ) {
        adj_curr_ns = adj_curr_node->getNamespaceEnum();

        if( adj_curr_ns == Namespace_e::HTML ) {
            goto process_as_html;
        }

        if( adj_curr_ns == Namespace_e::MATHML ) {
            static const auto SVG               = blogator::to_u32string( specs::infra::Element::SVG );
            static const auto MATHML_MALIGNMARK = blogator::to_u32string( specs::infra::Element::MATHML_MALIGNMARK );
            static const auto MATHML_MGLYPH     = blogator::to_u32string( specs::infra::Element::MATHML_MGLYPH );

            if( TreeBuilder::isMathMLIntegrationPoint( adj_curr_node ) ) {
                if( token->type() == TokenType::START_TAG && !( token->text() == MATHML_MGLYPH || token->text() == MATHML_MALIGNMARK ) ) {
                    goto process_as_html;
                }

                if( token->type() == TokenType::CHARACTER ) {
                    goto process_as_html;
                }
            }

            if( adj_curr_node->elementType() == Element_e::MATHML_ANNOTATION_XML ) {
                if( token->type() == TokenType::START_TAG && token->text() == SVG ) {
                    goto process_as_html;
                }
            }
        }

        if( TreeBuilder::isHTMLIntegrationPoint( adj_curr_node ) ) {
            if( token->type() == TokenType::START_TAG || token->type() == TokenType::CHARACTER ) {
                goto process_as_html;
            }
        }
    }

    if( token->type() == TokenType::END_OF_FILE ) {
        goto process_as_html;
    }

    this->processAsForeignContent( std::move( token ), adj_curr_ns );
    return; //DEFAULT

    //--------------
    process_as_html: {
        this->processAsHTMLContent( std::move( token ) );
        return; //EARLY RETURN
    };
}

/**
 * Extracts the Document node
 * @return Document node
 */
std::unique_ptr<blogator::parser::dom::node::Document> TreeBuilder::reset() {
    return std::move( _document );
}

/**
 * Gets the adjusted current node's namespace
 * @return Pair consisting of a 'exists' flag and a namespace
 */
std::pair<bool, blogator::parser::specs::infra::Namespace> TreeBuilder::adjustedCurrentNodeNS() const {
    if( _open_elements.empty() ) {
        return { false, Namespace_e::NONE }; //EARLY RETURN
    }

    if( _fragment_case && _open_elements.size() == 1 ) {
        return { true, _context_element->getNamespaceEnum() }; //EARLY RETURN
    }

    return { true, const_cast<TreeBuilder *>( this )->currentNode()->getNamespaceEnum() };
}

/**
 * Gets the number of parsing error raised
 * @return Parsing error count
 */
size_t TreeBuilder::errors() const {
    return _error_count;
}

/**
 * Creates a UTF-8 encoded HTML document
 * @param url Document URL
 * @param origin Document origin path
 * @param quirks Quirks (default: NO_QUIRKS)
 * @return New Document node
 */
std::unique_ptr<node::Document> TreeBuilder::createHtmlDocument( USVString_t url, std::filesystem::path origin, QuirksMode quirks ) {
    return std::make_unique<node::Document>( blogator::to_u32string( encoding::Format::UTF8 ),
                                             specs::infra::ContentType::APPLICATION_XHTML_XML,
                                             std::move( url ),
                                             std::move( origin ),
                                             node::Document::Type::HTML,
                                             quirks );
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Infra error code
 * @param desc Text description
 */
void TreeBuilder::logError( TextPos position, int err_code, const std::string & desc ) {
    std::stringstream ss;
    ss << "{ '" << _current_insert_mode  << "' | " << desc << " }";
    parser::logging::ParserLog::log( _document->filepath(), specs::Context::HTML5, err_code, ss.str(), position );
    ++_error_count;
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Infra error code
 * @param token Subject Token
 */
void TreeBuilder::logError( int err_code, const std::unique_ptr<token::html5::HTML5Tk> & token ) {
    using blogator::unicode::utf8::convert;

    std::stringstream ss;
    ss << "{ '" << _current_insert_mode << "'";

    switch( token->type() ) {
        case specs::infra::TokenType::START_TAG: {
            ss << " | <" << convert( token->text() ) << ">";
        } break;

        case specs::infra::TokenType::END_TAG: {
            ss << " | </" << convert( token->text() ) << ">";
        } break;

        case specs::infra::TokenType::CHARACTER: {
            ss << " | \"" << convert( token->text() ) << "\"";
        } break;

        default: {
            ss << " | [" << token->type() << "]";
        } break;
    }

    ss << " }";

    parser::logging::ParserLog::log( _document->filepath(), specs::Context::HTML5, err_code, ss.str(), token->position() );
    ++_error_count;
}

/**
 * Processes a token within foreign content
 * @param token HTML5 token
 * @param adj_ns Adjusted Namespace
 */
void TreeBuilder::processAsForeignContent( std::unique_ptr<token::html5::HTML5Tk> token, Namespace_e adj_ns ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inforeign
    static const auto SVG_SCRIPT = blogator::to_u32string( specs::infra::Element::SVG_SCRIPT );

    switch( token->type() ) {
        case specs::infra::TokenType::UNKNOWN: {
            LOG_ERROR(
                "[parser::dom::TreeBuilder::processAsForeignContent( std::unique_ptr<token::html5::HTML5Tk> )] "
                "Token type is UNKNOWN - skipped."
            );
        } break;

        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::INVALID_DOCTYPE_TOKEN_IN_FOREIGN_CONTENT, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );

            if( TreeBuilder::validateStartTagInForeignContent( *tk ) ) {
                auto * new_node = this->insertForeignElement( tk, adj_ns );

                if( tk->selfclosing() ) {
                    if( tk->name() == SVG_SCRIPT && new_node->getNamespaceEnum() == Namespace_e::SVG ) {
                        _acknowledgeSelfClosingTag_cb();
                        this->popOpenElements();
                        //"...then act as described in the steps for a "script" end tag below."
                        //note: script execution not implemented for blogator
                    } else {
                        this->popOpenElements();
                        _acknowledgeSelfClosingTag_cb();
                    }
                }

            } else {
                this->logError( specs::infra::ErrorCode::INVALID_STARTTAG_TOKEN_IN_FOREIGN_CONTENT, token );

                const auto html_ns_id = _document->namespaceMap().setNamespace( Namespace_e::HTML );

                while( this->currentNode() != nullptr &&
                       ( !TreeBuilder::isMathMLIntegrationPoint( this->currentNode() ) &&
                         !TreeBuilder::isHTMLIntegrationPoint( this->currentNode() )   &&
                         this->currentNode()->namespaceID() != html_ns_id ) )
                {
                    this->popOpenElements();
                }

                this->processAsHTMLContent( std::move( token ) );
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );

            if( TreeBuilder::validateEndTagInForeignContent( *tk ) ) {
                if( tk->name() == SVG_SCRIPT && this->currentNode()->getNamespaceEnum() == Namespace_e::SVG ) {
                    this->popOpenElements();
                    //note: script execution not implemented for blogator

                } else {
                    if( _open_elements.empty() ) { //for safety
                        return; //EARLY RETURN
                    }

                    auto       node_it    = this->currentNodeIterator();
                    const auto token_name = blogator::unicode::ascii::tolower( tk->name() );

                    if( blogator::unicode::ascii::tolower( (*node_it)->qualifiedName() ) != token_name ) {
                        this->logError( specs::infra::ErrorCode::MISMATCHING_ENDTAG_IN_FOREIGN_CONTENT, token );
                    }

                    while( (*node_it) != _open_elements.back() ) {
                        if( blogator::unicode::ascii::tolower( (*node_it)->qualifiedName() ) == token_name ) {
                            this->popOpenElementsUntil( token_name, true );
                            return; //EARLY RETURN
                        }

                        std::advance( node_it, 1 );

                        if( (*node_it)->getNamespaceEnum() == Namespace_e::HTML ) {
                            break;
                        }
                    }

                    this->processAsHTMLContent( std::move( token ) );
                }

            } else {
                this->logError( specs::infra::ErrorCode::INVALID_ENDTAG_TOKEN_IN_FOREIGN_CONTENT, token );

                const auto html_ns_id = _document->namespaceMap().setNamespace( Namespace_e::HTML );

                while( this->currentNode() != nullptr &&
                       ( !TreeBuilder::isMathMLIntegrationPoint( this->currentNode() ) &&
                         !TreeBuilder::isHTMLIntegrationPoint( this->currentNode() )   &&
                         this->currentNode()->namespaceID() != html_ns_id ) )
                {
                    this->popOpenElements();
                }

                this->processAsHTMLContent( std::move( token ) );
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto * tk = dynamic_cast<token::html5::CharacterTk *>( token.get() );

            for( auto & c : tk->text() ) {
                if( c == unicode::NUL ) {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_NULL_CHARACTER, token );

                    c = unicode::REPLACEMENT_CHAR;

                } else if( !unicode::ascii::iswspace( c ) ) {
                    _frameset_ok = false;
                }
            }

            this->insertCharacters( tk->text() );
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        default: break;
    }
}

/**
 * Processes a token within HTML content
 * @param token HTML5 token
 * @param resolved Element type if previously resolved
 */
void TreeBuilder::processAsHTMLContent( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    try {
        if( token->type() == specs::infra::TokenType::UNKNOWN ) {
            LOG_ERROR(
                "[parser::dom::TreeBuilder::processAsHTMLContent( std::unique_ptr<token::html5::HTML5Tk>, <", resolved, "> )] "
                "Token type is UNKNOWN - skipped."
            );

        } else {
            switch( _current_insert_mode ) {
                case specs::infra::InsertionMode::INITIAL:              { this->processAsHTMLContent<InsertionMode_e::INITIAL>( std::move( token ), resolved );              } break;
                case specs::infra::InsertionMode::BEFORE_HTML:          { this->processAsHTMLContent<InsertionMode_e::BEFORE_HTML>( std::move( token ), resolved );          } break;
                case specs::infra::InsertionMode::BEFORE_HEAD:          { this->processAsHTMLContent<InsertionMode_e::BEFORE_HEAD>( std::move( token ), resolved );          } break;
                case specs::infra::InsertionMode::IN_HEAD:              { this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );              } break;
                case specs::infra::InsertionMode::IN_HEAD_NOSCRIPT:     { this->processAsHTMLContent<InsertionMode_e::IN_HEAD_NOSCRIPT>( std::move( token ), resolved );     } break;
                case specs::infra::InsertionMode::AFTER_HEAD:           { this->processAsHTMLContent<InsertionMode_e::AFTER_HEAD>( std::move( token ), resolved );           } break;
                case specs::infra::InsertionMode::IN_BODY:              { this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );              } break;
                case specs::infra::InsertionMode::TEXT:                 { this->processAsHTMLContent<InsertionMode_e::TEXT>( std::move( token ), resolved );                 } break;
                case specs::infra::InsertionMode::IN_TABLE:             { this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );             } break;
                case specs::infra::InsertionMode::IN_TABLE_TEXT:        { this->processAsHTMLContent<InsertionMode_e::IN_TABLE_TEXT>( std::move( token ), resolved );        } break;
                case specs::infra::InsertionMode::IN_CAPTION:           { this->processAsHTMLContent<InsertionMode_e::IN_CAPTION>( std::move( token ), resolved );           } break;
                case specs::infra::InsertionMode::IN_COLUMN_GROUP:      { this->processAsHTMLContent<InsertionMode_e::IN_COLUMN_GROUP>( std::move( token ), resolved );      } break;
                case specs::infra::InsertionMode::IN_TABLE_BODY:        { this->processAsHTMLContent<InsertionMode_e::IN_TABLE_BODY>( std::move( token ), resolved );        } break;
                case specs::infra::InsertionMode::IN_ROW:               { this->processAsHTMLContent<InsertionMode_e::IN_ROW>( std::move( token ), resolved );               } break;
                case specs::infra::InsertionMode::IN_CELL:              { this->processAsHTMLContent<InsertionMode_e::IN_CELL>( std::move( token ), resolved );              } break;
                case specs::infra::InsertionMode::IN_SELECT:            { this->processAsHTMLContent<InsertionMode_e::IN_SELECT>( std::move( token ), resolved );            } break;
                case specs::infra::InsertionMode::IN_SELECT_IN_TABLE:   { this->processAsHTMLContent<InsertionMode_e::IN_SELECT_IN_TABLE>( std::move( token ), resolved );   } break;
                case specs::infra::InsertionMode::IN_TEMPLATE:          { this->processAsHTMLContent<InsertionMode_e::IN_TEMPLATE>( std::move( token ), resolved );          } break;
                case specs::infra::InsertionMode::AFTER_BODY:           { this->processAsHTMLContent<InsertionMode_e::AFTER_BODY>( std::move( token ), resolved );           } break;
                case specs::infra::InsertionMode::IN_FRAMESET:          { this->processAsHTMLContent<InsertionMode_e::IN_FRAMESET>( std::move( token ), resolved );          } break;
                case specs::infra::InsertionMode::AFTER_FRAMESET:       { this->processAsHTMLContent<InsertionMode_e::AFTER_FRAMESET>( std::move( token ), resolved );       } break;
                case specs::infra::InsertionMode::AFTER_AFTER_BODY:     { this->processAsHTMLContent<InsertionMode_e::AFTER_AFTER_BODY>( std::move( token ), resolved );     } break;
                case specs::infra::InsertionMode::AFTER_AFTER_FRAMESET: { this->processAsHTMLContent<InsertionMode_e::AFTER_AFTER_FRAMESET>( std::move( token ), resolved ); } break;
            }
        }

    } catch( blogator::exception::failed_expectation &e ) {
        using blogator::unicode::utf8::convert;

        LOG_CRITICAL(
            "[parser::builders::TreeBuilder::processAsHTMLContent( std::unique_ptr<token::html5::HTML5Tk> )] "
            "Failed to process token."
        );
    }
}

/**
 * Processes a token in a "initial" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::INITIAL>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#the-initial-insertion-mode
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            static const auto HTML = blogator::to_u32string( Element_e::HTML5_HTML );

            auto * tk = dynamic_cast<token::html5::DoctypeTk *>( token.get() );

            if( tk->name() != HTML || tk->hasPID() || ( tk->hasSID() && tk->systemIdentifier() != U"about:legacy-compat" ) ) {
                this->logError( specs::infra::ErrorCode::MALFORMED_DOCTYPE, token );
            }

            _document->appendChild(
                std::make_unique<node::DocumentType>(
                    tk->name(), tk->publicIdentifier(), tk->systemIdentifier()
                )
            );

            if( !this->isIFrameSrcDoc() && !_parser_cannot_change_the_mode ) {
                const auto quirks = specs::infra::DocType::validate( tk->publicIdentifier(),
                                                                     tk->hasSID(),
                                                                     tk->systemIdentifier() );

                if( tk->forceQuirks() || tk->name() != HTML || quirks == QuirksMode::QUIRKS ) {
                    _document->setQuirks( QuirksMode::QUIRKS );

                } else {
                    _document->setQuirks( quirks );
                }
            }

            this->setCurrentInsertMode( InsertionMode_e::BEFORE_HTML );
        } break;

        case specs::infra::TokenType::CHARACTER: {
            for( auto &c: token->text() ) {
                if( !blogator::unicode::ascii::iswspace( c ) ) {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            _document->appendChild( std::make_unique<node::Comment>( token->text() ) );
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        if( !this->isIFrameSrcDoc() ) {
            this->logError(  specs::infra::ErrorCode::MISSING_DOCTYPE, token );
        }

        if( !_parser_cannot_change_the_mode ) {
            _document->setQuirks( QuirksMode::QUIRKS );
        }

        this->setCurrentInsertMode( specs::infra::InsertionMode::BEFORE_HTML );
        this->processAsHTMLContent<InsertionMode_e::BEFORE_HTML>( std::move( token ), resolved ); //re-process token
    }
}

/**
 * Processes a token in a "before html" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::BEFORE_HTML>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#the-before-html-insertion-mode
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );

            resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() );

            if( resolved == Element_e::HTML5_HTML ) {
                auto * node = dynamic_cast<node::Element *>( _document->appendChild( std::make_unique<node::Element>( specs::infra::Element::HTML5_HTML ) ) );

                for( auto & attr : tk->attributes() ) {
                    if( attr.value.empty() ) {
                        node->createAttribute( attr.name );
                    } else {
                        node->createAttribute( attr.name, attr.value );
                    }
                }

                this->pushOpenElements( node );
                this->setCurrentInsertMode( InsertionMode_e::BEFORE_HEAD );

            } else {
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );

            resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() );

            if( resolved == Element_e::HTML5_HEAD ||
                resolved == Element_e::HTML5_BODY ||
                resolved == Element_e::HTML5_HTML ||
                resolved == Element_e::HTML5_BR )
            {
                goto anything_else;

            } else {
                this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            for( auto &c: token->text() ) {
                if( !blogator::unicode::ascii::iswspace( c ) ) {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            _document->appendChild( std::make_unique<node::Comment>( token->text() ) );
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        auto * element = _document->appendChild( std::make_unique<node::Element>( specs::infra::Element::HTML5_HTML ) );

        this->pushOpenElements( dynamic_cast<node::Element *>( element ) );
        this->setCurrentInsertMode( InsertionMode_e::BEFORE_HEAD );
        this->processAsHTMLContent<InsertionMode_e::BEFORE_HEAD>( std::move( token ), resolved ); //re-process token
    };
}

/**
 * Processes a token in a "before head" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::BEFORE_HEAD>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#the-before-head-insertion-mode
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() );

            if( resolved == Element_e::HTML5_HTML ) {
                this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );

            } else if( resolved == Element_e::HTML5_HEAD ) {
                this->setHeadElementPtr( this->insertHtmlElement( tk, resolved ) );
                this->setCurrentInsertMode( InsertionMode_e::IN_HEAD );

            } else {
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HEAD: [[fallthrough]];
                case Element_e::HTML5_BODY: [[fallthrough]];
                case Element_e::HTML5_HTML: [[fallthrough]];
                case Element_e::HTML5_BR: {
                    goto anything_else;
                } break;

                default: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );
                } break;
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            for( auto &c: token->text() ) {
                if( !blogator::unicode::ascii::iswspace( c ) ) {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        auto tk = token::html5::StartTagTk( blogator::to_u32string( Element_e::HTML5_HEAD ), token->position() );

        this->setHeadElementPtr( this->insertHtmlElement( &tk, Element_e::HTML5_HEAD ) );
        this->setCurrentInsertMode( InsertionMode_e::IN_HEAD );
        this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved ); //re-process token
    };
}

/**
 * Processes a token in a "in head" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_HEAD>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inhead
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_BASE:         [[fallthrough]];
                case Element_e::DEPR_HTML_BASEFONT: [[fallthrough]];
                case Element_e::DEPR_HTML_BGSOUND:  [[fallthrough]];
                case Element_e::HTML5_LINK:         [[fallthrough]];
                case Element_e::HTML5_META: { /* <- 'active speculation' implementation skipped for blogator */
                    this->insertHtmlElement( tk, resolved );
                    this->popOpenElements();

                    if( tk->selfclosing() ) {
                        _acknowledgeSelfClosingTag_cb();
                    }
                } break;

                case Element_e::HTML5_TITLE: {
                    this->insertGenericRCDATAElement( tk );
                } break;

                case Element_e::DEPR_HTML_NOFRAMES: [[fallthrough]];
                case Element_e::HTML5_STYLE: {
                    this->insertGenericRawTextElement( tk );
                } break;

                case Element_e::HTML5_NOSCRIPT: {
                    if( _scripting ) {
                        this->insertGenericRawTextElement( tk );

                    } else {
                        this->insertHtmlElement( tk, resolved );
                        this->setCurrentInsertMode( InsertionMode_e::IN_HEAD_NOSCRIPT );
                    }
                } break;

                case Element_e::HTML5_SCRIPT: {
                    //(Steps 3-5 of specs skipped since blogator is not interactive)
                    this->insertHtmlElement( tk, resolved );
                    _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::SCRIPT_DATA );
                    this->setOriginalInsertMode( _current_insert_mode );
                    this->setCurrentInsertMode( InsertionMode_e::TEXT );
                } break;

                case Element_e::HTML5_TEMPLATE: {
                    auto * template_element = this->insertHtmlElement( tk, resolved );

                    this->pushActiveFormattingElement( TreeBuilder::FORMATTING_MARKER );
                    _frameset_ok = false;
                    this->setCurrentInsertMode( InsertionMode_e::IN_TEMPLATE );
                    this->pushTemplateInsertionModes( InsertionMode_e::IN_TEMPLATE );

                    template_element->appendChild( std::make_unique<node::DocumentFragment>() );
                } break;

                case Element_e::HTML5_HEAD: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );
                } break;

                default: { goto anything_else; } break;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HEAD: {
                    this->popOpenElements();
                    this->setCurrentInsertMode( InsertionMode_e::AFTER_HEAD );
                } break;

                case Element_e::HTML5_BODY: [[fallthrough]];
                case Element_e::HTML5_HTML: [[fallthrough]];
                case Element_e::HTML5_BR: {
                    goto anything_else;
                } break;

                case Element_e::HTML5_TEMPLATE: {
                    if( !this->hasOpenElement( resolved ) ) {
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                        goto end; //token is ignored
                    }

                    this->generateAllImpliedEndTagsThoroughly();

                    if( this->currentNode()->elementType() != Element_e::HTML5_TEMPLATE ) {
                        this->logError( specs::infra::ErrorCode::ORPHANED_STARTTAG, token );
                    }

                    bool template_popped = false;

                    while( !template_popped ) {
                        if( this->currentNode()->elementType() == Element_e::HTML5_TEMPLATE ) {
                            template_popped = true;
                        }

                        this->popOpenElements();
                    }

                    this->clearActiveFormattingElementsUpToLastMarker();
                    this->popTemplateInsertionModes();
                    this->resetInsertionModeAppropriately();
                } break;

                default: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );
                } break;

            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return blogator::unicode::ascii::iswspace( c ); } );

            if( it == token->text().cend() ) { //all white-space
                this->insertCharacters( token->text() );

            } else if( it != token->text().cbegin() ) { //some white-space
                this->insertCharacters( token->text().begin(), it );
                token->text().erase( token->text().begin(), it );
                goto anything_else;

            } else { //no white-space
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        default: {
            goto anything_else;
        }
    }

    end:
        return; //DEFAULT

    //-----------
    anything_else: {
        this->popOpenElements();
        this->setCurrentInsertMode( InsertionMode_e::AFTER_HEAD );
        this->processAsHTMLContent<InsertionMode_e::AFTER_HEAD>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "in head noscript" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_HEAD_NOSCRIPT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inheadnoscript
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::DEPR_HTML_BASEFONT: [[fallthrough]];
                case Element_e::DEPR_HTML_BGSOUND:  [[fallthrough]];
                case Element_e::HTML5_LINK:         [[fallthrough]];
                case Element_e::HTML5_META:         [[fallthrough]];
                case Element_e::DEPR_HTML_NOFRAMES: [[fallthrough]];
                case Element_e::HTML5_STYLE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_HEAD: [[fallthrough]];
                case Element_e::HTML5_NOSCRIPT: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );
                } break;

                default: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );
                    goto anything_else;
                } break;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_NOSCRIPT: {
                    this->popOpenElements();
                    this->setCurrentInsertMode( InsertionMode_e::IN_HEAD );
                } break;

                case Element_e::HTML5_BR: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );
                    goto anything_else;
                } break;

                default: { //ignore token
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );
                } break;
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            for( auto &c: token->text() ) {
                if( !blogator::unicode::ascii::iswspace( c ) ) {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_CHARACTER, token );
                    goto anything_else;
                }
            }

            this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
        } break;

        default: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->popOpenElements();
        this->setCurrentInsertMode( InsertionMode_e::IN_HEAD );
        this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "after head" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_HEAD>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#the-after-head-insertion-mode
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_BODY: {
                    this->insertHtmlElement( tk, resolved );
                    _frameset_ok = false;
                    this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
                } break;

                case Element_e::DEPR_HTML_FRAMESET: {
                    this->insertHtmlElement( tk, resolved );
                    this->setCurrentInsertMode( InsertionMode_e::IN_FRAMESET );
                } break;

                case Element_e::HTML5_BASE:         [[fallthrough]];
                case Element_e::DEPR_HTML_BASEFONT: [[fallthrough]];
                case Element_e::DEPR_HTML_BGSOUND:  [[fallthrough]];
                case Element_e::HTML5_LINK:         [[fallthrough]];
                case Element_e::HTML5_META:         [[fallthrough]];
                case Element_e::DEPR_HTML_NOFRAMES: [[fallthrough]];
                case Element_e::HTML5_SCRIPT:       [[fallthrough]];
                case Element_e::HTML5_STYLE:        [[fallthrough]];
                case Element_e::HTML5_TEMPLATE:     [[fallthrough]];
                case Element_e::HTML5_TITLE: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );

                    if( _head_element != nullptr ) {
                        this->pushOpenElements( _head_element );
                    }

                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );

                    this->removeOpenElement(
                        std::find_if( _open_elements.begin(),
                                      _open_elements.end(),
                                      [this]( auto * ptr ) { return ptr == _head_element; } )
                    );
                } break;

                case Element_e::HTML5_HEAD: { //ignore token
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );
                } break;

                default: { goto anything_else; } break;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_BODY: [[fallthrough]];
                case Element_e::HTML5_HTML: [[fallthrough]];
                case Element_e::HTML5_BR: {
                    goto anything_else;
                } break;

                default: { //ignore token
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );
                } break;
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return unicode::ascii::iswspace( c ); } );

            if( it != token->text().begin() ) { //some or all whitespace
                this->insertCharacters( token->text().begin(), it );

                if( it != token->text().end() ) { //some whitespace
                    token->text().erase( token->text().begin(), it );
                    goto anything_else;
                }

            } else { //no whitespace
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        auto tk = token::html5::StartTagTk( blogator::to_u32string( Element_e::HTML5_BODY ), token->position() );

        this->insertHtmlElement( &tk, Element_e::HTML5_BODY );
        this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
        this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved ); //re-process token
    };
}

/**
 * Processes a token in a "in body" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inbody
    static const auto ELEMENT_CHECK_LIST = std::set<Element_e>( {
        Element_e::HTML5_DD,
        Element_e::HTML5_DT,
        Element_e::HTML5_LI,
        Element_e::HTML5_OPTGROUP,
        Element_e::HTML5_OPTION,
        Element_e::HTML5_P,
        Element_e::DEPR_HTML_RB,
        Element_e::HTML5_RP,
        Element_e::HTML5_RT,
        Element_e::DEPR_HTML_RTC,
        Element_e::HTML5_TBODY,
        Element_e::HTML5_TD,
        Element_e::HTML5_TFOOT,
        Element_e::HTML5_TH,
        Element_e::HTML5_THEAD,
        Element_e::HTML5_TR,
        Element_e::HTML5_BODY,
        Element_e::HTML5_HTML,
    } );

    static const auto HEADING_ELEMENTS = std::set<Element_e>( {
        Element_e::HTML5_H1,
        Element_e::HTML5_H2,
        Element_e::HTML5_H3,
        Element_e::HTML5_H4,
        Element_e::HTML5_H5,
        Element_e::HTML5_H6
    } );

    if( _ignore_next_token_in_body_LF && token->type() != specs::infra::TokenType::CHARACTER ) {
        _ignore_next_token_in_body_LF = false; //reset
    }

    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );

                    if( !this->hasOpenElement( Element_e::HTML5_TEMPLATE ) && tk->hasAttributes() ) {
                        auto * top_element = this->getOpenElement( 0 );

                        for( const auto & attr : tk->attributes() ) {
                            if( top_element != nullptr && !top_element->hasAttribute( attr.name ) ) {
                                if( attr.value.empty() ) {
                                    top_element->setAttribute( attr.name, _strict_validation );
                                } else {
                                    top_element->setAttribute( attr.name, attr.value, _strict_validation );
                                }
                            }
                        }
                    } //else: ignore token
                } break;

                case Element_e::HTML5_BASE:         [[fallthrough]];
                case Element_e::DEPR_HTML_BASEFONT: [[fallthrough]];
                case Element_e::DEPR_HTML_BGSOUND:  [[fallthrough]];
                case Element_e::HTML5_LINK:         [[fallthrough]];
                case Element_e::HTML5_META:         [[fallthrough]];
                case Element_e::DEPR_HTML_NOFRAMES: [[fallthrough]];
                case Element_e::HTML5_SCRIPT:       [[fallthrough]];
                case Element_e::HTML5_STYLE:        [[fallthrough]];
                case Element_e::HTML5_TEMPLATE:     [[fallthrough]];
                case Element_e::HTML5_TITLE : {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_BODY: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );

                    auto * second_element = this->getOpenElement( 1 );

                    if( second_element != nullptr &&
                        second_element->elementType() == Element_e::HTML5_BODY &&
                        !this->hasOpenElement( Element_e::HTML5_TEMPLATE ) )
                    {
                        _frameset_ok = false;

                        for( const auto & attr : tk->attributes() ) {
                            if( !second_element->hasAttribute( attr.name ) ) {
                                if( attr.value.empty() ) {
                                    second_element->setAttribute( attr.name, _strict_validation );
                                } else {
                                    second_element->setAttribute( attr.name, attr.value, _strict_validation );
                                }
                            }
                        }
                    } //else: ignore token
                } break;

                case Element_e::DEPR_HTML_FRAMESET: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );

                    auto * second_element = this->getOpenElement( 1 );

                    if( ( second_element != nullptr && second_element->elementType() == Element_e::HTML5_BODY ) && _frameset_ok ) {
                        auto * parent = second_element->parentNode();

                        while( this->currentNode() != this->getOpenElement( 0 ) ) {
                            this->popOpenElements();
                        }

                        if( parent != nullptr ) {
                            auto child_it = std::find_if( parent->childNodes().begin(),
                                                          parent->childNodes().end(),
                                                          [second_element]( auto & u_ptr ) { return  u_ptr.get() == second_element; } );

                            auto removed_child = parent->removeChild( *child_it );
                        }

                        this->insertHtmlElement( tk, resolved );
                        this->setCurrentInsertMode( InsertionMode_e::IN_FRAMESET );
                    } //else: ignore token
                } break;

                case Element_e::HTML5_ADDRESS:    [[fallthrough]];
                case Element_e::HTML5_ARTICLE:    [[fallthrough]];
                case Element_e::HTML5_ASIDE:      [[fallthrough]];
                case Element_e::HTML5_BLOCKQUOTE: [[fallthrough]];
                case Element_e::DEPR_HTML_CENTER: [[fallthrough]];
                case Element_e::HTML5_DETAILS:    [[fallthrough]];
                case Element_e::HTML5_DIALOG:     [[fallthrough]];
                case Element_e::DEPR_HTML_DIR:    [[fallthrough]];
                case Element_e::HTML5_DIV:        [[fallthrough]];
                case Element_e::HTML5_DL:         [[fallthrough]];
                case Element_e::HTML5_FIELDSET:   [[fallthrough]];
                case Element_e::HTML5_FIGCAPTION: [[fallthrough]];
                case Element_e::HTML5_FIGURE:     [[fallthrough]];
                case Element_e::HTML5_FOOTER:     [[fallthrough]];
                case Element_e::HTML5_HEADER:     [[fallthrough]];
                case Element_e::HTML5_HGROUP:     [[fallthrough]];
                case Element_e::HTML5_MAIN:       [[fallthrough]];
                case Element_e::HTML5_MENU:       [[fallthrough]];
                case Element_e::HTML5_NAV:        [[fallthrough]];
                case Element_e::HTML5_OL:         [[fallthrough]];
                case Element_e::HTML5_P:          [[fallthrough]];
                case Element_e::HTML5_SEARCH:     [[fallthrough]];
                case Element_e::HTML5_SECTION:    [[fallthrough]];
                case Element_e::HTML5_SUMMARY:    [[fallthrough]];
                case Element_e::HTML5_UL: {
                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, token->position() );
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::HTML5_H1: [[fallthrough]];
                case Element_e::HTML5_H2: [[fallthrough]];
                case Element_e::HTML5_H3: [[fallthrough]];
                case Element_e::HTML5_H4: [[fallthrough]];
                case Element_e::HTML5_H5: [[fallthrough]];
                case Element_e::HTML5_H6: {
                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, token->position() );
                    }

                    const auto curr_node_type = currentNode()->elementType();

                    if( curr_node_type == Element_e::HTML5_H1 ||
                        curr_node_type == Element_e::HTML5_H2 ||
                        curr_node_type == Element_e::HTML5_H3 ||
                        curr_node_type == Element_e::HTML5_H4 ||
                        curr_node_type == Element_e::HTML5_H5 ||
                        curr_node_type == Element_e::HTML5_H6 )
                    {
                        std::stringstream ss;
                        ss << "<" << resolved << "> in <" << curr_node_type << "> scope";
                        this->logError( specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, token );

                        this->popOpenElements();
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::HTML5_PRE:         [[fallthrough]];
                case Element_e::DEPR_HTML_LISTING: {
                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, token->position() );
                    }

                    this->insertHtmlElement( tk, resolved );
                    _ignore_next_token_in_body_LF = true;
                    _frameset_ok                  = false;
                } break;

                case Element_e::HTML5_FORM: {
                    const auto has_open_template = this->hasOpenElement( Element_e::HTML5_TEMPLATE );

                    if( _form_element != nullptr && !has_open_template ) {
                        std::stringstream ss;
                        ss << "<" << resolved << "> inside <" << Element_e::HTML5_FORM << "> scope";
                        this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );

                    } else {
                        if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                            this->closeElement( Element_e::HTML5_P, token->position() );
                        }

                        auto * node = this->insertHtmlElement( tk, resolved );

                        if( !has_open_template ) {
                            this->setFormElementPtr( node );
                        }
                    }
                } break;

                case Element_e::HTML5_LI: {
                    _frameset_ok = false;

                    auto loop      = true;
                    auto node_it   = this->currentNodeIterator();
                    auto node_type = (*node_it)->elementType();

                    do {
                        if( node_type == Element_e::HTML5_LI ) {
                            this->closeElement( Element_e::HTML5_LI, token->position() );
                            loop = false;

                        } else {
                            if( isSpecialElement( node_type, { Element_e::HTML5_ADDRESS, Element_e::HTML5_DIV, Element_e::HTML5_P } ) ) {
                                loop = false;

                            } else {
                                std::advance( node_it, 1 );
                                node_type = (*node_it)->elementType();
                            }
                        }
                    } while( loop );

                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, token->position() );
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::HTML5_DD: [[fallthrough]];
                case Element_e::HTML5_DT: {
                    _frameset_ok = false;

                    auto loop      = true;
                    auto node_it   = this->currentNodeIterator();
                    auto node_type = (*node_it)->elementType();

                    do {
                        if( node_type == Element_e::HTML5_DD || node_type == Element_e::HTML5_DT ) {
                            this->closeElement( node_type, token->position() );
                            loop = false;

                        } else {
                            if( isSpecialElement( node_type, { Element_e::HTML5_ADDRESS, Element_e::HTML5_DIV, Element_e::HTML5_P } ) ) {
                                loop = false;

                            } else {
                                std::advance( node_it, 1 );
                                node_type = (*node_it)->elementType();
                            }
                        }
                    } while( loop );

                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, token->position() );
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::DEPR_HTML_PLAINTEXT: {
                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, token->position() );
                    }

                    this->insertHtmlElement( tk, resolved );
                    _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::PLAIN_TEXT );
                } break;

                case Element_e::HTML5_BUTTON: {
                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_BUTTON ) ) {
                        std::stringstream ss;
                        ss << "<" << resolved << "> inside <" << Element_e::HTML5_BUTTON << "> scope";
                        this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );

                        this->generateImpliedEndTags();
                        this->popOpenElementsUntil( Element_e::HTML5_BUTTON, true );
                    }

                    this->reconstructActiveFormattingElements( token->position() );
                    this->insertHtmlElement( tk, resolved );
                    _frameset_ok = false;
                } break;

                case Element_e::HTML5_A: {
                    bool any_other_end_tag_flag = false;

                    auto fmt_it = findActiveFormattingElementAfterLastMarker( Element_e::HTML5_A );

                    if( fmt_it != _formatting_elements.end() ) {
                        const auto * fmt_ptr = (*fmt_it);

                        std::stringstream ss;
                        ss << "<" << resolved << "> inside <" << Element_e::HTML5_A << "> scope";
                        this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_FORMATTING_ELEMENT, ss.str() );

                        any_other_end_tag_flag = this->runAdoptionAgencyAlgorithm( tk, resolved );

                        if( !any_other_end_tag_flag ) {
                            this->removeOpenElement( this->findOpenElement( fmt_ptr ) );
                            this->removeActiveFormattingElement( this->findActiveFormattingElement( fmt_ptr ) );

                        } else {
                            LOG_WARNING(
                                "[parser::dom::TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_BODY>( std::unique_ptr<token::html5::HTML5Tk>, <", resolved, "> )] "
                                "Adoption agency algorithm returned the 'TREAT_AS_ANY_OTHER_END_TAG' flag which is unexpected for a StartTagTk."
                            );
                        }
                    }

                    this->reconstructActiveFormattingElements( token->position() );

                    auto * new_element = this->insertHtmlElement( tk, resolved );

                    if( !any_other_end_tag_flag && new_element != nullptr ) {
                        this->pushActiveFormattingElement( new_element );
                    }
                } break;

                case Element_e::HTML5_B:          [[fallthrough]];
                case Element_e::DEPR_HTML_BIG:    [[fallthrough]];
                case Element_e::HTML5_CODE:       [[fallthrough]];
                case Element_e::HTML5_EM:         [[fallthrough]];
                case Element_e::DEPR_HTML_FONT:   [[fallthrough]];
                case Element_e::HTML5_I:          [[fallthrough]];
                case Element_e::HTML5_S:          [[fallthrough]];
                case Element_e::HTML5_SMALL:      [[fallthrough]];
                case Element_e::DEPR_HTML_STRIKE: [[fallthrough]];
                case Element_e::HTML5_STRONG:     [[fallthrough]];
                case Element_e::DEPR_HTML_TT:     [[fallthrough]];
                case Element_e::HTML5_U: {
                    this->reconstructActiveFormattingElements( token->position() );
                    this->pushActiveFormattingElement( this->insertHtmlElement( tk, resolved ) );
                } break;

                case Element_e::DEPR_HTML_NOBR: {
                    this->reconstructActiveFormattingElements( token->position() );

                    if( this->hasParticularElementInScope( resolved ) ) {
                        std::stringstream ss;
                        ss << "<" << resolved << "> inside <" << Element_e::DEPR_HTML_NOBR << "> scope";
                        this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );

                        if( this->runAdoptionAgencyAlgorithm( tk, resolved ) == TREAT_AS_ANY_OTHER_END_TAG ) {
                            goto any_other_end_tag;
                        }

                        this->reconstructActiveFormattingElements( token->position() );
                    }

                    this->pushActiveFormattingElement( this->insertHtmlElement( tk, resolved ) );
                } break;

                case Element_e::DEPR_HTML_APPLET:  [[fallthrough]];
                case Element_e::DEPR_HTML_MARQUEE: [[fallthrough]];
                case Element_e::HTML5_OBJECT: {
                    this->reconstructActiveFormattingElements( token->position() );
                    this->insertHtmlElement( tk, resolved );
                    this->pushActiveFormattingElement( FORMATTING_MARKER );
                    _frameset_ok = false;
                } break;

                case Element_e::HTML5_TABLE: {
                    if( _document->quirksMode() != QuirksMode::QUIRKS &&
                        this->hasParticularElementInButtonScope( Element_e::HTML5_P ) )
                    {
                        this->closeElement( Element_e::HTML5_P, tk->position() );
                    }

                    this->insertHtmlElement( tk, resolved );
                    _frameset_ok = false;
                    this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
                } break;

                case Element_e::HTML5_AREA:       [[fallthrough]];
                case Element_e::HTML5_BR:         [[fallthrough]];
                case Element_e::HTML5_EMBED:      [[fallthrough]];
                case Element_e::HTML5_IMG:        [[fallthrough]];
                case Element_e::DEPR_HTML_KEYGEN: [[fallthrough]];
                case Element_e::HTML5_WBR: {
                    this->reconstructActiveFormattingElements( token->position() );
                    this->insertHtmlElement( tk, resolved );
                    this->popOpenElements();

                    if( tk->selfclosing() ) {
                        _acknowledgeSelfClosingTag_cb();
                    }

                    _frameset_ok = false;
                } break;

                case Element_e::HTML5_INPUT: {
                    this->reconstructActiveFormattingElements( token->position() );
                    this->insertHtmlElement( tk, resolved );
                    this->popOpenElements();

                    if( tk->selfclosing() ) {
                        _acknowledgeSelfClosingTag_cb();
                    }

                    const auto type_attr_it = std::find_if( tk->attributes().cbegin(),
                                                            tk->attributes().cend(),
                                                            []( const auto & attr ) { return attr.name == U"type"; } );

                    if( type_attr_it == tk->attributes().cend() ||
                        unicode::ascii::tolower( type_attr_it->value ) != U"hidden" )
                    {
                        _frameset_ok = false;
                    }
                } break;

                case Element_e::HTML5_PARAM:  [[fallthrough]];
                case Element_e::HTML5_SOURCE: [[fallthrough]];
                case Element_e::HTML5_TRACK: {
                    this->insertHtmlElement( tk, resolved );
                    this->popOpenElements();

                    if( tk->selfclosing() ) {
                        _acknowledgeSelfClosingTag_cb();
                    }
                } break;

                case Element_e::HTML5_HR: {
                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, tk->position() );
                    }

                    this->insertHtmlElement( tk, resolved );
                    this->popOpenElements();

                    if( tk->selfclosing() ) {
                        _acknowledgeSelfClosingTag_cb();
                    }

                    _frameset_ok = false;
                } break;

                case Element_e::DEPR_HTML_IMAGE: {
                    this->logError( specs::infra::ErrorCode::MALFORMED_TAG_IMAGE, token );

                    tk->name() = blogator::to_u32string( Element_e::HTML5_IMG );
                    resolved   = Element_e::HTML5_IMG;
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_TEXTAREA: {
                    this->insertHtmlElement( tk, resolved );
                    _ignore_next_token_in_body_LF = true;
                    _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::RCDATA );
                    this->setOriginalInsertMode( _current_insert_mode );
                    _frameset_ok = false;
                    this->setCurrentInsertMode( InsertionMode_e::TEXT );
                } break;

                case Element_e::DEPR_HTML_XMP: {
                    if( this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->closeElement( Element_e::HTML5_P, tk->position() );
                    }

                    this->reconstructActiveFormattingElements( token->position() );
                    _frameset_ok = false;
                    this->insertGenericRawTextElement( tk );
                } break;

                case Element_e::HTML5_IFRAME: {
                    _frameset_ok = false;
                    this->insertGenericRawTextElement( tk );
                } break;

                case Element_e::DEPR_HTML_NOEMEBED: {
                    this->insertGenericRawTextElement( tk );
                } break;

                case Element_e::HTML5_NOSCRIPT: {
                    if( _scripting ) {
                        this->insertGenericRawTextElement( tk );

                    } else { //like any other end tag
                        this->reconstructActiveFormattingElements( token->position() );
                        this->insertHtmlElement( tk, resolved );
                    }
                } break;

                case Element_e::HTML5_SELECT: {
                    this->reconstructActiveFormattingElements( token->position() );
                    this->insertHtmlElement( tk, resolved );
                    _frameset_ok = false;

                    switch( _current_insert_mode ) {
                        case InsertionMode_e::IN_TABLE:      [[fallthrough]];
                        case InsertionMode_e::IN_CAPTION:    [[fallthrough]];
                        case InsertionMode_e::IN_TABLE_BODY: [[fallthrough]];
                        case InsertionMode_e::IN_ROW:        [[fallthrough]];
                        case InsertionMode_e::IN_CELL: {
                            this->setCurrentInsertMode( InsertionMode_e::IN_SELECT_IN_TABLE );
                        } break;

                        default: {
                            this->setCurrentInsertMode( InsertionMode_e::IN_SELECT );
                        } break;
                    }
                } break;

                case Element_e::HTML5_OPTGROUP: [[fallthrough]];
                case Element_e::HTML5_OPTION: {
                    if( this->currentNodeType() == Element_e::HTML5_OPTION ) {
                        this->popOpenElements();
                    }

                    this->reconstructActiveFormattingElements( token->position() );
                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::DEPR_HTML_RB: [[fallthrough]];
                case Element_e::DEPR_HTML_RTC: {
                    if( this->hasParticularElementInScope( Element_e::HTML5_RUBY ) ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != Element_e::HTML5_RUBY ) {
                            std::stringstream ss;
                            ss << "<" << this->currentNodeType() << "> inside <" << Element_e::HTML5_RUBY << "> scope";
                            this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );
                        }
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::HTML5_RP: [[fallthrough]];
                case Element_e::HTML5_RT: {
                    if( this->hasParticularElementInScope( Element_e::HTML5_RUBY ) ) {
                        this->generateImpliedEndTags( { Element_e::DEPR_HTML_RTC } );

                        const auto curr_element_type = this->currentNodeType();

                        if( curr_element_type != Element_e::DEPR_HTML_RTC || curr_element_type != Element_e::HTML5_RUBY ) {
                            std::stringstream ss;
                            ss << "<" << this->currentNodeType() << "> "
                               << "inside <" << Element_e::DEPR_HTML_RTC << "> or <" << Element_e::HTML5_RUBY << "> scope";
                            this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );
                        }
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::MATH: {
                    this->reconstructActiveFormattingElements( token->position() );

                    for( auto & attr : tk->attributes() ) {
                        attr.name = specs::infra::adjustMathMLAttribute( attr.name );
                        //Note: "Adjust foreign attributes for the token" is done in `insertForeignElement(..)`
                    }

                    this->insertHtmlElement( tk, resolved );

                    if( tk->selfclosing() ) {
                        this->popOpenElements();
                        _acknowledgeSelfClosingTag_cb();
                    }
                } break;

                case Element_e::SVG: {
                    this->reconstructActiveFormattingElements( token->position() );

                    for( auto & attr : tk->attributes() ) {
                        attr.name = specs::infra::adjustSVGAttribute( attr.name );
                        //Note: "Adjust foreign attributes for the token" is done in `insertHtmlElement(..)`
                    }

                    this->insertHtmlElement( tk, resolved );

                    if( tk->selfclosing() ) {
                        this->popOpenElements();
                        _acknowledgeSelfClosingTag_cb();
                    }
                } break;

                case Element_e::HTML5_CAPTION:   [[fallthrough]];
                case Element_e::HTML5_COL:       [[fallthrough]];
                case Element_e::HTML5_COLGROUP:  [[fallthrough]];
                case Element_e::DEPR_HTML_FRAME: [[fallthrough]];
                case Element_e::HTML5_HEAD:      [[fallthrough]];
                case Element_e::HTML5_TBODY:     [[fallthrough]];
                case Element_e::HTML5_TD:        [[fallthrough]];
                case Element_e::HTML5_TFOOT:     [[fallthrough]];
                case Element_e::HTML5_TH:        [[fallthrough]];
                case Element_e::HTML5_THEAD:     [[fallthrough]];
                case Element_e::HTML5_TR: { //ignore token
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );
                } break;

                default: { //any other start tags
                    this->reconstructActiveFormattingElements( token->position() );
                    this->insertHtmlElement( tk, Element_e::OTHER ); //insert as 'ordinary' element (thus redefined as 'OTHER')
                } break;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_BODY: {
                    if( this->hasParticularElementInScope( resolved ) ) {
                        for( auto * el : _open_elements ) {
                            if( !ELEMENT_CHECK_LIST.contains( el->elementType() ) ) {
                                std::stringstream ss;
                                ss << "<" << blogator::unicode::utf8::convert( el->qualifiedName() ) << "> inside <" << Element_e::HTML5_BODY << "> scope";
                                this->logError( tk->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                            }
                        }

                        this->setCurrentInsertMode( InsertionMode_e::AFTER_BODY );

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_HTML: {
                    if( this->hasParticularElementInScope( resolved ) ) {
                        for( auto * el : _open_elements ) {
                            if( !ELEMENT_CHECK_LIST.contains( el->elementType() ) ) {
                                std::stringstream ss;
                                ss << "<" << blogator::unicode::utf8::convert( el->qualifiedName() ) << "> inside <" << Element_e::HTML5_HTML << "> scope";
                                this->logError( tk->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                            }
                        }

                        this->setCurrentInsertMode( InsertionMode_e::AFTER_BODY );
                        this->processAsHTMLContent<InsertionMode_e::AFTER_BODY>( std::move( token ), resolved );

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_ADDRESS:     [[fallthrough]];
                case Element_e::HTML5_ARTICLE:     [[fallthrough]];
                case Element_e::HTML5_ASIDE:       [[fallthrough]];
                case Element_e::HTML5_BLOCKQUOTE:  [[fallthrough]];
                case Element_e::HTML5_BUTTON:      [[fallthrough]];
                case Element_e::DEPR_HTML_CENTER:  [[fallthrough]];
                case Element_e::HTML5_DETAILS:     [[fallthrough]];
                case Element_e::HTML5_DIALOG:      [[fallthrough]];
                case Element_e::DEPR_HTML_DIR:     [[fallthrough]];
                case Element_e::HTML5_DIV:         [[fallthrough]];
                case Element_e::HTML5_DL:          [[fallthrough]];
                case Element_e::HTML5_FIELDSET:    [[fallthrough]];
                case Element_e::HTML5_FIGCAPTION:  [[fallthrough]];
                case Element_e::HTML5_FIGURE:      [[fallthrough]];
                case Element_e::HTML5_FOOTER:      [[fallthrough]];
                case Element_e::HTML5_HEADER:      [[fallthrough]];
                case Element_e::HTML5_HGROUP:      [[fallthrough]];
                case Element_e::DEPR_HTML_LISTING: [[fallthrough]];
                case Element_e::HTML5_MAIN:        [[fallthrough]];
                case Element_e::HTML5_MENU:        [[fallthrough]];
                case Element_e::HTML5_NAV:         [[fallthrough]];
                case Element_e::HTML5_OL:          [[fallthrough]];
                case Element_e::HTML5_PRE:         [[fallthrough]];
                case Element_e::HTML5_SEARCH:      [[fallthrough]];
                case Element_e::HTML5_SECTION:     [[fallthrough]];
                case Element_e::HTML5_SUMMARY:     [[fallthrough]];
                case Element_e::HTML5_UL: {
                    if( this->hasParticularElementInScope( resolved ) ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != resolved ) {
                            std::stringstream ss;
                            ss << "expected <" << resolved << ">, got <" << this->currentNodeType() << ">";
                            this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                        }

                        this->popOpenElementsUntil( resolved, true );

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_FORM: {
                    if( this->hasOpenElement( Element_e::HTML5_TEMPLATE ) ) {
                        if( this->hasParticularElementInScope( Element_e::HTML5_FORM ) ) {
                            this->generateImpliedEndTags();

                            if( this->currentNodeType() != resolved ) {
                                std::stringstream ss;
                                ss << "expected <" << resolved << ">, got <" << this->currentNodeType() << ">";
                                this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                            }

                            this->popOpenElementsUntil( Element_e::HTML5_FORM, true );

                        } else { //ignore token
                            this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                        }

                    } else {
                        auto * node = _form_element;
                        this->setFormElementPtr( nullptr );

                        if( node != nullptr || this->hasParticularElementInScope( Element_e::HTML5_FORM ) ) {
                            this->generateImpliedEndTags();

                            if( this->currentNode() != node ) {
                                std::stringstream ss;
                                ss << "expected <" << resolved << ">, got <" << this->currentNodeType() << ">";
                                this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                            }

                            this->removeOpenElement( this->findOpenElement( node ) );

                        } else { //ignore token
                            this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                        }
                    }
                } break;

                case Element_e::HTML5_P: {
                    if( !this->hasParticularElementInButtonScope( Element_e::HTML5_P ) ) {
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );

                        auto p_tk = token::html5::StartTagTk( blogator::to_u32string( Element_e::HTML5_P ), tk->position() );
                        this->insertHtmlElement( &p_tk, Element_e::HTML5_P );
                    }

                    this->closeElement( Element_e::HTML5_P, tk->position() );
                } break;

                case Element_e::HTML5_LI: {
                    if( this->hasParticularElementInListItemScope( Element_e::HTML5_LI ) ) {
                        this->closeElement( resolved, tk->position() );

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_DD: [[fallthrough]];
                case Element_e::HTML5_DT: {
                    if( this->hasParticularElementInListItemScope( resolved ) ) {
                        this->closeElement( resolved, tk->position() );

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_H1: [[fallthrough]];
                case Element_e::HTML5_H2: [[fallthrough]];
                case Element_e::HTML5_H3: [[fallthrough]];
                case Element_e::HTML5_H4: [[fallthrough]];
                case Element_e::HTML5_H5: [[fallthrough]];
                case Element_e::HTML5_H6: {
                    bool has_heading_element_in_scope = false;

                    for( auto el : HEADING_ELEMENTS ) {
                        if( this->hasParticularElementInScope( el ) ) {
                            has_heading_element_in_scope = true;
                            break;
                        }
                    }

                    if( has_heading_element_in_scope ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != resolved ) {
                            std::stringstream ss;
                            ss << "expected <" << resolved << ">, got <" << this->currentNodeType() << ">";
                            this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                        }

                        this->popOpenElementsUntil( HEADING_ELEMENTS, true );

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_A:          [[fallthrough]];
                case Element_e::HTML5_B:          [[fallthrough]];
                case Element_e::DEPR_HTML_BIG:    [[fallthrough]];
                case Element_e::HTML5_CODE:       [[fallthrough]];
                case Element_e::HTML5_EM:         [[fallthrough]];
                case Element_e::DEPR_HTML_FONT:   [[fallthrough]];
                case Element_e::HTML5_I:          [[fallthrough]];
                case Element_e::DEPR_HTML_NOBR:   [[fallthrough]];
                case Element_e::HTML5_S:          [[fallthrough]];
                case Element_e::HTML5_SMALL:      [[fallthrough]];
                case Element_e::DEPR_HTML_STRIKE: [[fallthrough]];
                case Element_e::HTML5_STRONG:     [[fallthrough]];
                case Element_e::DEPR_HTML_TT:     [[fallthrough]];
                case Element_e::HTML5_U: {
                    if( this->runAdoptionAgencyAlgorithm( tk, resolved ) == TREAT_AS_ANY_OTHER_END_TAG ) {
                        goto any_other_end_tag;
                    } //else: done
                } break;

                case Element_e::DEPR_HTML_APPLET:  [[fallthrough]];
                case Element_e::DEPR_HTML_MARQUEE: [[fallthrough]];
                case Element_e::HTML5_OBJECT: {
                    if( this->hasParticularElementInScope( resolved ) ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != resolved ) {
                            std::stringstream ss;
                            ss << "expected <" << resolved << ">, got <" << this->currentNodeType() << ">";
                            this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                        }

                        this->popOpenElementsUntil( resolved, true );
                        this->clearActiveFormattingElementsUpToLastMarker();

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_BR: {
                    this->logError( specs::infra::ErrorCode::MALFORMED_TAG_BR, token );

                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>(
                        std::make_unique<token::html5::StartTagTk>(
                            blogator::to_u32string( Element_e::HTML5_BR ),
                            tk->position()
                        ),
                        resolved
                    );
                } break;

                default: {
                    auto node_it = this->currentNodeIterator();

                    if( resolved == Element_e::OTHER ) { //weird tag name so use Element string instead of Element type
                        const auto node_name = blogator::unicode::ascii::tolower( tk->name() );

                        while( node_it != _open_elements.end() ) {
                            if( (*node_it)->elementType() == resolved && blogator::unicode::ascii::tolower( (*node_it)->qualifiedName() ) == node_name ) {
                                this->closeElement( node_name, {} );
                                break;
                            }

                            std::advance( node_it, 1 );
                        }

                    } else { //recognised element type
                        while( node_it != _open_elements.end() ) {
                            if( (*node_it)->elementType() == resolved ) {
                                this->closeElement( resolved, tk->position() );
                                break;
                            }

                            if( TreeBuilder::isSpecialElement( (*node_it)->elementType(), {} ) ) {
                                std::stringstream ss;
                                ss << "</" << resolved << "> | special element <" << (*node_it)->elementType() << ">";
                                this->logError( token->position(), specs::infra::ErrorCode::UNEXPECTED_ENDTAG_SPECIAL_ELEMENT, ss.str() );
                                return; //EARLY RETURN (ignore token)
                            }

                            std::advance( node_it, 1 );
                        }
                    }
                } break;
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            bool reconstruct_fmt_elements = false;
            auto buffer                   = std::vector<DOMString_t::value_type>();

            for( auto &c: token->text() ) {
                if( unicode::ascii::iswspace( c ) ) {
                    reconstruct_fmt_elements = true;
                    buffer.emplace_back( c );

                } else if ( c == unicode::NUL ) { //ignore token
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_NULL_CHARACTER, token );

                } else {
                    reconstruct_fmt_elements = true;
                    buffer.emplace_back( c );
                    _frameset_ok = false;
                }
            }

            if( reconstruct_fmt_elements ) {
                this->reconstructActiveFormattingElements( token->position() );
            }

            if( !buffer.empty() ) {
                this->insertCharacters( buffer.cbegin(), buffer.cend() );
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            if( _template_insert_modes.empty() ) {
                for( auto * el : _open_elements ) {
                    if( !ELEMENT_CHECK_LIST.contains( el->elementType() ) ) {
                        std::stringstream ss;
                        ss << "<" << blogator::unicode::utf8::convert( el->qualifiedName() ) << ">";
                        this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                    }
                }

                stopParsing();

            } else {
                this->processAsHTMLContent<InsertionMode_e::IN_TEMPLATE>( std::move( token ), resolved );
            }
        } break;

        default: {
            goto anything_else;
        };
    }

    return; //DEFAULT

    //-----------
    anything_else: { //can only be an end tag
        bool done    = false;
        auto node_it = this->currentNodeIterator();

        while( node_it != _open_elements.end() && !done ) {
            if( (*node_it)->elementType() == resolved ) {
                this->generateImpliedEndTags( { resolved } );

                if( (*node_it) != this->currentNode() ) {
                    std::stringstream ss;
                    ss << "expected <" << blogator::unicode::utf8::convert( this->currentNode()->qualifiedName() ) << ">, "
                       << "got <" << blogator::unicode::utf8::convert( (*node_it)->qualifiedName() ) << ">";
                    this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                }

                this->popOpenElementsUntil( (*node_it)->elementType(), true );
                done = true;

            } else if( TreeBuilder::isSpecialElement( (*node_it)->elementType(), {} ) ) {
                std::stringstream ss;
                ss << "</" << resolved << "> | special element <" << (*node_it)->elementType() << ">";
                this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG_SPECIAL_ELEMENT, token );
                done = true;

            } else {
                ++node_it;
            }
        }

        return;
    };

    //---------------
    any_other_end_tag: {
        auto node_it = this->currentNodeIterator();

        while( true ) {
            if( node_it != _open_elements.end() && (*node_it)->elementType() == resolved ) {
                this->closeElement( resolved, token->position() );
                break;
            }

            if( node_it == _open_elements.end() || TreeBuilder::isSpecialElement( (*node_it)->elementType(), {} ) ) {
                return; //EARLY RETURN
            }

            std::advance( node_it, 1 );
        }
    };
}

/**
 * Processes a token in a "text" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::TEXT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incdata
    switch( token->type() ) {
        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_SCRIPT: [[fallthrough]]; //Script execution not implemented for blogator
                default:{
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            this->insertCharacters( token->text() );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_EOF, token );
            //"If the current node is a script element, mark the script element as "already started"." - not implemented for blogator
            this->popOpenElements();
            this->setCurrentInsertMode( _original_insert_mode );
            this->processAsHTMLContent( std::move( token ), resolved ); //reprocess token
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->popOpenElements();
        this->setCurrentInsertMode( _original_insert_mode );
    };
}

/**
 * Processes a token in a "in table" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TABLE>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intable
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_CAPTION: {
                    this->clearStackBackToTableContext();
                    this->pushActiveFormattingElement( FORMATTING_MARKER );
                    this->insertHtmlElement( tk, resolved );
                    this->setCurrentInsertMode( InsertionMode_e::IN_CAPTION );
                } break;

                case Element_e::HTML5_COLGROUP: {
                    this->clearStackBackToTableContext();
                    this->insertHtmlElement( tk, resolved );
                    this->setCurrentInsertMode( InsertionMode_e::IN_COLUMN_GROUP );
                } break;

                case Element_e::HTML5_COL: {
                    this->clearStackBackToTableContext();

                    auto colgroup_tk = token::html5::StartTagTk( blogator::to_u32string( Element_e::HTML5_COLGROUP ), tk->position() );
                    this->insertForeignElement( &colgroup_tk, Namespace_e::HTML );

                    this->setCurrentInsertMode( InsertionMode_e::IN_COLUMN_GROUP );
                    this->processAsHTMLContent<InsertionMode_e::IN_COLUMN_GROUP>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_TBODY: [[fallthrough]];
                case Element_e::HTML5_TFOOT: [[fallthrough]];
                case Element_e::HTML5_THEAD: {
                    this->clearStackBackToTableContext();
                    this->insertHtmlElement( tk, resolved );
                    this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );
                } break;

                case Element_e::HTML5_TD: [[fallthrough]];
                case Element_e::HTML5_TH: [[fallthrough]];
                case Element_e::HTML5_TR: {
                    this->clearStackBackToTableContext();

                    auto colgroup_tk = token::html5::StartTagTk( blogator::to_u32string( Element_e::HTML5_TBODY ), tk->position() );
                    this->insertForeignElement( &colgroup_tk, Namespace_e::HTML );

                    this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );
                    this->processAsHTMLContent<InsertionMode_e::IN_TABLE_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_TABLE: {
                    std::stringstream ss;
                    ss << "<" << resolved << "> inside <" << Element_e::HTML5_TABLE << "> scope";
                    this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );

                    if( this->hasParticularElementInScope( resolved ) ) {
                        this->popOpenElementsUntil( resolved, true );
                        this->resetInsertionModeAppropriately();
                        this->processAsHTMLContent( std::move( token ) );
                    } //else: ignore token
                } break;

                case Element_e::HTML5_STYLE:  [[fallthrough]];
                case Element_e::HTML5_SCRIPT: [[fallthrough]];
                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_INPUT: {
                    const auto type_attr_it = std::find_if( tk->attributes().cbegin(),
                                                            tk->attributes().cend(),
                                                            []( const auto & attr ) { return attr.name == U"type"; } );

                    if( type_attr_it == tk->attributes().cend() ||
                        unicode::ascii::tolower( type_attr_it->value ) != U"hidden" )
                    {
                        goto anything_else;

                    } else {
                        this->logError( specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, token );

                        this->insertHtmlElement( tk, resolved );
                        this->popOpenElements();

                        if( tk->selfclosing() ) {
                            _acknowledgeSelfClosingTag_cb();
                        }
                    }

                } break;

                case Element_e::HTML5_FORM: {
                    std::stringstream ss;
                    ss << "<" << resolved << "> inside table";
                    this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );

                    if( !( this->hasOpenElement( Element_e::HTML5_TEMPLATE ) || _form_element != nullptr ) ) {
                        this->setFormElementPtr( this->insertHtmlElement( tk, resolved ) );
                        this->popOpenElements();
                    } //else: ignore token

                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TABLE: {
                    if( this->hasParticularElementInScope( resolved ) ) {
                        this->popOpenElementsUntil( resolved, true );
                        this->resetInsertionModeAppropriately();

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::ORPHANED_ENDTAG, token );
                    }
                } break;

                case Element_e::HTML5_BODY:     [[fallthrough]];
                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_HTML:     [[fallthrough]];
                case Element_e::HTML5_TBODY:    [[fallthrough]];
                case Element_e::HTML5_TD:       [[fallthrough]];
                case Element_e::HTML5_TFOOT:    [[fallthrough]];
                case Element_e::HTML5_TH:       [[fallthrough]];
                case Element_e::HTML5_THEAD:    [[fallthrough]];
                case Element_e::HTML5_TR: {
                    this->logError( specs::infra::ErrorCode::INVALID_END_TAG_IN_CURRENT_INSERTION_MODE, token );
                } break;

                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    this->logError( specs::infra::ErrorCode::INVALID_END_TAG_IN_CURRENT_INSERTION_MODE, token );
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            switch( this->currentNodeType() ) {
                case Element_e::HTML5_TABLE: [[fallthrough]];
                case Element_e::HTML5_TBODY: [[fallthrough]];
                case Element_e::HTML5_TFOOT: [[fallthrough]];
                case Element_e::HTML5_THEAD: [[fallthrough]];
                case Element_e::HTML5_TR: {
                    _pending_table_char_tokens.clear();
                    this->setOriginalInsertMode( _current_insert_mode );
                    this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_TEXT );
                    this->processAsHTMLContent<InsertionMode_e::IN_TABLE_TEXT>( std::move( token ), resolved );
                } break;

                default: {
                    this->logError( specs::infra::ErrorCode::FOSTER_PARENTING_TEXT, token );
                    goto anything_else;
                } break;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
        } break;

        default: {
            this->logError( specs::infra::ErrorCode::FOSTER_PARENTING_TOKEN, token );
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        _foster_parenting = true;
        this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
        _foster_parenting = false;
    };
}

/**
 * Processes a token in a "in table text" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TABLE_TEXT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intabletext
    switch( token->type() ) {
        case specs::infra::TokenType::CHARACTER: {
            for( auto c : token->text() ) {
                if( c == unicode::NUL ) {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_NULL_CHARACTER, token );
                } else {
                    _pending_table_char_tokens.emplace_back( c );
                }
            }
        } break;

        default: { //anything else
            const auto it = std::find_if( _pending_table_char_tokens.begin(),
                                          _pending_table_char_tokens.end(),
                                          []( auto c ) { return !unicode::ascii::iswspace( c ); } );

            if( it != _pending_table_char_tokens.end() ) {
                this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_FORWARDED, token );

                { //Reprocess as 'anything else', 'in table' insertion mode
                    _foster_parenting = true;
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>(
                        std::make_unique<token::html5::CharacterTk>(
                            DOMString_t( _pending_table_char_tokens.begin(), _pending_table_char_tokens.end() ),
                            token->position()
                        ),
                        resolved
                    );
                    _foster_parenting = false;
                }

            } else {
                this->insertCharacters( _pending_table_char_tokens.begin(), _pending_table_char_tokens.end() );
            }

            this->setCurrentInsertMode( _original_insert_mode );
            this->processAsHTMLContent( std::move( token ) );
        }
    }
}

/**
 * Processes a token in a "in caption" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_CAPTION>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incaption
    switch( token->type() ) {
        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_TBODY:    [[fallthrough]];
                case Element_e::HTML5_TD:       [[fallthrough]];
                case Element_e::HTML5_TFOOT:    [[fallthrough]];
                case Element_e::HTML5_TH:       [[fallthrough]];
                case Element_e::HTML5_THEAD:    [[fallthrough]];
                case Element_e::HTML5_TR: {
                    if( this->hasParticularElementInTableScope( Element_e::HTML5_CAPTION ) ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != Element_e::HTML5_CAPTION ) {
                            std::stringstream ss;
                            ss << "expected <" << Element_e::HTML5_CAPTION << ">, got <" << this->currentNodeType() << ">";
                            this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                        }

                        this->popOpenElementsUntil( Element_e::HTML5_CAPTION, true );
                        this->clearActiveFormattingElementsUpToLastMarker();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
                        this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << blogator::unicode::utf8::convert( tk->name() ) << "> inside caption";
                        this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );
                    }
                } break;

                default: {
                    goto anything_else;
                }
            }

        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_CAPTION: {
                    if( this->hasParticularElementInTableScope( resolved ) ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != resolved ) {
                            std::stringstream ss;
                            ss << "expected <" << resolved << ">, got <" << this->currentNodeType() << ">";
                            this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                        }

                        this->popOpenElementsUntil( resolved, true );
                        this->clearActiveFormattingElementsUpToLastMarker();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_TABLE: {
                    if( this->hasParticularElementInTableScope( Element_e::HTML5_CAPTION ) ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != Element_e::HTML5_CAPTION ) {
                            std::stringstream ss;
                            ss << "expected <" << Element_e::HTML5_CAPTION << ">, got <" << this->currentNodeType() << ">";
                            this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                        }

                        this->popOpenElementsUntil( Element_e::HTML5_CAPTION, true );
                        this->clearActiveFormattingElementsUpToLastMarker();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
                        this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> | <" << Element_e::HTML5_CAPTION << "> not found in table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_BODY:     [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_TBODY:    [[fallthrough]];
                case Element_e::HTML5_TD:       [[fallthrough]];
                case Element_e::HTML5_TFOOT:    [[fallthrough]];
                case Element_e::HTML5_TH:       [[fallthrough]];
                case Element_e::HTML5_THEAD:    [[fallthrough]];
                case Element_e::HTML5_TR: { //ignore token
                    this->logError( specs::infra::ErrorCode::INVALID_END_TAG_IN_CURRENT_INSERTION_MODE, token );
                } break;

                default: {
                    goto anything_else;
                }
            }

        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "in column group" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_COLUMN_GROUP>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-incolgroup
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_COL: {
                    this->insertHtmlElement( tk, resolved );
                    this->popOpenElements();

                    if( tk->selfclosing() ) {
                        _acknowledgeSelfClosingTag_cb();
                    }
                } break;

                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_COL: {
                    if( this->currentNodeType() == Element_e::HTML5_COLGROUP ) {
                        this->popOpenElements();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
                    }
                } break;

                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return blogator::unicode::ascii::iswspace( c ); } );

            if( it == token->text().cend() ) { //all white-space
                this->insertCharacters( token->text() );

            } else if( it != token->text().cbegin() ) { //some white-space
                this->insertCharacters( token->text().begin(), it );
                token->text().erase( token->text().begin(), it );
                goto anything_else;

            } else { //no white-space
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        if( this->currentNodeType() == Element_e::HTML5_COLGROUP ) {
            this->popOpenElements();
            this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
            this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );

        } else { //ignore token
            this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
        }
    };
}

/**
 * Processes a token in a "in table body" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TABLE_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intbody
    switch( token->type() ) {
        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TR: {
                    this->clearStackBackToTableBodyContext();
                    this->insertHtmlElement( tk, resolved );
                    this->setCurrentInsertMode( InsertionMode_e::IN_ROW );
                } break;

                case Element_e::HTML5_TH: [[fallthrough]];
                case Element_e::HTML5_TD: {
                    std::stringstream ss;
                    ss << "<" << resolved << "> inside table body";
                    this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );

                    this->clearStackBackToTableBodyContext();

                    auto tr_token = token::html5::StartTagTk( blogator::to_u32string( Element_e::HTML5_TR ), tk->position() );
                    this->insertForeignElement( &tr_token, Namespace_e::HTML );

                    this->setCurrentInsertMode( InsertionMode_e::IN_ROW );
                    this->processAsHTMLContent<InsertionMode_e::IN_ROW>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_TBODY:    [[fallthrough]];
                case Element_e::HTML5_TFOOT:    [[fallthrough]];
                case Element_e::HTML5_THEAD: {
                    if( this->hasParticularElementInTableScope( Element_e::HTML5_TBODY ) ||
                        this->hasParticularElementInTableScope( Element_e::HTML5_THEAD ) ||
                        this->hasParticularElementInTableScope( Element_e::HTML5_TFOOT ) )
                    {
                        this->clearStackBackToTableBodyContext();
                        this->popOpenElements();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
                        this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in either "
                           << "<" << Element_e::HTML5_TBODY << ">, <" << Element_e::HTML5_THEAD << "> or <" << Element_e::HTML5_TFOOT << "> table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TBODY: [[fallthrough]];
                case Element_e::HTML5_TFOOT: [[fallthrough]];
                case Element_e::HTML5_THEAD: {
                    if( this->hasParticularElementInTableScope( resolved ) ) {
                        this->clearStackBackToTableBodyContext();
                        this->popOpenElements();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_TABLE: {
                    if( this->hasParticularElementInTableScope( Element_e::HTML5_TBODY ) ||
                        this->hasParticularElementInTableScope( Element_e::HTML5_THEAD ) ||
                        this->hasParticularElementInTableScope( Element_e::HTML5_TFOOT ) )
                    {
                        this->clearStackBackToTableBodyContext();
                        this->popOpenElements();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
                        this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in either "
                           << "<" << Element_e::HTML5_TBODY << ">, <" << Element_e::HTML5_THEAD << "> or <" << Element_e::HTML5_TFOOT << "> table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_BODY:     [[fallthrough]];
                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_HTML:     [[fallthrough]];
                case Element_e::HTML5_TD:       [[fallthrough]];
                case Element_e::HTML5_TH:       [[fallthrough]];
                case Element_e::HTML5_TR: { //ignore token
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "in row" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_ROW>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intr
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TH: [[fallthrough]];
                case Element_e::HTML5_TD: {
                    this->clearStackBackToTableRowContext();
                    this->insertHtmlElement( tk, resolved );
                    this->setCurrentInsertMode( InsertionMode_e::IN_CELL );
                    this->pushActiveFormattingElement( FORMATTING_MARKER );
                } break;

                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_TBODY:    [[fallthrough]];
                case Element_e::HTML5_TFOOT:    [[fallthrough]];
                case Element_e::HTML5_THEAD:    [[fallthrough]];
                case Element_e::HTML5_TR: {
                    if( this->hasParticularElementInTableScope( Element_e::HTML5_TR ) ) {
                        this->clearStackBackToTableRowContext();
                        this->popOpenElements();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );
                        this->processAsHTMLContent<InsertionMode_e::IN_TABLE_BODY>( std::move( token ), resolved );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << blogator::unicode::utf8::convert( tk->name() ) << "> inside row";
                        this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );
                    }
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TR: {
                    if( this->hasParticularElementInTableScope( resolved ) ) {
                        this->clearStackBackToTableRowContext();
                        this->popOpenElements();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in <" << resolved << "> table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_TABLE: {
                    if( this->hasParticularElementInTableScope( Element_e::HTML5_TR ) ) {
                        this->clearStackBackToTableRowContext();
                        this->popOpenElements();
                        this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );
                        this->processAsHTMLContent<InsertionMode_e::IN_TABLE_BODY>( std::move( token ), resolved );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in <" << Element_e::HTML5_TR << "> table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_TBODY: [[fallthrough]];
                case Element_e::HTML5_TFOOT: [[fallthrough]];
                case Element_e::HTML5_THEAD: {
                    if( this->hasParticularElementInTableScope( resolved ) ) {
                        if( this->hasParticularElementInTableScope( Element_e::HTML5_TR ) ) {
                            this->clearStackBackToTableRowContext();
                            this->popOpenElements();
                            this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );
                            this->processAsHTMLContent<InsertionMode_e::IN_TABLE_BODY>( std::move( token ), resolved );
                        } //else: ignore token

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in <" << resolved << "> table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_BODY:     [[fallthrough]];
                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_HTML:     [[fallthrough]];
                case Element_e::HTML5_TD:       [[fallthrough]];
                case Element_e::HTML5_TH: { //ignore token
                    this->logError( specs::infra::ErrorCode::INVALID_END_TAG_IN_CURRENT_INSERTION_MODE, token );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "in cell" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_CELL>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intd
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_TBODY:    [[fallthrough]];
                case Element_e::HTML5_TD:       [[fallthrough]];
                case Element_e::HTML5_TFOOT:    [[fallthrough]];
                case Element_e::HTML5_TH:       [[fallthrough]];
                case Element_e::HTML5_THEAD:    [[fallthrough]];
                case Element_e::HTML5_TR: {
                    if( this->hasParticularElementInTableScope( Element_e::HTML5_TD ) ||
                        this->hasParticularElementInTableScope( Element_e::HTML5_TH ) )
                    {
                        this->closeCell( token->position() );
                        this->processAsHTMLContent( std::move( token ), resolved );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << blogator::unicode::utf8::convert( tk->name() ) << "> inside cell";
                        this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );
                    }
                } break;


                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TD: [[fallthrough]];
                case Element_e::HTML5_TH: {
                    if( this->hasParticularElementInTableScope( resolved ) ) {
                        this->generateImpliedEndTags();

                        if( this->currentNodeType() != resolved ) {
                            std::stringstream ss;
                            ss << "expected <" << resolved << ">, got <" << this->currentNodeType() << ">";
                            this->logError( token->position(), specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
                        }

                        this->popOpenElementsUntil( resolved, true );
                        this->clearActiveFormattingElementsUpToLastMarker();
                        this->setCurrentInsertMode( InsertionMode_e::IN_ROW );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in <" << resolved << "> table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_BODY:     [[fallthrough]];
                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COL:      [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_HTML: { //ignore token
                    this->logError( specs::infra::ErrorCode::INVALID_END_TAG_IN_CURRENT_INSERTION_MODE, token );
                } break;

                case Element_e::HTML5_TABLE: [[fallthrough]];
                case Element_e::HTML5_TBODY: [[fallthrough]];
                case Element_e::HTML5_TFOOT: [[fallthrough]];
                case Element_e::HTML5_THEAD: [[fallthrough]];
                case Element_e::HTML5_TR: {
                    if( this->hasParticularElementInTableScope( resolved ) ) {
                        this->closeCell( token->position() );
                        this->processAsHTMLContent( std::move( token ) );

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in <" << resolved << "> table scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "in select" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_SELECT>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inselect
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_OPTION: {
                    if( this->currentNodeType() == Element_e::HTML5_OPTION ) {
                        this->popOpenElements();
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::HTML5_OPTGROUP: {
                    if( this->currentNodeType() == Element_e::HTML5_OPTION ) {
                        this->popOpenElements();
                    }

                    if( this->currentNodeType() == Element_e::HTML5_OPTGROUP ) {
                        this->popOpenElements();
                    }

                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::HTML5_SELECT: {
                    this->logError( specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, token );

                    if( this->hasParticularElementInSelectScope( resolved ) ) {
                        this->popOpenElementsUntil( resolved, true );
                        this->resetInsertionModeAppropriately();
                    } //else: ignore token
                } break;

                case Element_e::HTML5_INPUT:      [[fallthrough]];
                case Element_e::DEPR_HTML_KEYGEN: [[fallthrough]];
                case Element_e::HTML5_TEXTAREA: {
                    this->logError( specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, token );

                    if( this->hasParticularElementInSelectScope( Element_e::HTML5_SELECT ) ) {
                        this->popOpenElementsUntil( Element_e::HTML5_SELECT, true );
                        this->resetInsertionModeAppropriately();
                        this->processAsHTMLContent( std::move( token ) );
                    } //else: ignore token
                } break;

                case Element_e::HTML5_SCRIPT: [[fallthrough]];
                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_STARTTAG, token );
                } break;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_OPTGROUP: {
                    if( this->currentNodeType() == Element_e::HTML5_OPTION ) {
                        const auto next = std::next( this->currentNodeIterator() );

                        if( (*next)->elementType() == Element_e::HTML5_OPTGROUP ) {
                            this->popOpenElements();
                        }
                    }

                    if( this->currentNodeType() == Element_e::HTML5_OPTGROUP ) {
                        this->popOpenElements();

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
                    }
                } break;

                case Element_e::HTML5_OPTION: {
                    if( this->currentNodeType() == Element_e::HTML5_OPTION ) {
                        this->popOpenElements();

                    } else { //ignore token
                        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
                    }
                } break;

                case Element_e::HTML5_SELECT: {
                    if( this->hasParticularElementInSelectScope( resolved ) ) {
                        this->popOpenElementsUntil( resolved, true );
                        this->resetInsertionModeAppropriately();

                    } else { //ignore token
                        std::stringstream ss;
                        ss << "<" << resolved << "> not found in <" << resolved << "> select scope";
                        this->logError( token->position(), specs::infra::ErrorCode::ORPHANED_ENDTAG_IN_SCOPE, ss.str() );
                    }
                } break;

                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );
                } break;
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto buffer = std::vector<DOMString_t::value_type>();

            for( auto &c: token->text() ) {
                if ( c == unicode::NUL ) { //ignore token
                    this->logError( specs::infra::ErrorCode::UNEXPECTED_NULL_CHARACTER, token );

                } else {
                    buffer.emplace_back( c );
                }
            }

            if( !buffer.empty() ) {
                this->insertCharacters( buffer.cbegin(), buffer.cend() );
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
        } break;

        default: break;
    }
}

/**
 * Processes a token in a "in select in table" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_SELECT_IN_TABLE>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inselectintable
    switch( token->type() ) {
        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_CAPTION: [[fallthrough]];
                case Element_e::HTML5_TABLE:   [[fallthrough]];
                case Element_e::HTML5_TBODY:   [[fallthrough]];
                case Element_e::HTML5_TFOOT:   [[fallthrough]];
                case Element_e::HTML5_THEAD:   [[fallthrough]];
                case Element_e::HTML5_TR:      [[fallthrough]];
                case Element_e::HTML5_TD:      [[fallthrough]];
                case Element_e::HTML5_TH: {
                    std::stringstream ss;
                    ss << "<" << resolved << "> inside a select table scope";
                    this->logError( token->position(), specs::infra::ErrorCode::INVALID_NESTING_OF_ELEMENT, ss.str() );

                    this->popOpenElementsUntil( Element_e::HTML5_SELECT, true );
                    this->resetInsertionModeAppropriately();
                    this->processAsHTMLContent( std::move( token ) );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_CAPTION: [[fallthrough]];
                case Element_e::HTML5_TABLE:   [[fallthrough]];
                case Element_e::HTML5_TBODY:   [[fallthrough]];
                case Element_e::HTML5_TFOOT:   [[fallthrough]];
                case Element_e::HTML5_THEAD:   [[fallthrough]];
                case Element_e::HTML5_TR:      [[fallthrough]];
                case Element_e::HTML5_TD:      [[fallthrough]];
                case Element_e::HTML5_TH: {
                    this->logError( specs::infra::ErrorCode::INVALID_END_TAG_IN_CURRENT_INSERTION_MODE, token );

                    if( this->hasParticularElementInTableScope( resolved ) ) {
                        this->popOpenElementsUntil( Element_e::HTML5_SELECT, true );
                        this->resetInsertionModeAppropriately();
                        this->processAsHTMLContent( std::move( token ) );
                    } //else: ignore token
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->processAsHTMLContent<InsertionMode_e::IN_SELECT>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "in template" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_TEMPLATE>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-intemplate
    switch( token->type() ) {
        case specs::infra::TokenType::CHARACTER: [[fallthrough]];
        case specs::infra::TokenType::COMMENT:   [[fallthrough]];
        case specs::infra::TokenType::DOCTYPE: {
            this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_BASE:         [[fallthrough]];
                case Element_e::DEPR_HTML_BASEFONT: [[fallthrough]];
                case Element_e::DEPR_HTML_BGSOUND:  [[fallthrough]];
                case Element_e::HTML5_LINK:         [[fallthrough]];
                case Element_e::HTML5_META:         [[fallthrough]];
                case Element_e::DEPR_HTML_NOFRAMES: [[fallthrough]];
                case Element_e::HTML5_SCRIPT:       [[fallthrough]];
                case Element_e::HTML5_STYLE:        [[fallthrough]];
                case Element_e::HTML5_TEMPLATE:     [[fallthrough]];
                case Element_e::HTML5_TITLE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_CAPTION:  [[fallthrough]];
                case Element_e::HTML5_COLGROUP: [[fallthrough]];
                case Element_e::HTML5_TBODY:    [[fallthrough]];
                case Element_e::HTML5_TFOOT:    [[fallthrough]];
                case Element_e::HTML5_THEAD: {
                    this->popTemplateInsertionModes();
                    this->pushTemplateInsertionModes( InsertionMode_e::IN_TABLE );
                    this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
                    this->processAsHTMLContent<InsertionMode_e::IN_TABLE>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_COL: {
                    this->popTemplateInsertionModes();
                    this->pushTemplateInsertionModes( InsertionMode_e::IN_COLUMN_GROUP );
                    this->setCurrentInsertMode( InsertionMode_e::IN_COLUMN_GROUP );
                    this->processAsHTMLContent<InsertionMode_e::IN_COLUMN_GROUP>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_TR: {
                    this->popTemplateInsertionModes();
                    this->pushTemplateInsertionModes( InsertionMode_e::IN_TABLE_BODY );
                    this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );
                    this->processAsHTMLContent<InsertionMode_e::IN_TABLE_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::HTML5_TH: [[fallthrough]];
                case Element_e::HTML5_TD: {
                    this->popTemplateInsertionModes();
                    this->pushTemplateInsertionModes( InsertionMode_e::IN_ROW );
                    this->setCurrentInsertMode( InsertionMode_e::IN_ROW );
                    this->processAsHTMLContent<InsertionMode_e::IN_ROW>( std::move( token ), resolved );
                } break;

                default: {
                    this->popTemplateInsertionModes();
                    this->pushTemplateInsertionModes( InsertionMode_e::IN_BODY );
                    this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_TEMPLATE: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: { //ignore token
                    this->logError( specs::infra::ErrorCode::INVALID_END_TAG_IN_CURRENT_INSERTION_MODE, token );
                } break;
            }
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            if( this->hasOpenElement( Element_e::HTML5_TEMPLATE ) ) {
                this->logError( specs::infra::ErrorCode::UNEXPECTED_EOF_IN_TEMPLATE, token );
                this->popOpenElementsUntil( Element_e::HTML5_TEMPLATE, true );
                this->clearActiveFormattingElementsUpToLastMarker();
                this->popTemplateInsertionModes();
                this->resetInsertionModeAppropriately();
                this->processAsHTMLContent( std::move( token ), resolved );

            } else {
                this->stopParsing();
            }
        } break;

        default: break;
    }
}

/**
 * Processes a token in a "after body" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterbody
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    if( _fragment_case ) { //ignore token
                        this->logError( specs::infra::ErrorCode::INVALID_ELEMENT_IN_FRAGMENT_CASE, token );

                    } else {
                        this->setCurrentInsertMode( InsertionMode_e::AFTER_AFTER_BODY );
                    }
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return blogator::unicode::ascii::iswspace( c ); } );

            if( it == token->text().cend() ) { //all white-space
                this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );

            } else if( it != token->text().cbegin() ) { //some white-space
                this->processAsHTMLContent<InsertionMode_e::IN_BODY>(
                    std::make_unique<token::html5::CharacterTk>( DOMString_t( token->text().begin(), it ), token->position() ),
                    resolved
                );

                token->text().erase( token->text().begin(), it );
                goto anything_else;

            } else { //no white-space
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token, { this->getOpenElement( 0 ), nullptr } );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->stopParsing();
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_FORWARDED, token );
        this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
        this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "in frameset" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::IN_FRAMESET>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-inframeset
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::DEPR_HTML_FRAMESET: {
                    this->insertHtmlElement( tk, resolved );
                } break;

                case Element_e::DEPR_HTML_FRAME: {
                    this->insertHtmlElement( tk, resolved );
                    this->popOpenElements();

                    if( tk->selfclosing() ) {
                        _acknowledgeSelfClosingTag_cb();
                    }
                } break;

                case Element_e::DEPR_HTML_NOFRAMES: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::DEPR_HTML_FRAMESET: {
                    if( _open_elements.size() == 1 && this->currentNodeType() == Element_e::HTML5_HTML ) { //ignore token
                        this->logError( specs::infra::ErrorCode::UNEXPECTED_ENDTAG, token );

                    } else {
                        this->popOpenElements();

                        if( !_fragment_case && this->currentNodeType() != Element_e::DEPR_HTML_FRAMESET ) {
                            this->setCurrentInsertMode( InsertionMode_e::AFTER_FRAMESET );
                        }
                    }
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return blogator::unicode::ascii::iswspace( c ); } );

            if( it == token->text().cend() ) { //all white-space
                this->insertCharacters( token->text() );

            } else if( it != token->text().cbegin() ) { //some white-space
                this->insertCharacters( token->text().begin(), it );
                token->text().erase( token->text().begin(), it );
                goto anything_else;

            } else { //no white-space
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            if( this->currentNodeType() != Element_e::HTML5_HTML ) {
                this->logError( specs::infra::ErrorCode::UNEXPECTED_EOF, token );
            }

            this->stopParsing();
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: { //ignore token
        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
    };
}

/**
 * Processes a token in a "after frameset" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_FRAMESET>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#parsing-main-afterframeset
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->logError( specs::infra::ErrorCode::UNEXPECTED_DOCTYPE, token );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );
            
            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::DEPR_HTML_NOFRAMES: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::END_TAG: {
            auto * tk = dynamic_cast<token::html5::EndTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->setCurrentInsertMode( InsertionMode_e::AFTER_AFTER_FRAMESET );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return blogator::unicode::ascii::iswspace( c ); } );

            if( it == token->text().cend() ) { //all white-space
                this->insertCharacters( token->text() );

            } else if( it != token->text().cbegin() ) { //some white-space
                this->insertCharacters( token->text().begin(), it );
                token->text().erase( token->text().begin(), it );
                goto anything_else;

            } else { //no white-space
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            this->insertComment( token );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->stopParsing();
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: { //ignore token
        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
    };
}

/**
 * Processes a token in a "after after body" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_AFTER_BODY>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#the-after-after-body-insertion-mode
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return blogator::unicode::ascii::iswspace( c ); } );

            if( it == token->text().cend() ) { //all white-space
                this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );

            } else if( it != token->text().cbegin() ) { //some white-space
                this->processAsHTMLContent<InsertionMode_e::IN_BODY>(
                    std::make_unique<token::html5::CharacterTk>( DOMString_t( token->text().begin(), it ), token->position() ),
                    resolved
                );

                token->text().erase( token->text().begin(), it );
                goto anything_else;

            } else { //no white-space
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            _document->appendChild( std::make_unique<node::Comment>( token->text() ) );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->stopParsing();
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: {
        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_FORWARDED, token );
        this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
        this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
    };
}

/**
 * Processes a token in a "after after frameset" insertion mode
 * @param token Token
 * @param resolved Element type if previously resolved
 */
template<> void TreeBuilder::processAsHTMLContent<TreeBuilder::InsertionMode_e::AFTER_AFTER_FRAMESET>( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#the-after-after-frameset-insertion-mode
    switch( token->type() ) {
        case specs::infra::TokenType::DOCTYPE: {
            this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
        } break;

        case specs::infra::TokenType::START_TAG: {
            auto * tk = dynamic_cast<token::html5::StartTagTk *>( token.get() );

            switch( ( resolved = this->resolveElement( resolved, Namespace_e::UNKNOWN, tk->name() ) ) ) {
                case Element_e::HTML5_HTML: {
                    this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );
                } break;

                case Element_e::DEPR_HTML_NOFRAMES: {
                    this->processAsHTMLContent<InsertionMode_e::IN_HEAD>( std::move( token ), resolved );
                } break;

                default: {
                    goto anything_else;
                }
            }
        } break;

        case specs::infra::TokenType::CHARACTER: {
            auto it = std::find_if_not( token->text().begin(),
                                        token->text().end(),
                                        []( auto c ) { return blogator::unicode::ascii::iswspace( c ); } );

            if( it == token->text().cend() ) { //all white-space
                this->processAsHTMLContent<InsertionMode_e::IN_BODY>( std::move( token ), resolved );

            } else if( it != token->text().cbegin() ) { //some white-space
                this->processAsHTMLContent<InsertionMode_e::IN_BODY>(
                    std::make_unique<token::html5::CharacterTk>( DOMString_t( token->text().begin(), it ), token->position() ),
                    resolved
                );

                token->text().erase( token->text().begin(), it );
                goto anything_else;

            } else { //no white-space
                goto anything_else;
            }
        } break;

        case specs::infra::TokenType::COMMENT: {
            _document->appendChild( std::make_unique<node::Comment>( token->text() ) );
        } break;

        case specs::infra::TokenType::END_OF_FILE: {
            this->stopParsing();
        } break;

        default: {
            goto anything_else;
        }
    }

    return; //DEFAULT

    //-----------
    anything_else: { //ignore token
        this->logError( specs::infra::ErrorCode::UNEXPECTED_TOKEN_DISCARDED, token );
    };
}

/**
 * Check of document is an iframe srcdoc document
 * @return IFrame srcdoc state
 */
bool TreeBuilder::isIFrameSrcDoc() const { //TODO
    return false;
}

/**
 * Sets the current insertion mode
 * @param mode InsertionMode enum
 */
inline void TreeBuilder::setCurrentInsertMode( TreeBuilder::InsertionMode_e mode ) {
    LOG_TRACE(
        "[parser::dom::TreeBuilder::setCurrentInsertMode( '", mode, "' )] "
        "Current node: ", ( this->currentNode() ? blogator::to_string( this->currentNodeType() ) : "NONE" )
    );
    _current_insert_mode = mode;
}

/**
 * Sets the original insertion mode
 * @param mode InsertionMode enum
 */
inline void TreeBuilder::setOriginalInsertMode( TreeBuilder::InsertionMode_e mode ) {
    _original_insert_mode = mode;
}

/**
 * Sets the 'head' element pointer
 * @param head_ptr Pointer to head element
 */
void TreeBuilder::setHeadElementPtr( node::Element * head_ptr ) {
    _head_element = head_ptr;
}

/**
 * Sets the 'form' element pointer
 * @param form_ptr Pointer to form element
 */
void TreeBuilder::setFormElementPtr( node::Element * form_ptr ) {
    _form_element = form_ptr;
}

/**
 * Gets the last element pushed onto the stack of open elements
 * @return Pointer to last element pushed (or nullptr if stack is empty)
 */
inline blogator::parser::dom::node::Element * TreeBuilder::currentNode() {
    return ( _open_elements.empty() ? nullptr : _open_elements.front() );
}

/**
 * Gets the iterator to the last element pushed onto the stack of open elements
 * @return Iterator to last element pushed (end() if stack is empty)
 */
inline TreeBuilder::OpenElements_t::iterator TreeBuilder::currentNodeIterator() {
    return _open_elements.begin();
}

/**
 * Gets the iterator to the last element pushed onto the stack of open elements
 * @return Const iterator to last element pushed (cend() if stack is empty)
 */
inline TreeBuilder::OpenElements_t::const_iterator TreeBuilder::currentNodeIterator() const {
    return _open_elements.cbegin();
}

/**
 * Gets the element type of the last element pushed onto the stack of open elements
 * @return specs::infra::Element type (if node is `nullptr`, `UNKNOWN` will be returned)
 */
inline TreeBuilder::Element_e TreeBuilder::currentNodeType() const {
    return ( _open_elements.empty() ? Element_e::UNKNOWN : _open_elements.front()->elementType() );
}

/**
 * Gets a node via index from the top (first pushed) of the open element stack
 * @param index_from_top Index
 * @return Pointer to element at index or nullptr when out-of-range
 */
inline blogator::parser::dom::node::Element *TreeBuilder::getOpenElement( size_t index_from_top ) {
    if( index_from_top < _open_elements.size() ) {
        return *( std::next( _open_elements.rbegin(), index_from_top ) ); //EARLY RETURN
    }

    return nullptr;
}

/**
 * Gets the adjusted current node
 * @return Adjusted current node
 */
blogator::parser::dom::node::Element * TreeBuilder::adjustedCurrentNode() {
    if( _fragment_case && _open_elements.size() == 1 ) {
        return _context_element; //EARLY RETURN
    }

    return this->currentNode();
}

/**
 * Push an Element onto the stack of open elements
 * @param node Pointer to Element node
 * @throws blogator::exception::failed_expectation when element pointer is null
 */
inline void TreeBuilder::pushOpenElements( node::Element * node ) {
    if( node != nullptr ) {
        LOG_TRACE(
            "[parser::dom::TreeBuilder::pushOpenElements( <", node->elementType(), "> )]"
        );

        _open_elements.push_front( node );

    } else {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::TreeBuilder::pushOpenElements( node::Element * )] nullptr Element passed."
        );
    }
}

/**
 * Pops the last Element pushed onto the stack of open elements
 */
inline void TreeBuilder::popOpenElements() {
    if( !_open_elements.empty() ) {
        _open_elements.pop_front();
    }
}

/**
 * Pops the last Element pushed onto the stack of open elements until element is found or <HTML> is reached
 * @param element Element type to pop up to
 * @param inclusive Include reached element in popping
 */
inline void TreeBuilder::popOpenElementsUntil( TreeBuilder::Element_e element, bool inclusive ) {
    while( !_open_elements.empty() ) {
        const auto curr_element = this->currentNodeType();

        if( curr_element == Element_e::HTML5_HTML ) {
            break;
        }

        if( curr_element == element ) {
            if( inclusive ) {
                _open_elements.pop_front();
            }

            break;
        }

        _open_elements.pop_front();
    }
}

/**
 * Pops the last Element pushed onto the stack of open elements until element is found or <HTML> is reached
 * @param element Element name (qualified, lowercase) to pop up to
 * @param inclusive Include reached element in popping
 */
void TreeBuilder::popOpenElementsUntil( const DOMString_t & element, bool inclusive ) {
    while( !_open_elements.empty() ) {
        const auto curr_type = this->currentNodeType();

        if( curr_type == Element_e::HTML5_HTML ) {
            break;
        }

        if( blogator::unicode::ascii::tolower( this->currentNode()->qualifiedName() ) == element ) {
            if( inclusive ) {
                _open_elements.pop_front();
            }

            break;
        }

        _open_elements.pop_front();
    }
}

/**
 * Pops the last Element pushed onto the stack of open elements until an element is reached
 * @param elements Element(s) to pop up to
 * @param inclusive Include reached element in popping
 */
inline void TreeBuilder::popOpenElementsUntil( const std::set<Element_e> & elements, bool inclusive ) {
    while( !_open_elements.empty() && !elements.contains( this->currentNodeType() ) ) {
        this->popOpenElements();
    }

    if( inclusive && !_open_elements.empty() ) {
        _open_elements.pop_front();
    }
}

/**
 * Checks if an element exists in the stack of open elements
 * @param element Element enum
 * @return Existence
 */
bool TreeBuilder::hasOpenElement( TreeBuilder::Element_e element ) const {
    const auto tag_name = blogator::to_u32string( element );

    const auto it = std::find_if( _open_elements.cbegin(),
                                  _open_elements.cend(),
                                  [&tag_name]( node::Element * node ) { return node->qualifiedName() == tag_name; } );

    return ( it != _open_elements.cend() );
}

/**
 * Finds an element in stack of open elements
 * @param node_ptr Element pointer
 * @return Iterator to first match
 */
TreeBuilder::OpenElements_t::iterator TreeBuilder::findOpenElement( const node::Element * node_ptr ) {
    return std::find_if( _open_elements.begin(),
                         _open_elements.end(),
                         [node_ptr]( node::Element * el ) { return el == node_ptr; } );
}

/**
 * Finds the 'furthest block' from a reference element in the stack of open elements
 * @param ref_it Reference element iterator
 * @return Iterator to 'furthest block' (or end() if none is found)
 */
TreeBuilder::OpenElements_t::iterator TreeBuilder::findFurthestBlockInOpenElements( TreeBuilder::OpenElements_t::iterator ref_it ) {
    // def: ".. topmost node in the stack of open elements that is lower in the stack than formatting element,
    //       and is an element in the special category. There might not be one."
    if( ref_it != _open_elements.begin() ) {
        do {
            std::advance( ref_it, -1 );

            if( (*ref_it) != FORMATTING_MARKER && TreeBuilder::isSpecialElement( (*ref_it)->elementType(), {} ) ) {
                return ref_it; //EARLY RETURN
            }

        } while( ref_it != _open_elements.begin() );
    }

    return _open_elements.end();
}

/**
 * Removes an element in the stack of open elements
 * @param it Iterator to erase (end() will return end())
 * @return Iterator pointing to next element (or end())
 */
TreeBuilder::OpenElements_t::iterator TreeBuilder::removeOpenElement( const OpenElements_t::iterator & it ) {
    if( it != _open_elements.end() ) {
        LOG_TRACE(
            "[parser::dom::TreeBuilder::removeOpenElement( <", *it, "> )]"
        );
        return _open_elements.erase( it ); //EARLY RETURN
    }

    return _open_elements.end();
}

/**
 * Pushes an element onto the list of active formatting elements
 * @param node Pointer to formatting Element (passing `nullptr` will do nothing)
 */
void TreeBuilder::pushActiveFormattingElement( dom::node::Element * node ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#push-onto-the-list-of-active-formatting-elements
    auto   matching_elements    = std::vector<ActiveFormattingElements_t::const_iterator>();
    auto   curr_it              = _formatting_elements.cbegin();
    size_t consecutive_elements = 0;

    while( curr_it != _formatting_elements.cend() ) {
        if( *curr_it == FORMATTING_MARKER ) {
            break;

        } else if( node != FORMATTING_MARKER ) {
            if( (*curr_it)->namespaceID() == node->namespaceID() &&
                (*curr_it)->tagName()     == node->tagName()     &&
                (*curr_it)->attributes()  == node->attributes() )
            {
                matching_elements.emplace_back( curr_it );
            }
        }

        ++curr_it;
        ++consecutive_elements;
    }

    if( matching_elements.size() == 3 ) {
        _formatting_elements.erase( matching_elements.back() );
    }

    _formatting_elements.push_front( node );
}

/**
 * Reconstructs the active formatting elements
 * @param position Position at calling token (to help with logging)
 */
void TreeBuilder::reconstructActiveFormattingElements( const TextPos & position ) {
    if( _formatting_elements.empty()                      ||
        _formatting_elements.front() == FORMATTING_MARKER ||
        std::find( _open_elements.cbegin(), _open_elements.cend(), _formatting_elements.front() ) != _open_elements.cend() )
    {
        return; //EARLY RETURN
    }

    auto entry_it = _formatting_elements.begin();

    rewind: {
        if( std::next( entry_it ) == _formatting_elements.cend() ) {
            goto create;
        }
    };

    std::advance( entry_it, 1 );

    if( *entry_it != FORMATTING_MARKER &&
        std::find( _open_elements.cbegin(), _open_elements.cend(), *entry_it ) == _open_elements.cend() )
    {
        goto rewind;
    }

    advance: {
        std::advance( entry_it, -1 );
    };

    create: {
        auto token = token::html5::StartTagTk( (*entry_it)->localName(), position );

        for( const auto & attr : (*entry_it)->attributes().list() ) {
            token.addAttribute( attr->name(), ( attr->hasValue() ? *attr->value() : DOMString_t() ) );
        }

        auto * new_element = this->insertForeignElement( &token, Namespace_e::HTML );

        if( new_element == nullptr ) {
            using blogator::unicode::utf8::convert;

            LOG_ERROR(
                "[parser::dom:: TreeBuilder::reconstructActiveFormattingElements()] "
                "Failed to insert new element \"", convert( (*entry_it)->localName() ) ,"\"."
            );
        }

        *entry_it = new_element;

        if( entry_it != _formatting_elements.cbegin() ) {
            goto advance;
        }
    };
}

/**
 * Clears the list of active formatting elements up to the last marker
 */
void TreeBuilder::clearActiveFormattingElementsUpToLastMarker() {
    if( !_formatting_elements.empty() ) {
        auto * entry = _formatting_elements.front();

        while( !_formatting_elements.empty() ) {
            entry = _formatting_elements.front();

            _formatting_elements.pop_front();

            if( entry == TreeBuilder::FORMATTING_MARKER ) {
                return; //EARLY RETURN
            }
        }
    }
}

/**
 * Finds an element in list of active formatting elements
 * @param node_ptr Element pointer
 * @return Iterator to first match (from latest to earliest)
 */
TreeBuilder::ActiveFormattingElements_t::iterator TreeBuilder::findActiveFormattingElement( const node::Element * node_ptr ) {
    auto it = _formatting_elements.begin();

    while( it != _formatting_elements.end() ) {
        if( (*it) == node_ptr ) {
            return it;
        }

        ++it;
    }

    return it;
}

/**
 * Finds a formatting element before the last marker in the list of active formatting elements
 * @param element Element to find
 * @return Iterator to first element found before last marker or the list if no markers are present
 */
TreeBuilder::ActiveFormattingElements_t::iterator TreeBuilder::findActiveFormattingElementAfterLastMarker( TreeBuilder::Element_e element ) {
    auto entry = _formatting_elements.begin();

    while( entry != _formatting_elements.end() && (*entry) != FORMATTING_MARKER ) {
        if( (*entry)->elementType() == element ) {
            return entry; //EARLY RETURN
        }

        ++entry;
    }

    return _formatting_elements.end();
}

/**
 * Inserts an element in the list of active formatting elements
 * @param index Index location to insert element before (if >= than size, will be appended to end)
 * @param element Element pointer to insert
 * @return Iterator to newly inserted element
 */
TreeBuilder::ActiveFormattingElements_t::iterator TreeBuilder::insertActiveFormattingElement( size_t index, node::Element * element ) {
    if( index < _formatting_elements.size() ) {
        return _formatting_elements.insert( std::next( _formatting_elements.begin(), index ), element );
    } else {
        return _formatting_elements.insert( _formatting_elements.end(), element );
    }
}

/**
 * Removes an element in the list of active formatting elements
 * @param it Iterator to erase (end() will return end())
 * @return Iterator pointing to next element (or end())
 */
TreeBuilder::ActiveFormattingElements_t::iterator TreeBuilder::removeActiveFormattingElement( const ActiveFormattingElements_t::iterator & it ) {
    if( it != _formatting_elements.end() ) {
        return _formatting_elements.erase( it ); //EARLY RETURN
    }

    return _formatting_elements.end();
}

/**
 * Pops a mode from the 'stack of template insertion modes'
 */
inline void TreeBuilder::popTemplateInsertionModes() {
    if( !_template_insert_modes.empty() ) {
        LOG_TRACE(
            "[parser::dom::TreeBuilder::popTemplateInsertionModes()] '", _template_insert_modes.top(), "'"
        );

        _template_insert_modes.pop();
    }
}

/**
 * Pushes a mode on the 'stack of template insertion modes'
 * @param mode Insertion mode
 */
inline void TreeBuilder::pushTemplateInsertionModes( TreeBuilder::InsertionMode_e mode ) {
    LOG_TRACE(
        "[parser::dom::TreeBuilder::pushTemplateInsertionModes( '", mode, "' )]"
    );

    _template_insert_modes.push( mode );
}

/**
 * Inserts an HTML Element
 * @param token Tag token
 * @param resolved_element Resolved Element type
 * @return Pointer to inserted element (or nullptr)
 */
node::Element * TreeBuilder::insertHtmlElement( token::html5::StartTagTk * token, Element_e resolved_element ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#insert-an-html-element

    auto   adj_insert_loc = this->appropriateInsertionPoint();
    auto * start_tk       = dynamic_cast<token::html5::StartTagTk *>( token );
    auto   ns_id          = NamespaceMap::NONE;

    if( resolved_element == Element_e::OTHER ) {
        ns_id = _document->namespaceMap().setNamespace( Namespace_e::HTML );
    } else {
        ns_id = _document->namespaceMap().setNamespace( specs::infra::resolveNamespace( resolved_element ) );
    }

    auto node = _document->createElementNS( _document->getNamespaceURI( ns_id ), start_tk->name(), _strict_validation );

    for( const auto & attr : start_tk->attributes() ) {
        if( TreeBuilder::isXLink( attr.name ) ) {
            node->createAttributeNS( specs::infra::to_namespaceURI( Namespace_e::XLINK ),
                                     attr.name,
                                     attr.value,
                                     _strict_validation );

        } else if( TreeBuilder::isXML( attr.name ) ) {
            node->createAttributeNS( specs::infra::to_namespaceURI( Namespace_e::XML ),
                                     attr.name,
                                     attr.value,
                                     _strict_validation );

        } else if( TreeBuilder::isXMLNS( attr.name ) ) {
            node->createAttributeNS( specs::infra::to_namespaceURI( Namespace_e::XMLNS ),
                                     attr.name,
                                     attr.value,
                                     _strict_validation );

        } else {
            if( attr.value.empty() ) {
                node->createAttribute( attr.name, _strict_validation );
            } else {
                node->createAttribute( attr.name, attr.value, _strict_validation );
            }
        }
    }

    auto * inserted_node = adj_insert_loc.parent->insertBefore( std::move( node ), adj_insert_loc.child );

    if( inserted_node != nullptr ) {
        this->pushOpenElements( dynamic_cast<node::Element *>( inserted_node ) );
    }

    return dynamic_cast<node::Element *>( inserted_node );
}


/**
 * Inserts a foreign Element
 * @param token Tag token
 * @param ns_hint Alleged namespace for the token name (`UNKNOWN`: auto-resolve to insertion point's namespace)
 * @return Pointer to inserted element (or nullptr)
 */
blogator::parser::dom::node::Element * TreeBuilder::insertForeignElement( token::html5::StartTagTk * token, Namespace_e ns_hint ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#insert-a-foreign-element
    auto   adj_insert_loc = this->appropriateInsertionPoint();
    auto * parent         = adj_insert_loc.parent;
    auto * start_tk       = dynamic_cast<token::html5::StartTagTk *>( token );
    auto   ns_id          = NamespaceMap::NONE;
    auto   ns             = Namespace_e::NONE;

    if( ns_hint == Namespace_e::UNKNOWN && parent->nodeType() == NodeType::ELEMENT_NODE ) {
        ns_id = dynamic_cast<node::Element *>( parent )->namespaceID();
        ns = _document->getNamespaceEnum( ns_id );

    } else {
        ns_id = _document->namespaceMap().setNamespace( ns_hint );
        ns    = ns_hint;
    }

    if( ns == Namespace_e::MATHML ) {
        for( auto & attr : start_tk->attributes() ) {
            attr.name = specs::infra::adjustMathMLAttribute( attr.name );
        }

    } else if( ns == Namespace_e::SVG ) {
        start_tk->name() = specs::infra::adjustSVGAttribute( start_tk->name() );

        for( auto & attr : start_tk->attributes() ) {
            attr.name = specs::infra::adjustSVGAttribute( attr.name );
        }
    }

    auto node = _document->createElementNS( _document->getNamespaceURI( ns_id ), start_tk->name(), _strict_validation );

    for( const auto & attr : start_tk->attributes() ) {
        if( TreeBuilder::isXLink( attr.name ) ) {
            node->createAttributeNS( specs::infra::to_namespaceURI( Namespace_e::XLINK ),
                                     attr.name,
                                     attr.value,
                                     _strict_validation );

        } else if( TreeBuilder::isXML( attr.name ) ) {
            node->createAttributeNS( specs::infra::to_namespaceURI( Namespace_e::XML ),
                                     attr.name,
                                     attr.value,
                                     _strict_validation );

        } else if( TreeBuilder::isXMLNS( attr.name ) ) {
            node->createAttributeNS( specs::infra::to_namespaceURI( Namespace_e::XMLNS ),
                                     attr.name,
                                     attr.value,
                                     _strict_validation );

        } else {
            if( attr.value.empty() ) {
                node->createAttribute( attr.name, ValidationState::ON );
            } else {
                node->createAttribute( attr.name, attr.value, ValidationState::ON );
            }
        }
    }

    auto * inserted_node = adj_insert_loc.parent->insertBefore( std::move( node ), adj_insert_loc.child );

    if( inserted_node != nullptr ) {
        this->pushOpenElements( dynamic_cast<node::Element *>( inserted_node ) );
    }

    return dynamic_cast<node::Element *>( inserted_node );
}

/**
 * Inserts characters
 * @param text Character(s) to insert
 */
void TreeBuilder::insertCharacters( const DOMString_t & text ) {
    this->insertCharacters( text.begin(), text.end() );
}

/**
 * Inserts characters
 * @tparam InputIt Text iterator type
 * @param begin Start iterator of the character range
 * @param end End iterator of the character range
 */
template<typename InputIt> void TreeBuilder::insertCharacters( InputIt begin, InputIt end ) {
    if( _ignore_next_token_in_body_LF &&
        begin != end &&
        *begin == unicode::LF)
    {
        std::advance( begin, 1 );
    }

    if( begin != end ) {
        auto adj_insert_loc = this->appropriateInsertionPoint();

        if( adj_insert_loc.parent->nodeType() == NodeType::DOCUMENT_NODE ) {
            return; //EARLY RETURN
        }

        if( adj_insert_loc.child &&
            adj_insert_loc.child->previousSibling() &&
            adj_insert_loc.child->previousSibling()->nodeType() == NodeType::TEXT_NODE )
        {
            auto * text_node = dynamic_cast<node::Text *>( adj_insert_loc.child->previousSibling() );
            std::copy( begin, end, back_inserter( text_node->data() ) );

        } else if( adj_insert_loc.parent->lastChild() &&
                   adj_insert_loc.parent->lastChild()->nodeType() == NodeType::TEXT_NODE )
        {
            auto * text_node = dynamic_cast<node::Text *>( adj_insert_loc.parent->lastChild() );
            std::copy( begin, end, back_inserter( text_node->data() ) );

        } else {
            adj_insert_loc.parent->insertBefore(
                std::make_unique<node::Text>( DOMString_t( begin, end ) ),
                adj_insert_loc.child
            );
        }
    }

    _ignore_next_token_in_body_LF = false; //reset
}

/**
 * Inserts an element using the 'generic raw text element' algorithm
 * @param token Token
 */
void TreeBuilder::insertGenericRawTextElement( token::html5::StartTagTk * token ) {
    this->insertForeignElement( token, Namespace_e::HTML );
    _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::RAWTEXT );
    this->setOriginalInsertMode( _current_insert_mode );
    this->setCurrentInsertMode( InsertionMode_e::TEXT );
}

/**
 * Inserts an element using the 'generic RCDATA element' algorithm
 * @param token Token
 */
void TreeBuilder::insertGenericRCDATAElement( token::html5::StartTagTk * token ) {
    this->insertForeignElement( token, Namespace_e::HTML );
    _changeHTML5TokeniserState_cb( specs::infra::TokeniserState::RCDATA );
    this->setOriginalInsertMode( _current_insert_mode );
    this->setCurrentInsertMode( InsertionMode_e::TEXT );
}

/**
 * Inserts a comment
 * @param token Pointer to Comment token
 * @param position Insertion position (default = {NULL, NULL})
 */
void TreeBuilder::insertComment( const std::unique_ptr<token::html5::HTML5Tk> & token, InsertionPoint position ) {
    if( position.parent == nullptr ) {
        position = this->appropriateInsertionPoint();
    }

    position.parent->insertBefore( std::make_unique<node::Comment>( token->text() ), position.child );
}

/**
 * Gets the appropriate place for inserting a node
 * @param override_target Target override (optional)
 * @return Insertion point
 * @throws blogator::exception::failed_expectation when encountering unexpected failure
 */
blogator::parser::dom::InsertionPoint TreeBuilder::appropriateInsertionPoint( node::Node * override_target ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#appropriate-place-for-inserting-a-node
    static const auto TABLE_ELEMENT_SET = std::set<Element_e>( {
        Element_e::HTML5_TABLE,
        Element_e::HTML5_TBODY,
        Element_e::HTML5_TFOOT,
        Element_e::HTML5_THEAD,
        Element_e::HTML5_TR
    } );

    node::Node *   target       = ( override_target == nullptr ? this->currentNode() : override_target );
    InsertionPoint insert_point = { target, nullptr };

    if( target == nullptr ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[blogator::parser::dom::TreeBuilder::appropriateInsertionPoint( node::Node * )] "
            "Insertion point parent is NULL."
        );
    }

    if( _foster_parenting &&
        target->nodeType() == NodeType::ELEMENT_NODE &&
        TABLE_ELEMENT_SET.contains( dynamic_cast<node::Element *>( target )->elementType() ) )
    {
        auto       last_template = _open_elements.end();
        auto       last_table    = _open_elements.end();
        const auto not_found     = _open_elements.end();

        for( auto it = _open_elements.begin(); it != _open_elements.end(); ++it ) {
            if( last_template == not_found && (*it)->elementType() == Element_e::HTML5_TEMPLATE ) {
                last_template = it;
            } else if( last_table == not_found && (*it)->elementType() == Element_e::HTML5_TABLE ) {
                last_table = it;
            }
        }

        if( last_template != not_found &&
            ( last_table == not_found || ( last_table != not_found && last_template < last_table ) ) )
        {
            insert_point = { (*last_template), nullptr };

        } else if( last_table == not_found  ) {
            //inside 'HTML' element after last child
            insert_point = { _open_elements.back(), nullptr };

        } else if( (*last_table)->parentNode() != nullptr ) {
            //inside last table's parent node immediately before last table
            insert_point = { (*last_table)->parentNode(), (*last_table) };

        } else {
            const auto prev_element = std::next( last_table );

            if( prev_element == not_found ) {
                throw FAILED_EXPECTATION_EXCEPTION(
                    "[parser::dom::TreeBuilder::appropriateInsertionPoint( node::Node * )] "
                    "No previous element in open element stack exist."
                );
            }

            //inside previous element, after its last child
            insert_point = { (*prev_element), nullptr };
        }
    }

    if( insert_point.parent->nodeType() == NodeType::ELEMENT_NODE && //checked in case it's already a DocumentFragment node
        dynamic_cast<node::Element *>( insert_point.parent )->elementType() == Element_e::HTML5_TEMPLATE )
    {
        auto * doc_fragment = insert_point.parent->lastChild();

        if( doc_fragment == nullptr ) { //i.e.: child-less
            auto new_document_fragment_node = std::make_unique<node::DocumentFragment>();

            doc_fragment = insert_point.parent->appendChild( std::move( new_document_fragment_node ) );

        } else if( doc_fragment->nodeType() != NodeType::DOCUMENT_FRAGMENT_NODE ) {
            throw FAILED_EXPECTATION_EXCEPTION(
                "[parser::dom::TreeBuilder::appropriateInsertionPoint( node::Node * )] "
                "Template child not a 'DocumentFragment' node."
            );
        }

        //inside last template's template contents (a 'DocumentFragment'), after last child
        insert_point = { doc_fragment, nullptr };
    }

    return insert_point;
}

/**
 * Resets the insertion mode appropriately
 */
void TreeBuilder::resetInsertionModeAppropriately() {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#reset-the-insertion-mode-appropriately
    bool last    = false;
    auto node_it = _open_elements.cbegin();

    while( true ) {
        if( std::next( node_it ) == _open_elements.cend() ) {
            last = true;
        }

        auto * node = *node_it;

        if( _fragment_case ) {
            node = _context_element;
        }

        const auto element = node->elementType();

        switch( element ) {
            case Element_e::HTML5_SELECT: {
                if( !last  ) {
                    auto ancestor_it = node_it;

                    while( ++ancestor_it != _open_elements.cend() &&
                           ( *ancestor_it )->elementType() != Element_e::HTML5_TEMPLATE )
                    {
                        if( ( *ancestor_it )->elementType() == Element_e::HTML5_TABLE ) {
                            this->setCurrentInsertMode( InsertionMode_e::IN_SELECT_IN_TABLE );
                            return; //EARLY RETURN
                        }
                    }
                }

                this->setCurrentInsertMode( InsertionMode_e::IN_SELECT );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_TD: [[fallthrough]];
            case Element_e::HTML5_TH: {
                if( !last ) {
                    this->setCurrentInsertMode( InsertionMode_e::IN_CELL );
                } else {
                    this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
                }

                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_TR: {
                this->setCurrentInsertMode( InsertionMode_e::IN_ROW );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_TBODY: [[fallthrough]];
            case Element_e::HTML5_THEAD: [[fallthrough]];
            case Element_e::HTML5_TFOOT: {
                this->setCurrentInsertMode( InsertionMode_e::IN_TABLE_BODY );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_CAPTION: {
                this->setCurrentInsertMode( InsertionMode_e::IN_CAPTION );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_COLGROUP: {
                this->setCurrentInsertMode( InsertionMode_e::IN_COLUMN_GROUP );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_TABLE: {
                this->setCurrentInsertMode( InsertionMode_e::IN_TABLE );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_TEMPLATE: {
                if( !_template_insert_modes.empty() ) {
                    this->setCurrentInsertMode( _template_insert_modes.top() );

                } else {
                    LOG_ERROR(
                        "[parser::dom::TreeBuilder::resetInsertionModeAppropriately()] "
                        "Stack of template insertion modes empty (case: '", blogator::to_string( element ), "')."
                    );
                }

                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_HEAD: {
                this->setCurrentInsertMode( InsertionMode_e::IN_HEAD );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_BODY: {
                this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
                return; //EARLY RETURN
            } break;

            case Element_e::DEPR_HTML_FRAMESET: {
                this->setCurrentInsertMode( InsertionMode_e::IN_FRAMESET );
                return; //EARLY RETURN
            } break;

            case Element_e::HTML5_HTML: {
                if( _head_element == nullptr ) {
                    this->setCurrentInsertMode( InsertionMode_e::BEFORE_HEAD );
                } else {
                    this->setCurrentInsertMode( InsertionMode_e::AFTER_HEAD );
                }

                return; //EARLY RETURN
            } break;

            default: {
                if( last ) {
                    this->setCurrentInsertMode( InsertionMode_e::IN_BODY );
                    return; //EARLY RETURN
                }
            } break;
        }

        ++node_it;
    }
}

/**
 * Generates implied end tags
 * @param skip_el Element type skip list
 */
void TreeBuilder::generateImpliedEndTags( const std::set<Element_e> & skip_el ) {
    static const auto CLOSABLE_TAGS = std::set<Element_e>( {
        Element_e::HTML5_DD,
        Element_e::HTML5_DT,
        Element_e::HTML5_LI,
        Element_e::HTML5_OPTGROUP,
        Element_e::HTML5_OPTION,
        Element_e::HTML5_P,
        Element_e::DEPR_HTML_RB,
        Element_e::HTML5_RP,
        Element_e::HTML5_RT,
        Element_e::DEPR_HTML_RTC,
    } );

    if( skip_el.empty() ) {
        while( !_open_elements.empty() && CLOSABLE_TAGS.contains( this->currentNodeType() ) ) {
            this->popOpenElements();
        }

    } else {
        while( !_open_elements.empty() && !skip_el.contains( this->currentNodeType() ) && CLOSABLE_TAGS.contains( this->currentNodeType() ) ) {
            this->popOpenElements();
        }
    }
}

/**
 * Generates implied end tags
 * @param skip_el Element tag name skip list
 */
void TreeBuilder::generateImpliedEndTags( const std::set<DOMString_t> &skip_el ) {
    static const auto CLOSABLE_TAGS = std::set<Element_e>( {
        Element_e::HTML5_DD,
        Element_e::HTML5_DT,
        Element_e::HTML5_LI,
        Element_e::HTML5_OPTGROUP,
        Element_e::HTML5_OPTION,
        Element_e::HTML5_P,
        Element_e::DEPR_HTML_RB,
        Element_e::HTML5_RP,
        Element_e::HTML5_RT,
        Element_e::DEPR_HTML_RTC,
    } );

    if( skip_el.empty() ) {
        this->generateImpliedEndTags();

    } else {
        while( !_open_elements.empty() && !skip_el.contains( this->currentNode()->qualifiedName() ) && CLOSABLE_TAGS.contains( this->currentNodeType() ) ) {
            this->popOpenElements();
        }
    }
}

/**
 * Generates all implied end tags thoroughly
 */
void TreeBuilder::generateAllImpliedEndTagsThoroughly() {
    static const auto CLOSABLE_TAGS = std::set<Element_e>( {
        Element_e::HTML5_CAPTION,
        Element_e::HTML5_COLGROUP,
        Element_e::HTML5_DD,
        Element_e::HTML5_DT,
        Element_e::HTML5_LI,
        Element_e::HTML5_OPTGROUP,
        Element_e::HTML5_OPTION,
        Element_e::HTML5_P,
        Element_e::DEPR_HTML_RB,
        Element_e::HTML5_RP,
        Element_e::HTML5_RT,
        Element_e::DEPR_HTML_RTC,
        Element_e::HTML5_TBODY,
        Element_e::HTML5_TD,
        Element_e::HTML5_TFOOT,
        Element_e::HTML5_TH,
        Element_e::HTML5_THEAD,
        Element_e::HTML5_TR,
    } );

    while( !_open_elements.empty() && CLOSABLE_TAGS.contains( this->currentNodeType() ) ) {
        this->popOpenElements();
    }
}

/**
 * Runs the "adoption agency algorithm" on a token element
 * @param tk_element Element type of token
 * @param pos Reference text position (for logging purpose)
 * @return Requirement to act as 'any other end tags'
 */
bool TreeBuilder::runAdoptionAgencyAlgorithm( const token::html5::GenericTagTk * token, Element_e resolved_element ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#adoption-agency-algorithm
    constexpr bool DEFAULT = false;

    // [1] Let subject be token's tag name.
    auto subject = resolved_element;

    // [2] If the current node is an HTML element whose tag name is subject,
    //     and the current node is not in the list of active formatting elements,
    //     then pop the current node off the stack of open elements and return.
    if( this->currentNodeType() == subject ) {
        auto fmt_it = findActiveFormattingElement( this->currentNode() );

        if( fmt_it == _formatting_elements.end() ) {
            this->popOpenElements();
            return DEFAULT; //EARLY RETURN
        }
    }

    // [3]   Let outer loop counter be 0.
    // [4]   While true;
    // [4.1] If outer loop counter is greater than or equal to 8, then return.
    // [4.2] Increment outer loop counter by 1.
    for( unsigned outer_loop_counter = 0; outer_loop_counter < 8; ++outer_loop_counter ) {
        // [4.3] Let formatting element be the last element in the list of active formatting elements that:
        //       · is between the end of the list and the last marker in the list, if any, or the start of the list otherwise, and
        //       · has the tag name subject.
        //       If there is no such element,
        //       then return and instead act as described in the "any other end tag" entry 'above'.
        const auto fmt_in_fmt_elements_it = this->findActiveFormattingElementAfterLastMarker( subject );

        if( fmt_in_fmt_elements_it == _formatting_elements.end() ) {
            return TREAT_AS_ANY_OTHER_END_TAG; //EARLY RETURN
        }

        const auto fmt_in_open_elements_it = this->findOpenElement( *fmt_in_fmt_elements_it );

        // [4.4] If formatting element is not in the stack of open elements,
        //       then this is a parse error; remove the element from the list, and return.
        if( fmt_in_open_elements_it == _open_elements.end() ) {
            std::stringstream ss;
            ss << "<" << (*fmt_in_fmt_elements_it)->elementType() << ">";
            this->logError( token->position(), specs::infra::ErrorCode::ADOPTION_AGENCY_ALGO_4_4, ss.str() );
            _formatting_elements.erase( fmt_in_fmt_elements_it );
            return DEFAULT; //EARLY RETURN
        }

        // [4.5] If formatting element is in the stack of open elements, but the element is not in scope,
        //       then this is a parse error; return.
        if( !this->hasParticularElementInScope( subject ) ) {
            std::stringstream ss;
            ss << "<" << blogator::unicode::utf8::convert( token->name() ) << ">";
            this->logError( token->position(), specs::infra::ErrorCode::ADOPTION_AGENCY_ALGO_4_5, ss.str() );
            return DEFAULT; //EARLY RETURN
        }

        // [4.6] If formatting element is not the current node, this is a parse error. (But do not return.)
        if( *fmt_in_fmt_elements_it != this->currentNode() ) {
            std::stringstream ss;
            ss << "formatting element <" << (*fmt_in_fmt_elements_it)->elementType() << "> not current node <" << this->currentNodeType() << ">";
            this->logError( token->position(), specs::infra::ErrorCode::ADOPTION_AGENCY_ALGO_4_6, ss.str() );
        }

        // [4.7] Let furthest block be the topmost node in the stack of open elements
        //       that is lower in the stack than formatting element,
        //       and is an element in the special category. There might not be one.
        auto   furthest_block_it = this->findFurthestBlockInOpenElements( fmt_in_open_elements_it );
        auto * furthest_block    = (*furthest_block_it);

        // [4.8] If there is no furthest block, then the UA must first pop all the nodes from the bottom
        //       of the stack of open elements, from the current node up to and including formatting element,
        //       then remove formatting element from the list of active formatting elements, and finally return.
        if( furthest_block_it == _open_elements.end() ) {
            _open_elements.erase( _open_elements.begin(), std::next( fmt_in_open_elements_it ) );
            _formatting_elements.erase( fmt_in_fmt_elements_it );
            return DEFAULT; //EARLY RETURN
        }

        // [4.9]  Let common ancestor be the element immediately above formatting element in the stack of open elements.
        // [4.10] Let a bookmark note the position of formatting element in the list of active formatting elements
        //        relative to the elements on either side of it in the list.
        // [4.11] Let node and last node be the furthest block.
        // [4.12] Let inner loop counter be 0.
        auto *   formatting_element = *fmt_in_fmt_elements_it;
        auto *   common_ancestor    = *std::next( fmt_in_open_elements_it );
        auto     bookmark           = std::distance( _formatting_elements.begin(), fmt_in_fmt_elements_it );
        auto     node_it            = furthest_block_it;
        auto *   last_node          = *node_it;
        unsigned inner_loop_counter = 0;
        bool     advance_flag       = true;

        // [4.13] While true
        while( true ) {
            // [4.13.1] Increment inner loop counter by 1.
            ++inner_loop_counter;

            // [4.13.2] Let node be the element immediately above node in the stack of open elements,
            //          or if node is no longer in the stack of open elements (e.g. because it got removed by this algorithm),
            //          the element that was immediately above node in the stack of open elements before node was removed.
            if( advance_flag ) {
                ++node_it;
            } else {
                advance_flag = true;
            }

            //[4.13.3] If node is formatting element, then break.
            if( (*node_it) == formatting_element ) {
                break; //skip down to [4.14]
            }

            auto node_in_fmt_elements_it = this->findActiveFormattingElement( (*node_it) );

            // [4.13.4] If inner loop counter is greater than 3 and node is in the list of active formatting elements,
            //          then remove node from the list of active formatting elements.
            if( inner_loop_counter > 3 && node_in_fmt_elements_it != _formatting_elements.end() ) {
                this->removeActiveFormattingElement( node_in_fmt_elements_it );
                node_in_fmt_elements_it = _formatting_elements.end(); //as it has been removed, forces [4.13.5]
            }

            // [4.13.5] If node is not in the list of active formatting elements,
            //          then remove node from the stack of open elements and continue.
            if( node_in_fmt_elements_it == _formatting_elements.end() ) {
                node_it      = _open_elements.erase( node_it );
                advance_flag = false; //^^^ since already on the next element
                continue; //skip back up to [4.13.1]
            }

            // [4.13.6] Create an element for the token for which the element node was created,
            //          in the HTML namespace, with common ancestor as the intended parent;
            //          replace the entry for node in the list of active formatting elements with
            //          an entry for the new element, replace the entry for node in the stack of
            //          open elements with an entry for the new element, and let node be the new element.
            auto * new_element = dynamic_cast<node::Element *>( common_ancestor->appendChild( (*node_it)->cloneNode( false ) ) );

            if( new_element == nullptr ) { //just in case...
                throw FAILED_EXPECTATION_EXCEPTION(
                    "Failed to create a new element <" + blogator::to_string( resolved_element ) + "> as child of "
                    "'common ancestor' \"" + blogator::to_string( common_ancestor->elementType() ) + "\" in adoption agency algorithm."
                );
            }

            (*node_in_fmt_elements_it) = new_element;
            (*node_it)                 = new_element;

            // [4.13.7] If last node is the furthest block, then move the aforementioned bookmark to be
            //          immediately after the new node in the list of active formatting elements.
            if( last_node == furthest_block ) {
                bookmark = std::distance( _formatting_elements.begin(), std::next( node_in_fmt_elements_it ) );
            }

            // [4.13.8] Append last node to node.
            (*node_it)->appendChild( last_node->detach() );

            // [4.13.9] Set last node to node.
            last_node = *node_it;
        }

        // [4.14] Insert whatever last node ended up being in the previous step at the appropriate place
        //        for inserting a node, but using common ancestor as the override target.
        auto insert_loc = this->appropriateInsertionPoint( common_ancestor );

        insert_loc.parent->insertBefore( last_node->detach(), insert_loc.child ); //last_node_it should still point to the 'last_node'

        // [4.15] Create an element for the token for which formatting element was created,
        //        in the HTML namespace, with the furthest block as the intended parent.
        auto   new_element     = formatting_element->cloneNode( false );

        // [4.16] Take all of the child nodes of the furthest block and append them
        //        to the element created in the last step.
        while( furthest_block->hasChildNodes() ) {
            new_element->appendChild( furthest_block->firstChild()->detach() );
        }

        // [4.17] Append that new element to the furthest block.
        auto * new_element_ptr = dynamic_cast<node::Element *>( furthest_block->appendChild( std::move( new_element ) ) );

        // [4.18] Remove formatting element from the list of active formatting elements,
        //        and insert the new element into the list of active formatting elements
        //        at the position of the aforementioned bookmark.
        this->removeActiveFormattingElement( this->findActiveFormattingElement( formatting_element ) );
        this->insertActiveFormattingElement( bookmark, new_element_ptr );

        // [4.19] Remove formatting element from the stack of open elements,
        //        and insert the new element into the stack of open elements immediately
        //        below the position of the furthest block in that stack.
        this->removeOpenElement( this->findOpenElement( formatting_element ) );
        _open_elements.insert( this->findOpenElement( furthest_block ), new_element_ptr );
    }

    return DEFAULT;
}

/**
 * Closes an element in the stack of open elements
 * @param el Element type to close
 * @param pos Reference text position (for logging purposes)
 */
void TreeBuilder::closeElement( TreeBuilder::Element_e el, const TextPos & pos ) {
    this->generateImpliedEndTags( { el } );

    if( this->currentNodeType() != el ) {
        std::stringstream ss;
        ss << "expected <" << el << ">, got <" << this->currentNodeType() << ">";
        this->logError( pos, specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
    }

    this->popOpenElementsUntil( el, true );
}

/**
 * Closes an element in the stack of open elements
 * @param el Element (qualified name) to close
 * @param pos Reference text position (for logging purposes)
 */
void TreeBuilder::closeElement( const DOMString_t & el, const TextPos & pos ) {
    this->generateImpliedEndTags( { el } );

    if( this->currentNode()->qualifiedName() != el ) {
        std::stringstream ss;
        ss << "expected <" << blogator::unicode::utf8::convert( el ) << ">, "
           << "got <" << blogator::unicode::utf8::convert( this->currentNode()->qualifiedName() ) << ">";

        this->logError( pos, specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
    }

    this->popOpenElementsUntil( el, true );
}

/**
 * Clears the stack of open elements back to a table context
 */
void TreeBuilder::clearStackBackToTableContext() {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#clear-the-stack-back-to-a-table-context
    static const auto elements = std::set<Element_e>( {
        Element_e::HTML5_TABLE,
        Element_e::HTML5_TEMPLATE,
        Element_e::HTML5_HTML,
    } );

    this->popOpenElementsUntil( elements, false );
}

/**
 * Clears the stack of open elements back to a table body context
 */
void TreeBuilder::clearStackBackToTableBodyContext() {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#clear-the-stack-back-to-a-table-body-context
    static const auto elements = std::set<Element_e>( {
        Element_e::HTML5_TBODY,
        Element_e::HTML5_TFOOT,
        Element_e::HTML5_THEAD,
        Element_e::HTML5_TEMPLATE,
        Element_e::HTML5_HTML,
    } );

    this->popOpenElementsUntil( elements, false );
}

/**
 * Clears the stack of open elements back to a table row context
 */
void TreeBuilder::clearStackBackToTableRowContext() {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#clear-the-stack-back-to-a-table-row-context
    static const auto elements = std::set<Element_e>( {
        Element_e::HTML5_TR,
        Element_e::HTML5_TEMPLATE,
        Element_e::HTML5_HTML,
    } );

    this->popOpenElementsUntil( elements, false );
}

/**
 * Closes the cell
 * @param pos Reference text position (for logging purpose)
 */
void TreeBuilder::closeCell( const TextPos & pos ) {
    this->generateImpliedEndTags();

    const auto curr_type = currentNodeType();

    if( !( curr_type == Element_e::HTML5_TH || curr_type == Element_e::HTML5_TD ) ) {
        std::stringstream ss;
        ss << "expected <" << Element_e::HTML5_TH << "> or <" << Element_e::HTML5_TD << ">, "
           << "got <" << curr_type << ">";

        this->logError( pos, specs::infra::ErrorCode::UNCLOSED_TAG, ss.str() );
    }

    this->popOpenElementsUntil( { Element_e::HTML5_TH, Element_e::HTML5_TD }, true );
    this->clearActiveFormattingElementsUpToLastMarker();
    this->setCurrentInsertMode( InsertionMode_e::IN_ROW );
}

/**
 * Checks target node is in a specific scope
 * @param target Target element
 * @param element_types List of element types to check
 * @param exclusive Flag to switch the element type list as an exception list instead of a inclusive list (default=true)
 * @return Scoped state
 */
bool TreeBuilder::hasTargetInSpecificScope( Element_e target, const std::set<Element_e> & element_types, bool exclusive ) const {
    static const auto VALID_NAMESPACES = std::set<Namespace_e>( {
        Namespace_e::HTML,
        Namespace_e::MATHML,
        Namespace_e::SVG
    } );

    auto node_it = this->currentNodeIterator();

    if( exclusive ) {
        while( node_it != _open_elements.cend() ) {
            const auto node_type = ( *node_it )->elementType();

            if( node_type == target ) {
                return true; //EARLY RETURN - match state
            }

            if( element_types.contains( node_type ) ) {
                return false; //EARLY RETURN - failure state
            }

            ++node_it;
        }

    } else { //inclusive list
        while( node_it != _open_elements.cend() ) {
            const auto node_type = ( *node_it )->elementType();

            if( node_type == target ) {
                return true; //EARLY RETURN - match state
            }

            const auto node_ns = specs::infra::resolveNamespace( node_type );

            if( VALID_NAMESPACES.contains( node_ns ) && !element_types.contains( ( *node_it )->elementType() ) ) {
                return false; //EARLY RETURN - failure state
            }

            ++node_it;
        }
    }

    return false;
}

/**
 * Checks stack of opened elements has a particular element in scope
 * @param target Target element
 * @return Match state
 */
bool TreeBuilder::hasParticularElementInScope( Element_e target ) const {
    static const auto list = std::set<Element_e>( {
        Element_e::DEPR_HTML_APPLET,
        Element_e::HTML5_CAPTION,
        Element_e::HTML5_HTML,
        Element_e::HTML5_TABLE,
        Element_e::HTML5_TD,
        Element_e::HTML5_TH,
        Element_e::DEPR_HTML_MARQUEE,
        Element_e::HTML5_OBJECT,
        Element_e::HTML5_TEMPLATE,
        Element_e::MATHML_MI,
        Element_e::MATHML_MO,
        Element_e::MATHML_MN,
        Element_e::MATHML_MS,
        Element_e::MATHML_MTEXT,
        Element_e::MATHML_ANNOTATION_XML,
        Element_e::SVG_FOREIGNOBJECT,
        Element_e::SVG_DESC,
        Element_e::SVG_TITLE,
    } );

    return this->hasTargetInSpecificScope( target, list );
}

/**
 * Checks stack of opened elements has a particular element in list item scope
 * @param target Target element
 * @return Match state
 */
bool TreeBuilder::hasParticularElementInListItemScope( Element_e target ) const {
    static const auto list = std::set<Element_e>( {
        Element_e::DEPR_HTML_APPLET,
        Element_e::HTML5_CAPTION,
        Element_e::HTML5_HTML,
        Element_e::HTML5_TABLE,
        Element_e::HTML5_TD,
        Element_e::HTML5_TH,
        Element_e::DEPR_HTML_MARQUEE,
        Element_e::HTML5_OBJECT,
        Element_e::HTML5_TEMPLATE,
        Element_e::MATHML_MI,
        Element_e::MATHML_MO,
        Element_e::MATHML_MN,
        Element_e::MATHML_MS,
        Element_e::MATHML_MTEXT,
        Element_e::MATHML_ANNOTATION_XML,
        Element_e::SVG_FOREIGNOBJECT,
        Element_e::SVG_DESC,
        Element_e::SVG_TITLE,
        Element_e::HTML5_UL,
        Element_e::HTML5_OL,
    } );

    return this->hasTargetInSpecificScope( target, list );
}

/**
 * Checks stack of opened elements has a particular element in button scope
 * @param target Target element
 * @return Match state
 */
bool TreeBuilder::hasParticularElementInButtonScope( Element_e target ) const {
    static const auto list = std::set<Element_e>( {
        Element_e::DEPR_HTML_APPLET,
        Element_e::HTML5_CAPTION,
        Element_e::HTML5_HTML,
        Element_e::HTML5_TABLE,
        Element_e::HTML5_TD,
        Element_e::HTML5_TH,
        Element_e::DEPR_HTML_MARQUEE,
        Element_e::HTML5_OBJECT,
        Element_e::HTML5_TEMPLATE,
        Element_e::MATHML_MI,
        Element_e::MATHML_MO,
        Element_e::MATHML_MN,
        Element_e::MATHML_MS,
        Element_e::MATHML_MTEXT,
        Element_e::MATHML_ANNOTATION_XML,
        Element_e::SVG_FOREIGNOBJECT,
        Element_e::SVG_DESC,
        Element_e::SVG_TITLE,
        Element_e::HTML5_BUTTON,
    } );

    return this->hasTargetInSpecificScope( target, list );
}

/**
 * Checks stack of opened elements has a particular element in table scope
 * @param target Target element
 * @return Match state
 */
bool TreeBuilder::hasParticularElementInTableScope( Element_e target ) const {
    static const auto list = std::set<Element_e>( {
        Element_e::HTML5_HTML,
        Element_e::HTML5_TABLE,
        Element_e::HTML5_TEMPLATE,
    } );

    return this->hasTargetInSpecificScope( target, list );
}

/**
 * Checks stack of opened elements has a particular element in select scope
 * @param target Target element
 * @return Match state
 */
bool TreeBuilder::hasParticularElementInSelectScope( Element_e target ) const {
    static const auto list = std::set<Element_e>( {
        Element_e::HTML5_OPTGROUP,
        Element_e::HTML5_OPTION,
    } );

    return this->hasTargetInSpecificScope( target, list, false );
}

/**
 * Stops the parsing
 */
void TreeBuilder::stopParsing() {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#stop-parsing
    //Steps 1-3 skipped as not interactive in blogator
    this->_open_elements.clear();
    //Steps 5-11 skipped as scripts are not executed in blogator
}

/**
 * Resolves an Element
 * @param curr_element Current Element type (only resolves if 'UNKNOWN')
 * @param ns_hint Namespace hint (defaults to current open element's namespace on ambiguous tags or HTML as a fallback)
 * @param tag_name Lowercase element tag name to resolve
 * @return Resolved element
 */
TreeBuilder::Element_e TreeBuilder::resolveElement( Element_e curr_element, Namespace_e ns_hint, DOMString_t & tag_name ) {
    if( curr_element == Element_e::UNKNOWN ) {

        if( specs::infra::isAmbiguousElement( tag_name ) ) {
            const auto * curr_node = this->currentNode();

            ns_hint = ( curr_node == nullptr
                        ? Namespace_e::HTML //fallback
                        : curr_node->getNamespaceEnum() );
        }

        return specs::infra::resolveElementType( ns_hint, tag_name );
    } //else: already resolved

    return curr_element;
}

/**
 * Checks if element requires 'special' parsing rules
 * @param element Element to check
 * @param exceptions List of elements to omit from the search
 * @return 'special' state
 */
bool TreeBuilder::isSpecialElement( Element_e element, const std::set<Element_e> & exceptions ) {
    static const auto SPECIALS = std::set<Element_e>( {
        Element_e::HTML5_ADDRESS,
        Element_e::DEPR_HTML_APPLET,
        Element_e::HTML5_AREA,
        Element_e::HTML5_ARTICLE,
        Element_e::HTML5_ASIDE,
        Element_e::HTML5_BASE,
        Element_e::DEPR_HTML_BASEFONT,
        Element_e::DEPR_HTML_BGSOUND,
        Element_e::HTML5_BLOCKQUOTE,
        Element_e::HTML5_BODY,
        Element_e::HTML5_BR,
        Element_e::HTML5_BUTTON,
        Element_e::HTML5_CAPTION,
        Element_e::DEPR_HTML_CENTER,
        Element_e::HTML5_COL,
        Element_e::HTML5_COLGROUP,
        Element_e::HTML5_DD,
        Element_e::HTML5_DETAILS,
        Element_e::DEPR_HTML_DIR,
        Element_e::HTML5_DIV,
        Element_e::HTML5_DL,
        Element_e::HTML5_DT,
        Element_e::HTML5_EMBED,
        Element_e::HTML5_FIELDSET,
        Element_e::HTML5_FIGCAPTION,
        Element_e::HTML5_FIGURE,
        Element_e::HTML5_FOOTER,
        Element_e::HTML5_FORM,
        Element_e::DEPR_HTML_FRAME,
        Element_e::DEPR_HTML_FRAMESET,
        Element_e::HTML5_H1,
        Element_e::HTML5_H2,
        Element_e::HTML5_H3,
        Element_e::HTML5_H4,
        Element_e::HTML5_H5,
        Element_e::HTML5_H6,
        Element_e::HTML5_HEAD,
        Element_e::HTML5_HEADER,
        Element_e::HTML5_HGROUP,
        Element_e::HTML5_HR,
        Element_e::HTML5_HTML,
        Element_e::HTML5_IFRAME,
        Element_e::HTML5_IMG,
        Element_e::HTML5_INPUT,
        Element_e::DEPR_HTML_KEYGEN,
        Element_e::HTML5_LI,
        Element_e::HTML5_LINK,
        Element_e::DEPR_HTML_LISTING,
        Element_e::HTML5_MAIN,
        Element_e::DEPR_HTML_MARQUEE,
        Element_e::HTML5_MENU,
        Element_e::HTML5_META,
        Element_e::HTML5_NAV,
        Element_e::DEPR_HTML_NOEMEBED,
        Element_e::DEPR_HTML_NOFRAMES,
        Element_e::HTML5_NOSCRIPT,
        Element_e::HTML5_OBJECT,
        Element_e::HTML5_OL,
        Element_e::HTML5_P,
        Element_e::HTML5_PARAM,
        Element_e::DEPR_HTML_PLAINTEXT,
        Element_e::HTML5_PRE,
        Element_e::HTML5_SCRIPT,
        Element_e::HTML5_SECTION,
        Element_e::HTML5_SELECT,
        Element_e::HTML5_SOURCE,
        Element_e::HTML5_STYLE,
        Element_e::HTML5_SUMMARY,
        Element_e::HTML5_TABLE,
        Element_e::HTML5_TBODY,
        Element_e::HTML5_TD,
        Element_e::HTML5_TEMPLATE,
        Element_e::HTML5_TEXTAREA,
        Element_e::HTML5_TFOOT,
        Element_e::HTML5_TH,
        Element_e::HTML5_THEAD,
        Element_e::HTML5_TITLE,
        Element_e::HTML5_TR,
        Element_e::HTML5_TRACK,
        Element_e::HTML5_UL,
        Element_e::HTML5_WBR,
        Element_e::DEPR_HTML_XMP,
        Element_e::MATHML_MI,
        Element_e::MATHML_MO,
        Element_e::MATHML_MN,
        Element_e::MATHML_MS,
        Element_e::MATHML_MTEXT,
        Element_e::MATHML_ANNOTATION_XML,
        Element_e::SVG_FOREIGNOBJECT,
        Element_e::SVG_DESC,
        Element_e::SVG_TITLE,
    } );

    return ( SPECIALS.contains( element ) && !exceptions.contains( element ) );
}

/**
 * Checks if element is a 'formatting' element
 * @param element Element
 * @return 'formatting' state
 */
bool TreeBuilder::isFormattingElement( TreeBuilder::Element_e element ) {
    static const auto FMT_ELEMENTS = std::set<Element_e>( {
        Element_e::HTML5_A,
        Element_e::HTML5_B,
        Element_e::DEPR_HTML_BIG,
        Element_e::HTML5_CODE,
        Element_e::HTML5_EM,
        Element_e::DEPR_HTML_FONT,
        Element_e::HTML5_I,
        Element_e::DEPR_HTML_NOBR,
        Element_e::HTML5_S,
        Element_e::HTML5_SMALL,
        Element_e::DEPR_HTML_STRIKE,
        Element_e::HTML5_STRONG,
        Element_e::DEPR_HTML_TT,
        Element_e::HTML5_U,
    } );

    return FMT_ELEMENTS.contains( element );
}

/**
 * Checks if a node is a MathML integration point
 * @param node Pointer to MathML Element node
 * @return Is MathML integration point
 */
bool TreeBuilder::isMathMLIntegrationPoint( dom::node::Element * node ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#mathml-text-integration-point
    static const auto MATHML_ELEMENT_SET = std::set<Element_e>( {
        Element_e::MATHML_MI,
        Element_e::MATHML_MO,
        Element_e::MATHML_MN,
        Element_e::MATHML_MS,
        Element_e::MATHML_MTEXT
    } );

    return MATHML_ELEMENT_SET.contains( node->elementType() );
}

/**
 * Checks if a node is an HTML integration point
 * @param node Pointer to Element node
 * @return Is HTML integration point
 */
bool TreeBuilder::isHTMLIntegrationPoint( dom::node::Element * node ) {
    //ref: https://html.spec.whatwg.org/multipage/parsing.html#html-integration-point
    static const auto APPLICATION_XHTML_XML = blogator::to_u32string(
        specs::infra::ContentType::APPLICATION_XHTML_XML
    );

    static const auto SVG_ELEMENT_SET = std::set<Element_e>( {
        Element_e::SVG_FOREIGNOBJECT,
        Element_e::SVG_DESC,
        Element_e::SVG_TITLE
    } );

    const auto element = node->elementType();

    if( element == Element_e::MATHML_ANNOTATION_XML ) {
        const auto & attributes = node->attributes();
        const auto * attr       = attributes.getNamedItem( U"encoding" );
        const auto * value      = ( attr == nullptr ? nullptr : attr->value() );

        if( value != nullptr ) {
            const auto value_lowercase = blogator::unicode::ascii::tolower( *value );

            return ( value_lowercase == U"text/html" ||
                     value_lowercase == APPLICATION_XHTML_XML ); //EARLY RETURN

        }

        return false; //EARLY RETURN
    }

    return SVG_ELEMENT_SET.contains( element );
}

/**
 * Validate a StartTag token in the context of foreign content
 * @param tk StartTagTk token
 * @return Valid state
 */
bool TreeBuilder::validateStartTagInForeignContent( token::html5::StartTagTk & token ) {
    static const auto INVALID_ELEMENTS = std::set<Element_e>( {
        Element_e::HTML5_B,
        Element_e::DEPR_HTML_BIG,
        Element_e::HTML5_BLOCKQUOTE,
        Element_e::HTML5_BODY,
        Element_e::HTML5_BR,
        Element_e::DEPR_HTML_CENTER,
        Element_e::HTML5_CODE,
        Element_e::HTML5_DD,
        Element_e::HTML5_DIV,
        Element_e::HTML5_DL,
        Element_e::HTML5_DT,
        Element_e::HTML5_EM,
        Element_e::HTML5_EMBED,
        Element_e::HTML5_H1,
        Element_e::HTML5_H2,
        Element_e::HTML5_H3,
        Element_e::HTML5_H4,
        Element_e::HTML5_H5,
        Element_e::HTML5_H6,
        Element_e::HTML5_HEAD,
        Element_e::HTML5_HR,
        Element_e::HTML5_I,
        Element_e::HTML5_IMG,
        Element_e::HTML5_LI,
        Element_e::DEPR_HTML_LISTING,
        Element_e::HTML5_MENU,
        Element_e::HTML5_META,
        Element_e::DEPR_HTML_NOBR,
        Element_e::HTML5_OL,
        Element_e::HTML5_P,
        Element_e::HTML5_PRE,
        Element_e::HTML5_RUBY,
        Element_e::HTML5_S,
        Element_e::HTML5_SMALL,
        Element_e::HTML5_SPAN,
        Element_e::HTML5_STRONG,
        Element_e::DEPR_HTML_STRIKE,
        Element_e::HTML5_SUB,
        Element_e::HTML5_SUP,
        Element_e::HTML5_TABLE,
        Element_e::DEPR_HTML_TT,
        Element_e::HTML5_U,
        Element_e::HTML5_UL,
        Element_e::HTML5_VAR,
    } );

    const auto tk_element = specs::infra::resolveElementType( Namespace_e::HTML, token.name() );

    if( INVALID_ELEMENTS.contains( tk_element ) ) {
        return false; //EARLY RETURN
    }

    if( tk_element == Element_e::DEPR_HTML_FONT ) {
        auto it = std::find_if( token.attributes().cbegin(),
                                token.attributes().cend(),
                                []( const auto & attr ) { return attr.name == U"color"
                                                              || attr.name == U"face"
                                                              || attr.name == U"size"; } );

        return ( it == token.attributes().cend() ); //EARLY RETURN
    }

    return true;
}

/**
 * Validate an EndTag token in the context of foreign content
 * @param token EndTagTk token
 * @return Valid state
 */
bool TreeBuilder::validateEndTagInForeignContent( token::html5::EndTagTk & token ) {
    static const auto INVALID_ELEMENTS = std::set<Element_e>( {
        Element_e::HTML5_BR,
        Element_e::HTML5_P,
    } );

    return !INVALID_ELEMENTS.contains(
        specs::infra::resolveElementType( Namespace_e::UNKNOWN, token.name() ) );
}

/**
 * Checks if attribute is a valid 'xlink'
 * @param attr_name Attribute name
 * @return State state
 */
bool TreeBuilder::isXLink( const blogator::parser::dom::DOMString_t &attr_name ) {
    static const auto XLINK_QNAME = std::set<dom::DOMString_t>( {
        U"xlink:actuate",
        U"xlink:arcrole",
        U"xlink:href",
        U"xlink:role",
        U"xlink:show",
        U"xlink:title",
        U"xlink:type",
    } );

    return XLINK_QNAME.contains( attr_name );
}

/**
 * Checks if attribute is 'xml:lang' or 'xml:space'
 * @param attr_name Attribute name
 * @return State
 */
bool TreeBuilder::isXML( const blogator::parser::dom::DOMString_t &attr_name ) {
    static const auto XML_QNAME = std::set<dom::DOMString_t>( {
        U"xml:lang",
        U"xml:space",
    } );

    return XML_QNAME.contains( attr_name );
}

/**
 * Checks if attribute is 'xmlns' or 'xmlns:xlink'
 * @param attr_name Attribute name
 * @return State
 */
bool TreeBuilder::isXMLNS( const blogator::parser::dom::DOMString_t &attr_name ) {
    static const auto XMLNS_QNAME = std::set<dom::DOMString_t>( {
        U"xmlns",
        U"xmlns:xlink",
    } );

    return XMLNS_QNAME.contains( attr_name );
}

/**
 * (For debugging) Print token to LOG_DEBUG
 * @param token Token
 */
void TreeBuilder::printDBG( const std::unique_ptr<token::html5::HTML5Tk> & token ) {
    using blogator::unicode::utf8::convert;

    std::stringstream ss;

    ss << "[" << token->type() << "]";

    switch( token->type() ) {
        case specs::infra::TokenType::UNKNOWN:
            break;
        case specs::infra::TokenType::DOCTYPE: {
            const auto * tk = dynamic_cast<const token::html5::DoctypeTk *>( token.get() );

            ss << " = { <" << convert( tk->name() );

            if( tk->hasPID() ) {
                ss << " \"" << convert( tk->publicIdentifier() ) << "\"";
            }

            if( tk->hasSID() ) {
                ss << " \"" << convert( tk->systemIdentifier() ) << "\"";
            }

            ss << "> }";
        } break;

        case specs::infra::TokenType::START_TAG: {
            const auto * tk = dynamic_cast<const token::html5::StartTagTk *>( token.get() );

            ss << " = { <" << convert( tk->name() );

            if( tk->hasAttributes() ) {
                ss << " ";
                for( auto it = tk->attributes().cbegin(); it != tk->attributes().cend(); ++it ) {
                    ss << convert( it->name ) << "=\"" << convert( it->value ) << "\"";

                    if( std::next( it ) != tk->attributes().cend() ) {
                        ss << " ";
                    }
                }
            }

            ss << "> }";
        } break;

        case specs::infra::TokenType::END_TAG: {
            const auto * tk = dynamic_cast<const token::html5::EndTagTk *>( token.get() );

            ss << " = { <" << convert( tk->name() );

            if( tk->hasAttributes() ) {
                ss << " ";
                for( auto it = tk->attributes().cbegin(); it != tk->attributes().cend(); ++it ) {
                    ss << convert( it->name ) << "=\"" << convert( it->value ) << "\"";

                    if( std::next( it ) != tk->attributes().cend() ) {
                        ss << " ";
                    }
                }
            }

            ss << "> }";
        } break;

        case specs::infra::TokenType::CHARACTER: [[fallthrough]];
        case specs::infra::TokenType::COMMENT: {
            ss << " = { \"" << convert( token->text() ) << "\" }";
        } break;

        default: break;
    }

    LOG_DEBUG( ss.str() );
}

/**
 * Pretty-prints a collection of node::Elements to a string
 * @param elements Collection of Elements
 * @return Printable string
 */
std::string blogator::to_string( std::deque<blogator::parser::dom::node::Element *> & elements ) {
    using           blogator::unicode::utf8::convert;
    using namespace blogator::parser::dom::node;

    std::stringstream ss;

    ss << "[";

    if( !elements.empty() ) {
        ss << " ";

        for( auto it = elements.cbegin(); it != elements.cend(); ++it ) {
            ss << "<" << ( *it == nullptr ? "NULL" : convert( (*it)->qualifiedName() ) ) << "> (" << *it << ")";

            if( std::next( it ) != elements.cend() ) {
                ss << ", ";
            }
        }

        ss << " ";
    }

    ss << "]";

    return ss.str();
}
