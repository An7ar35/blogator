#include "HTML.h"

#include <regex>

/**
 * Equality operator
 * @param rhs HTML DTO to compare to
 * @return result
 */
bool blogator::dto::HTML::operator ==( const blogator::dto::HTML & rhs ) const {
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
 * @param rhs HTML DTO to compare to
 * @return result
 */
bool blogator::dto::HTML::operator !=( const blogator::dto::HTML & rhs ) const {
    return !( *this == rhs );
}

/**
 * Finds the line index of a tag's first occurrence
 * @param tag Tag to find
 * @return Line index of the tag
 * @throws std::invalid_argument when the tag is empty (i.e.: "")
 * @throws std::out_of_range when the tag cannot be found on any lines
 */
size_t blogator::dto::HTML::findLineOfTag( const std::string & tag ) {
    if( tag.empty() )
        throw std::invalid_argument( "No tag was given." );

    for( auto i = 0; i < _lines.size(); ++i ) {
        auto pos = _lines.at( i ).find( tag );
        if( pos != std::string::npos )
            return i;
    }

    throw std::out_of_range( "Tag \"" + tag + "\" could not be found in dto::HTML." );
}