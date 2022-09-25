#include "Text.h"

#include "../../../unicode/ascii.h"
#include "../../../unicode/utf8.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param str Text string
 */
Text::Text( blogator::parser::dom::DOMString_t str ) :
    node::CharacterData( NodeType::TEXT_NODE, std::move( str ) )
{}

/**
 * Constructor
 * @param document Pointer to owner document
 * @param str Text string
 */
Text::Text( Document * document, DOMString_t str ) :
    node::CharacterData( document, NodeType::TEXT_NODE, std::move( str ) )
{}

/**
 * Constructor
 * @param type Node type
 * @param str Text string
 */
Text::Text( NodeType type, DOMString_t str )
    : CharacterData( type, std::move( str ) )
{}

/**
 * Constructor
 * @param document Pointer to owner document
 * @param type Node type
 * @param str Text string
 */
Text::Text( Document * document, NodeType type, DOMString_t str )
    : CharacterData( document, type, std::move( str ) )
{}

/**
 * [OVERRIDE] Shallow swaps nodes
 * @param rhs node to swap with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void Text::swap( Node &rhs ) {
    CharacterData::swap( rhs );
}

/**
 * Shallow swaps Text nodes inclusive of their attributes
 * @param rhs node::Text to swap with
 */
void Text::swap( Text &rhs ) {
    CharacterData::swap( static_cast<CharacterData &>( rhs ) );
}

/**
 * Split the Text node
 * @param offset Split offset
 * @return New Text node (or this if offset == 0 || offset == length)
 * @throws dom::exception::RangeError when node length is greater than the given offset
 * @throws blogator::exception::failed_expectation when the DOM tree structure is not as expected
 */
blogator::parser::dom::node::Text * Text::splitText( size_t offset ) {
    const auto length = this->length();

    if( offset == 0 ||  offset == length ) {
        return this; //EARLY RETURN

    } else if( offset > length ) {
        throw exception::RangeError(
            "[parser::dom::node::Text::splitText( " + std::to_string( offset ) + " )] "
            "Offset > length (" + std::to_string( length ) +  ")."
        );
    }

    const auto count = length - offset;
    const auto new_data = substringData( offset, count );
    auto node = std::make_unique<Text>( new_data );

    node->setParent( _parent );

    try {
        if( _parent != nullptr ) {
            if( _next_sibling == nullptr ) { //append at end
                this->deleteData( offset, ( this->length() - offset ) );
                return dynamic_cast<Text *>( _parent->appendChild( std::move( node ) ) );

            } else { //insert before next sibling
                auto sibling_it = this->getParentChildListIterator( _next_sibling ); //can throw `failed_expectation`
                this->deleteData( offset, ( this->length() - offset ) );
                return dynamic_cast<Text *>( this->insertBefore( std::move( node ), sibling_it->get() ) );
            }

        } else {
            throw FAILED_EXPECTATION_EXCEPTION(
                "[blogator::parser::dom::node::Text::splitText( " + std::to_string( offset ) + " )] "
                "Node is an orphan."
            );
        }

        //note: included 'live range' modifications found in the standard is not implemented here
        //      since this functionality is not required in blogator.
        //      (ref: https://dom.spec.whatwg.org/#concept-live-range)

    } catch( blogator::exception::failed_expectation &e ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::node::Text::splitText( " + std::to_string( offset ) + " )] "
            "Possible DOM tree corruption detected."
        );
    }
}

/**
 * Gets the concatenation of the data of and any contiguous Text nodes from this node
 * @return Concatenated text
 */
blogator::parser::dom::DOMString_t Text::wholeText() const {
    DOMString_t str = this->data();

    if( _next_sibling != nullptr && _next_sibling->nodeType() == NodeType::TEXT_NODE ) {
        str += dynamic_cast<node::Text *>( _next_sibling )->wholeText();
    }

    return str;
}

/**
 * Gets the node's name
 * @return Name string
 */
blogator::parser::dom::DOMString_t Text::nodeName() const {
    return U"#text";
}

/**
 * Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t Text::cloneNode( bool deep ) const {
    auto clone = std::make_unique<Text>( this->data() );

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
Node * Text::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "Text nodes cannot have children." );
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t Text::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "Text nodes do not have children." );
}

/**
 * [OVERRIDE] Outputs the node as UTF-8 formatted html into a stream
 * @param os Output stream
 */
void Text::toUTF8Stream( std::ostream &os ) const {
    for( auto c : this->data() ) {
        switch( c ) {
            case unicode::QUOTATION_MARK   : { os << "&quot;"; } break;
            case unicode::AMPERSAND        : { os << "&amp;";  } break;
            case unicode::LESS_THAN_SIGN   : { os << "&lt;";   } break;
            case unicode::GREATER_THAN_SIGN: { os << "&gt;";   } break;
            default                        : { blogator::unicode::utf8::convert( os, c ); } break;
        }
    }
}

/**
 * Shallow swaps Text nodes
 * @param lhs Text node
 * @param rhs Text node
 */
void blogator::parser::dom::node::swap( Text &lhs, Text &rhs ) {
    lhs.swap( rhs );
}
