#include "TextPos.h"

using namespace blogator::parser;

/**
 * Constructor
 */
TextPos::TextPos() :
    line( 1 ),
    col( 1 )
{}

/**
 * Constructor
 * @param line Line
 * @param col Column
 */
TextPos::TextPos( size_t line, size_t col ) :
    line( line ),
    col( col )
{}

/**
 * Equal operator
 * @param rhs TextPos to compare to
 * @return Equal state
 */
bool TextPos::operator ==( const TextPos &rhs ) const {
    return line == rhs.line
        && col  == rhs.col;
}

/**
 * Not equal operator
 * @param rhs TextPos to compare to
 * @return Not equal state
 */
bool TextPos::operator !=( const TextPos &rhs ) const {
    return !( *this == rhs );
}

/**
 * Smaller-than operator
 * @param rhs TextPos to compare to
 * @return Smaller-than state
 */
bool TextPos::operator <( const TextPos &rhs ) const {
    if( line == rhs.line ) {
        return ( col < rhs.col );
    } else {
        return ( line < rhs.line );
    }
}

/**
 * Greater-than operator
 * @param rhs TextPos to compare to
 * @return Greater-than state
 */
bool TextPos::operator >( const TextPos &rhs ) const {
    if( line == rhs.line ) {
        return ( col > rhs.col );
    } else {
        return ( line > rhs.line );
    }
}

/**
 * Smaller-equal operator
 * @param rhs TextPos to compare to
 * @return Smaller-equal state
 */
bool TextPos::operator <=( const TextPos &rhs ) const {
    if( line == rhs.line ) {
        return ( col <= rhs.col );
    } else {
        return ( line < rhs.line );
    }
}

/**
 * Greater-equal operator
 * @param rhs TextPos to compare to
 * @return Greater-equal state
 */
bool TextPos::operator >=( const TextPos &rhs ) const {
    if( line == rhs.line ) {
        return ( col >= rhs.col );
    } else {
        return ( line > rhs.line );
    }
}