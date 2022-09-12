#include "utf32.h"

#include <vector>
#include <algorithm>
#include <ostream>

#include "utf8.h"

using namespace blogator::unicode;

constexpr char32_t UTF16_LEAD_OFFSET = ( 0xD800 - ( 0x10000 >> 10 ) ); //0xD7FF

/**
 * Joins 4 bytes into a UTF32 code unit
 * @param byte1 First most significant byte (XXXX 0000 0000 0000)
 * @param byte2 Second byte (0000 XXXX 0000 0000)
 * @param byte3 Third byte  (0000 0000 XXXX 0000)
 * @param byte4 Fourth byte (0000 0000 0000 XXXX)
 * @return U32 code unit
 */
char32_t utf32::join( char8_t byte1, char8_t byte2, char8_t byte3, char8_t byte4 ) noexcept {
    return ( (char32_t) byte1 << 24 ) + ( (char32_t) byte2 << 16 ) + ( (char32_t) byte3 << 8 ) + ( (char32_t) byte4 );
}

/**
 * Checks if code point is a surrogate
 * @param c Unicode code point
 * @return Is surrogate
 */
bool utf32::issurrogate( char32_t c ) noexcept {
    return ( c >= 0xD800 && c <= 0xDFFF );
}

/**
 * Checks if code point is scalar (i.e. not surrogate)
 * @param c Unicode code point
 * @return Is scalar
 */
bool utf32::isscalar( char32_t c ) noexcept {
    return !issurrogate( c );
}

/**
 * Checks if code point is a non-character
 * @param c Unicode code point
 * @return Is non-character
 */
bool utf32::isnonchar( char32_t c ) noexcept {
    return ( c >= 0x00FDD0 && c <= 0x00FDEF )
          || c == 0x00FFFE || c == 0x00FFFF
          || c == 0x01FFFE || c == 0x01FFFF
          || c == 0x02FFFE || c == 0x02FFFF
          || c == 0x03FFFE || c == 0x03FFFF
          || c == 0x04FFFE || c == 0x04FFFF
          || c == 0x05FFFE || c == 0x05FFFF
          || c == 0x06FFFE || c == 0x06FFFF
          || c == 0x07FFFE || c == 0x07FFFF
          || c == 0x08FFFE || c == 0x08FFFF
          || c == 0x09FFFE || c == 0x09FFFF
          || c == 0x0AFFFE || c == 0x0AFFFF
          || c == 0x0BFFFE || c == 0x0BFFFF
          || c == 0x0CFFFE || c == 0x0CFFFF
          || c == 0x0DFFFE || c == 0x0DFFFF
          || c == 0x0EFFFE || c == 0x0EFFFF
          || c == 0x0FFFFE || c == 0x0FFFFF
          || c == 0x10FFFE || c == 0x10FFFF;
}

/**
 * Check if character is a control character
 * @param c Unicode code point
 * @return Is control character (0x00->0x1F || 0x7F->0x9F)
 */
bool utf32::iscntrl( char32_t c ) noexcept {
    return ( c <= 0x1F || ( c >= 0x7F && c <= 0x9F ) );
}

/**
 * Convert surrogate character to replacement character (U+FFFD)
 * @param c Unicode code point
 * @return Replacement character or input character if not surrogate
 */
char32_t utf32::toscalar( char32_t c ) noexcept {
    if( issurrogate( c ) )
        return 0xFFFD;
    else
        return c;
}

/**
 * Converts a string to scalar code points
 * @param str String to convert
 */
void utf32::toscalar( std::u32string &str ) {
    std::for_each( str.begin(), str.end(), []( auto &c ) { c = toscalar( c ); } );
}

/**
 * Converts a UTF-32 code point into UTF-8
 * @param codepoint UTF-32 codepoint to convert
 * @param byte1 Byte 1 target container
 * @param byte2 Byte 2 target container
 * @param byte3 Byte 3 target container
 * @param byte4 Byte 4 target container
 * @return Number of 8bit code units used (0 if codepoint exceeds 0x10FFFF)
 */
