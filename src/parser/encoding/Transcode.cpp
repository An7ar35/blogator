#include "Transcode.h"

#include <sstream>
#include <codecvt>

using namespace blogator::parser::encoding;

/**
 * Converts an input byte stream into UTF-32
 * @param in Input byte stream
 * @param out UTF-32 output
 * @param hint Input format hint (default: UNKNOWN) - bypasses the format sniffing
 * @return Input format
 */
Format Transcode::convert( std::istream &in, std::vector<uint32_t> &out, Format hint ) {
    std::deque<uint8_t> buffer;

    if( hint == Format::UNKNOWN ) {
        for( size_t i = 0; ( !in.eof() && in.good() && i < 3 ); ++i ) {
            buffer.emplace_back( in.get() );
        }

        hint = sniffBOM( buffer );
    }

    switch( hint ) {
        case Format::UTF8    : { Transcode::U8toU32( buffer, in, out );    } break;
        case Format::UTF16_BE: { Transcode::U16BEtoU32( buffer, in, out ); } break;
        case Format::UTF16_LE: { Transcode::U16LEtoU32( buffer, in, out ); } break;
        case Format::UTF32   : { Transcode::U32toU32( buffer, in, out );   } break;
        case Format::UNKNOWN : throw std::invalid_argument( "Failed to detect format of input." );
        default              : throw std::invalid_argument( "Input format not supported." );
    }

    return hint;
}

/**
 * BOM sniffing
 * @param bom Buffer containing a BOM at the beginning
 * @return Format deduced from BOM
 */
Format Transcode::sniffBOM( std::deque<uint8_t> &bom ) {
    if( bom.size() >= 3 && bom[ 0 ] == 0xEF && bom[ 1 ] == 0xBB && bom[ 2 ] == 0xBF ) { //UTF-8: 0xEF 0xBB 0xBF
        bom.erase( bom.begin(), std::next( bom.begin(), 3 ) );
        return Format::UTF8;

    } else if( bom.size() >= 2 && bom[ 0 ] == 0xFE && bom[ 1 ] == 0xFF ) { //UTF-16BE: 0xFE 0xFF
        bom.erase( bom.begin(), std::next( bom.begin(), 2 ) );
        return Format::UTF16_BE;

    } else if( bom.size() >= 2 && bom[ 0 ] == 0xFF && bom[ 1 ] == 0xFE ) { //UTF-16LE: 0xFF 0xFE
        bom.erase( bom.begin(), std::next( bom.begin(), 2 ) );
        return Format::UTF16_LE;
    }

    return Format::UNKNOWN;
}

/**
 * Converts a UTF-32 string to a 8-bit stream
 * @param in UTF-32 string
 * @param out Output byte stream
 * @return Success
 */
bool Transcode::U32toByteStream( const std::u32string &in, std::ostream &out ) {
    if( out.bad() || out.eof() ) {
        //TODO LOG
        return false; //EARLY RETURN
    }

    for( const auto & cp : in ) { //TODO little endian (x64 intel) and big endian of source matters! -> template with endianess baked in?
        out << (char)( ( cp & 0b0000'0000'0000'0000'0000'0000'1111'1111 ) )
            << (char)( ( cp & 0b0000'0000'0000'0000'1111'1111'0000'0000 ) >>  8 )
            << (char)( ( cp & 0b0000'0000'1111'1111'0000'0000'0000'0000 ) >> 16 )
            << (char)( ( cp & 0b1111'1111'0000'0000'0000'0000'0000'0000 ) >> 24 );
    }

    return true;
}

/**
 * Converts an UTF-8 encoded byte stream to UTF-32
 * @param in Input UTF-8 byte stream
 * @param out UTF-32 collection
 * @throws std::invalid_argument when //TODO
 */
void Transcode::U8toU32( std::istream &in, std::vector<uint32_t> &out ) {
    uint8_t buffer[4];

    while( in.good() && !in.eof() && in.peek() >= 0 ) { //TODO optimise
        auto expected = unicode::utf8::bytelength( in.peek() );
        auto received = 0;

        if( expected == 0 ) {
            //TODO log
            throw std::invalid_argument( "Invalid UTF8 start byte: " + unicode::utf8::toxunicode( in.peek(), "" ) );
        }

        while( received < expected && !in.eof() ) {
            buffer[received] = in.get();
            ++received;
        }

        if( received == expected ) {
            out.emplace_back( unicode::utf8::toU32( buffer, received ) );
        } else {
            //TODO log
            throw std::invalid_argument( "EOF reached mid-codepoint sequence." );
        }
    }
}

/**
 * Converts an UTF-8 encoded byte stream to UTF-32
 * @param pre_buffer Buffered input bytes
 * @param in Input UTF-8 byte stream
 * @param out UTF-32 collection
 */
void Transcode::U8toU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out ) {
    //TODO pre_buffer
    U8toU32( in, out );

}

void Transcode::U16BEtoU32( std::istream &in, std::vector<uint32_t> &out ) { //TODO
}

void Transcode::U16BEtoU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out ) { //TODO
    //TODO pre_buffer
    U16BEtoU32( in, out );
}

void Transcode::U16LEtoU32( std::istream &in, std::vector<uint32_t> &out ) { //TODO

}

void Transcode::U16LEtoU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out ) { //TODO
    //TODO pre_buffer
    U16LEtoU32( in, out );
}

/**
 * Converts content of a UTF-32 byte stream to a collection of u32 characters
 * @param in Input byte stream
 * @param out UTF-32 collection
 * @return Success
 */
void Transcode::U32toU32( std::istream &in, std::vector<uint32_t> &out ) {
    uint32_t bits;

    while( in.peek() >= 0 && !in.eof() ) {
        in.read( ( char * ) &bits, sizeof( bits ) );
        out.emplace_back( bits );
    }
}

/**
 * Converts content of a UTF-32 byte stream to a collection of u32 characters
 * @param pre_buffer Buffered input bytes
 * @param in Input byte stream
 * @param out UTF-32 collection
 */
void Transcode::U32toU32( std::deque<uint8_t> &pre_buffer, std::istream &in, std::vector<uint32_t> &out ) {
    if( !pre_buffer.empty() ) {
        if( ( pre_buffer.size() % 4 ) > 0  ) { //i.e.: incomplete code point
            size_t remainder = ( pre_buffer.size() % 4 );
            size_t required  = 4 - remainder;

            for( auto i = 0; !in.eof() && in.good() && i < required; ++i ) {
                pre_buffer.emplace_back( in.get() );
            }
        }

        //TODO what if there isn't enough bytes in the source??

        size_t code_point_count = ( pre_buffer.size() / 4 );

        for( int i = 0, byte_i = 0; i < code_point_count; ++i, byte_i += 4 ) { //TODO byte order will matter based on endianess of source
            uint32_t codepoint = 0x0;
            codepoint += (uint32_t) pre_buffer.at( byte_i )        << 24;
            codepoint += (uint32_t) pre_buffer.at( byte_i + 1 ) << 16;
            codepoint += (uint32_t) pre_buffer.at( byte_i + 2 ) << 8;
            codepoint += (uint32_t) pre_buffer.at( byte_i + 3 );
            out.emplace_back( codepoint );
        }
    }

    U32toU32( in, out );
}