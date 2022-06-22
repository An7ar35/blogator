#include "ascii.h"

#include <cctype>
#include <algorithm>

/**
 * Checks if character a space
 * @param c Character
 * @return Is ASCII space (0x20)
 */
bool blogator::unicode::ascii::isspace( char32_t c ) noexcept {
    return ( c == unicode::SPACE );
}

/**
 * Checks if character is a whitespace
 * @param c Character
 * @return Is ASCII whitespace (TAB, LF, FF, CR, SPACE)
 */
bool blogator::unicode::ascii::iswspace( char32_t c ) noexcept {
    return ( c == unicode::TAB || unicode::ascii::isfeed( c ) || c == unicode::CR || c == unicode::SPACE );
}

/**
 * Checks if character is a either LF or FF
 * @param c Character
 * @return Is feed character (0x0A || 0x0C)
 */
bool blogator::unicode::ascii::isfeed( char32_t c ) noexcept {
    return ( c == unicode::LF || c == unicode::FF );
}

/**
 * Checks code point is an ASCII tab
 * @param c Code point
 * @return Is ASCII tab (0x09)
 */
bool blogator::unicode::ascii::istab( char32_t c ) noexcept {
    return ( c == unicode::TAB );
}

/**
 * Check if character is an ASCII alphabetic
 * @param c Character
 * @return Is ASCII alphabetic
 */
bool blogator::unicode::ascii::isalpha( char32_t c ) noexcept {
    return (char32_t)( ( 'a' - 1 - ( c | 32 ) ) & ( ( c | 32 ) - ( 'z' + 1 ) ) ) >> 31;
}

/**
 * Check if character is an ASCII decimal digit
 * @param c Character
 * @return Is ASCII decimal digit
 */
bool blogator::unicode::ascii::isdigit( char32_t c ) noexcept {
    return (char32_t)( ( '0' - 1 - c ) & ( c - ( '9' + 1 ) ) ) >> 31;
}

/**
 * Check if character is an ASCII alphanumeric
 * @param c Character
 * @return Is ASCII alphanumeric
 */
bool blogator::unicode::ascii::isalnum( char32_t c ) noexcept {
    return ( ascii::isdigit( c ) || ascii::isalpha( c ) );
}



/**
 * Check if character is an ASCII blank
 * @param c Character
 * @return Is ASCII blank
 */
bool blogator::unicode::ascii::isblank( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isblank( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is a control character
 * @param c Character
 * @return Is ASCII control character
 */
bool blogator::unicode::ascii::iscntrl( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::iscntrl( static_cast<int>( c ) );
    else
        return false;
}



/**
 * Check if character is hexadecimal digit
 * @param c Character
 * @return Is hexadecimal digit
 */
bool blogator::unicode::ascii::isxdigit( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isxdigit( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character has graphical representation
 * @param c Character
 * @return Is graphical representation
 */
bool blogator::unicode::ascii::isgraph( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isgraph( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is uppercase letter
 * @param c Character
 * @return Is uppercase ASCII letter
 */
bool blogator::unicode::ascii::isupper( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isupper( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is lowercase letter
 * @param c Character
 * @return Is lowercase ASCII letter
 */
bool blogator::unicode::ascii::islower( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::islower( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is printable
 * @param c Character
 * @return Is ASCII printable
 */
bool blogator::unicode::ascii::isprint( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isprint( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII punctuation character
 * @param c Character
 * @return Is ASCII punctuation
 */
bool blogator::unicode::ascii::ispunct( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::ispunct( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Checks code point is either LF or CR
 * @param c Code point
 * @return Is new line (0x0A || 0x0D)
 */
bool blogator::unicode::ascii::isnewline( char32_t c ) noexcept {
    return ( c == LF || c == CR );
}

/**
 * Checks code point is upper hex digit (0-9 or A-F)
 * @param c Code point
 * @return Is upper hexadecimal digit
 */
bool blogator::unicode::ascii::isuxdigit( char32_t c ) noexcept {
    return ( ascii::isdigit( c ) || ( c >= 0x41 && c <= 0x46 ) );
}

/**
 * Checks code point is lowe hex digit (0-9 or a-f)
 * @param c Code point
 * @return Is lower hexadecimal digit
 */
bool blogator::unicode::ascii::islxdigit( char32_t c ) noexcept {
    return ( ascii::isdigit( c ) || ( c >= 0x61 && c <= 0x66 ) );
}

/**
 * Convert uppercase ASCII letter to lowercase
 * @param c Character
 * @return Lowercase ASCII letter or unchanged input if not ASCII
 */
char32_t blogator::unicode::ascii::tolower( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::tolower( static_cast<int>( c ) );
    return c;
}

/**
 * Convert lowercase ASCII letter to uppercase
 * @param c Character
 * @return Uppercase ASCII letter or unchanged input if not ASCII
 */
char32_t blogator::unicode::ascii::toupper( char32_t c ) noexcept {
    if( c <= 0x7F )
        return std::toupper( static_cast<int>( c ) );
    return c;
}

/**
 * Converts a string's ASCII alpha code points into lowercase
 * @param str String to convert
 * @return Ref to converted string
 */
std::u32string & blogator::unicode::ascii::tolower( std::u32string &str ) {
    std::for_each( str.begin(), str.end(), []( auto & c ) { c = ascii::tolower( c ); } );
    return str;
}

/**
 * Converts a string's ASCII alpha code points into lowercase
 * @param str String to convert
 * @return New string
 */
std::u32string blogator::unicode::ascii::tolower( const std::u32string &str ) {
    auto copy = std::u32string( str );
    ascii::tolower( copy );
    return copy;
}

/**
 * Converts a string's ASCII alpha code points into uppercase
 * @param str String to convert
 * @return Ref to converted string
 */
std::u32string & blogator::unicode::ascii::toupper( std::u32string &str ) {
    std::for_each( str.begin(), str.end(), []( auto & c ) { c = ascii::toupper( c ); } );
    return str;
}

/**
 * Converts a string's ASCII alpha code points into uppercase
 * @param str String to convert
 * @return New string
 */
std::u32string blogator::unicode::ascii::toupper( const std::u32string &str ) {
    auto copy = std::u32string( str );
    ascii::toupper( copy );
    return copy;
}