size_t utf32::toU8( char32_t codepoint, char8_t &byte1, char8_t &byte2, char8_t &byte3, char8_t &byte4 ) noexcept {
    if( codepoint < 0b1000'0000 ) { //0x80
        byte1 = static_cast<int8_t>( codepoint );
        return 1;

    } else if( codepoint < 0b1000'0000'0000 ) { //0x800
        byte1 = 0b1100'0000 | ( ( codepoint & 0b0000'0000'0000'1111'1100'0000 ) >> 6 );
        byte2 = 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'0000'0011'1111 ) );
        return 2;

    } else if( codepoint < 0b1'0000'0000'0000'0000 ) { //0x10000
        byte1 = 0b1110'0000 | ( ( codepoint & 0b0000'0011'1111'0000'0000'0000 ) >> 12 );
        byte2 = 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'1111'1100'0000 ) >>  6 );
        byte3 = 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'0000'0011'1111 ) );
        return 3;

    } else if( codepoint <= 0b1'0000'1111'1111'1111'1111 ) { //0x10FFFF
        byte1 = 0b1111'0000 | ( ( codepoint & 0b1111'1100'0000'0000'0000'0000 ) >> 18 );
        byte2 = 0b1000'0000 | ( ( codepoint & 0b0000'0011'1111'0000'0000'0000 ) >> 12 );
        byte3 = 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'1111'1100'0000 ) >>  6 );
        byte4 = 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'0000'0011'1111 ) );
        return 4;
    }

    return 0; //RFC3629 §3 limits UTF-8 encoding to code point U+10FFFF (matching UTF-16)
}

/**
 * Converts a UTF-32 code point into UTF-8
 * @param codepoint UTF-32 codepoint to convert
 * @param out Output byte stream (unchecked!)
 * @return Number of 8bit code units used (0 if codepoint exceeds 0x10FFFF)
 */
size_t utf32::toU8( char32_t codepoint, std::ostream &out ) {
    if( codepoint < 0b1000'0000 ) { //0x80
        out << static_cast<int8_t>( codepoint );
        return 1;

    } else if( codepoint < 0b1000'0000'0000 ) { //0x800
        out << (char) ( 0b1100'0000 | ( ( codepoint & 0b0000'0000'0000'1111'1100'0000 ) >> 6 ) );
        out << (char) ( 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'0000'0011'1111 ) ) );
        return 2;

    } else if( codepoint < 0b1'0000'0000'0000'0000 ) { //0x10000
        out << (char) ( 0b1110'0000 | ( ( codepoint & 0b0000'0011'1111'0000'0000'0000 ) >> 12 ) );
        out << (char) ( 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'1111'1100'0000 ) >>  6 ) );
        out << (char) ( 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'0000'0011'1111 ) ) );
        return 3;

    } else if( codepoint <= 0b1'0000'1111'1111'1111'1111 ) { //0x10FFFF
        out << (char) ( 0b1111'0000 | ( ( codepoint & 0b1111'1100'0000'0000'0000'0000 ) >> 18 ) );
        out << (char) ( 0b1000'0000 | ( ( codepoint & 0b0000'0011'1111'0000'0000'0000 ) >> 12 ) );
        out << (char) ( 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'1111'1100'0000 ) >>  6 ) );
        out << (char) ( 0b1000'0000 | ( ( codepoint & 0b0000'0000'0000'0000'0011'1111 ) ) );
        return 4;
    }

    return 0; //RFC3629 §3 limits UTF-8 encoding to code point U+10FFFF (matching UTF-16)
}

/**
 * Converts a UTF-32 code point into UTF-16 code units
 * @param codepoint UTF-32 codepoint to convert
 * @param hi High surrogate target container
 * @param lo Low surrogate target container
 * @return Number of 16bit code units used
 */
size_t utf32::toU16( char32_t codepoint, uint16_t &hi, uint16_t &lo ) noexcept {
    hi = UTF16_LEAD_OFFSET + ( codepoint >> 10 );

    if( hi >= 0xD800 && hi <= 0xDBFF ) {
        lo = 0xDC00 + ( codepoint & 0x3FF );
        return 2;

    } else { //U+0000 to U+D7FF || U+E000 to U+FFFF
        hi = static_cast<uint16_t>( codepoint );
        return 1;
    }
}

/**
 * Converts a UTF-32 code point into UTF-16LE
 * @param codepoint UTF-32 codepoint to convert
 * @param out Output byte stream (unchecked!)
 * @return Number of 16bit code units used
 */
