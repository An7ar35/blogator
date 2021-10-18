#include "Encoding.h"

#include <sstream>
#include <codecvt>

bool blogator::parser::encoding::Encoding::U32toU32( std::istream &in, std::vector<uint32_t> &out ) {
    uint32_t bits;

    while( in.peek() >= 0 && !in.eof() ) {
        in.read( ( char * ) &bits, sizeof( bits ) );
        out.emplace_back( bits );
    }

    return false;
}

bool blogator::parser::encoding::Encoding::U32toU32( const std::u32string &in, std::ostream &out ) {
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

bool blogator::parser::encoding::Encoding::U8toU32( std::istream &in, std::vector<uint32_t> &out ) {
    uint8_t buffer[4];

    while( in.good() && !in.eof() && in.peek() >= 0 ) { //TODO optimise
        auto expected = unicode::utf8::bytelength( in.peek() );
        auto received = 0;

        if( expected == 0 ) {
            //TODO log
            throw std::invalid_argument( "Invalid UTF8 start byte: " + std::to_string( in.peek() ) );
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


    return false;
}
