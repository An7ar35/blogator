#ifndef BLOGATOR_PARSER_UNICODE_UTILITIES_H
#define BLOGATOR_PARSER_UNICODE_UTILITIES_H

#include <cstdint>

/**
 * Functions from <cctype> adapted for UTF32 format
 */
namespace blogator::parser::unicode::ascii {
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

    uint32_t tolower( uint32_t c ) noexcept;
    uint32_t toupper( uint32_t c ) noexcept;
}

#endif //BLOGATOR_PARSER_UNICODE_UTILITIES_H
