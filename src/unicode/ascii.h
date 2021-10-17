#ifndef BLOGATOR_UNICODE_ASCII
#define BLOGATOR_UNICODE_ASCII

#include <cstdint>
#include <string>

/**
 * Character code constants in the ASCII range
 */
namespace blogator::unicode {
    static const uint32_t NUL   { 0x00 };
    static const uint32_t TAB   { 0x09 };
    static const uint32_t LF    { 0x0A };
    static const uint32_t FF    { 0x0C };
    static const uint32_t CR    { 0x0D };
    static const uint32_t SPACE { 0x20 };
    static const uint32_t DEL   { 0x7F };
}

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
    bool iswspace( uint32_t c ) noexcept;
    bool isfeed( uint32_t c ) noexcept;
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

    std::u32string & tolower( std::u32string & str );
    std::u32string tolower( const std::u32string & str );
    std::u32string & toupper( std::u32string & str );
    std::u32string toupper( const std::u32string & str );
}

#endif //BLOGATOR_UNICODE_ASCII
