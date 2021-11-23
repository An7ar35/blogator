#ifndef BLOGATOR_UNICODE_UTF32_H
#define BLOGATOR_UNICODE_UTF32_H

#include <cstdint>
#include <string>

/**
 * UTF-32 specific methods
 */
namespace blogator::unicode::utf32 {
    uint32_t join( uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4 ) noexcept;

    bool issurrogate( uint32_t c ) noexcept;
    bool isscalar( uint32_t c ) noexcept;
    bool isnonchar( uint32_t c ) noexcept;
    bool isascii( uint32_t c ) noexcept;
    bool isascii( const std::u32string &str );
    bool iscntrl( uint32_t c ) noexcept;

    uint32_t toscalar( uint32_t c ) noexcept;
    void toscalar( std::u32string &str );

    size_t toU8( uint32_t codepoint, uint8_t &byte1, uint8_t &byte2, uint8_t &byte3, uint8_t &byte4 ) noexcept;
    size_t toU8( uint32_t codepoint, std::ostream &out );
    size_t toU16( uint32_t codepoint, uint16_t &hi, uint16_t &lo ) noexcept;
    size_t toU16LE( uint32_t codepoint, std::ostream &out );
    size_t toU16BE( uint32_t codepoint, std::ostream &out );

    std::u32string toxunicode( uint32_t val, const std::u32string &prefix = U"\\u" );
}

#endif //BLOGATOR_UNICODE_UTF32_H