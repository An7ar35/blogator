#include "encoding.h"

#include <codecvt>
#include <locale>
#include <sstream>
#include <algorithm>

/**
 * Encodes a UTF-32 character into a UTF-8 string
 * @param c UTF-32 Character
 * @return Encoded character as a string
 */
std::string blogator::encoding::encodeToUTF8( char32_t c ) {
    std::u32string str;
    str.append( 1, c );
    std::stringstream ss;
    encodeToUTF8( ss, str );
    return ss.str();
}

/**
 * Encodes a UTF-32 string into a UTF-8 string
 * @param str UTF-32 string
 * @return Encoded string
 */
std::string blogator::encoding::encodeToUTF8( const std::u32string &utf32_str ) {
    std::stringstream ss;
    encodeToUTF8( ss, utf32_str );
    return ss.str();
}

/**
 * Encodes a UTF-32 string into a UTF-8 output stream
 * @param out Output stream
 * @param str UTF-32 string
 * @return Output stream
 */
std::ostream & blogator::encoding::encodeToUTF8( std::ostream &out, const std::u32string &utf32_str ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    out << converter.to_bytes( utf32_str );
    return out;
}

/**
 * Encode a UTF-8 string into a UTF-32 string
 * @param str UTF-8 string
 * @return Encoded string
 */
std::u32string blogator::encoding::encodeToUTF32( const std::string &utf8_str ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    auto char_integers = converter.from_bytes( utf8_str );
    return std::u32string( reinterpret_cast<char32_t const *>( char_integers.data() ), char_integers.length() );
}

/**
 * Encodes a UTF-8 string into a UTF-32 output string
 * @param out Output string
 * @param str UTF-8 string
 * @return Output string
 */
blogator::u32stringstream_t & blogator::encoding::encodeToUTF32( blogator::u32stringstream_t &out, const std::string &utf8_str ) {
    out << encodeToUTF32( utf8_str );
    return out;
}

/**
 * Converts a character to its Hexadecimal representation
 * @param c UTF-32 character
 * @return Hexadecimal representation as a string
 */
std::u32string blogator::encoding::convertToHex( char32_t c ) {
    static const uint32_t mask = ( uint32_t( 1 ) << uint32_t( 4 ) ) - 1;
    static const auto     hex  = std::vector<char32_t>{ U'0', U'1', U'2', U'3', U'4', U'5', U'6', U'7',
                                                        U'8', U'9', U'A', U'B', U'C', U'D', U'E', U'F' };

    auto i = static_cast<uint32_t>( c );
    u32stringstream_t ss;

    while( i > 0 ) {
        uint32_t num  = i & mask; //last 4 bits
        i >>= uint32_t( 4 ); //shift right by 4 bits
        ss << hex.at( num );
    }

    auto str = ss.str();
    std::reverse( str.begin(), str.end() );
    return str;
}

/**
 * Checks non-character state of a UFT-32 character
 * @param c UTF-32 character
 * @return Non-character state
 */
bool blogator::encoding::isNonCharacter( char32_t c ) {
    return ( c >= U'\U0000FDD0' && c <= U'\U0000FDEF' )
        || c == U'\U0000FFFE'
        || c == U'\U0000FFFF'
        || c == U'\U0001FFFE'
        || c == U'\U0001FFFF'
        || c == U'\U0002FFFE'
        || c == U'\U0002FFFF'
        || c == U'\U0003FFFE'
        || c == U'\U0003FFFF'
        || c == U'\U0004FFFE'
        || c == U'\U0004FFFF'
        || c == U'\U0005FFFE'
        || c == U'\U0005FFFF'
        || c == U'\U0006FFFE'
        || c == U'\U0006FFFF'
        || c == U'\U0007FFFE'
        || c == U'\U0007FFFF'
        || c == U'\U0008FFFE'
        || c == U'\U0008FFFF'
        || c == U'\U0009FFFE'
        || c == U'\U0009FFFF'
        || c == U'\U000AFFFE'
        || c == U'\U000AFFFF'
        || c == U'\U000BFFFE'
        || c == U'\U000BFFFF'
        || c == U'\U000CFFFE'
        || c == U'\U000CFFFF'
        || c == U'\U000DFFFE'
        || c == U'\U000DFFFF'
        || c == U'\U000EFFFE'
        || c == U'\U000EFFFF'
        || c == U'\U000FFFFE'
        || c == U'\U000FFFFF'
        || c == U'\U0010FFFE'
        || c == U'\U0010FFFF';
}
