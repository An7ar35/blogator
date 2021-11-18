#include "Transcode.h"

#include <sstream>

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
 * Grabs a code unit of n bytes from a stream
 * @param stream Source stream
 * @param buffer Byte buffer
 * @param byte_ln Number of bytes to get
 * @return Number of bytes fetched successfully
 */
size_t Transcode::fetchCodeUnit(std::istream &stream, uint8_t * buffer, size_t byte_ln ) {
    size_t byte_n = 0;

    while( stream.good() && byte_n < byte_ln  ) {
        buffer[ byte_n ] = stream.get();
        ++byte_n;
    }

    return byte_n;
}

/**
 * Adds a UTF-32 codepoint to an output UTF-32 collection
 * @param src Source
 * @param prev_codepoint Previous codepoint (used to help with normalizing newlines)
 * @param new_codepoint  New codepoint to add
 * @param out Output collection
 */
void Transcode::addCodePoint( Source &src, uint32_t prev_codepoint, uint32_t new_codepoint, std::vector<uint32_t> &out ) {
    auto & pos = src.position();

    if( unicode::utf32::isnonchar( new_codepoint ) ) {
        pos.increment( false );
        out.emplace_back( new_codepoint );

        logging::ParserLog::log( src.path(),
                                 specs::Context::HTML5,
                                 specs::html5::ErrorCode::NONCHARACTER_IN_INPUT_STREAM,
                                 pos );

    } else if( new_codepoint != 0x00 && unicode::ascii::iscntrl( new_codepoint ) && !unicode::ascii::iswspace( new_codepoint ) ) {
        pos.increment( false );
        out.emplace_back( new_codepoint );

        logging::ParserLog::log( src.path(),
                                 specs::Context::HTML5,
                                 specs::html5::ErrorCode::CONTROL_CHARACTER_IN_INPUT_STREAM,
                                 pos );

    } else if( unicode::ascii::isnewline( new_codepoint ) ) {
        if( !( prev_codepoint == unicode::CR && new_codepoint == unicode::LF ) ) {
            pos.increment( true );
            out.emplace_back( unicode::LF );
        }

    } else {
        pos.increment( false );
        out.emplace_back( new_codepoint );
    }
}

