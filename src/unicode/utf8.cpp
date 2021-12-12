#include "utf8.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <codecvt>

const char HEX_TABLE[16] = { '0', '1', '2', '3', '4',
                             '5', '6', '7', '8', '9',
                             'A', 'B', 'C', 'D', 'E',
                             'F' };

/**
 * Gets the number of bytes a UTF-8 encoded codepoint has
 * @param first First byte of the UTF-8 code point sequence
 * @return Number of bytes to expect (0 if not valid)
 */
size_t blogator::unicode::utf8::bytelength( uint8_t byte ) {
    if     ( byte < 0x80 ) { return 1; } /* (0xxxxxxx) 0000 0000 -> 0111 1111 */
    else if( byte < 0xC0 ) { return 0; } /* INVALID */
    else if( byte < 0xE0 ) { return 2; } /* (110xxxxx) 1000 0000 -> 1101 1111 */
    else if( byte < 0xF0 ) { return 3; } /* (1110xxxx) 1110 0000 -> 1110 1111 */
    else if( byte < 0xF8 ) { return 4; } /* (11110xxx) 1111 0000 -> 1111 0111 */
    else                   { return 0; } /* INVALID */
}

/**
 * Convert a UTF-8 sequence into a UTF-32 code point
 * @param sequence UTF8 sequence
 * @param ln Size of sequence in bytes
 * @return UTF32 code point
 * @throws std::invalid_argument when byte length is invalid
 */
uint32_t blogator::unicode::utf8::toU32( const uint8_t sequence[4], size_t ln ) {
    switch( ln ) {
        case 1: return toU32( sequence[0] );
        case 2: return toU32( sequence[0], sequence[1] );
        case 3: return toU32( sequence[0], sequence[1], sequence[2] );
        case 4: return toU32( sequence[0], sequence[1], sequence[2], sequence[3] );
        default: throw std::invalid_argument( "Number of bytes given not between 1-4 inclusive." );
    }
}

/**
 * Converts a UTF-8 sequence int a UTF-32 code point
 * @param u8_byte1 1st byte of a 1-byte sequence
 * @return UTF-32 code point
 */
uint32_t blogator::unicode::utf8::toU32( uint8_t u8_byte1 ) noexcept {
    return static_cast<uint32_t>( u8_byte1 );
}

/**
 * Converts a UTF-8 sequence int a UTF-32 code point
 * @param u8_byte1 1st byte of a 2-byte sequence
 * @param u8_byte2 2nd byte of a 2-byte sequence
 * @return UTF-32 code point
 */
uint32_t blogator::unicode::utf8::toU32( uint8_t u8_byte1, uint8_t u8_byte2 ) noexcept {
    return (uint32_t)( ( u8_byte1 & 0x1F ) << 6  ) | ( u8_byte2  & 0x3F );;
}

/**
 * Converts a UTF-8 sequence int a UTF-32 code point
 * @param u8_byte1 1st byte of a 3-byte sequence
 * @param u8_byte2 2nd byte of a 3-byte sequence
 * @param u8_byte3 3rd byte of a 3-byte sequence
 * @return UTF-32 code point
 */
uint32_t blogator::unicode::utf8::toU32( uint8_t u8_byte1, uint8_t u8_byte2, uint8_t u8_byte3 ) noexcept {
    return (uint32_t)( ( u8_byte1 & 0x0F ) << 12 ) | ( ( u8_byte2 & 0x3F ) << 6 ) | ( u8_byte3 & 0x3F );;
}

/**
 * Converts a UTF-8 sequence int a UTF-32 code point
 * @param u8_byte1 1st byte of a 4-byte sequence
 * @param u8_byte2 2nd byte of a 4-byte sequence
 * @param u8_byte3 3rd byte of a 4-byte sequence
 * @param u8_byte4 4th byte of a 4-byte sequence
 * @return UTF-32 code point
 */
uint32_t blogator::unicode::utf8::toU32( uint8_t u8_byte1, uint8_t u8_byte2, uint8_t u8_byte3, uint8_t u8_byte4 ) noexcept {
    return (uint32_t)( ( u8_byte1 & 0x07 ) << 18 ) | ( ( u8_byte2 & 0x3F ) << 12 ) | ( ( u8_byte3 & 0x3F ) << 6 ) | ( u8_byte4 & 0x3F );
}

/**
 * Converts a unicode integer value into its hexadecimal representation
 * @param val Integer value to convert
 * @param prefix Hex code prefix (default="\\u")
 * @return Unicode hex string
 */
std::string blogator::unicode::utf8::toxunicode( uint8_t val, const std::string &prefix ) {
    char arr[2] = { 0, 0 };

    arr[1] = HEX_TABLE[ (val % 16) ];
    val /= 16;
    arr[0] = HEX_TABLE[ val ];

    return std::string( prefix + arr );
}

/**
 * Converts a unicode integer value into its hexadecimal representation
 * @param val Integer value to convert
 * @param prefix Hex code prefix (default="\\u")
 * @return Unicode hex string
 */
std::string blogator::unicode::utf8::toxunicode( uint16_t val, const std::string &prefix ) {
    char arr[4] = { 0, 0, 0, 0 };

    for( int i = 3; i > 0; --i ) {
        arr[i] = HEX_TABLE[ ( val % 16 ) ];
        val /= 16;
    }

    arr[0] = HEX_TABLE[ val ];

    return std::string( prefix + arr );
}

/**
 * Converts a unicode integer value into its hexadecimal representation
 * @param val Integer value to convert
 * @param prefix Hex code prefix (default="\\u")
 * @return Unicode hex string
 */
std::string blogator::unicode::utf8::toxunicode( uint32_t val, const std::string& prefix ) {
    char arr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for( int i = 7; i > 0; --i ) {
        arr[i] = HEX_TABLE[ (val % 16) ];
        val /= 16;
    }

    arr[0] = HEX_TABLE[ val ];

    return std::string( prefix + arr );
}

/**
 * Converts and outputs a UTF32 string to a UTF8 stream
 * @param stream UTF8 stream output
 * @param str UTF32 string
 * @return Output stream
 */
std::ostream & blogator::unicode::utf8::convert( std::ostream &stream, const std::u32string &str ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    stream << converter.to_bytes( str );
    return stream;
}

/**
 * Converts and outputs a UTF32 code point to a UTF8 stream
 * @param stream UTF8 stream output
 * @param str UTF32 code point
 * @return Output stream
 */
std::ostream & blogator::unicode::utf8::convert( std::ostream &stream, char32_t c ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    stream << converter.to_bytes( c );
    return stream;
}

/**
 * Converts a UTF32 string into UTF8
 * @param str UFT32 string
 * @return UFT8 string
 */
std::string blogator::unicode::utf8::convert( const std::u32string &str ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes( str );
}