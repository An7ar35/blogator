#ifndef BLOGATOR_UNICODE_UTF16_H
#define BLOGATOR_UNICODE_UTF16_H

#include <cstdint>
#include <cstddef>

/**
 * UTF-16 specific methods for UTF-16 code points
 */
namespace blogator::unicode::utf16 {
    char16_t join( char8_t byte1, char8_t byte2 ) noexcept;

    size_t codeunits( char16_t hi_surrogate ) noexcept;

    bool islowsurrogate( char16_t cu ) noexcept;
    bool ishighsurrogate( char16_t cu ) noexcept;
    bool isscalar( char16_t cu ) noexcept;

    uint32_t toU32( char16_t hi_surrogate ) noexcept;
    uint32_t toU32( char16_t hi_surrogate, char16_t lo_surrogate ) noexcept;
}

#endif //BLOGATOR_UNICODE_UTF16_H