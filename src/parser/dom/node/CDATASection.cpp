#include "CDATASection.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param str Content string
 */
CDATASection::CDATASection( DOMString_t str ) :
    Text( NodeType::CDATA_SECTION_NODE, std::move( str ) )
{}

/**
 * Constructor
 * @param document Pointer to owner document
 * @param str Content string
 */
CDATASection::CDATASection( Document * document, blogator::parser::dom::DOMString_t str ) :
    Text( document, NodeType::CDATA_SECTION_NODE, std::move( str ) )
{}

/**
 * [OVERRRIDE] Shallow swaps nodes
 * @param rhs node to swap with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void CDATASection::swap( Node &rhs ) {
    Text::swap( rhs );
}

/**
 * Shallow swaps CDATASection nodes
 * @param rhs node::CDATASection to swap with
 */
void CDATASection::swap( CDATASection &rhs ) {
    CharacterData::swap( dynamic_cast<CharacterData &>( rhs ) );
}

/**
 * Gets the node's name
 * @return Name string
 */
blogator::parser::dom::DOMString_t CDATASection::nodeName() const {
    return U"#cdata-section";
}

/**
 * Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t CDATASection::cloneNode( bool deep ) const {
    auto clone = std::make_unique<CDATASection>( this->data() );

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
Node * CDATASection::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "CDATASection nodes cannot have children." );
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t CDATASection::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "CDATASection nodes do not have children." );
}

/**
 * Shallow swaps CDATASection nodes
 * @param lhs CDATASection node
 * @param rhs CDATASection node
 */
void blogator::parser::dom::node::swap( CDATASection &lhs, CDATASection &rhs ) {
    lhs.swap( rhs );
}
