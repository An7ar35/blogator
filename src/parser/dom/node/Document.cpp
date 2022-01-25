#include "Document.h"

#include <utility>

#include "../../../logger/Logger.h"
#include "Attr.h"
#include "CDATASection.h"
#include "Comment.h"
#include "DocumentType.h"
#include "Element.h"
#include "Text.h"
#include "../DOMImplementation.h"
#include "../validation/HTML5Validator.h"
#include "../exception/exception.h"
#include "../iterator/NodeIterator.h"
#include "../iterator/TreeWalker.h"
#include "../../../unicode/unicode.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor (default XHTML/no quirks)
 */
Document::Document() :
    node::Node( NodeType::DOCUMENT_NODE ),
    _content_type( specs::html5::ContentType::APPLICATION_XHTML_XML ),
    _type( Document::Type::HTML ),
    _quirks( QuirksMode::NO_QUIRKS ),
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
Document::Document( USVString_t encoding, specs::html5::ContentType content_type, USVString_t url, std::filesystem::path origin, Type type, QuirksMode quirks ) :
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
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( document );
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
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( document );
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
const blogator::parser::dom::node::Element * Document::documentElement() const {
    return _element;
}






blogator::parser::dom::HTMLCollection_t Document::getElementsByTagName( const DOMString_t & qualified_name ) {
    return HTMLCollection_t(); //TODO
}

blogator::parser::dom::HTMLCollection_t Document::getElementsByTagNameNS( const DOMString_t & ns, const DOMString_t & local_name ) {
    return HTMLCollection_t(); //TODO
}

blogator::parser::dom::HTMLCollection_t Document::getElementsByClassName( const DOMString_t & class_name ) {
    return HTMLCollection_t(); //TODO
}

/**
 * Creates a root element in the document
 * @param local_name Element name
 * @return Pointer to created Element
 * @throws DOMException when name string is not valid
 */
std::unique_ptr<blogator::parser::dom::node::Element> Document::createElement( DOMString_t local_name ) {
    if( !dom::validation::HTML5Validator::isValidName( local_name ) ) {
        throw exception::DOMException( exception::DOMExceptionType::InvalidCharacterError );
    }

    if( _type == Type::HTML ) {
        std::transform( local_name.begin(),
                        local_name.end(),
                        local_name.begin(),
                        []( const auto c ) { return blogator::unicode::ascii::tolower( c ); }
        );

        return std::make_unique<Element>(
            specs::html5::to_namespaceURI( specs::html5::Namespace::HTML5 ),
            std::move( local_name )
        );

    } else {
        return std::make_unique<Element>(
            ( _content_type == specs::html5::ContentType::APPLICATION_XHTML_XML
              ? specs::html5::to_namespaceURI( specs::html5::Namespace::HTML5 )
              : U"" ),
            std::move( local_name )
        );
    }
}

/**
 * Creates a root element in the document
 * @param element_type HTML5 Element type enum
 * @return Pointer to created Element
 */
std::unique_ptr<blogator::parser::dom::node::Element> Document::createElement( specs::html5::Element element_type ) {
    using specs::html5::ContentType;
    using specs::html5::to_namespaceURI;

    const auto u8name = blogator::to_string( element_type );
    auto       ns_str = ( _type == Type::HTML || _content_type == ContentType::APPLICATION_XHTML_XML
                          ? to_namespaceURI( specs::html5::Namespace::HTML5 )
                          : DOMString_t() );

    return std::make_unique<node::Element>(
        std::move( ns_str ),
        DOMString_t( u8name.cbegin(), u8name.cend() )
    );
}

/**
 * Creates a root element in the document
 * @param ns Namespace
 * @param prefix Prefix namespace
 * @param local_name Qualified name
 * @return Pointer to created Element
 */
std::unique_ptr<blogator::parser::dom::node::Element> Document::createElementNS( DOMString_t ns, DOMString_t prefix, DOMString_t local_name ) {
    //TODO do we need to check the ns and name format?
    return std::make_unique<node::Element>(
        std::move( ns ),
        std::move( prefix ),
        std::move( local_name )
    );
}

