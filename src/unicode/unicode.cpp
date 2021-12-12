#include "unicode.h"

#include <vector>
#include <sstream>


/**
 * Normalize a UTF32 string (control codes, ''', '"' and '\\' exchanged with a unicode representations)
 * @param u32str UTF32 string
 * @return Normalised UTF8 string
 */
std::string blogator::unicode::normalize( const std::u32string  &u32str ) {
    std::stringstream ss;
    normalize( ss, u32str );
    return ss.str();
}

/**
 * Normalize a UTF32 string (control codes, ''', '"' and '\\' exchanged with a unicode representations)
 * @param os Output stream
 * @param u32str UTF32 string
 * @return Output stream where the normalised UTF8 is placed
 */
std::ostream &blogator::unicode::normalize( std::ostream &os, const std::u32string & u32str ) {
    for( auto c : u32str ) {
        if( unicode::utf32::iscntrl( c ) || c == '\'' || c == '\"' || c == '\\' || c == 0x00 ) {
            unicode::utf8::convert( os, unicode::utf32::toxunicode( c ) );
        } else {
            unicode::utf8::convert( os, c );
        }
    }

    return os;
}

/**
 * Normalize a UTF8 string (control codes, ''', '"' and '\\' exchanged with a unicode representations)
 * @param os Output stream
 * @param u8str UTF8 string to normalise
 * @return Output stream
 */
std::ostream &blogator::unicode::normalize( std::ostream &os, const std::string & u8str ) {
    for( auto c : u8str ) {
        if( unicode::ascii::iscntrl( c ) || c == '\'' || c == '\"' || c == '\\' || c == 0x00 ) {
            os<< unicode::utf8::toxunicode( (uint8_t) c );
        } else {
            os << c;
        }
    }

    return os;
}