#ifndef BLOGATOR_PARSER_UNICODE_UTF8_H
#define BLOGATOR_PARSER_UNICODE_UTF8_H

#include <iostream>
#include <vector>

/**
 * UTF-8 specific methods for UTF32 code points
 */
namespace blogator::unicode::utf8 {
    size_t bytelength( char8_t first );

    uint32_t toU32( const char8_t sequence[4], size_t ln );
    uint32_t toU32( char8_t u8_byte1 ) noexcept;
    uint32_t toU32( char8_t u8_byte1, char8_t u8_byte2 ) noexcept;
    uint32_t toU32( char8_t u8_byte1, char8_t u8_byte2, char8_t u8_byte3 ) noexcept;
    uint32_t toU32( char8_t u8_byte1, char8_t u8_byte2, char8_t u8_byte3, char8_t u8_byte4 ) noexcept;

    std::string toxunicode( char8_t val, const std::string & prefix = "\\u" );
    std::string toxunicode( char16_t val, const std::string & prefix = "\\u" );
    std::string toxunicode( char32_t val, const std::string & prefix = "\\u" );
    
    std::ostream & convert( std::ostream & stream, const std::u32string & str );
    std::ostream & convert( std::ostream & stream, char32_t c );
    std::string convert( const std::u32string & str );
}

#endif //BLOGATOR_PARSER_UNICODE_UTF8_H