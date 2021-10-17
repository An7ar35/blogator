#include "utf8.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>

/**
 * Gets the number of bytes a utf8 encoded codepoint has
 * @param first First byte of the utf8 code point sequence
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
 * Convert a UTF8 sequence into a UTF32 code point
 * @param sequence UTF8 sequence
 * @param ln Size of sequence in bytes
 * @return UTF32 code point
 */
uint32_t blogator::unicode::utf8::toU32( const uint8_t sequence[4], size_t ln ) {
    switch( ln ) {
        case 1: return static_cast<uint32_t>( sequence[0] );
        case 2: return (uint32_t)( ( sequence[0] & 0x1F ) << 6  ) | ( sequence[1]  & 0x3F );
        case 3: return (uint32_t)( ( sequence[0] & 0x0F ) << 12 ) | ( ( sequence[1] & 0x3F ) << 6 ) | ( sequence[2] & 0x3F );
        case 4: return (uint32_t)( ( sequence[0] & 0x07 ) << 18 ) | ( ( sequence[1] & 0x3F ) << 12 ) | ( ( sequence[2] & 0x3F ) << 6 ) | ( sequence[3] & 0x3F );
        default: throw std::invalid_argument( "Number of bytes given not between 1-4 inclusive." );
    }
}

/**
 * Converts a unicode integer value into its hexadecimal representation
 * @param val Integer value to convert
 * @param prefix Hex code prefix (default="\\u")
 * @return Unicode hex string
 */
std::string blogator::unicode::utf8::toxunicode( uint32_t val, const std::string& prefix ) {
    std::stringstream ss;

    ss << prefix
       << std::setfill( '0' )
       << std::setw( sizeof( uint32_t ) )
       << std::hex
       << std::uppercase
       << val;

    return ss.str();
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