/**
 * Creates a Text in the document
 * @param data Text data
 * @return Pointer to created Text
 */
std::unique_ptr<blogator::parser::dom::node::Text> Document::createTextNode( DOMString_t data ) {
    return std::make_unique<node::Text>(
        std::move( data )
    );
}

/**
 * Creates a CDATASection in the document
 * @param data Text data
 * @return Pointer to created CDATASection
 */
std::unique_ptr<blogator::parser::dom::node::CDATASection> Document::createCDATASection( DOMString_t data ) {
    return std::make_unique<node::CDATASection>(
        std::move( data )
    );
}

/**
 * Creates a Comment in the document
 * @param data Text data
 * @return Pointer to created Comment
 */
std::unique_ptr<blogator::parser::dom::node::Comment> Document::createComment( DOMString_t data ) {
    return std::make_unique<node::Comment>(
        std::move( data )
    );
}

/**
 * Imports a node
 * @param node Pointer to target node
 * @param deep Deep copy flag
 * @return Copied node
 */
blogator::parser::dom::NodePtr_t Document::importNode( const Node * node, bool deep ) {
    if( node == nullptr ) {
        throw blogator::exception::failed_expectation(
            "[parser::dom::node::Document::importNode( const Node *, deep=" + std::to_string( deep ) + " )] Node is NULL."
        );
    }

    if( node->nodeType() == NodeType::DOCUMENT_NODE ) {
        throw exception::DOMException( exception::DOMExceptionType::NotSupportedError, "Cannot import a 'Document' node." );

    } else {
        return node->cloneNode( deep );
    }
}

/**
 * Adopts a node into the document
 * @param node Node to adopt
 * @return Pointer to adopted node
 * @throws DOMException when action is not supported or node is null
 */
blogator::parser::dom::node::Node * Document::adoptNode( NodePtr_t &node ) {
    if( !node ) {
        throw blogator::exception::failed_expectation(
            "[parser::dom::node::Document::adoptNode( NodePtr_t & )] Node is NULL."
        );
    }

    if( node->nodeType() == NodeType::DOCUMENT_NODE ) {
        throw exception::DOMException( exception::DOMExceptionType::NotSupportedError, "Cannot adopt a 'Document' node." );
    } else {
        return this->appendChild( this->removeChild( node ) );
    }
}

std::unique_ptr<Attr> Document::createAttribute( DOMString_t local_name ) { //TODO
    //DOMString_t prefix, DOMString_t name, DOMString_t value
    return {};
}

std::unique_ptr<Attr> Document::createAttributeNS( DOMString_t ns, DOMString_t qualified_name ) { //TODO

    return {};
}

//blogator::parser::dom::NodeIterator Document::createNodeIterator( node::Node * root, unsigned long what_to_show ) { //TODO
//    return blogator::parser::dom::NodeIterator();
//}
//
//blogator::parser::dom::NodeIterator Document::createNodeIterator( node::Node * root, dom::NodeFilter filter ) { //TODO
//    return blogator::parser::dom::NodeIterator();
//}
//
//blogator::parser::dom::TreeWalker Document::createTreeWalker( node::Node * root, unsigned long what_to_show ) { //TODO
//    return blogator::parser::dom::TreeWalker();
//}
//
//blogator::parser::dom::TreeWalker Document::createTreeWalker( node::Node * root, dom::NodeFilter filter ) { //TODO
//    return blogator::parser::dom::TreeWalker();
//}

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
 * Sets the 'quirks' flag for the document
 * @param mode quirks mode
 */
