#include "Transcode.h"

#include <sstream>
#include <codecvt>
#include <bit>

#include "../logging/ParserLog.h"

using namespace blogator::parser::encoding;

/**
 * Converts an input byte stream into UTF-32
 * @param src Source (if format == UNKNOWN then sniffing attempt will be made for the BOM)
 * @param out UTF-32 output
 * @return Success
 * @throws std::invalid_argument when format is unknown or unsupported
 */
bool Transcode::convert( Source &src, std::vector<uint32_t> &out ) {
    auto & in = src.stream();

    std::deque<uint8_t> buffer;

    if( src.format() == Format::UNKNOWN ) {
        const auto BYTE_SAMPLE_SIZE = 4;

        for( size_t i = 0; ( !in.eof() && in.good() && i < BYTE_SAMPLE_SIZE ); ++i ) {
            buffer.emplace_back( in.get() );
        }

        src.setFormat( sniffBOM( buffer ) );
        src.setConfidence( src.format() == Format::UNKNOWN ? Confidence::TENTATIVE : Confidence::CERTAIN );
    }

    switch( src.format() ) {
        case Format::UTF8    : return Transcode::U8toU32( buffer, src, out );
        case Format::UTF16_LE: return Transcode::U16LEtoU32( buffer, src, out );
        case Format::UTF16_BE: return Transcode::U16BEtoU32( buffer, src, out );
        case Format::UTF32_LE: return Transcode::U32LEtoU32( buffer, src, out );
        case Format::UTF32_BE: return Transcode::U32BEtoU32( buffer, src, out );
        case Format::UNKNOWN : throw std::invalid_argument( "Failed to detect format of input." );
        default              : throw std::invalid_argument( "Input format not supported." );
    }
}

/**
 * BOM sniffing (UTF8/16LE/16BE/32LE/32BE)
 * @param bom Buffer containing a BOM at the beginning
 * @return Format deduced from BOM
 */
