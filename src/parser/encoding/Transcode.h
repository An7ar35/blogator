#ifndef BLOGATOR_PARSER_ENCODING_TRANSCODE_H
#define BLOGATOR_PARSER_ENCODING_TRANSCODE_H

#include <vector>
#include <deque>
#include <istream>

#include "../../unicode/unicode.h"
#include "../dto/Source.h"

namespace blogator::parser::encoding {
    enum class Endianness {
        LE,
        BE
    };

    std::ostream & operator <<( std::ostream &os, Endianness e );

    struct Transcode {
      public:
        static bool convert( Source &src, std::vector<uint32_t> &out );
        static Format sniffBOM( std::deque<uint8_t> &bom );

        static bool U32toByteStream( const std::u32string &in, std::ostream &out, Endianness endianness );
        static bool U32toByteStream( const std::vector<uint32_t> &in, std::ostream &out, Endianness endianness );
        static size_t fetchCodeUnit( std::istream &stream, uint8_t * buffer, size_t byte_ln );
        static size_t fetchCodeUnit( std::istream &stream, std::deque<uint8_t> &buffer, size_t byte_ln );
        static void addCodePoint( Source &src, uint32_t prev_codepoint, uint32_t new_codepoint, std::vector<uint32_t> &out );

        static bool U8toU32( Source &src, std::vector<uint32_t> &out );
        static bool U8toU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out );
        static bool U16BEtoU32( Source &src, std::vector<uint32_t> &out );
        static bool U16BEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out );
        static bool U16LEtoU32( Source &src, std::vector<uint32_t> &out );
        static bool U16LEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out );
        static bool U32LEtoU32( Source &src, std::vector<uint32_t> &out );
        static bool U32LEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out );
        static bool U32BEtoU32( Source &src, std::vector<uint32_t> &out );
        static bool U32BEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out );

      private:
        static uint16_t joinU16LE( uint8_t byte1, uint8_t byte2 ) noexcept;
        static uint32_t joinU32LE( uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4 ) noexcept;

        static bool U16toU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out );
        static bool U16toU32( Source &src, std::vector<uint32_t> &out );
        static bool U32toU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out );
        static bool U32toU32( Source &src, std::vector<uint32_t> &out );
    };
}

namespace blogator {
    std::string to_string( blogator::parser::encoding::Endianness e );
}

#endif //BLOGATOR_PARSER_ENCODING_TRANSCODE_H