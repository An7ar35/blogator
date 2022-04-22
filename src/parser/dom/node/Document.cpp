#include "Document.h"

#include <utility>
#include <memory>

#include "../../../logger/Logger.h"
#include "../../../string/helpers.h"
#include "Attr.h"
#include "CDATASection.h"
#include "Comment.h"
#include "DocumentType.h"
#include "DocumentFragment.h"
#include "Element.h"
#include "Text.h"
#include "../validation/XML.h"
#include "../../../unicode/unicode.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor (default XHTML/no quirks)
 */
Document::Document() :
    node::Node( NodeType::DOCUMENT_NODE ),
    _content_type( specs::infra::ContentType::APPLICATION_XHTML_XML ),
    _type( Document::Type::HTML ),
    _quirks( QuirksMode::NO_QUIRKS ),
    _doctype( nullptr ),
    _element( nullptr )
{}

/**
 * Constructor (Type is auto-set)
 * @param encoding Encoding
 * @param content_type ContentType enum
 * @param url Document URL
 * @param origin Document origin path
 * @param quirks (optional) Quirks (default: NO_QUIRKS)
 */
Document::Document( USVString_t encoding, specs::infra::ContentType content_type, USVString_t url, std::filesystem::path origin, QuirksMode quirks ) :
    node::Node( NodeType::DOCUMENT_NODE ),
    _encoding( std::move( encoding ) ),
    _content_type( content_type ),
    _url( std::move( url ) ),
    _origin( std::move( origin ) ),
    _type( content_type == specs::infra::ContentType::APPLICATION_XHTML_XML ? Type::HTML : Type::XML ),
    _quirks( quirks ),
    _doctype( nullptr ),
    _element( nullptr )
{}

/**
 * Constructor
 * @param encoding Encoding
 * @param content_type ContentType enum
 * @param url Document URL
 * @param origin Document origin path
 * @param type Document type
 * @param quirks (optional) Quirks (default: NO_QUIRKS)
 */
Document::Document( USVString_t encoding, specs::infra::ContentType content_type, USVString_t url, std::filesystem::path origin, Type type, QuirksMode quirks ) :
    node::Node( NodeType::DOCUMENT_NODE ),
    _encoding( std::move( encoding ) ),
    _content_type( content_type ),
    _url( std::move( url ) ),
    _origin( std::move( origin ) ),
    _type( type ),
    _quirks( quirks ),
    _doctype( nullptr ),
    _element( nullptr )
{}


/**
 * Copy-constructor
 * @param document Document node to copy
 */
Document::Document( const Document &document ) :
    node::Node( document ),
    _encoding( document._encoding ),
    _content_type( document._content_type ),
    _url( document._url ),
    _origin( document._origin ),
    _type( document._type ),
    _quirks( document._quirks ),
    _doctype( nullptr ),
    _element( nullptr )
{
    if( document._doctype ) {
        auto it = std::find_if( this->_children.begin(),
                                this->_children.end(),
                                []( const auto & ptr ) { return ptr->nodeType() == NodeType::DOCUMENT_TYPE_NODE; } );

        if( it != this->_children.end() ) {
            _doctype = dynamic_cast<DocumentType *>( it->get() );
        } else {
            LOG_CRITICAL(
                "[parser::dom::node::Document::Document( const Document & )] "
                "Failed to find the child of type 'DocumentType' in children list."
            );
        }
    }

    if( document._element ) {
        auto it = std::find_if( this->_children.begin(),
                                this->_children.end(),
                                []( const auto & ptr ) { return ptr->nodeType() == NodeType::ELEMENT_NODE; } );

        if( it != this->_children.end() ) {
            _element = dynamic_cast<Element *>( it->get() );
        } else {
            LOG_CRITICAL(
                "[parser::dom::node::Document::Document( const Document & )] "
                "Failed to find the child of type 'Element' in children list."
            );
        }
    }
}

/**
 * Move-constructor
 * @param document Document node to move
 */
