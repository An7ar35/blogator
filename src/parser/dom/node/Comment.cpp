#include "Comment.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param str Comment string
 */
Comment::Comment( DOMString_t str ) :
    node::CharacterData( NodeType::COMMENT_NODE, std::move( str ) )
{}

/**
 * Constructor
 * @param str Comment string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
Comment::Comment( DOMString_t str, node::Node * parent, node::Node * prev_sibling ) :
    node::CharacterData( NodeType::COMMENT_NODE, std::move( str ), parent, prev_sibling )
{}

/**
 * Constructor
 * @param str Comment string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
Comment::Comment( DOMString_t str, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling ) :
    node::CharacterData( NodeType::COMMENT_NODE, std::move( str ), parent, prev_sibling, next_sibling )
{}

/**
 * Gets the node's name
 * @return Name string
 */
blogator::parser::dom::DOMString_t Comment::nodeName() const {
    return U"#comment";
}

/**
 * Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t Comment::cloneNode( bool deep ) const {
    auto clone = std::make_unique<Comment>( this->data() );

    if( deep ) {
        for( const auto & child : _children ) {
            clone->appendChild( child->cloneNode( deep ) );
        }
    }

    return std::move( clone );
}

/**
 * [OVERRRIDE] Insert node before a child
 * @param node Node to insert
 * @param child Pointer to child (nullptr if append at end)
 * @return Pointer to inserted child
 * @throws DOMException when insertion breaks DOM tree validity
 */
Node * Comment::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "Comment nodes cannot have children." );
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t Comment::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "Comment nodes do not have children." );
}