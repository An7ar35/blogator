#ifndef BLOGATOR_PARSER_ENCODING_ENCODING_H
#define BLOGATOR_PARSER_ENCODING_ENCODING_H

#include <vector>
#include <deque>
#include <istream>

#include "../../unicode/unicode.h"
#include "Confidence.h"
#include "Format.h"

namespace blogator::parser::encoding {
    struct Transcode {
        static Format convert( std::istream &in, std::vector<uint32_t> &out, Format hint = Format::UNKNOWN );
        static Format sniffBOM( std::deque<uint8_t> &bom );

        static bool U32toByteStream( const std::u32string &in, std::ostream &out );

        static void U8toU32( std::istream &in, std::vector<uint32_t> &out );
        static void U8toU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out );
        static void U16BEtoU32( std::istream &in, std::vector<uint32_t> &out );
        static void U16BEtoU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out );
        static void U16LEtoU32( std::istream &in, std::vector<uint32_t> &out );
        static void U16LEtoU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out );
        static void U32toU32( std::istream &in, std::vector<uint32_t> &out );
        static void U32toU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out );
    };
}

#endif //BLOGATOR_PARSER_ENCODING_ENCODING_H