Document::Document( Document &&document ) noexcept :
    node::Node( std::move( document ) ),
    _encoding( std::move( document._encoding ) ),
    _content_type( document._content_type ),
    _url( std::move( document._url ) ),
    _origin( std::move( document._origin ) ),
    _type( document._type ),
    _quirks( document._quirks ),
    _doctype( nullptr ),
    _element( nullptr )
{
    if( document._doctype ) {
        auto it = std::find_if( this->_children.begin(),
                                this->_children.end(),
                                []( const auto & ptr ) { return ptr->nodeType() == NodeType::DOCUMENT_TYPE_NODE; } );

        if( it != this->_children.end() ) {
            _doctype = dynamic_cast<DocumentType *>( it->get() );
        } else {
            LOG_CRITICAL(
                "[parser::dom::node::Document::Document( Document && )] "
                "Failed to find the child of type 'DocumentType' in children list."
            );
        }
    }

    if( document._element ) {
        auto it = std::find_if( this->_children.begin(),
                                this->_children.end(),
                                []( const auto & ptr ) { return ptr->nodeType() == NodeType::ELEMENT_NODE; } );

        if( it != this->_children.end() ) {
            _element = dynamic_cast<Element *>( it->get() );
        } else {
            LOG_CRITICAL(
                "[parser::dom::node::Document::Document( Document && )] "
                "Failed to find the child of type 'Element' in children list."
            );
        }
    }
}

/**
 * Copy-assignment operator
 * @param document Document node to copy
 * @return Copy
 */
Document & Document::operator =( const Document &document ) {
    if( &document != this ) {
        Node::operator =( dynamic_cast<const node::Node &>( document ) );
        this->_encoding     = document._encoding;
        this->_content_type = document._content_type;
        this->_url          = document._url;
        this->_origin       = document._origin;
        this->_type         = document._type;
        this->_quirks       = document._quirks;

        if( document._doctype ) {
            auto it = std::find_if( this->_children.begin(),
                                    this->_children.end(),
                                    []( const auto & ptr ) { return ptr->nodeType() == NodeType::DOCUMENT_TYPE_NODE; } );

            if( it != this->_children.end() ) {
                _doctype = dynamic_cast<DocumentType *>( it->get() );
            } else {
                LOG_CRITICAL(
                    "[parser::dom::node::Document::operator =( const Document & )] "
                    "Failed to find the child of type 'DocumentType' in children list."
                );
            }
        }

        if( document._element ) {
            auto it = std::find_if( this->_children.begin(),
                                    this->_children.end(),
                                    []( const auto & ptr ) { return ptr->nodeType() == NodeType::ELEMENT_NODE; } );

            if( it != this->_children.end() ) {
                _element = dynamic_cast<Element *>( it->get() );
            } else {
                LOG_CRITICAL(
                    "[parser::dom::node::Document::operator =( const Document & )] "
                    "Failed to find the child of type 'Element' in children list."
                );
            }
        }
    }

    return *this;
}

/**
 * Move-assignment operator
 * @param document Document node to move
 * @return Moved document
 */
Document & Document::operator =( Document &&document ) noexcept {
    if( &document != this ) {
        Node::operator =( dynamic_cast<node::Node &&>( document ) );
        this->_encoding     = std::move( document._encoding );
        this->_content_type = document._content_type;
        this->_url          = std::move( document._url );
        this->_origin       = std::move( document._origin );
        this->_type         = document._type;
        this->_quirks       = document._quirks;

        if( document._doctype ) {
            auto it = std::find_if( this->_children.begin(),
                                    this->_children.end(),
                                    []( const auto & ptr ) { return ptr->nodeType() == NodeType::DOCUMENT_TYPE_NODE; } );

            if( it != this->_children.end() ) {
                _doctype = dynamic_cast<DocumentType *>( it->get() );
            } else {
                LOG_CRITICAL(
                    "[parser::dom::node::Document::operator =( Document && )] "
                    "Failed to find the child of type 'DocumentType' in children list."
                );
            }
        }

        if( document._element ) {
            auto it = std::find_if( this->_children.begin(),
                                    this->_children.end(),
                                    []( const auto & ptr ) { return ptr->nodeType() == NodeType::ELEMENT_NODE; } );

            if( it != this->_children.end() ) {
                _element = dynamic_cast<Element *>( it->get() );
            } else {
                LOG_CRITICAL(
                    "[parser::dom::node::Document::operator =( Document && )] "
                    "Failed to find the child of type 'Element' in children list."
                );
            }
        }
    }

    return *this;
}

/**
 * [OVERRRIDE] Shallow swaps nodes
 * @param rhs node to swap with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void Document::swap( Node &rhs ) {
    Node::swap( rhs );
    this->swap( dynamic_cast<Document &>( rhs ) );
}

/**
 * Shallow swaps Document nodes
 * @param rhs node::Document to swap with
 */
void Document::swap( Document &rhs ) {
    if( &rhs != this ) {
        std::swap( this->_encoding, rhs._encoding );
        std::swap( this->_content_type, rhs._content_type );
        std::swap( this->_url, rhs._url );
        std::swap( this->_origin, rhs._origin );
        std::swap( this->_type, rhs._type );
        std::swap( this->_quirks, rhs._quirks );
        std::swap( this->_doctype, rhs._doctype );
        std::swap( this->_element, rhs._element );
    }
}

