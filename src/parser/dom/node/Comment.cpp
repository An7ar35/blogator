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
