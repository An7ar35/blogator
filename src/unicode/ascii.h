#ifndef BLOGATOR_UNICODE_ASCII
#define BLOGATOR_UNICODE_ASCII

#include <cstdint>

/**
 * ASCII specific methods for UTF32 code points with range checks
 */
namespace blogator::unicode::ascii {
    bool isalnum( uint32_t c ) noexcept;
    bool isalpha( uint32_t c ) noexcept;
    bool isblank( uint32_t c ) noexcept;
    bool iscntrl( uint32_t c ) noexcept;
    bool isdigit( uint32_t c ) noexcept;
    bool isxdigit( uint32_t c ) noexcept;
    bool isgraph( uint32_t c ) noexcept;
    bool isspace( uint32_t c ) noexcept;
    bool isupper( uint32_t c ) noexcept;
    bool islower( uint32_t c ) noexcept;
    bool isprint( uint32_t c ) noexcept;
    bool ispunct( uint32_t c ) noexcept;
    bool istab( uint32_t c ) noexcept;
    bool isnewline( uint32_t c ) noexcept;
    bool isuxdigit( uint32_t c ) noexcept;
    bool islxdigit( uint32_t c ) noexcept;

    uint32_t tolower( uint32_t c ) noexcept;
    uint32_t toupper( uint32_t c ) noexcept;
}

#endif //BLOGATOR_UNICODE_ASCII
