#include "utf8.h"

#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>

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
std::ostream &blogator::unicode::utf8::convert( std::ostream &stream, const std::u32string &str ) {
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
std::ostream &blogator::unicode::utf8::convert( std::ostream &stream, char32_t c ) {
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