void Document::setQuirks( QuirksMode mode ) {
    _quirks = mode;
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
 * [OVERRRIDE] Appends a child node to the children list (auto-sets the parent/sibling pointers)
 * @param node_ptr Pointer to child node to append
 * @return Pointer to newly created child
 */
Node * Document::appendChild( NodePtr_t &node_ptr ) {
    auto * child = Node::appendChild( std::move( node_ptr ) );

    if( child->nodeType() == NodeType::ELEMENT_NODE ) {
        _element = dynamic_cast<Element *>( child );
    } else if( child->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        _doctype = dynamic_cast<DocumentType *>( child );
    }

    return child;
}

/**
 * [OVERRRIDE] Appends a child node to the children list (auto-sets the parent/sibling pointers)
 * @param node_ptr Pointer to child node to append
 * @return Pointer to newly created child
 */
Node * Document::appendChild( NodePtr_t &&node_ptr ) {
    auto * child = Node::appendChild( std::move( node_ptr ) );

    if( child->nodeType() == NodeType::ELEMENT_NODE ) {
        _element = dynamic_cast<Element *>( child );
    } else if( child->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        _doctype = dynamic_cast<DocumentType *>( child );
    }

    return child;
}

/**
 * [OVERRRIDE] Inserts a child node before a specified child (auto-sets the parent/sibling pointers)
 * @param node Pointer to Node to insert
 * @param child Pointer to reference node for insertion placement
 * @return Pointer to inserted node
 * @throws DOMException when valid hierarchy is violated
 * @throws std::invalid_argument if node is null
 */
Node * Document::insertBefore( NodePtr_t &node, Node *child ) {
    auto * inserted = Node::insertBefore( node, child );

    if( child->nodeType() == NodeType::ELEMENT_NODE ) {
        _element = dynamic_cast<Element *>( inserted );
    } else if( child->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        _doctype = dynamic_cast<DocumentType *>( inserted );
    }

    return inserted;
}

/**
* [OVERRRIDE] Inserts a child node before a specified child (auto-sets the parent/sibling pointers)
* @param node Pointer to Node to insert
* @param child Pointer to reference node for insertion placement
* @return Pointer to inserted node
* @throws DOMException when valid hierarchy is violated
* @throws std::invalid_argument if node is null
*/
Node * Document::insertBefore( NodePtr_t && node, Node * child ) {
    auto * inserted = Node::insertBefore( node, child );

    if( child->nodeType() == NodeType::ELEMENT_NODE ) {
        _element = dynamic_cast<Element *>( inserted );
    } else if( child->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        _doctype = dynamic_cast<DocumentType *>( inserted );
    }

    return inserted;
}

/**
 * [OVERRRIDE]
 * @param node
 * @param child
 * @return
 */
blogator::parser::dom::NodePtr_t Document::replaceChild( NodePtr_t &node, NodePtr_t &child ) { //TODO
    auto * replacement = node.get();
    auto   replaced    = Node::replaceChild( node, child );

    if( replacement->nodeType() == NodeType::ELEMENT_NODE ) {
        _element = dynamic_cast<Element *>( replacement );
    } else if( replacement->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        _doctype = dynamic_cast<DocumentType *>( replacement );
    }

    return std::move( replaced );
}

/**
 * [OVERRRIDE]
 * @param child
 * @return
 */
blogator::parser::dom::NodePtr_t Document::removeChild( NodePtr_t &child ) { //TODO
    return Node::removeChild( child );
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
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Attr children" );
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

        case NodeType::TEXT_NODE: {
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Text children." );
        };

        default:
            break;
    }

    return Node::insertNodeBefore( std::move( node ), child );
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

    switch( node->nodeType() ) {
        case NodeType::ELEMENT_NODE: {
            if( this->documentElement() && this->documentElement() != target.get() ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "Document Element already exists." );
            }
            if( target && target->nextSibling() && target->nextSibling()->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "Element cannot be inserted before a DocumentType." );
            }
        } break;

        case NodeType::TEXT_NODE: {
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Text children." );
        }

        case NodeType::DOCUMENT_NODE: {
            throw DOMException( DOMExceptionType::HierarchyRequestError, "Document nodes cannot have Document children." ); //TODO
        }

        case NodeType::DOCUMENT_TYPE_NODE: {
            if( this->doctype() && this->doctype() != target.get() ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType already exists in Document." );
            }
            if( target && target->previousSibling() && target->previousSibling()->nodeType() == NodeType::ELEMENT_NODE ) {
                throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType cannot be inserted after an Element." );
            }
        } break;

        default:
            break;
    }

    return Node::replaceChildNode( node, target );
}