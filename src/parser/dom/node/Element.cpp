#include "Element.h"

#include <stack>

#include "CDATASection.h"
#include "Comment.h"
#include "Document.h"
#include "Text.h"
#include "../../../unicode/unicode.h"
#include "../validation/HTML5Validator.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param element
 */
Element::Element( specs::html5::Element element ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _element_ns( dom::validation::HTML5Validator::getNamespace( element ) ),
    _local_name( blogator::to_u32string( element ) )
{}

/**
 * Constructor
 * @param ns Element namespace
 * @param name Name string
 */
Element::Element( DOMString_t ns, DOMString_t name ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _element_ns( specs::html5::Namespace::UNKNOWN ),
    _namespace( std::move( ns ) ),
    _local_name( std::move( name ) )
{}

/**
 * Constructor
 * @param ns Namespace
 * @param prefix Prefix namespace
 * @param name Local name
 */
Element::Element( DOMString_t ns, DOMString_t prefix, DOMString_t name ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _element_ns( specs::html5::Namespace::UNKNOWN ),
    _prefix( std::move( prefix ) ),
    _namespace( std::move( ns ) ),
    _local_name( std::move( name ) )
{}

/**
 * Copy-constructor
 * @param node Node to copy
 */
Element::Element( const Element &node ) :
    node::Node( node ),
    _element_ns( specs::html5::Namespace::UNKNOWN ),
    _namespace( node._namespace ),
    _prefix( node._prefix ),
    _local_name( node._local_name )
{}

/**
 * Move-constructor
 * @param node Node to move
 */
Element::Element( Element &&node ) noexcept :
    node::Node( std::move( node ) ),
    _element_ns( specs::html5::Namespace::UNKNOWN ),
    _namespace( std::move( node._namespace ) ),
    _prefix( std::move( node._prefix ) ),
    _local_name( std::move( node._local_name ) )
{}

/**
 * Copy-assignment operator
 * @param node Node to copy
 * @return Copy
 */
Element &Element::operator =( const Element &node ) {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( node );
        this->_element_ns = node._element_ns;
        this->_namespace  = node._namespace;
        this->_prefix     = node._prefix;
        this->_local_name = node._local_name;
    }

    return *this;
}

/**
 * Move-assignment operator
 * @param node Node to move
 * @return Node
 */
Element &Element::operator =( Element &&node ) noexcept {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( node );
        this->_element_ns = node._element_ns;
        this->_namespace  = std::move( node._namespace );
        this->_prefix     = std::move( node._prefix );
        this->_local_name = std::move( node._local_name );
    }

    return *this;
}

/**
 * Gets the element's matching HTML type
 * @return Matching specs::html5::Element enum (UNKNOWN if string not in Element list)
 */
blogator::parser::specs::html5::Element Element::elementType() const {
    return dom::validation::HTML5Validator::getElementType( _local_name );
}

/**
 * Creates and appends an Attr(ibute) Node
 * @param name Name string
 * @return Pointer to newly created Attr(ibute) node
 */
blogator::parser::dom::node::Attr * Element::createAttribute( const DOMString_t & name ) {
    Attr * prev_attr = ( _attributes.empty() ? nullptr : _attributes.back().get() );

    return dynamic_cast<node::Attr *>(
        _attributes.emplace_back(
            std::make_unique<node::Attr>( DOMString_t(), name, this, prev_attr )
        ).get()
    );
}

/**
 * Creates and appends an Attr(ibute) Node
 * @param name Name string
 * @param value Value string
 * @return Pointer to newly created Attr(ibute) node
 */
blogator::parser::dom::node::Attr * Element::createAttribute( const DOMString_t & name, const DOMString_t & value ) {
    Attr * prev_attr = ( _attributes.empty() ? nullptr : _attributes.back().get() );

    return dynamic_cast<node::Attr *>(
        _attributes.emplace_back(
            std::make_unique<node::Attr>( DOMString_t(), name, value, this, prev_attr )
        ).get()
    );
}

/**
 * Creates and appends an Attr(ibute) Node
 * @param prefix Prefix string
 * @param name Name string
 * @param value Value string
 * @return Pointer to newly created Attr(ibute) node
 */
Attr * Element::createAttribute( const DOMString_t &prefix, const DOMString_t &name, const DOMString_t &value ) {
    Attr * prev_attr = ( _attributes.empty() ? nullptr : _attributes.back().get() );

    return dynamic_cast<node::Attr *>(
        _attributes.emplace_back(
            std::make_unique<node::Attr>( prefix, name, value, this, prev_attr )
        ).get()
    );
}

/**
 * Creates and appends a child CDATASection node
 * @param str Content string
 * @return Pointer to created CDATASection node
 */
blogator::parser::dom::node::CDATASection * Element::createCDATASection( const DOMString_t & str ) {
    return dynamic_cast<node::CDATASection *>(
        this->appendChild(
            std::make_unique<node::CDATASection>( str, this, this->lastChild() )
        )
    );
}

/**
 * Creates and append a child Comment node
 * @param comment Comment string
 * @return Pointer to created Comment node
 */
blogator::parser::dom::node::Comment * Element::createComment( const DOMString_t & comment ) {
    return dynamic_cast<node::Comment *>(
        this->appendChild(
            std::make_unique<node::Comment>( comment, this, this->lastChild() )
        )
    );
}

/**
 * Creates and append a child Element node (auto-namespace)
 * @param type HTML5/SVG/MathML element (enum)
 * @return Pointer to created Element node
 */
blogator::parser::dom::node::Element * Element::createElement( specs::html5::Element type ) {
    const auto u8name = blogator::to_string( type );

    return dynamic_cast<node::Element *>(
        this->appendChild(
            std::make_unique<node::Element>(
                this->_namespace,
                DOMString_t( u8name.cbegin(), u8name.cend() )
            )
        )
    );
}

/**
 * Creates and append a child Element node
 * @param ns Element namespace
 * @param name Element name string
 * @return Pointer to created Element node
 */
Element * Element::createElement( DOMString_t ns, DOMString_t name ) {
    return dynamic_cast<node::Element *>(
        this->appendChild(
            std::make_unique<node::Element>(
                std::move( ns ),
                std::move( name )
            )
        )
    );
}

/**
 * Creates and append a child Text node
 * @param txt Text string
 * @return Pointer to created Text node
 */
blogator::parser::dom::node::Text * Element::createText( const DOMString_t & txt ) {
    return dynamic_cast<Text *>(
        this->appendChild(
            std::make_unique<Text>( txt, this, this->lastChild() )
        )
    );
}

/**
 * Checks an attribute exists
 * @param local_name Local attribute name
 * @return Existence state
 */
bool Element::attributeExists( const DOMString_t &local_name ) const {
    auto it = std::find_if(
        _attributes.cbegin(),
        _attributes.cend(),
        [&local_name]( const AttrPtr_t &attr ) { return attr->name() == local_name; }
    );

    return ( it != _attributes.cend() );
}

/**
 * Checks an attribute exists
 * @param prefix Prefix namespace
 * @param local_name Local attribute name
 * @return Existence state
 */
bool Element::attributeExists( const DOMString_t &prefix, const DOMString_t &local_name ) const {
    auto it = std::find_if(
        _attributes.cbegin(),
        _attributes.cend(),
        [&prefix, &local_name]( const AttrPtr_t &attr ) { return attr->prefix() == prefix && attr->name() == local_name; }
    );

    return ( it != _attributes.cend() );
}

/**
 * Gets the namespace URI
 * @return Namespace URI
 */
blogator::parser::dom::DOMString_t Element::namespaceURI() const {
    return _namespace;
}

/**
 * Gets the element's prefix namespace
 * @return Prefix namespace
 */
blogator::parser::dom::DOMString_t Element::prefix() const {
    return _prefix;
}

/**
 * Gets the element's local name
 * @return Local name
 */
blogator::parser::dom::DOMString_t Element::localName() const {
    return _local_name;
}

/**
 * Gets the element's tag name
 * @return Tag name
 */
blogator::parser::dom::DOMString_t Element::tagName() const {
    return unicode::ascii::toupper( qualifiedName() );
}

blogator::parser::dom::DOMString_t Element::id() const {
    return DOMString_t();
}

blogator::parser::dom::DOMString_t Element::className() const {
    return DOMString_t();
}

blogator::parser::dom::DOMTokenList_t Element::classList() const {
    return blogator::parser::dom::DOMTokenList_t();
}

blogator::parser::dom::DOMString_t Element::slot() const {
    return DOMString_t();
}

/**
 * Checks Element had attribute(s)
 * @return Has attributes
 */
bool Element::hasAttributes() const {
    return !_attributes.empty();
}














/**
 * Gets the element's attributes
 * @return Attributes
 */
blogator::parser::dom::Attributes_t & Element::attributes() {
    return _attributes;
}

/**
 * Gets the element's attributes
 * @return Attributes
 */
const blogator::parser::dom::Attributes_t & Element::attributes() const {
    return _attributes;
}

/**
 * [OVERRIDE] Gets the text content
 * @return Text content string
 */
blogator::parser::dom::DOMString_t Element::textContent() const {
    auto concatenation = DOMString_t();

    for( const auto &node : _children ) {
        if( node->nodeType() == NodeType::TEXT_NODE ) {
            if( node->nodeValue() ) {
                concatenation.append( *node->nodeValue() );
            }

        } else if( node->nodeType() == NodeType::ELEMENT_NODE ) {
            concatenation.append( node->textContent() );
        }
    }

    return std::move( concatenation );
}

/**
 * Gets the node's name
 * @return HTML-upper-cased qualified name string
 */
blogator::parser::dom::DOMString_t Element::nodeName() const {
    using dom::validation::HTML5Validator;
    using specs::html5::Namespace;

    const node::Document * doc = const_cast<Element *>( this )->ownerDocument();
    const auto             el  = this->elementType();

    if( doc && doc->type() == Document::Type::HTML && HTML5Validator::getNamespace( el ) == Namespace::HTML5 ) {
        return unicode::ascii::toupper( _local_name );

    } else {
        return qualifiedName();
    }
}

/**
 * [OVERRIDE] Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t Element::cloneNode( bool deep ) const {
    auto clone = std::make_unique<Element>( _namespace, _prefix, _local_name );

    for( const auto &attr : _attributes ) {
        if( attr->hasValue() ) {
            std::ignore = clone->createAttribute( attr->prefix(), attr->name(), attr->value() );
        } else {
            std::ignore = clone->createAttribute( attr->prefix(), attr->name() );
        }
    }

    if( deep ) {
        for( const auto & child : _children ) {
            clone->appendChild( child->cloneNode( deep ) );
        }
    }

    return std::move( clone );
}

/**
 * [OVERRIDE] Checks node is equivalent to reference node
 * @param node Node to check
 * @return Are equivalent nodes
 */
bool Element::isEqualNode( const Node &other ) const {
    if( this->nodeType()          == other.nodeType() &&
        this->childNodes().size() == other.childNodes().size() )
    {
        const auto * rhs = dynamic_cast<const Element *>( &other );

        bool equal = this->namespaceURI()      == rhs->namespaceURI()
                  && this->nodeName()          == rhs->nodeName()
                  && this->attributes().size() == rhs->attributes().size();

        if( equal ) {
            for( size_t i = 0; i < this->childNodes().size(); ++i ) {
                if( !this->childNodes()[i]->isEqualNode( *other.childNodes()[i] ) ) {
                    return false;
                }
            }

            return true;
        }
    }

    return false;
}

/**
 * [OVERRIDE] Insert node before a child
 * @param node Node to insert
 * @param child Pointer to child (nullptr if append at end)
 * @return Pointer to inserted child
 * @throws DOMException when insertion breaks DOM tree validity
 */
Node * Element::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( node->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType must be a Document child." );

    } else if( node->nodeType() == NodeType::ATTRIBUTE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "Attr must be added using the dedicated API." );

    } else {
        return Node::insertNodeBefore( std::move( node ), child ); //EARLY RETURN
    }
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t Element::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( node->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType must be a Document child." );

    } else if( node->nodeType() == NodeType::ATTRIBUTE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "Attr must be added using the dedicated API." );

    } else {
        return Node::replaceChildNode( node, target );
    }
}

/**
 * [PRIVATE] Gets the element's qualified name
 * @return Qualified name
 */
blogator::parser::dom::DOMString_t Element::qualifiedName() const {
    if( _prefix.empty() ) {
        return _local_name;
    } else {
        return _prefix + U":" + _local_name;
    }
}