Format Transcode::sniffBOM( std::deque<uint8_t> &bom ) {
    static const std::array<uint8_t, 4> UTF32_LE_BOM = { 0x00, 0x00, 0xFE, 0xFF };
    static const std::array<uint8_t, 4> UTF32_BE_BOM = { 0xFF, 0xFE, 0x00, 0x00 };
    static const std::array<uint8_t, 3> UTF8_BOM     = { 0xEF, 0xBB, 0xBF };
    static const std::array<uint8_t, 2> UTF16_LE_BOM = { 0xFF, 0xFE };
    static const std::array<uint8_t, 2> UTF16_BE_BOM = { 0xFE, 0xFF };


    if( bom.size() >= UTF32_LE_BOM.size() && std::equal( UTF32_LE_BOM.begin(), UTF32_LE_BOM.end(), bom.begin() ) ) {
        bom.erase( bom.begin(), std::next( bom.begin(), UTF32_LE_BOM.size() ) );
        return Format::UTF32_LE;

    } else if( bom.size() >= UTF32_BE_BOM.size() && std::equal( UTF32_BE_BOM.begin(), UTF32_BE_BOM.end(), bom.begin() ) ) {
        bom.erase( bom.begin(), std::next( bom.begin(), UTF32_BE_BOM.size() ) );
        return Format::UTF32_BE;

    } else if( bom.size() >= UTF8_BOM.size() && std::equal( UTF8_BOM.begin(), UTF8_BOM.end(), bom.begin() ) ) {
        bom.erase( bom.begin(), std::next( bom.begin(), UTF8_BOM.size() ) );
        return Format::UTF8;

    } else if( bom.size() >= UTF16_LE_BOM.size() && std::equal( UTF16_LE_BOM.begin(), UTF16_LE_BOM.end(), bom.begin() ) ) {
        bom.erase( bom.begin(), std::next( bom.begin(), UTF16_LE_BOM.size() ) );
        return Format::UTF16_LE;

    } else if( bom.size() >= UTF16_BE_BOM.size() && std::equal( UTF16_BE_BOM.begin(), UTF16_BE_BOM.end(), bom.begin() ) ) {
        bom.erase( bom.begin(), std::next( bom.begin(), UTF16_BE_BOM.size() ) );
        return Format::UTF16_BE;
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

    for( const auto &cp: in ) {
        out << ( char ) ( ( cp & 0b0000'0000'0000'0000'0000'0000'1111'1111 ) )
            << ( char ) ( ( cp & 0b0000'0000'0000'0000'1111'1111'0000'0000 ) >> 8 )
            << ( char ) ( ( cp & 0b0000'0000'1111'1111'0000'0000'0000'0000 ) >> 16 )
            << ( char ) ( ( cp & 0b1111'1111'0000'0000'0000'0000'0000'0000 ) >> 24 );
    }

    return true;
}

/**
 * Converts an UTF-8 encoded byte stream to UTF-32
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U8toU32( Source &src, std::vector<uint32_t> &out ) {
    auto  & in  = src.stream();
    auto  & pos = src.position();
    uint8_t buffer[4];

    while( in.good() && !in.eof() && in.peek() >= 0 ) { //TODO optimise
        auto expected = unicode::utf8::bytelength( in.peek() );
        auto received = 0;

        if( expected == 0 ) {
            logging::ParserLog::log( src.path(),
                                     specs::Context::BLOGATOR,
                                     specs::blogator::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                     pos
            );
            //TODO log ERROR "Invalid UTF8 start byte: " + unicode::utf8::toxunicode( in.peek(), "" )
            return false; //EARLY RETURN
        }

        while( received < expected && !in.eof() ) {
            buffer[received] = in.get();
            ++received;
        }

        if( received == expected ) {
            uint32_t codepoint = unicode::utf8::toU32( buffer, received );

            pos.increment( unicode::ascii::isnewline( codepoint ) );
            out.emplace_back( codepoint );

        } else {
            logging::ParserLog::log( src.path(),
                                     specs::Context::BLOGATOR,
                                     specs::blogator::ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
                                     pos
            );
            //TODO log ERROR "EOF reached mid-codepoint sequence."
            return false; //EARLY RETURN
        }
    }

    return true;
}

/**
 * Converts an UTF-8 encoded byte stream to UTF-32
 * @param pre_buffer Buffered input bytes
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U8toU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out ) { //TODO smoke test
    auto & in  = src.stream();
    auto & pos = src.position();

    if( !pre_buffer.empty() ) {
        while( !pre_buffer.empty() ) {
            const auto byte_length   = unicode::utf8::bytelength( pre_buffer.front() );
            const auto missing_bytes = ( byte_length > pre_buffer.size() ? ( byte_length - pre_buffer.size() ) : 0 );

            for( int b = 0; !in.eof() && in.good() && b < missing_bytes; ++b ) {
                pre_buffer.emplace_back( in.get() );
            }

            if( byte_length > pre_buffer.size() ) {
                logging::ParserLog::log( src.path(),
                                         specs::Context::BLOGATOR,
                                         specs::blogator::ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
                                         pos
                );

                return false; //EARLY RETURN
            }

            uint32_t codepoint = 0x00;

            if( byte_length == 1 ) {
                codepoint = unicode::utf8::toU32( pre_buffer[0] );
            } else if( byte_length == 2 ) {
                codepoint = unicode::utf8::toU32( pre_buffer[0], pre_buffer[1] );
            } else if( byte_length == 3 ) {
                codepoint = unicode::utf8::toU32( pre_buffer[0], pre_buffer[1], pre_buffer[2] );
            } else if( byte_length == 4 ) {
                codepoint = unicode::utf8::toU32( pre_buffer[0], pre_buffer[1], pre_buffer[2], pre_buffer[3] );
            } else {
                logging::ParserLog::log( src.path(),
                                         specs::Context::BLOGATOR,
                                         specs::blogator::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                         pos
                );

                return false; //EARLY RETURN
            }

            pos.increment( unicode::ascii::isnewline( codepoint ) );
            out.emplace_back( codepoint );

            for( auto i = 0; i < byte_length; ++i ) {
                pre_buffer.pop_front();
            }
        }
    }

    return U8toU32( src, out );
}

bool Transcode::U16BEtoU32( Source &src, std::vector<uint32_t> &out ) { //TODO
    auto & in  = src.stream();
    auto & pos = src.position();

    return false;
}

bool Transcode::U16BEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out ) { //TODO
    auto & in  = src.stream();
    auto & pos = src.position();
    //TODO pre_buffer
    return U16BEtoU32( src, out );
}

bool Transcode::U16LEtoU32( Source &src, std::vector<uint32_t> &out ) { //TODO
    auto & in  = src.stream();
    auto & pos = src.position();

    uint16_t high_surrogate = 0x00;
    uint16_t low_surrogate  = 0x00;

    while( in.good() && !in.eof() ) {

        uint8_t high_surrogate_bytes[2];
        uint8_t low_surrogate_bytes [2];

        if constexpr( std::endian::native == std::endian::little ) {
            high_surrogate_bytes[0] = in.get();



        } else { //big-endian

        }


        in.read( (char *) &high_surrogate_bytes, sizeof( high_surrogate_bytes ) );

        if( high_surrogate <= 0xFFFF ) { //1 x 16 bits
            if( high_surrogate > 0xD7FF && high_surrogate < 0xE000 ) {
//                logging::ParserLog::log() //TODO
            } else {
                out.emplace_back( (uint32_t) high_surrogate );
            }

        } else { //0x10000 - 0x10FFFF: 2 x 16 bits
            in.read( (char *) &low_surrogate_bytes, sizeof( low_surrogate_bytes ) );

            //TODO calculate code point
        }

        out.emplace_back( high_surrogate );
    }

    return true;
}

bool Transcode::U16LEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out ) { //TODO
    auto & in  = src.stream();
    auto & pos = src.position();
    //TODO pre_buffer
    return U16LEtoU32( src, out );
}

/**
 * Converts content of a UTF-32 little-endian byte stream to a collection of u32 characters
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32LEtoU32( Source &src, std::vector<uint32_t> &out ) {
    auto & in  = src.stream();
    auto & pos = src.position();

    uint8_t bytes[4];
    in.read( ( char * ) &bytes, 4 );

    while( in.good() && !in.eof() ) {
        uint32_t codepoint = 0x00;
        codepoint += ( uint32_t ) bytes[0];
        codepoint += ( uint32_t ) bytes[1] <<  8;
        codepoint += ( uint32_t ) bytes[2] << 16;
        codepoint += ( uint32_t ) bytes[3] << 24;

        pos.increment( unicode::ascii::isnewline( codepoint ) );
        out.emplace_back( codepoint );
        in.read( ( char * ) &bytes, 4 );
    }

    return true;
}

/**
 * Converts content of a UTF-32 little-endian byte stream to a collection of u32 characters
 * @param pre_buffer Buffered input bytes
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32LEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out ) {
    const auto hasIncompleteCodePoint = []( std::deque<uint8_t> &buffer ) { return ( ( buffer.size() % 4 ) > 0 ); };

    auto & in  = src.stream();
    auto & pos = src.position();

    if( !pre_buffer.empty() ) {
        if( hasIncompleteCodePoint( pre_buffer ) ) {
            size_t remainder = ( pre_buffer.size() % 4 );
            size_t required  = ( 4 - remainder );

            for( auto i = 0; !in.eof() && in.good() && i < required; ++i ) {
                pre_buffer.emplace_back( in.get() );
            }
        }

        const size_t code_point_count = ( pre_buffer.size() / 4 );

        for( int i = 0, byte_i = 0; i < code_point_count; ++i, byte_i += 4 ) {
            uint32_t codepoint = 0x00;

            codepoint += (uint32_t) pre_buffer.at( byte_i );
            codepoint += (uint32_t) pre_buffer.at( byte_i + 1 ) <<  8;
            codepoint += (uint32_t) pre_buffer.at( byte_i + 2 ) << 16;
            codepoint += (uint32_t) pre_buffer.at( byte_i + 3 ) << 24;

            pos.increment( unicode::ascii::isnewline( codepoint ) );
            out.emplace_back( codepoint );
        }

        if( hasIncompleteCodePoint( pre_buffer ) ) {
            logging::ParserLog::log( src.path(),
                                     specs::Context::BLOGATOR,
                                     specs::blogator::ErrorCode::INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM,
                                     pos.increment()
            );

            return false; //EARLY RETURN
        }
    }

    return U32LEtoU32( src, out );
}

/**
 * Converts content of a UTF-32 Big-endian byte stream to a collection of u32 characters
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32BEtoU32( Source &src, std::vector<uint32_t> &out ) {
    auto & in  = src.stream();
    auto & pos = src.position();

    uint8_t bytes[4];
    in.read( ( char * ) &bytes, 4 );

    while( in.good() && !in.eof() ) {
        uint32_t codepoint = 0x00;
        codepoint += ( uint32_t ) bytes[0] << 24;
        codepoint += ( uint32_t ) bytes[1] << 16;
        codepoint += ( uint32_t ) bytes[2] <<  8;
        codepoint += ( uint32_t ) bytes[3];

        pos.increment( unicode::ascii::isnewline( codepoint ) );
        out.emplace_back( codepoint );
        in.read( ( char * ) &bytes, 4 );
    }

    return true;
}

/**
 * Converts content of a UTF-32 big-endian byte stream to a collection of u32 characters
 * @param pre_buffer Buffered input bytes
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32BEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out ) {
    const auto hasIncompleteCodePoint = []( std::deque<uint8_t> &buffer ) { return ( ( buffer.size() % 4 ) > 0 ); };

    auto & in  = src.stream();
    auto & pos = src.position();

    if( !pre_buffer.empty() ) {
        if( hasIncompleteCodePoint( pre_buffer ) ) {
            size_t remainder = ( pre_buffer.size() % 4 );
            size_t required  = ( 4 - remainder );

            for( auto i = 0; !in.eof() && in.good() && i < required; ++i ) {
                pre_buffer.emplace_back( in.get() );
            }
        }

        const size_t code_point_count = ( pre_buffer.size() / 4 );

        for( int i = 0, byte_i = 0; i < code_point_count; ++i, byte_i += 4 ) {
            uint32_t codepoint = 0x00;

            codepoint += ( uint32_t ) pre_buffer.at( byte_i )        << 24;
            codepoint += ( uint32_t ) pre_buffer.at( byte_i + 1 ) << 16;
            codepoint += ( uint32_t ) pre_buffer.at( byte_i + 2 ) <<  8;
            codepoint += ( uint32_t ) pre_buffer.at( byte_i + 3 );

            pos.increment( unicode::ascii::isnewline( codepoint ) );
            out.emplace_back( codepoint );
        }

        if( hasIncompleteCodePoint( pre_buffer ) ) {
            logging::ParserLog::log( src.path(),
                                     specs::Context::BLOGATOR,
                                     specs::blogator::ErrorCode::INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM,
                                     pos.increment()
            );

            return false; //EARLY RETURN
        }
    }

    return U32BEtoU32( src, out );
}
