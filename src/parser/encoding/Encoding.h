#ifndef BLOGATOR_PARSER_ENCODING_ENCODING_H
#define BLOGATOR_PARSER_ENCODING_ENCODING_H

#include <vector>
#include <istream>

#include "../../unicode/unicode.h"
#include "Confidence.h"
#include "Format.h"

namespace blogator::parser::encoding {
    struct Encoding {
        static bool U32toU32( std::istream &in, std::vector<uint32_t> &out );
        static bool U32toU32( const std::u32string &in, std::ostream &out );
        static bool U8toU32( std::istream &in, std::vector<uint32_t> &out );
    };
}

#endif //BLOGATOR_PARSER_ENCODING_ENCODING_H
