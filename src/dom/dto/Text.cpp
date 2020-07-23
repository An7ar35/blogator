#include "Text.h"

/**
 * Equality operator
 * @param rhs Text DTO to compare to
 * @return result
 */
bool blogator::dom::dto::Text::operator ==( const blogator::dom::dto::Text &rhs ) const {
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
bool blogator::dom::dto::Text::operator !=( const blogator::dom::dto::Text &rhs ) const {
    return !( *this == rhs );
}

/**
 * Helper method to check if an iterator is at the end of a Text line
 * @param it   Character iterator
 * @param line Text line
 * @return EOL state
 */
bool blogator::dom::dto::Text::isEOL( const Text::Line_t::const_iterator &it, const Text::Line_t &line ) {
    return it == line.cend();
}
