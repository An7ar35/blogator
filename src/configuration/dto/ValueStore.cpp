#include "ValueStore.h"

using namespace blogator::configuration;

/**
 * Move-constructor
 * @param vs ValueStore to move over
 */
ValueStore::ValueStore( ValueStore &&vs ) noexcept :
    _is_array( vs._is_array )
{
    for( auto & ptr : vs._store ) {
        if( ptr ) {
            _store.emplace_back( std::move( ptr ) );
        }
    }
}

/**
 * Copy-constructor
 * @param vs ValueStore to copy over
 */
ValueStore::ValueStore( const ValueStore &vs ) :
    _is_array( vs._is_array )
{
    for( const auto & ptr : vs._store ) {
        if( ptr ) {
            _store.emplace_back( std::make_unique<Value>( *ptr ) );
        }
    }
}

/**
 * Move-operator
 * @param rhs ValueStore to move over
 * @return Moved object
 */
ValueStore & ValueStore::operator =( ValueStore &&rhs ) noexcept {
    _is_array = rhs._is_array;

    for( auto & ptr : rhs._store ) {
        if( ptr ) {
            _store.emplace_back( std::move( ptr ) );
        }
    }

    return *this;
}

/**
 * Copy-operator
 * @param rhs ValueStore to copy over
 * @return Moved object
 */
ValueStore & ValueStore::operator =( const ValueStore &rhs ) {
    _is_array = rhs._is_array;

    for( const auto & ptr : rhs._store ) {
        if( ptr ) {
            _store.emplace_back( std::make_unique<Value>( *ptr ) );
        }
    }

    return *this;
}

/**
 * Gets access to internal value array
 * @return Value array
 */
ValueStore::ValueArray_t & ValueStore::operator ()() {
    return _store;
}

/**
 * Gets access to internal value array
 * @return Value array
 */
const ValueStore::ValueArray_t &ValueStore::operator ()() const {
    return _store;
}

/**
 * Equality operator
 * @param rhs Store to compare with
 * @return Equality state
 */
bool ValueStore::operator ==( const ValueStore & rhs ) const {
    if( this->size() == rhs.size() ) {
        for( auto it1 = this->_store.cbegin(), it2 = rhs._store.cbegin(); it1 < this->_store.cend() && it2 < rhs._store.cend(); ++it1, ++it2 ) {
            if( ( (*it1).operator bool() != (*it2).operator bool() ) || //one has a value, the other not
                ( *it1 && *it2 && ( **it1 != **it2 ) ) )                //both have values but not the same
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

/**
 * Inequality state
 * @param store Store to compare with
 * @return Inequality state
 */
bool ValueStore::operator !=( const ValueStore & rhs ) const {
    return !( *this == rhs );
}

/**
 * Gets access to internal value array
 * @return Value array
 */
ValueStore::ValueArray_t & ValueStore::values() {
    return _store;
}

/**
 * Emplace back a Value in the internal value array store
 * @param ptr Value to add
 */
void ValueStore::emplace_back( std::unique_ptr<Value> && ptr ) {
    _store.emplace_back( std::move( ptr ) );
}

/**
 * Sets the array flag for the store
 * @param flag Flag value (default: true)
 */
void ValueStore::setArrayFlag( bool flag ) {
    _is_array = flag;
}

/**
 * Clears all values from the store
 */
void ValueStore::clear() {
    _store.clear();
}

/**
 * Checks if store holds an array
 * @return Array state (>1 values or flag set)
 */
bool ValueStore::isArray() const {
    return ( _is_array || _store.size() > 1 );
}

/**
 * Gets the number of values in the store
 * @return Size of store
 */
size_t ValueStore::size() const {
    return _store.size();
}

/**
 * Gets the empty state of the store
 * @return Empty state
 */
bool ValueStore::empty() const {
    return _store.empty();
}

/**
 * Output stream operator
 * @param os Output stream
 * @param node configuration:ValueStore to print out
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const ValueStore &vs ) {
    if( vs.empty() ) {
        os << ( vs.isArray() ? "[]" : "" );

    } else {
        if( vs.isArray() ) {
            os << "[ ";

            for( size_t i = 0; i < vs._store.size(); ++i ) {
                const auto & ptr = vs._store.at( i );

                if( ptr ) {
                    os << *ptr;
                } else {
                    os << "NULL";
                }

                if( i < (  vs._store.size() - 1 ) ) { //not last item
                    os << ", ";
                }
            }

            os << " ]";

        } else {
            if( vs._store.front() ) {
                os << *vs._store.front();
            } else {
                os << "NULL";
            }
        }
    }

    return os;
}