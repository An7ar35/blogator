#include "ascii.h"

/**
 * Check if character is an ASCII alphanumeric
 * @param c Character
 * @return Is ASCII alphanumeric
 */
bool blogator::parser::unicode::ascii::isalnum( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isalnum( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII alphabetic
 * @param c Character
 * @return Is ASCII alphabetic
 */
bool blogator::parser::unicode::ascii::isalpha( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isalpha( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII blank
 * @param c Character
 * @return Is ASCII blank
 */
bool blogator::parser::unicode::ascii::isblank( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isblank( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is a control character
 * @param c Character
 * @return Is ASCII control character
 */
bool blogator::parser::unicode::ascii::iscntrl( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return iscntrl( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII decimal digit
 * @param c Character
 * @return Is ASCII decimal digit
 */
bool blogator::parser::unicode::ascii::isdigit( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isdigit( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is hexadecimal digit
 * @param c Character
 * @return Is hexadecimal digit
 */
bool blogator::parser::unicode::ascii::isxdigit( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isxdigit( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character has graphical representation
 * @param c Character
 * @return Is graphical representation
 */
bool blogator::parser::unicode::ascii::isgraph( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isgraph( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Checks if character a space
 * @param c Character
 * @return Is ASCII space
 */
bool blogator::parser::unicode::ascii::isspace( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isspace( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is uppercase letter
 * @param c Character
 * @return Is uppercase ASCII letter
 */
bool blogator::parser::unicode::ascii::isupper( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isupper( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is lowercase letter
 * @param c Character
 * @return Is lowercase ASCII letter
 */
bool blogator::parser::unicode::ascii::islower( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return islower( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is printable
 * @param c Character
 * @return Is ASCII printable
 */
bool blogator::parser::unicode::ascii::isprint( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return isprint( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Check if character is an ASCII punctuation character
 * @param c Character
 * @return Is ASCII punctuation
 */
bool blogator::parser::unicode::ascii::ispunct( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return ispunct( static_cast<int>( c ) );
    else
        return false;
}

/**
 * Convert uppercase ASCII letter to lowercase
 * @param c Character
 * @return Lowercase ASCII letter or unchanged input if not ASCII
 */
uint32_t blogator::parser::unicode::ascii::tolower( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return tolower( static_cast<int>( c ) );
    return c;
}

/**
 * Convert lowercase ASCII letter to uppercase
 * @param c Character
 * @return Uppercase ASCII letter or unchanged input if not ASCII
 */
uint32_t blogator::parser::unicode::ascii::toupper( uint32_t c ) noexcept {
    if( c <= 0x7F )
        return toupper( static_cast<int>( c ) );
    return c;
}
