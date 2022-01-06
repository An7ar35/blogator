#include "CharacterData.h"

#include <utility>

#include "../exception/RangeError.h"

#include <sstream>

using           blogator::parser::dom::DOMString_t;
using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param type Node type
 * @param data Data string
 */
CharacterData::CharacterData( blogator::parser::dom::NodeType type, DOMString_t data ) :
    node::Node( type ),
    _data( std::move( data ) )
{}

/**
 * Constructor
 * @param type Node type
 * @param data Data string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
CharacterData::CharacterData( NodeType type, DOMString_t data, node::Node * parent, node::Node * prev_sibling ) :
    node::Node( type, parent, prev_sibling ),
    _data( std::move( data ) )
{}

/**
 * Constructor
 * @param type Node type
 * @param data Data string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
CharacterData::CharacterData( NodeType type, DOMString_t data, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling ) :
    node::Node( type, parent, prev_sibling, next_sibling ),
    _data( std::move( data ) )
{}

/**
 * Copy-constructor
 * @param node Node to copy
 */
CharacterData::CharacterData( const CharacterData &node ) :
    node::Node( node ),
    _data( node._data )
{}

/**
 * Move-constructor
 * @param node Node to move
 */
CharacterData::CharacterData( CharacterData &&node )  noexcept :
    node::Node( std::move( node ) ),
    _data( std::move( node._data ) )
{}

/**
 * Copy-operator
 * @param node Node to copy
 * @return Copy
 */
CharacterData & CharacterData::operator =( const CharacterData & node ) {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( node );
        this->_data = node._data;
    }

    return *this;
}

/**
 * Move-operator
 * @param node Node to move
 * @return Moved node
 */
CharacterData & CharacterData::operator =( CharacterData && node ) noexcept {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<node::Node &&>( node );
        this->_data = std::move( node._data );
    }

    return *this;
}

/**
 * Gets access to data string
 * @return Data string
 */
DOMString_t & CharacterData::data() {
    return _data;
}

/**
 * Gets access to data string
 * @return Data string
 */
const DOMString_t &CharacterData::data() const {
    return _data;
}

/**
 * Length of the data
 * @return Character length of data
 */
size_t CharacterData::length() const {
    return _data.length();
}

/**
 * Gets a substring of the data
 * @param offset Starting offset
 * @param length Length of substring
 * @return Substring
 * @throws RangeError when offset if greater than the available size of the data
 */
DOMString_t CharacterData::substringData( size_t offset, size_t length ) const {
    if( offset > _data.length() ) {
        throw blogator::parser::dom::exception::RangeError( "offset > available length" );
    }

    return _data.substr( offset, length );
}


/**
 * Appends string to data
 * @param data String to append
 * @return Concatenated data
 * @throws RangeError when offset if greater than the available size of the data
 */
const DOMString_t & CharacterData::appendData( const DOMString_t & data ) {
    try {
        return this->replaceData( _data.length(), 0, data );

    } catch( exception::RangeError &e ) {
        throw blogator::parser::dom::exception::RangeError(
            "[parser::dom::node::CharacterData::appendData( const DOMString_t & )] "
            "Failed to append data."
        );
    }
}

/**
 * Inserts data into the string
 * @param offset Offset to insert at
 * @param data Data to insert
 * @return Modified data
 * @throws RangeError when offset if greater than the available size of the data
 */
const DOMString_t &CharacterData::insertData( size_t offset, const DOMString_t & data ) {
    try {
        return this->replaceData( offset, 0, data );

    } catch( exception::RangeError &e ) {
        std::stringstream err_ss;
        err_ss << "[parser::dom::node::CharacterData::insertData( " << offset << ", const DOMString_t & )] "
               << "Failed to insert data.";
        throw blogator::parser::dom::exception::RangeError( err_ss.str() );
    }
}

/**
 * Removes data from the string
 * @param offset Offset to remove from
 * @param count Length to remove
 * @return Modified data
 * @throws RangeError when offset if greater than the available size of the data
 */
const DOMString_t & CharacterData::deleteData( size_t offset, size_t count ) {
    try {
        return this->replaceData( offset, count, U"" );

    } catch( exception::RangeError &e ) {
        std::stringstream err_ss;
        err_ss << "[parser::dom::node::CharacterData::deleteData( " << offset << ", " << count << " )] "
               << "Failed to delete data.";
        throw blogator::parser::dom::exception::RangeError( err_ss.str() );
    }
}

/**
 * Replaces data in the string
 * @param offset Offset to replace from
 * @param count Length or replacement
 * @param replacement_data Source replacement data
 * @return Modified data
 * @throws RangeError when offset if greater than the available size of the data
 */
const DOMString_t & CharacterData::replaceData( size_t offset, size_t count, const DOMString_t & replacement_data ) {
    //ref: https://dom.spec.whatwg.org/#concept-cd-replace

    const auto length = _data.length();

    if( offset > length ) {
        std::stringstream err_ss;
        err_ss << "[parser::dom::node::CharacterData::replaceData( " << offset << ", " << count << ", DOMString_t )] "
               << "Offset > length (" << length << ").";
        throw blogator::parser::dom::exception::RangeError( err_ss.str() );
    }

    if( ( offset + count ) > length ) {
        count = length - offset;
    }

    //note: included queuing of 'mutation record' found in standard is not implemented here
    //      since this functionality is not required in blogator
    //      (ref: https://dom.spec.whatwg.org/#queue-a-mutation-record)

    _data.insert( offset, replacement_data );

    const auto delete_offset = offset + replacement_data.length();

    _data.erase( delete_offset, count );

    //note: included 'live range' modifications found in the standard is not implemented here
    //      since this functionality is not required in blogator.
    //      (ref: https://dom.spec.whatwg.org/#concept-live-range)

    return _data;
}

/**
 * Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t CharacterData::cloneNode( bool deep ) const {
    auto clone = std::make_unique<CharacterData>( _node_type, _data );

    if( deep ) {
        for( const auto & child : _children ) {
            clone->appendChild( child->cloneNode( deep ) );
        }
    }

    return std::move( clone );
}