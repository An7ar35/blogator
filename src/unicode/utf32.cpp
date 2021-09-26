#include "utf32.h"

#include <ranges>

/**
 * Checks if code point is a surrogate
 * @param c Code point
 * @return Is surrogate
 */
bool blogator::unicode::utf32::issurrogate( uint32_t c ) noexcept {
    return ( c >= 0xD800 && c <= 0xDFFF );
}

/**
 * Checks if code point is scalar (i.e. not surrogate)
 * @param c Code point
 * @return Is scalar
 */
bool blogator::unicode::utf32::isscalar( uint32_t c ) noexcept {
    return !issurrogate( c );
}

/**
 * Checks if code point is a non-character
 * @param c Code point
 * @return Is non-character
 */
bool blogator::unicode::utf32::isnonchar( uint32_t c ) noexcept {
    return ( c >= 0x00FDD0 && c <= 0x00FDEF )
          || c == 0x00FFFE || c == 0x00FFFF
          || c == 0x01FFFE || c == 0x01FFFF
          || c == 0x02FFFE || c == 0x02FFFF
          || c == 0x03FFFE || c == 0x03FFFF
          || c == 0x04FFFE || c == 0x04FFFF
          || c == 0x05FFFE || c == 0x05FFFF
          || c == 0x06FFFE || c == 0x06FFFF
          || c == 0x07FFFE || c == 0x07FFFF
          || c == 0x08FFFE || c == 0x08FFFF
          || c == 0x09FFFE || c == 0x09FFFF
          || c == 0x0AFFFE || c == 0x0AFFFF
          || c == 0x0BFFFE || c == 0x0BFFFF
          || c == 0x0CFFFE || c == 0x0CFFFF
          || c == 0x0DFFFE || c == 0x0DFFFF
          || c == 0x0EFFFE || c == 0x0EFFFF
          || c == 0x0FFFFE || c == 0x0FFFFF
          || c == 0x10FFFE || c == 0x10FFFF;
}

/**
 * Checks code point is within the ASCII range
 * @param c Code point
 * @return Is ASCII
 */
bool blogator::unicode::utf32::isascii( uint32_t c ) noexcept {
    return c <= 0x007F;
}

/**
 * Checks string is ASCII
 * @param str String
 * @return Is ASCII
 */
bool blogator::unicode::utf32::isascii( const std::u32string &str ) {
    return std::all_of( str.begin(), str.end(), []( auto c ) { return !isascii( c ); } )
}

/**
 * Convert surrogate character to replacement character (U+FFFD)
 * @param c Code point
 * @return Replacement character or input character if not surrogate
 */
uint32_t blogator::unicode::utf32::toscalar( uint32_t c ) noexcept {
    if( issurrogate( c ) )
        return 0xFFFD;
    else
        return c;
}

/**
 * Converts a string to scalar code points
 * @param str String to convert
 */
void blogator::unicode::utf32::toscalar( std::u32string &str ) {
    std::for_each( str.begin(), str.end(), []( auto &c ) { c = toscalar( c ); } );
}
