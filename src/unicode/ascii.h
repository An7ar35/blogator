#ifndef BLOGATOR_UNICODE_ASCII
#define BLOGATOR_UNICODE_ASCII

#include <cstdint>
#include <string>

/**
 * Character code constants in the ASCII range
 */
namespace blogator::unicode {
    static constexpr char32_t NUL                    { 0x00 }; //'\0' Null (^@)
    static constexpr char32_t SOH                    { 0x01 }; //Start of heading (^A)
    static constexpr char32_t STX                    { 0x02 }; //Start of text (^B)
    static constexpr char32_t ETX                    { 0x03 }; //End of text (^C)
    static constexpr char32_t EOT                    { 0x04 }; //End of transmission (^D)
    static constexpr char32_t INQ                    { 0x05 }; //Inquiry (^E)
    static constexpr char32_t ACK                    { 0x06 }; //Acknowledgement (^F)
    static constexpr char32_t BEL                    { 0x07 }; //Bell (^G)
    static constexpr char32_t BS                     { 0x08 }; //Backspace (^H)
    static constexpr char32_t TAB                    { 0x09 }; //'\t' Horizontal tab (^I)
    static constexpr char32_t LF                     { 0x0A }; //'\n' Line feed (^J)
    static constexpr char32_t VT                     { 0x0B }; //'\v' Vertical Tab (^K)
    static constexpr char32_t FF                     { 0x0C }; //'\f' Form feed (^L)
    static constexpr char32_t CR                     { 0x0D }; //'\r' Carriage return (^M)
    static constexpr char32_t SO                     { 0x0E }; //Shift out (^N)
    static constexpr char32_t SI                     { 0x0F }; //Shift in (^O)
    static constexpr char32_t DLE                    { 0x10 }; //Data link escape (^P)
    static constexpr char32_t DC1                    { 0x11 }; //Device control 1/XON (^Q)
    static constexpr char32_t DC2                    { 0x13 }; //Device control 3 (^R)
    static constexpr char32_t DC3                    { 0x13 }; //Device control 3/XOFF (^S)
    static constexpr char32_t DC4                    { 0x14 }; //Device control 4 (^T)
    static constexpr char32_t NAK                    { 0x15 }; //Negative acknowledgement (^U)
    static constexpr char32_t SYN                    { 0x16 }; //Synchronous idle (^V)
    static constexpr char32_t ETB                    { 0x17 }; //End of transmission block (^W)
    static constexpr char32_t CAN                    { 0x18 }; //Cancel (^X)
    static constexpr char32_t EM                     { 0x19 }; //End of medium (^Y)
    static constexpr char32_t SUB                    { 0x1A }; //Substitute (^Z)
    static constexpr char32_t ESC                    { 0x1B }; //'\e' Escape (^[)
    static constexpr char32_t FS                     { 0x1C }; //File separator (^\)
    static constexpr char32_t GS                     { 0x1D }; //Group separator (^])
    static constexpr char32_t RS                     { 0x1E }; //Record separator (^^)
    static constexpr char32_t US                     { 0x1F }; //Unit separator (^_)
    static constexpr char32_t SPACE                  { 0x20 }; //' '
    static constexpr char32_t EXCLAMATION_MARK       { 0x21 }; //'!'
    static constexpr char32_t QUOTATION_MARK         { 0x22 }; //'"'
    static constexpr char32_t NUMBER_SIGN            { 0x23 }; //'#'
    static constexpr char32_t DOLLAR                 { 0x24 }; //'$'
    static constexpr char32_t PERCENT_SIGN           { 0x25 }; //'%'
    static constexpr char32_t AMPERSAND              { 0x26 }; //'&'
    static constexpr char32_t APOSTROPHE             { 0x27 }; //'''
    static constexpr char32_t LEFT_PARENTHESIS       { 0x28 }; //'('
    static constexpr char32_t SINGLE_QUOTE           { 0x27 }; //'''
    static constexpr char32_t RIGHT_PARENTHESIS      { 0x29 }; //')'
    static constexpr char32_t ASTERISK               { 0x2A }; //'*'
    static constexpr char32_t PLUS_SIGN              { 0x2B }; //'+'
    static constexpr char32_t COMMA                  { 0x2C }; //','
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
    static constexpr char32_t AT_SYMBOL              { 0x40 }; //'@'
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
    static constexpr char32_t DEL                    { 0x7F }; //Delete (^?)
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

    bool isOperator( char32_t c );
    bool isPunctuator( char32_t c );
    bool isBracket( char32_t c );

    char32_t tolower( char32_t c ) noexcept;
    char32_t toupper( char32_t c ) noexcept;

    std::u32string & tolower( std::u32string & str );
    std::u32string tolower( const std::u32string & str );
    std::u32string & toupper( std::u32string & str );
    std::u32string toupper( const std::u32string & str );
}

#endif //BLOGATOR_UNICODE_ASCII
