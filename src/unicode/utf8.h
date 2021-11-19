#ifndef BLOGATOR_PARSER_UNICODE_UTF8_H
#define BLOGATOR_PARSER_UNICODE_UTF8_H

#include <iostream>
#include <vector>

/**
 * UTF-8 specific methods for UTF32 code points
 */
namespace blogator::unicode::utf8 {
    size_t bytelength( uint8_t first );

    uint32_t toU32( const uint8_t sequence[4], size_t ln );
    uint32_t toU32( uint8_t u8_byte1 ) noexcept;
    uint32_t toU32( uint8_t u8_byte1, uint8_t u8_byte2 ) noexcept;
    uint32_t toU32( uint8_t u8_byte1, uint8_t u8_byte2, uint8_t u8_byte3 ) noexcept;
    uint32_t toU32( uint8_t u8_byte1, uint8_t u8_byte2, uint8_t u8_byte3, uint8_t u8_byte4 ) noexcept;

    std::string toxunicode( uint32_t val, const std::string & prefix = "\\u" );
    std::ostream & convert( std::ostream & stream, const std::u32string & str );
    std::ostream & convert( std::ostream & stream, char32_t c );
    std::string convert( const std::u32string & str );
}

#endif //BLOGATOR_PARSER_UNICODE_UTF8_H