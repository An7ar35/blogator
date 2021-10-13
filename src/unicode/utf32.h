#ifndef BLOGATOR_UNICODE_UTF32_H
#define BLOGATOR_UNICODE_UTF32_H

#include <cstdint>
#include <string>

namespace blogator::unicode::utf32 {
    bool issurrogate( uint32_t c ) noexcept;
    bool isscalar( uint32_t c ) noexcept;
    bool isnonchar( uint32_t c ) noexcept;
    bool isascii( uint32_t c ) noexcept;
    bool isascii( const std::u32string &str );
    bool iscntrl( uint32_t c ) noexcept;

    uint32_t toscalar( uint32_t c ) noexcept;
    void toscalar( std::u32string &str );

    std::u32string toxunicode( uint32_t val, const std::u32string &prefix = U"\\u" );
    std::u32string convert( const std::string & u8str );
}

#endif //BLOGATOR_UNICODE_UTF32_H