/**
 * Converts an UTF-8 encoded byte stream to UTF-32
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U8toU32( Source &src, std::vector<uint32_t> &out ) {
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;
    uint8_t  buffer[4];

    while( in.good() && !in.eof() && in.peek() >= 0 ) {
        auto expected = unicode::utf8::bytelength( in.peek() );

        if( expected == 0 ) {
            logging::ParserLog::log( src.path(),
                                     specs::Context::BLOGATOR,
                                     specs::blogator::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                     pos
            );
            //TODO log ERROR "Invalid UTF8 start byte: " + unicode::utf8::toxunicode( in.peek(), "" )
            return false; //EARLY RETURN
        }

        auto received = fetchCodeUnit( in, buffer, expected );

        if( received == expected ) {
            uint32_t codepoint = unicode::utf8::toU32( buffer, received );

            Transcode::addCodePoint( src, prev, codepoint, out );
            prev = codepoint;

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
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;

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
                codepoint = unicode::utf8::toU32( pre_buffer[0],
                                                  pre_buffer[1] );
            } else if( byte_length == 3 ) {
                codepoint = unicode::utf8::toU32( pre_buffer[0],
                                                  pre_buffer[1],
                                                  pre_buffer[2] );
            } else if( byte_length == 4 ) {
                codepoint = unicode::utf8::toU32( pre_buffer[0],
                                                  pre_buffer[1],
                                                  pre_buffer[2],
                                                  pre_buffer[3] );
            } else {
                logging::ParserLog::log( src.path(),
                                         specs::Context::BLOGATOR,
                                         specs::blogator::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                         pos
                );

                return false; //EARLY RETURN
            }

            Transcode::addCodePoint( src, prev, codepoint, out );
            prev = codepoint;

            for( auto i = 0; i < byte_length; ++i ) {
                pre_buffer.pop_front();
            }
        }
    }

    return Transcode::U8toU32( src, out );
}

bool Transcode::U16BEtoU32( Source &src, std::vector<uint32_t> &out ) { //TODO
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;

    return false;
}

bool Transcode::U16BEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out ) { //TODO
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    //TODO pre_buffer
    return Transcode::U16BEtoU32( src, out );
}

bool Transcode::U16LEtoU32( Source &src, std::vector<uint32_t> &out ) { //TODO
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;
    bool     eof  = ( in.good() && !in.eof() );

    uint8_t high_surrogate_bytes[2];
    uint8_t low_surrogate_bytes [2];

    while( !eof ) {
        uint32_t codepoint = 0x00;
        size_t   bytes     = fetchCodeUnit( in, high_surrogate_bytes, 2 );

        if( bytes == 2 ) {
            auto high_surrogate = unicode::utf16::join( high_surrogate_bytes[1], high_surrogate_bytes[0] );
            auto code_units     = unicode::utf16::codeunits( high_surrogate );

            switch( code_units ) {
                case 1: {
                    codepoint = unicode::utf16::toU32( high_surrogate );
                } break;

                case 2: {
                    if( fetchCodeUnit( in, low_surrogate_bytes, 2 ) != 2 ) {
                        logging::ParserLog::log( src.path(),
                                                 specs::Context::BLOGATOR,
                                                 specs::blogator::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                                 pos
                        );

                        return false; //EARLY RETURN
                    }

                    const auto low_surrogate = unicode::utf16::join( low_surrogate_bytes[1], low_surrogate_bytes[0] );

                    if( !unicode::utf16::islowsurrogate( low_surrogate ) ) {
                        logging::ParserLog::log( src.path(),
                                                 specs::Context::HTML5,
                                                 specs::html5::ErrorCode::SURROGATE_IN_INPUT_STREAM,
                                                 pos );
                        //TODO log error: low surrogate in invalid range
                        //TODO decide what to do with that
                    }

                    codepoint = unicode::utf16::toU32( high_surrogate, low_surrogate );
                } break;

                default: {
                    codepoint = unicode::utf16::toU32( high_surrogate );

                    logging::ParserLog::log( src.path(),
                                             specs::Context::HTML5,
                                             specs::html5::ErrorCode::SURROGATE_IN_INPUT_STREAM,
                                             pos );

                    //TODO log error: high surrogate in low surrogate range
                }
            }

            Transcode::addCodePoint( src, prev, codepoint, out );
            prev = codepoint;

        } else { //EOF and error-control cases
            if( bytes == 0 ) {
                eof = true;

            } else {
                logging::ParserLog::log( src.path(),
                                         specs::Context::BLOGATOR,
                                         specs::blogator::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                         pos
                );

                return false; //EARLY RETURN
            }
        }
    }

    return true;
}

bool Transcode::U16LEtoU32( std::deque<uint8_t> &pre_buffer, Source &src, std::vector<uint32_t> &out ) { //TODO
    auto & in  = src.stream();
    auto & pos = src.position();
    //TODO pre_buffer
    return Transcode::U16LEtoU32( src, out );
}

/**
 * Converts content of a UTF-32 little-endian byte stream to a collection of u32 characters
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32LEtoU32( Source &src, std::vector<uint32_t> &out ) {
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;

    uint8_t bytes[4];
    in.read( ( char * ) &bytes, 4 );

    while( in.good() && !in.eof() ) {
        uint32_t codepoint = unicode::utf32::join( bytes[3],
                                                   bytes[2],
                                                   bytes[1],
                                                   bytes[0] );

        Transcode::addCodePoint( src, prev, codepoint, out );
        prev = codepoint;
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

    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;

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
            uint32_t codepoint = unicode::utf32::join( pre_buffer.at( byte_i + 3 ),
                                                       pre_buffer.at( byte_i + 2 ),
                                                       pre_buffer.at( byte_i + 1 ),
                                                       pre_buffer.at( byte_i ) );

            Transcode::addCodePoint( src, prev, codepoint, out );
            prev = codepoint;
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

    return Transcode::U32LEtoU32( src, out );
}

/**
 * Converts content of a UTF-32 Big-endian byte stream to a collection of u32 characters
 * @param src Source
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32BEtoU32( Source &src, std::vector<uint32_t> &out ) {
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;

    uint8_t bytes[4];
    in.read( ( char * ) &bytes, 4 );

    while( in.good() && !in.eof() ) {
        uint32_t codepoint = unicode::utf32::join( bytes[0],
                                                   bytes[1],
                                                   bytes[2],
                                                   bytes[3] );

        Transcode::addCodePoint( src, prev, codepoint, out );
        prev = codepoint;
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

    auto &   in   = src.stream();
    auto &   pos  = src.position();
    uint32_t prev = 0x00;

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
            uint32_t codepoint = unicode::utf32::join( pre_buffer.at( byte_i ),
                                                       pre_buffer.at( byte_i + 1 ),
                                                       pre_buffer.at( byte_i + 2 ),
                                                       pre_buffer.at( byte_i + 3 ) );

            Transcode::addCodePoint( src, prev, codepoint, out );
            prev = codepoint;
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

    return Transcode::U32BEtoU32( src, out );
}