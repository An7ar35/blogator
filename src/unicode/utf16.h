#ifndef BLOGATOR_UNICODE_UTF16_H
#define BLOGATOR_UNICODE_UTF16_H

#include <cstdint>
#include <cstddef>

/**
 * UTF-16 specific methods for UTF-16 code points
 */
namespace blogator::unicode::utf16 {
    uint16_t join( uint8_t byte1, uint8_t byte2 ) noexcept;

    size_t codeunits( uint16_t hi_surrogate ) noexcept;

    bool islowsurrogate( uint16_t cu ) noexcept;
    bool ishighsurrogate( uint16_t cu ) noexcept;
    bool isscalar( uint16_t cu ) noexcept;

    uint32_t toU32( uint16_t hi_surrogate ) noexcept;
    uint32_t toU32( uint16_t hi_surrogate, uint16_t lo_surrogate ) noexcept;
}

#endif //BLOGATOR_UNICODE_UTF16_H