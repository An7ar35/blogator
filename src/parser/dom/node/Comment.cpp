#include "Comment.h"

#include "../../../unicode/utf8.h"

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
 * @param document Pointer to owner document
 * @param str Comment string
 */
Comment::Comment( Document * document, DOMString_t str ) :
    node::CharacterData( document, NodeType::COMMENT_NODE, std::move( str ) )
{}

/**
 * [OVERRRIDE] Shallow swaps nodes
 * @param rhs node to swap with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void Comment::swap( Node &rhs ) {
    CharacterData::swap( rhs );
}

/**
 * Shallow swaps Comment nodes
 * @param rhs node::Comment to swap with
 */
void Comment::swap( Comment &rhs ) {
    CharacterData::swap( static_cast<CharacterData &>( rhs ) );
}

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

/**
 * [OVERRIDE] Outputs the node as UTF-8 formatted html into a stream
 * @param os Output stream
 */
void Comment::toUTF8Stream( std::ostream &os ) const {
    os << "<!-- ";
    blogator::unicode::utf8::convert( os, this->data() );
    os << " -->";
}

/**
 * Shallow swaps Comment nodes
 * @param lhs Comment node
 * @param rhs Comment node
 */
void blogator::parser::dom::node::swap( Comment &lhs, Comment &rhs ) {
    lhs.swap( rhs );
}