size_t utf32::toU16LE( char32_t codepoint, std::ostream &out ) {
    uint16_t hi = UTF16_LEAD_OFFSET + ( codepoint >> 10 );

    if( hi >= 0xD800 && hi <= 0xDBFF ) {
        uint16_t lo = 0xDC00 + ( codepoint & 0x3FF );

        out << ( char ) ( ( hi & 0b0000'0000'1111'1111 ) )
            << ( char ) ( ( hi & 0b1111'1111'0000'0000 ) >> 8 )
            << ( char ) ( ( lo & 0b0000'0000'1111'1111 ) )
            << ( char ) ( ( lo & 0b1111'1111'0000'0000 ) >> 8 );

        return 2;

    } else {
        out << ( char ) ( ( codepoint & 0b0000'0000'1111'1111 ) )
            << ( char ) ( ( codepoint & 0b1111'1111'0000'0000 ) >> 8 );

        return 1;
    }
}

/**
 * Converts a UTF-32 code point into UTF-16BE
 * @param codepoint UTF-32 codepoint to convert
 * @param out Output byte stream (unchecked!)
 * @return Number of 16bit code units used
 */
size_t utf32::toU16BE( char32_t codepoint, std::ostream &out ) {
    uint16_t hi = UTF16_LEAD_OFFSET + (codepoint >> 10 );

    if( hi >= 0xD800 && hi <= 0xDBFF ) {
        uint16_t lo = 0xDC00 + ( codepoint & 0x3FF );

        out << ( char ) ( ( hi & 0b1111'1111'0000'0000 ) >> 8 )
            << ( char ) ( ( hi & 0b0000'0000'1111'1111 ) )
            << ( char ) ( ( lo & 0b1111'1111'0000'0000 ) >> 8 )
            << ( char ) ( ( lo & 0b0000'0000'1111'1111 ) );

        return 2;

    } else {
        out << ( char ) ( ( codepoint & 0b1111'1111'0000'0000 ) >> 8 )
            << ( char ) ( ( codepoint & 0b0000'0000'1111'1111 ) );

        return 1;
    }
}

/**
 * Converts a unicode integer value into its hexadecimal representation
 * @param val Integer value to convert
 * @param prefix Hex code prefix (default="\\u")
 * @return Unicode hex string
 */
std::u32string utf32::toxunicode( char32_t val, const std::u32string &prefix ) {
    static const char32_t hex[] = { U'0', U'1', U'2', U'3', U'4', U'5', U'6', U'7', U'8', U'9',
                                    U'A', U'B', U'C', U'D', U'E', U'F' };

    std::vector<char32_t> out_buffer;
    std::vector<char32_t> hex_buffer;

    out_buffer.insert( out_buffer.begin(), prefix.cbegin(), prefix.cend() );

    while( val >= 16 ) {
        char32_t remainder = val % 16;
        val = ( val - remainder ) / 16;
        hex_buffer.emplace_back( remainder );
    }

    hex_buffer.emplace_back( val );

    if( hex_buffer.size() < 4 ) {
        for( auto i = 0; i < 4 - hex_buffer.size(); ++i ) {
            out_buffer.emplace_back( '0' );
        }
    }

    std::for_each( hex_buffer.crbegin(),
                   hex_buffer.crend(),
                   [&]( const auto & h ) { out_buffer.emplace_back( hex[h] ); }
    );

    return { out_buffer.begin(), out_buffer.end() };
}

/**
 * Converts a number in a UTF-32 formatted string to a signed long
 * @param str String representing a number
 * @param base Base (default=10)
 * @return Converted number
 * @throws std::invalid_argument when conversion fails
 */
int64_t utf32::toLong( const std::u32string &str, int base ) {
    std::string            u8str  = unicode::utf8::convert( str );
    std::string::size_type i      = 0;
    int64_t                result = std::stol( u8str, &i, base );

    if( i < str.size() ) {
        throw std::invalid_argument(
            "[blogator::string::toLong( \"" + u8str + "\", " + std::to_string( base ) + " )] "
            "String is not a valid number for the base given."
        );
    }

    return result;
}

/**
 * Converts a floating number in a UTF-32 formatted string to a double
 * @param str String representing a float
 * @return Converted number
 * @throws std::invalid_argument when conversion fail or in cases of overflow/underflow
 */
double utf32::toDouble( const std::u32string &str ) {
    try {
        return std::stod( unicode::utf8::convert( str ) );
    } catch( const std::out_of_range &e ) {
        throw std::invalid_argument( e.what() );
    }
}