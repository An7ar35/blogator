#include "utf8.h"

#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

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
 * Converts a UTF32 string into UTF8
 * @param str UFT32 string
 * @return UFT8 string
 */
std::string blogator::unicode::utf8::convert( const std::u32string &str ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes( str );
}
