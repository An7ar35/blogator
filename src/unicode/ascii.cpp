#include "ascii.h"

#include <cctype>

/**
 * Check if character is an ASCII alphanumeric
 * @param c Character
 * @return Is ASCII alphanumeric
 */
bool blogator::unicode::ascii::isalnum( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isalnum( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII alphabetic
 * @param c Character
 * @return Is ASCII alphabetic
 */
bool blogator::unicode::ascii::isalpha( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isalpha( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII blank
 * @param c Character
 * @return Is ASCII blank
 */
bool blogator::unicode::ascii::isblank( uint32_t c ) noexcept {
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
bool blogator::unicode::ascii::iscntrl( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::iscntrl( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII decimal digit
 * @param c Character
 * @return Is ASCII decimal digit
 */
bool blogator::unicode::ascii::isdigit( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isdigit( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is hexadecimal digit
 * @param c Character
 * @return Is hexadecimal digit
 */
bool blogator::unicode::ascii::isxdigit( uint32_t c ) noexcept {
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
bool blogator::unicode::ascii::isgraph( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isgraph( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Checks if character a space
 * @param c Character
 * @return Is ASCII space
 */
bool blogator::unicode::ascii::isspace( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::isspace( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is uppercase letter
 * @param c Character
 * @return Is uppercase ASCII letter
 */
bool blogator::unicode::ascii::isupper( uint32_t c ) noexcept {
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
bool blogator::unicode::ascii::islower( uint32_t c ) noexcept {
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
bool blogator::unicode::ascii::isprint( uint32_t c ) noexcept {
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
bool blogator::unicode::ascii::ispunct( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::ispunct( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Checks code point is an ASCII tab
 * @param c Code point
 * @return Is ASCII tab
 */
bool blogator::unicode::ascii::istab( uint32_t c ) noexcept {
    return ( c == 0x0009 );
}

/**
 * Checks code point is either LF or CR
 * @param c Code point
 * @return Is new line
 */
bool blogator::unicode::ascii::isnewline( uint32_t c ) noexcept {
    return ( c == 0x000A || c == 0x000D );
}

/**
 * Checks code point is upper hex digit (0-9 or A-F)
 * @param c Code point
 * @return Is upper hexadecimal digit
 */
bool blogator::unicode::ascii::isuxdigit( uint32_t c ) noexcept {
    return ( ascii::isdigit( c ) || ( c >= 0x41 && c <= 0x46 ) );
}

/**
 * Checks code point is lowe hex digit (0-9 or a-f)
 * @param c Code point
 * @return Is lower hexadecimal digit
 */
bool blogator::unicode::ascii::islxdigit( uint32_t c ) noexcept {
    return ( ascii::isdigit( c ) || ( c >= 0x61 && c <= 0x66 ) );
}

/**
 * Convert uppercase ASCII letter to lowercase
 * @param c Character
 * @return Lowercase ASCII letter or unchanged input if not ASCII
 */
uint32_t blogator::unicode::ascii::tolower( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::tolower( static_cast<int>( c ) );
    return c;
}

/**
 * Convert lowercase ASCII letter to uppercase
 * @param c Character
 * @return Uppercase ASCII letter or unchanged input if not ASCII
 */
uint32_t blogator::unicode::ascii::toupper( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return std::toupper( static_cast<int>( c ) );
    return c;
}