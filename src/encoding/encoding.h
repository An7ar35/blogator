#ifndef BLOGATOR_ENCODING_H
#define BLOGATOR_ENCODING_H

#include <string>
#include <ostream>
#include <sstream>

#include "../dto/HTML.h"

namespace blogator {
    typedef std::basic_ostream<char32_t>      u32ostream_t;
    typedef std::basic_stringstream<char32_t> u32stringstream_t;
}

namespace blogator::encoding {
    std::string encodeToUTF8( char32_t c );
    std::string encodeToUTF8( const std::u32string &utf32_str );
    std::ostream & encodeToUTF8( std::ostream &out, const std::u32string &utf32_str );

    std::u32string encodeToUTF32( const std::string &utf8_str );
    u32stringstream_t & encodeToUTF32( u32stringstream_t &out, const std::string &utf8_str );

    bool isNonCharacter( char32_t c );

}

#endif //BLOGATOR_ENCODING_H
