#include "Text.h"

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
 * @param str Text string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
Text::Text( DOMString_t str, node::Node * parent, node::Node * prev_sibling ) :
    node::CharacterData( NodeType::TEXT_NODE, std::move( str ), parent, prev_sibling )
{}

/**
 * Constructor
 * @param str Text string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
Text::Text(DOMString_t str, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling ) :
    node::CharacterData( NodeType::TEXT_NODE, std::move( str ), parent, prev_sibling, next_sibling )
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
 * @param type Node type
 * @param str Text string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
Text::Text( NodeType type, DOMString_t str, node::Node *parent, node::Node *prev_sibling ) :
    node::CharacterData( type, std::move( str ), parent, prev_sibling )
{}

/**
 * Constructor
 * @param type Node type
 * @param str Text string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
Text::Text( NodeType type, DOMString_t str, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling ) :
    node::CharacterData( type, std::move( str ), parent, prev_sibling, next_sibling )
{}

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
                auto sibling_it = this->getChildIterator( _next_sibling ); //can throw `failed_expectation`
                this->deleteData( offset, ( this->length() - offset ) );
                return dynamic_cast<Text *>( this->insertBefore( std::move( node ), *sibling_it ) );
            }

        } else {
            throw blogator::exception::failed_expectation(
                "[blogator::parser::dom::node::Text::splitText( " + std::to_string( offset ) + " )] "
                "Node is an orphan."
            );
        }

        //note: included 'live range' modifications found in the standard is not implemented here
        //      since this functionality is not required in blogator.
        //      (ref: https://dom.spec.whatwg.org/#concept-live-range)

    } catch( blogator::exception::failed_expectation &e ) {
        throw blogator::exception::failed_expectation(
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
        str += reinterpret_cast<node::Text *>( _next_sibling )->wholeText();
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