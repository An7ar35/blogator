#include "Attr.h"

#include "Element.h"
#include "../../../unicode/unicode.h"
#include "../../../exception/failed_expectation.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param local_name Local name
 */
Attr::Attr( DOMString_t local_name ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _namespace_id( NamespaceMap::NONE ),
    _name( std::move( local_name ) ),
    _has_value( false )
{}


/**
 * Constructor
 * @param local_name Attribute name string
 */
Attr::Attr( DOMString_t local_name, DOMString_t value ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _namespace_id( NamespaceMap::NONE ),
    _name( std::move( local_name ) ),
    _value( std::move( value ) ),
    _has_value( true )
{}

/**
 * Constructor
 * @param ns Namespace
 * @param prefix Prefix
 * @param local_name Local name
 */
Attr::Attr( const DOMString_t & ns, DOMString_t prefix, DOMString_t local_name ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _namespace_id( Node::namespace_map.setNamespace( ns, prefix ) ),
    _prefix( std::move( prefix ) ),
    _name( std::move( local_name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param ns_id Namespace ID
 * @param prefix Prefix
 * @param local_name Local name
 */
Attr::Attr( NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t local_name ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _namespace_id( ns_id ),
    _prefix( std::move( prefix ) ),
    _name( std::move( local_name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param ns Namespace
 * @param prefix Prefix
 * @param local_name Local name
 * @param value Value string
 */
Attr::Attr( const DOMString_t & ns, DOMString_t prefix, DOMString_t local_name, DOMString_t value ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _namespace_id( Node::namespace_map.setNamespace( ns, prefix ) ),
    _prefix( std::move( prefix ) ),
    _name( std::move( local_name ) ),
    _value( std::move( value ) ),
    _has_value( true )
{}

/**
 * Constructor
 * @param document Pointer to owner document
 * @param local_name Local name
 */
Attr::Attr( Document * document, DOMString_t local_name ) :
    node::Node( document, NodeType::ATTRIBUTE_NODE ),
    _namespace_id( NamespaceMap::NONE ),
    _name( std::move( local_name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param document Pointer to owner document
 * @param local_name Local name
 * @param value Value string
 */
Attr::Attr( Document * document, DOMString_t local_name, DOMString_t value ) :
    node::Node( document, NodeType::ATTRIBUTE_NODE ),
    _namespace_id( NamespaceMap::NONE ),
    _name( std::move( local_name ) ),
    _value( std::move( value ) ),
    _has_value( true )
{}

/**
 * Constructor
 * @param document Pointer to owner document
 * @param ns Namespace
 * @param prefix Prefix
 * @param local_name Local name
 */
Attr::Attr( Document * document, const DOMString_t &ns, DOMString_t prefix, DOMString_t local_name ) :
    node::Node( document, NodeType::ATTRIBUTE_NODE ),
    _namespace_id( Node::namespace_map.setNamespace( ns, prefix ) ),
    _prefix( std::move( prefix ) ),
    _name( std::move( local_name ) ),
    _has_value( false )
{}

/**
 * Constructor
 * @param document Pointer to owner document
 * @param ns Namespace
 * @param prefix Prefix
 * @param local_name Local name
 * @param value Value string
 */
Attr::Attr( Document * document, const DOMString_t &ns, DOMString_t prefix, DOMString_t local_name, DOMString_t value ) :
    node::Node( document, NodeType::ATTRIBUTE_NODE ),
    _namespace_id( Node::namespace_map.setNamespace( ns, prefix ) ),
    _prefix( std::move( prefix ) ),
    _name( std::move( local_name ) ),
    _value( std::move( value ) ),
    _has_value( true )
{}

/**
 * Constructor
 * @param ns_id Namespace ID
 * @param prefix Prefix
 * @param name Local name
 * @param value Value string
 */
Attr::Attr( NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t name, DOMString_t value ) :
    node::Node( NodeType::ATTRIBUTE_NODE ),
    _namespace_id( ns_id ),
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
    _namespace_id( node._namespace_id ),
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
    _namespace_id( node._namespace_id ),
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
Attr & Attr::operator =( const Attr &node ) {
    if( &node != this ) {
        Node::operator =( dynamic_cast<const node::Node &>( node ) );
        this->_namespace_id = node._namespace_id ;
        this->_prefix       = node._prefix;
        this->_name         = node._name;
        this->_value        = node._value;
        this->_has_value    = node._has_value;
    }

    return *this;
}

/**
 * Move-constructor
 * @param node Node to move
 * @return Node
 */
Attr & Attr::operator =( Attr &&node ) noexcept {
    if( &node != this ) {
        Node::operator =( dynamic_cast<node::Node &&>( node ) );
        this->_namespace_id = node._namespace_id ;
        this->_prefix       = std::move( node._prefix );
        this->_name         = std::move( node._name );
        this->_value        = std::move( node._value );
        this->_has_value    = node._has_value;
    }

    return *this;
}

/**
 * [OVERRRIDE] Shallow swaps nodes
 * @param rhs node to swap with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void Attr::swap( Node &rhs ) {
    Node::swap( rhs );
    this->swap( dynamic_cast<Attr &>( rhs ) );
}

/**
 * Shallow swaps Attr nodes
 * @param rhs node::Attr to swap with
 */
void Attr::swap( Attr &rhs ) {
    if( &rhs != this ) {
        std::swap( this->_namespace_id, rhs._namespace_id );
        std::swap( this->_prefix, rhs._prefix );
        std::swap( this->_name, rhs._name );
        std::swap( this->_value, rhs._value );
        std::swap( this->_has_value, rhs._has_value );
    }
}

/**
 * Check equivalence
 * @param rhs Attribute to compare to
 * @return Equivalence state
 */
bool Attr::equivalent( const Attr &rhs ) const {
    return this->_namespace_id == rhs._namespace_id
        && this->_prefix       == rhs._prefix
        && this->_name         == rhs._name
        && this->_value        == rhs._value
        && this->_has_value    == rhs._has_value;
}

/**
 * Gets the namespace NamespaceMap ID
 * @return NamespaceMap ID for the Attribute's namespace
 */
blogator::parser::dom::NamespaceMap::id_t Attr::namespaceID() const {
    return _namespace_id;
}

/**
 * Gets the namespace URI
 * @return Namespace URI
 * @throws blogator::exception::failed_expectation when namespace ID doesn't exist in global NamespaceMap
 */
blogator::parser::dom::DOMString_t Attr::namespaceURI() const {
    try {
        return Node::namespace_map.getNamespaceURI( _namespace_id );

    } catch( const std::out_of_range &e ) {
        using blogator::unicode::utf8::convert;

        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::node::Attr::namespaceURI()] "
            "Failed to get URI for node '" + convert( this->nodeName() ) + "'."
        );
    }
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
const blogator::parser::dom::DOMString_t & Attr::name() const {
    return _name;
}

/**
 * Gets the value
 * @return Value string
 */
blogator::parser::dom::DOMString_t * Attr::value() {
    return ( hasValue() ? &_value : nullptr );
}

/**
 * Gets the value
 * @return Value string
 */
const blogator::parser::dom::DOMString_t * Attr::value() const {
    return const_cast<DOMString_t *>( const_cast<Attr *>( this )->value() );
}

/**
 * Checks if attribute has a value
 * @return Value state
 */
bool Attr::hasValue() const {
    return !_value.empty();
}

/**
 * Gets ownerNode Element
 * @return Pointer to ownerNode Element (nullptr if orphaned/parent is not Element node)
 */
blogator::parser::dom::node::Element * Attr::ownerElement() {
    if( this->parentNode() && this->parentNode()->nodeType() == NodeType::ELEMENT_NODE ) {
        return dynamic_cast<node::Element *>( this->parentNode() );
    } else {
        return nullptr;
    }
}

/**
 * [OVERRIDE] Gets the Namespace enum associated with this node's ID
 * @return specs::infra::Namespace enum associated with the ID
 * @throws blogator::exception::failed_expectation when mapping inconsistency is detected
 */
blogator::parser::specs::infra::Namespace Attr::getNamespaceEnum() const {
    return Node::getNamespaceEnum( this->_namespace_id );
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
        this->hasValue() ? std::make_unique<Attr>( _namespace_id, _prefix, _name, _value )
                         : std::make_unique<Attr>( _namespace_id, _prefix, _name )
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

        bool equal = this->_namespace_id == rhs->_namespace_id
                  && this->_name         == rhs->_name
                  && this->_has_value    == rhs->_has_value
                  && this->_value        == rhs->_value;

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
 * [OVERRIDE] Lookup a namespace's prefix
 * @param ns Namespace to find prefix for
 * @return Prefix found (empty string == null)
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t Attr::lookupPrefix( const blogator::parser::dom::DOMString_t &ns ) const {
    if( this->parentElement() ) {
        return this->parentElement()->lookupPrefix( ns );
    }

    return {};
}

/**
 * [OVERRIDE] Find the namespace URI of a given prefix
 * @param prefix Prefix string (empty == null)
 * @return Namespace URI
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t Attr::lookupNamespaceURI( const DOMString_t &prefix ) const {
    if( this->parentElement() ) {
        return this->parentElement()->lookupNamespaceURI( prefix );
    }

    return {};
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

/**
 * Shallow swaps Attribute nodes
 * @param lhs Attr
 * @param rhs Attr
 */
void blogator::parser::dom::node::swap( Attr &lhs, Attr &rhs ) {
    lhs.swap( rhs );
}
