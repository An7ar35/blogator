#ifndef BLOGATOR_UNICODE_ASCII
#define BLOGATOR_UNICODE_ASCII

#include <cstdint>
#include <string>

/**
 * Character code constants in the ASCII range
 */
namespace blogator::unicode {
    static constexpr uint32_t NUL                    { 0x00 }; //'\0'
    static constexpr uint32_t TAB                    { 0x09 }; //'\t'
    static constexpr uint32_t LF                     { 0x0A }; //'\n'
    static constexpr uint32_t FF                     { 0x0C }; //'\f'
    static constexpr uint32_t CR                     { 0x0D }; //'\r'
    static constexpr uint32_t SPACE                  { 0x20 }; //' '
    static constexpr uint32_t EXCLAMATION_MARK       { 0x21 }; //'!'
    static constexpr uint32_t QUOTATION_MARK         { 0x22 }; //'"'
    static constexpr uint32_t NUMBER_SIGN            { 0x23 }; //'#'
    static constexpr uint32_t AMPERSAND              { 0x26 }; //'&'
    static constexpr uint32_t APOSTROPHE             { 0x27 }; //'''
    static constexpr uint32_t ASTERISK               { 0x2A }; //'*'
    static constexpr uint32_t PLUS_SIGN              { 0x2B }; //'+'
    static constexpr uint32_t HYPHEN_MINUS           { 0x2D }; //'-'
    static constexpr uint32_t SOLIDUS                { 0x2F }; //'/'
    static constexpr uint32_t DIGIT_ZERO             { 0x30 }; //'0'
    static constexpr uint32_t SEMICOLON              { 0x3B }; //';'
    static constexpr uint32_t LESS_THAN_SIGN         { 0x3C }; //'<'
    static constexpr uint32_t EQUALS_SIGN            { 0x3D }; //'='
    static constexpr uint32_t GREATER_THAN_SIGN      { 0x3E }; //'>'
    static constexpr uint32_t QUESTION_MARK          { 0x3F }; //'?'
    static constexpr uint32_t LATIN_CAPITAL_LETTER_X { 0x58 }; //'X'
    static constexpr uint32_t LEFT_SQUARE_BRACKET    { 0x5B }; //'['
    static constexpr uint32_t RIGHT_SQUARE_BRACKET   { 0x5D }; //']'
    static constexpr uint32_t GRAVE_ACCENT           { 0x60 }; //'`'
    static constexpr uint32_t LATIN_SMALL_LETTER_X   { 0x78 }; //'x'
    static constexpr uint32_t LEFT_CURLY_BRACKET     { 0x7B }; //'{'
    static constexpr uint32_t RIGHT_CURLY_BRACKET    { 0x7D }; //'}'
    static constexpr uint32_t TILDE                  { 0x7E }; //'~'
    static constexpr uint32_t DEL                    { 0x7F };
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
