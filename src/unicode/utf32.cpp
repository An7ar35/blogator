#include "utf32.h"

#include <ranges>
#include <locale>
#include <codecvt>
#include <vector>

/**
 * Joins 4 bytes into a UTF32 code unit
 * @param byte1 First most significant byte (XXXX 0000 0000 0000)
 * @param byte2 Second byte (0000 XXXX 0000 0000)
 * @param byte3 Third byte  (0000 0000 XXXX 0000)
 * @param byte4 Fourth byte (0000 0000 0000 XXXX)
 * @return U32 code unit
 */
uint32_t blogator::unicode::utf32::join( uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4 ) noexcept {
    return ( (uint32_t) byte1 << 24 ) + ( (uint32_t) byte2 << 16 ) + ( (uint32_t) byte3 << 8 ) + ( (uint32_t) byte4 );
}

/**
 * Checks if code point is a surrogate
 * @param c Unicode code point
 * @return Is surrogate
 */
bool blogator::unicode::utf32::issurrogate( uint32_t c ) noexcept {
    return ( c >= 0xD800 && c <= 0xDFFF );
}

/**
 * Checks if code point is scalar (i.e. not surrogate)
 * @param c Unicode code point
 * @return Is scalar
 */
bool blogator::unicode::utf32::isscalar( uint32_t c ) noexcept {
    return !issurrogate( c );
}

/**
 * Checks if code point is a non-character
 * @param c Unicode code point
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
 * @param c Unicode code point
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
    return std::all_of( str.begin(), str.end(), []( auto c ) { return !isascii( c ); } );
}

/**
 * Check if character is a control character
 * @param c Unicode code point
 * @return Is control character (0x00->0x1F || 0x7F->0x9F)
 */
bool blogator::unicode::utf32::iscntrl( uint32_t c ) noexcept {
    return ( c <= 0x1F || ( c >= 0x7F && c <= 0x9F ) );
}

/**
 * Convert surrogate character to replacement character (U+FFFD)
 * @param c Unicode code point
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

/**
 * Converts a UTF8 string into UTF32
 * @param u8str UTF8 string
 * @return UTF32 string
 */
std::u32string blogator::unicode::utf32::convert( const std::string &u8str ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.from_bytes( u8str );
}

/**
 * Converts a unicode integer value into its hexadecimal representation
 * @param val Integer value to convert
 * @param prefix Hex code prefix (default="\\u")
 * @return Unicode hex string
 */
std::u32string blogator::unicode::utf32::toxunicode( uint32_t val, const std::u32string &prefix ) {
    static const uint32_t hex[] = { U'0', U'1', U'2', U'3', U'4', U'5', U'6', U'7', U'8', U'9',
                                    U'A', U'B', U'C', U'D', U'E', U'F' };

    std::vector<uint32_t> out_buffer;
    std::vector<uint32_t> hex_buffer;

    out_buffer.insert( out_buffer.begin(), prefix.cbegin(), prefix.cend() );

    while( val >= 16 ) {
        uint32_t remainder = val % 16;
        val = ( val - remainder ) / 16;
        hex_buffer.emplace_back( remainder );
    }

    hex_buffer.emplace_back( val );

    if( hex_buffer.size() < 4 ) {
        for( auto i = 0; i < 4 - hex_buffer.size(); ++i ) {
            out_buffer.emplace_back( '0' );
        }
    }

    std::for_each( hex_buffer.crbegin(),
                   hex_buffer.crend(),
                   [&]( const auto & h ) { out_buffer.emplace_back( hex[h] ); }
    );

    return { out_buffer.begin(), out_buffer.end() };
}