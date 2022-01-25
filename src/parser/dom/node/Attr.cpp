#include "Attr.h"

#include "Element.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param name Attribute name string
 */
Attr::Attr( DOMString_t name ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _name( std::move( name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param prefix Namespace prefix
 * @param name Attribute name string
 */
Attr::Attr( DOMString_t prefix, DOMString_t name ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _prefix( std::move( prefix ) ),
    _name( std::move( name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param prefix Namespace prefix
 * @param name Attribute name string
 * @param value Attribute value string
 */
Attr::Attr( DOMString_t prefix, DOMString_t name, DOMString_t value ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _prefix( std::move( prefix ) ),
    _name( std::move( name ) ),
    _value( std::move( value ) ),
    _has_value( true )
{}

/**
 * Constructor
 * @param prefix Namespace prefix
 * @param name Attribute name string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
Attr::Attr( DOMString_t prefix, DOMString_t name, node::Node * parent, node::Node * prev_sibling ) :
    node::Node( NodeType::ATTRIBUTE_NODE, parent, prev_sibling ),
    _prefix( std::move( prefix ) ),
    _name( std::move( name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param prefix Namespace prefix
 * @param name Attribute name string
 * @param value Attribute value string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
Attr::Attr( DOMString_t prefix, DOMString_t name, DOMString_t value, node::Node * parent, node::Node * prev_sibling ) :
    node::Node( NodeType::ATTRIBUTE_NODE, parent, prev_sibling ),
    _prefix( std::move( prefix ) ),
    _name( std::move( name ) ),
    _value( std::move( value ) ),
    _has_value( true )
{}

/**
 * Constructor
 * @param prefix Namespace prefix
 * @param name Attribute name string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
Attr::Attr( DOMString_t prefix, DOMString_t name, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling ) :
    node::Node( NodeType::ATTRIBUTE_NODE, parent, prev_sibling, next_sibling ),
    _prefix( std::move( prefix ) ),
    _name( std::move( name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param prefix Namespace prefix
 * @param name Attribute name string
 * @param value Attribute value string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
Attr::Attr( DOMString_t prefix, DOMString_t name, DOMString_t value, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling ) :
    node::Node( NodeType::ATTRIBUTE_NODE, parent, prev_sibling, next_sibling ),
    _prefix( std::move( prefix ) ),
    _name( std::move( name ) ),
    _value( std::move( value ) ),
    _has_value( true )
{}

/**
 * Copy-constructor
 * @param node Node to shallow copy
 */
Attr::Attr( const Attr &node ) :
    node::Node( node ),
    _prefix( node._prefix ),
    _name( node._name ),
    _has_value( node._has_value ),
    _value( node._value )
{}

/**
 * Move-constructor
 * @param node Node to move
 */
Attr::Attr( Attr &&node ) noexcept :
    node::Node( std::move( node ) ),
    _prefix( std::move( node._prefix ) ),
    _name( std::move( node._name ) ),
    _value( std::move( node._value ) ),
    _has_value( node._has_value )
{}

/**
 * Copy-operator
 * @param node Node to copy
 * @return Copy
 */
Attr &Attr::operator =( const Attr &node ) {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( node );
        this->_prefix    = node._prefix;
        this->_name      = node._name;
        this->_value     = node._value;
        this->_has_value = node._has_value;
    }

    return *this;
}

/**
 * Move-constructor
 * @param node Node to move
 * @return Node
 */
Attr &Attr::operator =( Attr &&node ) noexcept {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<node::Node &&>( node );
        this->_prefix    = std::move( node._prefix );
        this->_name      = std::move( node._name );
        this->_value     = std::move( node._value );
        this->_has_value = node._has_value;
    }

    return *this;
}

/**
 * Gets the prefix (namespace)
 * @return Prefix
 */
const blogator::parser::dom::DOMString_t & Attr::prefix() const {
    return _prefix;
}

/**
 * Gets the local name
 * @return 'name' string
 */
const blogator::parser::dom::DOMString_t & Attr::localName() const {
    return _name;
}

/**
 * Gets the name
 * @return Name string
 */
const blogator::parser::dom::DOMString_t &Attr::name() const {
    return _name;
}

/**
 * Gets the value
 * @return Value string
 */
const blogator::parser::dom::DOMString_t & Attr::value() const {
    return _value;
}

/**
 * Checks if attribute has a value
 * @return Value state
 */
bool Attr::hasValue() const {
    return !_value.empty();
}

/**
 * Gets owner Element
 * @return Pointer to owner Element (nullptr if orphaned/parent is not Element node)
 */
blogator::parser::dom::node::Element * Attr::ownerElement() {
    if( this->parentNode() && this->parentNode()->nodeType() == NodeType::ELEMENT_NODE ) {
        return dynamic_cast<node::Element *>( this->parentNode() );
    } else {
        return nullptr;
    }
}

/**
 * [OVERRIDE] Gets the node's name
 * @return Attribute's qualified name string
 */
blogator::parser::dom::DOMString_t Attr::nodeName() const {
    if( _prefix.empty() ) {
        return _name;
    } else {
        return _prefix + U":" + _name;
    }
}

/**
 * [OVERRRIDE] Gets the node's value
 * @return Pointer to node value string
 */
blogator::parser::dom::DOMString_t * Attr::nodeValue() {
    return &_value;
}

/**
 * [OVERRRIDE] Gets the text content
 * @return Text content string
 */
blogator::parser::dom::DOMString_t Attr::textContent() const {
    return _value;
}

/**
 * [OVERRRIDE] Gets the length of the node
 * @return 0
 */
size_t Attr::length() const {
    return 0;
}

/**
 * [OVERRIDE] Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t Attr::cloneNode( bool deep ) const {
    auto clone = (
        this->hasValue() ? std::make_unique<Attr>( _prefix, _name, _value )
                         : std::make_unique<Attr>( _prefix, _name )
    );

    return std::move( clone );
}

/**
 * [OVERRIDE] Checks node is equivalent to reference node
 * @param node Node to check
 * @return Are equivalent nodes
 */
bool Attr::isEqualNode( const Node &other ) const {
    if( this->nodeType()          == other.nodeType() &&
        this->childNodes().size() == other.childNodes().size() )
    {
        const auto * rhs = dynamic_cast<const Attr *>( &other );

        bool equal = this->prefix()   == rhs->prefix()
                  && this->name()     == rhs->name()
                  && this->hasValue() == rhs->hasValue()
                  && this->value()    == rhs->value();

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
Node * Attr::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "Attr(ibute) nodes cannot have children." );
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t Attr::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "Attr(ibute) nodes do not have children." );
}