/**
 * Gets the document init type
 * @return Type (XML/HTML)
 */
Document::Type Document::type() const {
    return _type;
}

/**
 * Gets the file path associated with the document
 * @return Document file path
 */
const std::filesystem::path & Document::filepath() const {
    return _origin;
}

/**
 * Gets the document's quirk mode
 * @return Quirks mode
 */
blogator::parser::dom::QuirksMode Document::quirksMode() const {
    return _quirks;
}

/**
 * Sets the 'quirks' flag for the document
 * @param mode quirks mode
 */
void Document::setQuirks( QuirksMode mode ) {
    _quirks = mode;
}

/**
 * Creates this Document's Element node
 * @param local_name Local name
 * @return Pointer to created Element
 * @throws DOMException when an Element already exists or name is invalid
 */
Element * Document::createDocumentElement( blogator::parser::dom::DOMString_t local_name ) {
    using blogator::parser::dom::exception::DOMException;
    using blogator::parser::dom::exception::DOMExceptionType;
    using blogator::unicode::utf8::convert;

    if( this->documentElement() ) {
        throw DOMException(
            DOMExceptionType::HierarchyRequestError,
            "[parser::dom::node::Document::createDocumentElement( \"" + convert( local_name ) + "\" )] Document Element already exists."
        );
    }

    if( !dom::validation::XML::checkName( local_name ) ) {
        throw DOMException(
            DOMExceptionType::InvalidCharacterError,
            "[parser::dom::node::Document::createDocumentElement( \"" + convert( local_name ) + "\" )] Invalid name."
        );
    }

    if( _type == Type::HTML ) {
        blogator::unicode::ascii::tolower( local_name );

        return dynamic_cast<Element *>(
            this->appendChild( std::make_unique<Element>(
                specs::infra::to_namespaceURI( specs::infra::Namespace::HTML ),
                std::move( local_name )
            ) )
        );

    } else {
        return dynamic_cast<Element *>(
            this->appendChild( std::make_unique<Element>(
                ( _content_type == specs::infra::ContentType::APPLICATION_XHTML_XML
                  ? specs::infra::to_namespaceURI( specs::infra::Namespace::HTML )
                  : U"" ),
                std::move( local_name )
            ) )
        );
    }
}

/**
 * Creates this Document's Element node
 * @param element_type Element type enum
 * @return when an Element already exists
 */
Element *Document::createDocumentElement( blogator::parser::specs::infra::Element element_type ) {
    using blogator::parser::dom::exception::DOMException;
    using blogator::parser::dom::exception::DOMExceptionType;
    using specs::infra::ContentType;
    using specs::infra::to_namespaceURI;

    const auto u8name = blogator::to_string( element_type );

    if( this->documentElement() ) {
        throw DOMException(
            DOMExceptionType::HierarchyRequestError,
            "[parser::dom::node::Document::createDocumentElement( \"" + u8name + "\" )] Document Element already exists."
        );
    }

    auto ns_str = ( _type == Type::HTML || _content_type == ContentType::APPLICATION_XHTML_XML
                    ? to_namespaceURI( specs::infra::Namespace::HTML )
                    : DOMString_t() );

    return dynamic_cast<Element *>(
        this->appendChild( std::make_unique<Element>(
            std::move( ns_str ),
            DOMString_t( u8name.cbegin(), u8name.cend() )
        ) )
    );
}

/**
 * Gets the URL
 * @return URL
 */
blogator::parser::dom::USVString_t Document::URL() const {
    return _url;
}

/**
 * Gets the document URI (i.e.: url)
 * @return URI
 */
blogator::parser::dom::USVString_t Document::documentURI() const {
    return _url;
}

/**
 * Gets the compatibility mode (based on 'quirks')
 * @return Compatibility mode string
 */
blogator::parser::dom::DOMString_t Document::compatMode() const {
    return blogator::to_u32string( _quirks );
}

/**
 * Gets the document encoding name
 * @return Encoding name
 */
blogator::parser::dom::USVString_t Document::characterSet() const {
    return _encoding;
}

/**
 * Gets the content type string
 * @return Content type string
 */
blogator::parser::dom::DOMString_t Document::contentType() const {
    return blogator::to_u32string( _content_type );
}

/**
 * Gets the document's associated 'DocumentType' node
 * @return DocumentType node (or nullptr)
 */
