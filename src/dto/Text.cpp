#include "Text.h"

/**
 * Equality operator
 * @param rhs Text DTO to compare to
 * @return result
 */
bool blogator::dto::Text::operator ==( const blogator::dto::Text &rhs ) const {
    if( _lines.size() != rhs._lines.size() )
        return false;

    auto it1 = _lines.cbegin();
    auto it2 = rhs._lines.cbegin();

    while( it1 != _lines.cend() && it2 !=_lines.cend() ){
        if( *it1 != *it2 )
            return false;
        ++it1; ++it2;
    }

    return true;
}

/**
 * Inequality operator
 * @param rhs Text DTO to compare to
 * @return result
 */
bool blogator::dto::Text::operator !=( const blogator::dto::Text &rhs ) const {
    return !( *this == rhs );
}