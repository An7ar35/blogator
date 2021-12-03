#include "Node.h"

#include <stdexcept>

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param type Node type
 */
Node::Node( NodeType type ) :
    _parent( nullptr ),
    _prev_sibling( nullptr ),
    _next_sibling( nullptr ),
    _type( type )
{}

/**
 * Constructor
 * @param type Node type
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
Node::Node( blogator::parser::dom::NodeType type, Node * parent, Node * prev_sibling ) :
    _parent( parent ),
    _prev_sibling( prev_sibling ),
    _next_sibling( nullptr ),
    _type( type )
{
    if( hasSiblingPrev() ) {
        _next_sibling                = _prev_sibling->_next_sibling;
        _prev_sibling->_next_sibling = this;

        if( hasSiblingNext() ) {
            _next_sibling->_prev_sibling = this;
        }
    }
}

/**
 * Constructor
 * @param type Node type
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
Node::Node( NodeType type, Node * parent, Node * prev_sibling, Node * next_sibling ) :
    _parent( parent ),
    _prev_sibling( prev_sibling ),
    _next_sibling( next_sibling ),
    _type( type )
{
    if( hasSiblingPrev() ) {
        _prev_sibling->_next_sibling = this;
    }

    if( hasSiblingNext() ) {
        _next_sibling->_prev_sibling = this;
    }
}

/**
 * Destructor
 */
Node::~Node() {
    if( _prev_sibling ) {
        _prev_sibling->_next_sibling = _next_sibling;
    }

    if( _next_sibling ) {
        _next_sibling->_prev_sibling = _prev_sibling;
    }
}

/**
 * Equality comparator
 * @param rhs Node to compare to
 * @return Is same
 */
bool Node::operator ==( const Node &rhs ) const {
    return this == &rhs;
}

/**
 * Not-equal comparator
 * @param rhs Node to compare to
 * @return Is not same
 */
bool Node::operator !=( const Node &rhs ) const {
    return !( *this == rhs );
}

/**
 * Gets the node's type
 * @return Type as an enum
 */
blogator::parser::dom::NodeType Node::nodeType() const {
    return _type;
}

/**
 * Checks if node has a sibling before it
 * @return Previous sibling state
 */
bool Node::hasSiblingPrev() const {
    return ( _prev_sibling != nullptr );
}

/**
 * Checks if node has a sibling after it
 * @return Next sibling state
 */
bool Node::hasSiblingNext() const {
    return ( _next_sibling != nullptr );
}

/**
 * Checks if node has a parent
 * @return Has parent state
 */
bool Node::hasParent() const {
    return ( _parent != nullptr );
}

/**
 * Gets the parent node
 * @return parent node
 * @throws std::out_of_range if node has no parent
 */
const Node & Node::parentNode() const {
    if( _parent ) {
        return *_parent;
    }

    throw std::out_of_range( "Node has no parent." );
}

/**
 * Gets the node's previous sibling
 * @return Previous sibling
 */
const Node & Node::previousSibling() const {
    if( _prev_sibling ) {
        return *_prev_sibling;
    }

    throw std::out_of_range( "Node has no parent." );
}

/**
 * Gets the node's next sibling
 * @return
 */
const Node  & Node::nextSibling() const {
    if( _next_sibling ) {
        return *_next_sibling;
    }

    throw std::out_of_range( "Node has no parent." );
}

/**
 * Checks if node has a value
 * @return Node value state
 */
bool Node::hasValue() const {
    return false;
}

/**
 * Checks if node has children
 * @return Has children
 */
bool Node::hasChildNodes() const {
    return false;
}

/**
 * Checks node is same as reference node
 * @param node Node to check
 * @return Are same node
 */
bool Node::isSameNode( const std::unique_ptr<Node> &node ) const {
    if( node ) {
        return ( *this == *node );
    } else {
        return false;
    }
}