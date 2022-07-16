#ifndef BLOGATOR_UNICODE_ASCII
#define BLOGATOR_UNICODE_ASCII

#include <cstdint>
#include <string>

/**
 * Character code constants in the ASCII range
 */
namespace blogator::unicode {
    static constexpr char32_t NUL                    { 0x00 }; //'\0'
    static constexpr char32_t TAB                    { 0x09 }; //'\t'
    static constexpr char32_t LF                     { 0x0A }; //'\n'
    static constexpr char32_t FF                     { 0x0C }; //'\f'
    static constexpr char32_t CR                     { 0x0D }; //'\r'
    static constexpr char32_t SPACE                  { 0x20 }; //' '
    static constexpr char32_t EXCLAMATION_MARK       { 0x21 }; //'!'
    static constexpr char32_t QUOTATION_MARK         { 0x22 }; //'"'
    static constexpr char32_t NUMBER_SIGN            { 0x23 }; //'#'
    static constexpr char32_t AMPERSAND              { 0x26 }; //'&'
    static constexpr char32_t APOSTROPHE             { 0x27 }; //'''
    static constexpr char32_t LEFT_PARENTHESIS       { 0x28 }; //'('
    static constexpr char32_t RIGHT_PARENTHESIS      { 0x29 }; //')'
    static constexpr char32_t ASTERISK               { 0x2A }; //'*'
    static constexpr char32_t PLUS_SIGN              { 0x2B }; //'+'
    static constexpr char32_t HYPHEN_MINUS           { 0x2D }; //'-'
    static constexpr char32_t FULL_STOP              { 0x2E }; //'.'
    static constexpr char32_t SOLIDUS                { 0x2F }; //'/'
    static constexpr char32_t DIGIT_ZERO             { 0x30 }; //'0'
    static constexpr char32_t COLON                  { 0x3A }; //':'
    static constexpr char32_t SEMICOLON              { 0x3B }; //';'
    static constexpr char32_t LESS_THAN_SIGN         { 0x3C }; //'<'
    static constexpr char32_t EQUALS_SIGN            { 0x3D }; //'='
    static constexpr char32_t GREATER_THAN_SIGN      { 0x3E }; //'>'
    static constexpr char32_t QUESTION_MARK          { 0x3F }; //'?'
    static constexpr char32_t LATIN_CAPITAL_LETTER_X { 0x58 }; //'X'
    static constexpr char32_t LEFT_SQUARE_BRACKET    { 0x5B }; //'['
    static constexpr char32_t REVERSE_SOLIDUS        { 0x5C }; //'\'
    static constexpr char32_t RIGHT_SQUARE_BRACKET   { 0x5D }; //']'
    static constexpr char32_t CIRCUMFLEX_ACCENT      { 0x5E }; //'^'
    static constexpr char32_t UNDERSCORE             { 0x5F }; //'_'
    static constexpr char32_t GRAVE_ACCENT           { 0x60 }; //'`'
    static constexpr char32_t LATIN_SMALL_LETTER_X   { 0x78 }; //'x'
    static constexpr char32_t LEFT_CURLY_BRACKET     { 0x7B }; //'{'
    static constexpr char32_t VERTICAL_LINE          { 0x7C }; //'|'
    static constexpr char32_t RIGHT_CURLY_BRACKET    { 0x7D }; //'}'
    static constexpr char32_t TILDE                  { 0x7E }; //'~'
    static constexpr char32_t DEL                    { 0x7F };
}

/**
 * ASCII specific methods for UTF32 code points with range checks
 */
namespace blogator::unicode::ascii {
    bool isalnum( char32_t c ) noexcept;
    bool isalpha( char32_t c ) noexcept;
    bool isblank( char32_t c ) noexcept;
    bool iscntrl( char32_t c ) noexcept;
    bool isdigit( char32_t c ) noexcept;
    bool isxdigit( char32_t c ) noexcept;
    bool isgraph( char32_t c ) noexcept;
    bool isspace( char32_t c ) noexcept;
    bool iswspace( char32_t c ) noexcept;
    bool isfeed( char32_t c ) noexcept;
    bool isupper( char32_t c ) noexcept;
    bool islower( char32_t c ) noexcept;
    bool isprint( char32_t c ) noexcept;
    bool ispunct( char32_t c ) noexcept;
    bool istab( char32_t c ) noexcept;
    bool isnewline( char32_t c ) noexcept;
    bool isuxdigit( char32_t c ) noexcept;
    bool islxdigit( char32_t c ) noexcept;

    char32_t tolower( char32_t c ) noexcept;
    char32_t toupper( char32_t c ) noexcept;

    std::u32string & tolower( std::u32string & str );
    std::u32string tolower( const std::u32string & str );
    std::u32string & toupper( std::u32string & str );
    std::u32string toupper( const std::u32string & str );
}

#endif //BLOGATOR_UNICODE_ASCII
