#include "unicode.h"

#include <vector>
#include <sstream>


/**
 * Converts a UTF32 string into a UTF8 string where all control codes are exchanged with a unicode string
 * @param u32str UTF32 string
 * @return Normalised UTF8 string
 */
std::string blogator::unicode::normalize( const std::u32string  &u32str ) {
    std::stringstream ss;
    normalize( ss, u32str );
    return ss.str();
}

/**
 * Converts a UTF32 string where all control codes are exchanged with a unicode string
 * @param os Output stream
 * @param u32str UTF32 string
 * @return Output stream where the normalised UTF8 is placed
 */
std::ostream &blogator::unicode::normalize( std::ostream &os, const std::u32string & u32str ) {
    auto u8str = unicode::utf8::convert( u32str );
    return normalize( os, u8str );
}

/**
 * Normalised a UTF8 string (control codes exchanged with a unicode representations)
 * @param os Output stream
 * @param u8str UTF8 string to normalise
 * @return Output stream
 */
std::ostream &blogator::unicode::normalize( std::ostream &os, const std::string & u8str ) {
    for( auto c : u8str ) {
        if( unicode::ascii::iscntrl( c ) )
        {
            os<< unicode::utf8::toxunicode( c );
        } else {
            os << c;
        }
    }

    return os;
}