const blogator::parser::dom::node::DocumentType * Document::doctype() const {
    return _doctype;
}

/**
 * Gets the document's Element node
 * @return Element node (or nullptr)
 */
blogator::parser::dom::node::Element * Document::documentElement() {
    return _element;
}

/**
 * Gets the document's Element node
 * @return Element node (or nullptr)
 */
const blogator::parser::dom::node::Element * Document::documentElement() const {
    return const_cast<Element *>( const_cast<Document *>( this )->documentElement() );
}

/**
 * Gets an Element by its ID
 * @param id ID string
 * @return Pointer to Element or nullptr
 */
Element * Document::getElementById( const DOMString_t &id ) {
    const auto filter_fn = [&id]( const node::Node &node ) {
        const auto * element = dynamic_cast<const Element *>( &node );
        return ( element->id() == id ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
    };

    auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

    return ( it == this->end()
             ? nullptr
             : dynamic_cast<Element *>( it.node() )
    );
}

/**
 * Gets an Element by its ID
 * @param id ID string
 * @return Pointer to Element or nullptr
 */
const Element * Document::getElementById( const DOMString_t &id ) const {
    return const_cast<Element *>( const_cast<Document *>( this )->getElementById( id ) );
}

/**
 * Get Elements by their tag names
 * @param qualified_name Qualified name
 * @return Collection of pointers to elements whose tag names match the qualified name specified
 */
blogator::parser::dom::HTMLCollection_t Document::getElementsByTagName( const DOMString_t & qualified_name ) {
    //ref: https://dom.spec.whatwg.org/#concept-getelementsbytagname
    auto list = HTMLCollection_t();

    if( qualified_name == U"*" /* U+002A */ ) {
        auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT ) );

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }

    } else if( this->type() == Document::Type::HTML ) {
        const auto lowercase_qname = blogator::unicode::ascii::tolower( qualified_name );

        const auto filter_fn = [&qualified_name, &lowercase_qname]( const node::Node &node ) {
            const auto * element = dynamic_cast<const Element *>( &node );

            if( element->getNamespaceEnum() == specs::infra::Namespace::HTML ) {
                return ( element->qualifiedName() == lowercase_qname ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
            }

            return ( element->qualifiedName() == qualified_name ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
        };

        auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }

    } else {
        const auto filter_fn = [&qualified_name]( const node::Node &node ) {
            const auto * element = dynamic_cast<const Element *>( &node );
            return ( element->qualifiedName() == qualified_name ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
        };

        auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }
    }

    return std::move( list );
}

/**
 * Get Elements by their tag names within a namespace
 * @param ns Namespace
 * @param local_name Local name
 * @return Collection of pointers to elements whose tag names match the qualified name specified
 */
blogator::parser::dom::HTMLCollection_t Document::getElementsByTagNameNS( const DOMString_t & ns, const DOMString_t & local_name ) {
    //ref: https://dom.spec.whatwg.org/#concept-getelementsbytagnamens

    auto list = HTMLCollection_t();

    if( ns == U"*" /* U+002A */ && local_name == U"*" /* U+002A */ ) {
        auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT ) );

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }

    } else if( ns == U"*" /* U+002A */ ) {
        const auto filter_fn = [&local_name]( const node::Node &node ) {
            const auto * element = dynamic_cast<const Element *>( &node );
            return ( element->localName() == local_name ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
        };

        auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }

    } else if( local_name == U"*" /* U+002A */ ) {
        auto ns_id = Node::namespace_map.getID( ns );

        if( ns_id != NamespaceMap::INVALID ) {
            const auto filter_fn = [&ns_id]( const node::Node &node ) {
                const auto * element = dynamic_cast<const Element *>( &node );
                return ( element->namespaceID() == ns_id ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
            };

            auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

            while( it != this->end() ) {
                list.emplace_back( dynamic_cast<Element *>( it.node() ) );
                ++it;
            }
        }

    } else {
        auto ns_id = Node::namespace_map.getID( ns );

        if( ns_id != NamespaceMap::INVALID ) {
            const auto filter_fn = [&ns_id, &local_name]( const node::Node &node ) {
                const auto * element = dynamic_cast<const Element *>( &node );
                return ( element->namespaceID() == ns_id && element->localName() == local_name
                         ? NodeFilter::Result::FILTER_ACCEPT
                         : NodeFilter::Result::FILTER_SKIP );
            };

            auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

            while( it != this->end() ) {
                list.emplace_back( dynamic_cast<Element *>( it.node() ) );
                ++it;
            }
        }
    }

    return std::move( list );
}

