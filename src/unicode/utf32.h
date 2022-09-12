#ifndef BLOGATOR_UNICODE_UTF32_H
#define BLOGATOR_UNICODE_UTF32_H

#include <cstdint>
#include <string>

/**
 * Character code constants
 */
namespace blogator::unicode {
    static constexpr char32_t REPLACEMENT_CHAR { 0xFFFD };
}

/**
 * UTF-32 specific methods
 */
namespace blogator::unicode::utf32 {
    char32_t join( char8_t byte1, char8_t byte2, char8_t byte3, char8_t byte4 ) noexcept;

    bool issurrogate( char32_t c ) noexcept;
    bool isscalar( char32_t c ) noexcept;
    bool isnonchar( char32_t c ) noexcept;
    bool iscntrl( char32_t c ) noexcept;

    char32_t toscalar( char32_t c ) noexcept;
    void toscalar( std::u32string &str );

    size_t toU8( char32_t codepoint, char8_t & byte1, char8_t & byte2, char8_t & byte3, char8_t & byte4 ) noexcept;
    size_t toU8( char32_t codepoint, std::ostream & out );
    size_t toU16( char32_t codepoint, uint16_t & hi, uint16_t & lo ) noexcept;
    size_t toU16LE( char32_t codepoint, std::ostream & out );
    size_t toU16BE( char32_t codepoint, std::ostream & out );

    std::u32string toxunicode( char32_t val, const std::u32string & prefix = U"\\u" );
    int64_t toLong( const std::u32string & str, int base = 10 );
    double toDouble( const std::u32string & str );
}

#endif //BLOGATOR_UNICODE_UTF32_H