/**
 * Get Elements by their class names
 * @param class_names Set of class names
 * @return Collection of pointers to elements whose class match the class name(s) specified
 */
blogator::parser::dom::HTMLCollection_t Document::getElementsByClassName( std::set<DOMString_t> class_names ) {
    //ref: https://dom.spec.whatwg.org/#concept-getelementsbyclassname

    if( class_names.empty() ) {
        return {}; //EARLY RETURN
    }

    HTMLCollection_t                           list;
    std::function<bool( const DOMString_t & )> compare_fn;

    if( _document && _document->quirksMode() != QuirksMode::NO_QUIRKS ) { //i.e.: 'quirks' mode
        compare_fn = [&class_names]( auto name ) { return class_names.contains( blogator::unicode::ascii::tolower( name ) ); };

        for( auto & name : class_names ) {
            blogator::unicode::ascii::tolower( name );
        }

    } else {
        compare_fn = [&class_names]( const auto & name ) { return class_names.contains( name ); };
    }

    const auto filter_fn = [&compare_fn]( const node::Node &node ) {
        const auto * element    = dynamic_cast<const Element *>( &node );
        const auto   class_list = element->classList();
        const auto   it         = std::find_if( class_list.cbegin(), class_list.cend(), compare_fn );

        return ( it != class_list.cend() ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
    };

    auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

    while( it != this->end() ) {
        list.emplace_back( dynamic_cast<Element *>( it.node() ) );
        ++it;
    }

    return std::move( list );
}

/**
 * Creates a an Element node
 * @param local_name Element name
 * @param validation Validation state (default: 'ON')
 * @return Pointer to created Element node
 * @throws DOMException when name string is not valid
 */
std::unique_ptr<blogator::parser::dom::node::Element> Document::createElement( DOMString_t local_name, ValidationState validation ) const {
    using blogator::parser::dom::exception::DOMException;
    using blogator::parser::dom::exception::DOMExceptionType;

    if( validation == ValidationState::ON && !dom::validation::XML::checkNCName( local_name ) ) {
        using blogator::unicode::utf8::convert;

        throw DOMException(
            DOMExceptionType::InvalidCharacterError,
            "[parser::dom::node::Document::createElement( \"" + convert( local_name ) + "\" )] Invalid name."
        );
    }

    if( _type == Type::HTML ) {
        blogator::unicode::ascii::tolower( local_name );

        return std::make_unique<Element>(
            const_cast<Document *>( this ),
            specs::infra::to_namespaceURI( specs::infra::Namespace::HTML ),
            std::move( local_name )
        );

    } else {
        return std::make_unique<Element>(
            const_cast<Document *>( this ),
            specs::infra::to_namespaceURI( specs::infra::to_namespace( _content_type ) ),
            std::move( local_name )
        );
    }
}

/**
 * Creates an Element node
 * @param element_type HTML5 Element type enum
 * @return Pointer to created Element node
 */
std::unique_ptr<blogator::parser::dom::node::Element> Document::createElement( specs::infra::Element element_type ) const {
    using blogator::parser::dom::exception::DOMException;
    using blogator::parser::dom::exception::DOMExceptionType;
    using specs::infra::ContentType;
    using specs::infra::to_namespaceURI;

    const auto u8name = blogator::to_string( element_type );

    if( _type == Type::HTML || _content_type == ContentType::APPLICATION_XHTML_XML ) {
        return std::make_unique<node::Element>(
            const_cast<Document *>( this ),
            to_namespaceURI( specs::infra::Namespace::HTML ),
            DOMString_t( u8name.cbegin(), u8name.cend() )
        );

    } else {
        return std::make_unique<node::Element>(
            const_cast<Document *>( this ),
            specs::infra::to_namespaceURI( specs::infra::to_namespace( _content_type ) ),
            DOMString_t( u8name.cbegin(), u8name.cend() )
        );
    }
}

/**
 * Creates an Element node
 * @param ns Namespace
 * @param qualified_name Qualified name
 * @param validation Validation state (default: 'ON')
 * @return Pointer to created Element node
 * @throws blogator::parser::dom::exception::DOMException when validation failed
 */
std::unique_ptr<blogator::parser::dom::node::Element> Document::createElementNS( const DOMString_t & ns, const DOMString_t & qualified_name, ValidationState validation ) const {
    try {
        if( validation == ValidationState::ON ) {
            auto v = validation::XML::validateNS( ns, qualified_name ); //throws

            if( v.size() == 1 ) {
                return std::make_unique<Element>( const_cast<Document *>( this ), ns, DOMString_t(), v[0] ); //EARLY RETURN
            } else {
                return std::make_unique<node::Element>( const_cast<Document *>( this ), ns, v[0], v[1] ); //EARLY RETURN
            }

        } else {
            auto v = blogator::string::split<char32_t>( qualified_name, ':', false );

            if( v.size() == 1 ) {
                return std::make_unique<Element>( const_cast<Document *>( this ), ns, DOMString_t(), v[0] ); //EARLY RETURN
            } else {
                return std::make_unique<node::Element>( const_cast<Document *>( this ), ns, v[0], v[1] ); //EARLY RETURN
            }
        }

    } catch( const exception::DOMException &e ) {
        using blogator::unicode::utf8::convert;

        throw exception::DOMException(
            e.type(),
            "[parser::dom::node::Document::createElementNS( \"" + convert( ns ) + "\", \"" + convert( qualified_name ) + "\" )] " + e.what()
        );
    }
}

/**
 * Creates a Text node
 * @param data Text data
 * @return Pointer to created Text node
 */
std::unique_ptr<blogator::parser::dom::node::Text> Document::createTextNode( DOMString_t data ) const {
    return std::make_unique<node::Text>(
        const_cast<Document *>( this ),
        std::move( data )
    );
}

/**
 * Creates a CDATASection in the document
 * @param data Text data
 * @return Pointer to created CDATASection
 */
std::unique_ptr<blogator::parser::dom::node::CDATASection> Document::createCDATASection( DOMString_t data ) const {
    return std::make_unique<node::CDATASection>(
        const_cast<Document *>( this ),
        std::move( data )
    );
}

/**
 * Creates a Comment node whose Document is this
 * @param data Text data
 * @return Pointer to created Comment node
 */
std::unique_ptr<blogator::parser::dom::node::Comment> Document::createComment( DOMString_t data ) const {
    return std::make_unique<node::Comment>(
        const_cast<Document *>( this ),
        std::move( data )
    );
}

/**
 * Creates an attribute node whose owner is this Document
 * @param local_name Local name string
 * @return Attribute (AttrPtr_t)
 */
blogator::parser::dom::AttrPtr_t Document::createAttribute( DOMString_t local_name ) {
    if( !dom::validation::XML::checkAttrName( local_name ) ) {
        using blogator::unicode::utf8::convert;

        throw dom::exception::DOMException(
            dom::exception::DOMExceptionType::InvalidCharacterError,
            "[parser::dom::node::Document::createAttribute( \"" + convert( local_name ) + "\" )] Invalid name."
        );
    }

    if( this->type() == Document::Type::HTML ) {
        unicode::ascii::tolower( local_name );
    }

    return std::make_unique<node::Attr>( this, local_name ); //EARLY RETURN
}

/**
 * Creates a namespaced attribute node whose owner is this Document
 * @param ns Namespace URI
 * @param qualified_name Qualified name
 * @return Attribute (AttrPtr_t)
 * @throws blogator::parser::dom::exception::DOMException when validation fails
 */
blogator::parser::dom::AttrPtr_t Document::createAttributeNS( const DOMString_t & ns, const DOMString_t &qualified_name ) {
    try {
        auto v = validation::XML::validateNS( ns, qualified_name );

        if( v.size() == 1 ) {
            return std::make_unique<node::Attr>( this, ns, U"", v[0] ); //EARLY RETURN
        } else {
            return std::make_unique<node::Attr>( this, ns, v[0], v[1] ); //EARLY RETURN
        }

    } catch( const exception::DOMException &e ) {
        using blogator::unicode::utf8::convert;

        throw exception::DOMException(
            e.type(),
            "[parser::dom::node::Document::createAttributeNS( \"" + convert( ns ) + "\", \"" + convert( qualified_name ) + "\" )] " + e.what()
        );
    }
}

/**
 * Creates a node iterator
 * @param root Pointer to root node of the iterator's subtree (nullptr will default to this Document)
 * @param what_to_show Filter bit-mask
 * @return Iterator to the start
 */
blogator::parser::dom::NodeIterator Document::createNodeIterator( Node * root, unsigned long what_to_show ) {
    auto * subtree_root = ( root == nullptr ? this : root );

    if( what_to_show == NodeFilter::SHOW_ALL ) {
        return { subtree_root,
                 false };

    } else {
        return { subtree_root,
                 std::make_shared<NodeFilter>( what_to_show ),
                 false };
    }
}

/**
 * Creates a node iterator
 * @param root Pointer to root node of the iterator's subtree (nullptr will default to this Document)
 * @param filter Filter to use
 * @return Iterator to the start
 */
blogator::parser::dom::NodeIterator Document::createNodeIterator( Node * root, dom::NodeFilter filter ) {
    return { ( root == nullptr ? this : root ),
             std::make_shared<NodeFilter>( std::move( filter ) ),
             false };
}

/**
 * Creates a TreeWalker
 * @param root Pointer to root node of the iterator's subtree (nullptr will default to this Document)
 * @param what_to_show Filter bit-mask
 * @return TreeWalker
 */
blogator::parser::dom::TreeWalker Document::createTreeWalker( node::Node * root, unsigned long what_to_show ) {
    auto * subtree_root = ( root == nullptr ? this : root );

    if( what_to_show == NodeFilter::SHOW_ALL ) {
        return TreeWalker( subtree_root );
    } else {
        return { subtree_root, std::make_shared<NodeFilter>( what_to_show ) };
    }
}

/**
 * Creates a TreeWalker
 * @param root Pointer to root node of the iterator's subtree (nullptr will default to this Document)
 * @param filter Filter to use
 * @return TreeWalker
 */
blogator::parser::dom::TreeWalker Document::createTreeWalker( node::Node * root, dom::NodeFilter filter ) {
    return { ( root == nullptr ? this : root ),
             std::make_shared<NodeFilter>( std::move( filter ) ) };
}

/**
 * [OVERRRIDE] Gets the node's name
 * @return Name string
 */
blogator::parser::dom::DOMString_t Document::nodeName() const {
    return U"#document";
}

/**
 * [OVERRRIDE] Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t Document::cloneNode( bool deep ) const {
    auto clone = std::make_unique<Document>( _encoding, _content_type, _url, _origin, _type, _quirks );

    if( deep ) {
        for( const auto & child : _children ) {
            auto * cloned_child = clone->appendChild( child->cloneNode( deep ) );

            if( cloned_child->nodeType() == NodeType::DOCUMENT_TYPE_NODE && clone->_doctype == nullptr) {
                clone->_doctype = dynamic_cast<DocumentType *>( cloned_child );

            } else if( cloned_child->nodeType() == NodeType::ELEMENT_NODE && clone->_element == nullptr) {
                clone->_element = dynamic_cast<Element *>( cloned_child );
            }
        }
    }

    return std::move( clone );
}

/**
 * [OVERRIDE] Lookup a namespace's prefix
 * @param ns Namespace to find prefix for
 * @return Prefix found (empty string == null)
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t Document::lookupPrefix( const blogator::parser::dom::DOMString_t &ns ) const {
    return Node::lookupPrefix( ns );
}

/**
 * [OVERRIDE] Find the namespace URI of a given prefix
 * @param prefix Prefix string (empty == null)
 * @return Namespace URI
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t Document::lookupNamespaceURI( const DOMString_t &prefix ) const {
    if( this->documentElement() ) {
        return this->documentElement()->lookupNamespaceURI( prefix );
    }

    return {};
}

/**
 * [OVERRRIDE] Insert node before a child
 * @param node Node to insert
 * @param child Pointer to child (nullptr if append at end)
 * @return Pointer to inserted child
 * @throws DOMException when insertion breaks DOM tree validity
 */
Node * Document::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    switch( node->nodeType() ) {
        case NodeType::UNDEFINED: {
            LOG_ERROR(
                "[parser::dom::node::Node::insertNodeBefore( NodePtr_t, Node * )] "
                "Inserting an '", blogator::to_string( node->nodeType() ), "' node "
                "- ignore this if for testing purposes."
            );
        } break;

        case NodeType::ATTRIBUTE_NODE: {
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Attr children." );
        }

        case NodeType::ELEMENT_NODE: {
            if( this->documentElement() ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "Document Element already exists." );
            }
            if( child && child->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "Element cannot be inserted before a DocumentType." );
            }
            if( child && child->nextSibling() && child->nextSibling()->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "Element cannot be inserted before a DocumentType." );
            }
        } break;

        case NodeType::DOCUMENT_TYPE_NODE: {
            if( this->doctype() != nullptr ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType already exists in Document." );
            }
            if( child && child->previousSibling() && child->previousSibling()->nodeType() == NodeType::ELEMENT_NODE ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType cannot be inserted after an Element." );
            }
            if( !child && this->documentElement() ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType cannot be inserted after an Element." );
            }
        } break;

        case NodeType::DOCUMENT_FRAGMENT_NODE: {
            size_t element_count = 0;

            for( const auto & node_child : node->childNodes() ) {
                if( ( node_child->nodeType() == NodeType::ELEMENT_NODE && ++element_count > 1 ) ) {
                    throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have more than 1 child Element." );
                } else if( node_child->nodeType() == NodeType::TEXT_NODE ) {
                    throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Text children." );
                }
            }

            if( element_count == 1 ) {
                if( this->documentElement() != nullptr ) {
                    throw DOMException( DOMExceptionType::HierarchyRequestError, "Document node already has a child Element." );

                } else {
                    for( auto * sibling = child; sibling != nullptr; sibling = sibling->nextSibling() ) {
                        if( sibling->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
                            throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType cannot be inserted after an Element." );
                        }
                    }
                }
            }
        } break;

        case NodeType::TEXT_NODE: {
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Text children." );
        };

        default:
            break;
    }

    auto * inserted = Node::insertNodeBefore( std::move( node ), child );

    if( inserted && inserted->nodeType() == NodeType::ELEMENT_NODE ) {
        _element = dynamic_cast<Element *>( inserted );
    } else if( inserted && inserted->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        _doctype = dynamic_cast<DocumentType *>( inserted );
    }

    return inserted;
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t Document::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    const auto * child = target.get();

    switch( node->nodeType() ) {
        case NodeType::ELEMENT_NODE: {
            if( this->documentElement() && this->documentElement() != child ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "Document Element already exists." );
            }

            auto * curr = ( target ? target->nextSibling() : nullptr );

            while( curr != nullptr ) {
                if( curr->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
                    throw DOMException( DOMExceptionType::HierarchyRequestError, "Element cannot be inserted before a DocumentType." );
                }

                curr = curr->nextSibling();
            }

        } break;

        case NodeType::TEXT_NODE: {
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Text children." );
        }

        case NodeType::DOCUMENT_NODE: {
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Document children." );
        }

        case NodeType::DOCUMENT_TYPE_NODE: {
            if( this->doctype() && this->doctype() != child ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType already exists in Document." );
            }
            if( target && target->previousSibling() && target->previousSibling()->nodeType() == NodeType::ELEMENT_NODE ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType cannot be inserted after an Element." );
            }
        } break;

        case NodeType::DOCUMENT_FRAGMENT_NODE: {
            node::Node * node_child_element = nullptr; //pointer to first found

            for( const auto & node_child : node->childNodes() ) {
                if( node_child->nodeType() == NodeType::ELEMENT_NODE ) {
                    if( node_child_element ) { //
                        throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have more than 1 child Element." );
                    } else {
                        node_child_element = node_child.get();
                    }

                } else if( node_child->nodeType() == NodeType::TEXT_NODE ) {
                    throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Text children." );
                }
            }

            if( node_child_element ) {
                if( this->documentElement() && this->documentElement() != child ) {
                    throw DOMException( DOMExceptionType::HierarchyRequestError, "Document node already has a child Element." );

                } else if( child ) {
                    for( auto * sibling = child->nextSibling(); sibling != nullptr; sibling = sibling->nextSibling() ) {
                        if( sibling->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
                            throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType cannot be inserted after an Element." );
                        }
                    }
                }
            }

        } break;

        default:
            break;
    }

    auto * replacement = node.get();
    auto   replaced    = Node::replaceChildNode( node, target );

    if( replacement->nodeType() == NodeType::ELEMENT_NODE ) {
        _element = dynamic_cast<Element *>( replacement );
    } else if( replacement->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        _doctype = dynamic_cast<DocumentType *>( replacement );
    }

    return std::move( replaced );
}

/**
 * [OVERRIDE] Removes a child node
 * @param it Child iterator to node to remove
 * @return Removed node
 */
blogator::parser::dom::NodePtr_t Document::removeChildNode( Nodes_t::iterator it ) {
    auto removed = Node::removeChildNode( it );

    if( removed ) {
        if( _element == removed.get() ) {
            _element = nullptr;
        } else if( _doctype == removed.get() ) {
            _doctype = nullptr;
        }
    }

    return std::move( removed );
}

/**
 * Shallow swaps Document nodes
 * @param lhs Document
 * @param rhs Document
 */
void blogator::parser::dom::node::swap( Document &lhs, Document &rhs ) {
    lhs.swap( rhs );
}

/**
 * Output stream operator
 * @param os Output stream
 * @param el Document::Type enum
 * @return Output stream
 */
std::ostream & blogator::parser::dom::node::operator <<( std::ostream &os, Document::Type &type ) {
    switch( type ) {
        case Document::Type::XML:  { os << "XML";  } break;
        case Document::Type::HTML: { os << "HTML"; } break;
    }

    return os;
}
