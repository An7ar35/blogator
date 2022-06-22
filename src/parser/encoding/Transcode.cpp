#include "Transcode.h"

#include <sstream>

#include "../../unicode/unicode.h"
#include "../../logger/Logger.h"
#include "../logging/ParserLog.h"

using namespace blogator::parser::encoding;

/**
 * Output stream operator
 * @param os Output stream
 * @param type Endianness enum
 * @return Output stream
 */
std::ostream &blogator::parser::encoding::operator <<( std::ostream &os, Endianness e ) {
    switch( e ) {
        case Endianness::LE: { os << "LE";      } break;
        case Endianness::BE: { os << "BE";      } break;
        default:             { os << "UNKNOWN"; } break;
    }

    return os;
}

/**
 * Converts an input byte stream into UTF-32
 * @param src Source (if format == UNKNOWN then sniffing attempt will be made for the BOM)
 * @param out UTF-32 output
 * @return Success
 * @throws std::invalid_argument when format is unknown or unsupported
 */
bool Transcode::convert( Source &src, std::vector<char32_t> &out ) {
    auto & in = src.stream();

    std::deque<char8_t> buffer;

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
        case Format::UTF16_LE: [[fallthrough]];
        case Format::UTF16_BE: return Transcode::U16toU32( buffer, src, out );
        case Format::UTF32_LE: [[fallthrough]];
        case Format::UTF32_BE: return Transcode::U32toU32( buffer, src, out );
        case Format::UNKNOWN : throw std::invalid_argument( "Failed to detect format of input." );
        default              : throw std::invalid_argument( "Input format not supported." );
    }
}

/**
 * BOM sniffing (UTF8/16LE/16BE/32LE/32BE)
 * @param bom Buffer containing a BOM at the beginning
 * @return Format deduced from BOM
 */
Format Transcode::sniffBOM( std::deque<char8_t> &bom ) {
    static const std::array<char8_t, 4> UTF32_LE_BOM = { 0x00, 0x00, 0xFE, 0xFF };
    static const std::array<char8_t, 4> UTF32_BE_BOM = { 0xFF, 0xFE, 0x00, 0x00 };
    static const std::array<char8_t, 3> UTF8_BOM     = { 0xEF, 0xBB, 0xBF };
    static const std::array<char8_t, 2> UTF16_LE_BOM = { 0xFF, 0xFE };
    static const std::array<char8_t, 2> UTF16_BE_BOM = { 0xFE, 0xFF };

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
 * @param endianness Output endianness
 * @return Success
 */
bool Transcode::U32toByteStream( const std::u32string &in, std::ostream &out, Endianness endianness ) {
    if( out.bad() || out.eof() ) {
        LOG_ERROR( "[parser::encoding::Transcode::U32toByteStream( const std::u32string &, std::ostream &, ", blogator::to_string( endianness ), " )] "
                   "Output stream eof/bad bit(s) set." );
        return false; //EARLY RETURN
    }

    if( endianness == Endianness::LE ) {
        for( const auto &cp: in ) {
            out << ( char ) ( ( cp & 0b0000'0000'0000'0000'0000'0000'1111'1111 ) )
                << ( char ) ( ( cp & 0b0000'0000'0000'0000'1111'1111'0000'0000 ) >> 8 )
                << ( char ) ( ( cp & 0b0000'0000'1111'1111'0000'0000'0000'0000 ) >> 16 )
                << ( char ) ( ( cp & 0b1111'1111'0000'0000'0000'0000'0000'0000 ) >> 24 );
        }

    } else { //Endianness::BE
        for( const auto &cp: in ) {
            out << ( char ) ( ( cp & 0b1111'1111'0000'0000'0000'0000'0000'0000 ) >> 24 )
                << ( char ) ( ( cp & 0b0000'0000'1111'1111'0000'0000'0000'0000 ) >> 16 )
                << ( char ) ( ( cp & 0b0000'0000'0000'0000'1111'1111'0000'0000 ) >> 8 )
                << ( char ) ( ( cp & 0b0000'0000'0000'0000'0000'0000'1111'1111 ) );
        }
    }

    out.flush();

    if( out.bad() || out.fail() ) {
        LOG_ERROR( "[parser::encoding::Transcode::U32toByteStream( const std::u32string &, std::ostream &, ", blogator::to_string( endianness ), " )] "
                   "Output stream eof/bad bit(s) set (post flush)." );
        return false; //EARLY RETURN
    }

    return true;
}

/**
 * Converts a UTF-32 collection to a 8-bit stream
 * @param in UTF-32 collection
 * @param out Output byte stream
 * @param endianness Output endianness
 * @return Success
 */
bool Transcode::U32toByteStream( const std::vector<char32_t> &in, std::ostream &out, Endianness endianness ) {
    if( out.bad() || out.eof() ) {
        LOG_ERROR( "[parser::encoding::Transcode::U32toByteStream( const std::vector<char32_t> &, std::ostream &, ", blogator::to_string( endianness ), " )] "
                   "Output stream eof/bad bit(s) set." );
        return false; //EARLY RETURN
    }

    if( endianness == Endianness::LE ) {
        for( const auto &cp: in ) {
            out << ( char ) ( ( cp & 0b0000'0000'0000'0000'0000'0000'1111'1111 ) )
                << ( char ) ( ( cp & 0b0000'0000'0000'0000'1111'1111'0000'0000 ) >> 8 )
                << ( char ) ( ( cp & 0b0000'0000'1111'1111'0000'0000'0000'0000 ) >> 16 )
                << ( char ) ( ( cp & 0b1111'1111'0000'0000'0000'0000'0000'0000 ) >> 24 );
        }

    } else { //Endianness::BE
        for( const auto &cp: in ) {
            out << ( char ) ( ( cp & 0b1111'1111'0000'0000'0000'0000'0000'0000 ) >> 24 )
                << ( char ) ( ( cp & 0b0000'0000'1111'1111'0000'0000'0000'0000 ) >> 16 )
                << ( char ) ( ( cp & 0b0000'0000'0000'0000'1111'1111'0000'0000 ) >> 8 )
                << ( char ) ( ( cp & 0b0000'0000'0000'0000'0000'0000'1111'1111 ) );
        }
    }

    out.flush();

    if( out.bad() || out.fail() ) {
        LOG_ERROR( "[parser::encoding::Transcode::U32toByteStream( const std::vector<char32_t> &, std::ostream &, ", blogator::to_string( endianness ), " )] "
                   "Output stream eof/bad bit(s) set (post flush)." );
        return false; //EARLY RETURN
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
size_t Transcode::fetchCodeUnit( std::istream &stream, char8_t * buffer, size_t byte_ln ) {
    size_t byte_n = 0;

    while( stream.good() && byte_n < byte_ln  ) {
        buffer[ byte_n ] = stream.get();

        if( stream ) {
            ++byte_n;
        }
    }

    return byte_n;
}

/**
 * Grabs a code unit of n bytes from a stream
 * @param stream Source stream
 * @param buffer Byte buffer
 * @param byte_ln Number of bytes to get
 * @return Number of bytes fetched successfully
 */
size_t Transcode::fetchCodeUnit( std::istream &stream, std::deque<char8_t> &buffer, size_t byte_ln ) {
    size_t byte_n = 0;

    while( stream.good() && byte_n < byte_ln  ) {
        buffer.emplace_back( stream.get() );

        if( stream ) {
            ++byte_n;
        }
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
void Transcode::addCodePoint( Source &src, char32_t prev_codepoint, char32_t new_codepoint, std::vector<char32_t> &out ) {
    auto & pos = src.position();

    if( unicode::utf32::isnonchar( new_codepoint ) ) {
        logging::ParserLog::log( src.path(),
                                 specs::Context::HTML5,
                                 specs::infra::ErrorCode::NONCHARACTER_IN_INPUT_STREAM,
                                 pos );

        pos.increment( false );
        out.emplace_back( new_codepoint );

    } else if( new_codepoint != 0x00 && unicode::ascii::iscntrl( new_codepoint ) && !unicode::ascii::iswspace( new_codepoint ) ) {
        logging::ParserLog::log( src.path(),
                                 specs::Context::HTML5,
                                 specs::infra::ErrorCode::CONTROL_CHARACTER_IN_INPUT_STREAM,
                                 pos );

        pos.increment( false );
        out.emplace_back( new_codepoint );

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
bool Transcode::U8toU32( Source &src, std::vector<char32_t> &out ) {
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    char32_t prev = 0x00;
    char8_t  buffer[4];

    while( in.good() && !in.eof() && in.peek() >= 0 ) {
        auto expected = unicode::utf8::bytelength( in.peek() );

        if( expected == 0 ) {
            logging::ParserLog::log( src.path(),
                                     specs::Context::NATIVE,
                                     specs::native::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                     pos
            );

            LOG_ERROR( "[parser::encoding::Transcode::U8toU32( Source &, std::vector<char32_t> & )] "
                       "Invalid UTF-8 start byte: ", unicode::utf8::toxunicode( (char8_t) in.peek(), "0x" ) );
            return false; //EARLY RETURN
        }

        auto received = fetchCodeUnit( in, buffer, expected );

        if( received == expected ) {
            char32_t codepoint = unicode::utf8::toU32( buffer, received );

            Transcode::addCodePoint( src, prev, codepoint, out );
            prev = codepoint;

        } else {
            logging::ParserLog::log( src.path(),
                                     specs::Context::NATIVE,
                                     specs::native::ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
                                     pos
            );

            std::stringstream ss;
            for( auto i = 0; i < received; ++i ) {
                ss << " " << unicode::utf8::toxunicode( (char8_t) buffer[i], "0x" );
            }

            LOG_ERROR( "[parser::encoding::Transcode::U8toU32( Source &, std::vector<char32_t> & )] "
                       "EOF reached mid-codepoint sequence. Got:", ss.str() );

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
bool Transcode::U8toU32( std::deque<char8_t> &pre_buffer, Source &src, std::vector<char32_t> &out ) {
    auto &   in   = src.stream();
    auto &   pos  = src.position();
    char32_t prev = 0x00;

    if( !pre_buffer.empty() ) {
        while( !pre_buffer.empty() ) {
            const auto byte_length   = unicode::utf8::bytelength( pre_buffer.front() );
            const auto missing_bytes = ( byte_length > pre_buffer.size() ? ( byte_length - pre_buffer.size() ) : 0 );

            for( int b = 0; !in.eof() && in.good() && b < missing_bytes; ++b ) {
                pre_buffer.emplace_back( in.get() );
            }

            if( byte_length > pre_buffer.size() ) {
                logging::ParserLog::log( src.path(),
                                         specs::Context::NATIVE,
                                         specs::native::ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
                                         pos
                );

                return false; //EARLY RETURN
            }

            char32_t codepoint = 0x00;

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
                                         specs::Context::NATIVE,
                                         specs::native::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                         pos
                );

                LOG_ERROR( "[parser::encoding::Transcode::U8toU32( std::deque<char8_t> &, Source &, std::vector<char32_t> & )] "
                           "Invalid UTF-8 start byte: ", unicode::utf8::toxunicode( pre_buffer[0], "0x" ) );

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

/**
 * Converts a UTF-16BE encoded byte stream to UTF-32
 * @param src Source (sets its format to `UTF16_BE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U16BEtoU32( Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF16_BE );
    return Transcode::U16toU32( src, out );
}

/**
 * Converts a UTF-16BE encoded byte stream to UTF-32
 * @param pre_buffer Buffered input bytes
 * @param src Source (sets its format to `UTF16_BE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U16BEtoU32( std::deque<char8_t> &pre_buffer, Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF16_BE );
    return Transcode::U16toU32( pre_buffer, src, out );
}

/**
 * Converts an UTF-16LE encoded byte stream to UTF-32
 * @param src Source (sets its format to `UTF16_LE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U16LEtoU32( Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF16_LE );
    return Transcode::U16toU32( src, out );
}

/**
 * Converts an UTF-16LE encoded byte stream to UTF-32
 * @param pre_buffer Buffered input bytes
 * @param src Source (sets its format to `UTF16_LE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U16LEtoU32( std::deque<char8_t> &pre_buffer, Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF16_LE );
    return Transcode::U16toU32( pre_buffer, src, out );
}

/**
 * Converts content of a UTF-32 little-endian byte stream to a collection of u32 characters
 * @param src Source (sets its format to `UTF32_LE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32LEtoU32( Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF32_LE );
    return Transcode::U32toU32( src, out );
}

/**
 * Converts content of a UTF-32 little-endian byte stream to a collection of u32 characters
 * @param pre_buffer Buffered input bytes
 * @param src Source (sets its format to `UTF32_LE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32LEtoU32( std::deque<char8_t> &pre_buffer, Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF32_LE );
    return Transcode::U32toU32( pre_buffer, src, out );
}

/**
 * Converts content of a UTF-32 Big-endian byte stream to a collection of u32 characters
 * @param src Source (sets its format to `UTF32_BE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32BEtoU32( Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF32_BE );
    return Transcode::U32toU32( src, out );
}

/**
 * Converts content of a UTF-32 big-endian byte stream to a collection of u32 characters
 * @param pre_buffer Buffered input bytes
 * @param src Source (sets its format to `UTF32_BE`)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32BEtoU32( std::deque<char8_t> &pre_buffer, Source &src, std::vector<char32_t> &out ) {
    src.setFormat( Format::UTF32_BE );
    return Transcode::U32toU32( pre_buffer, src, out );
}


/**
 * Byte-joining for U16 little endian
 * @param byte1 First byte in order
 * @param byte2 Second byte in order
 * @return U16 code unit
 */
char16_t Transcode::joinU16LE( char8_t byte1, char8_t byte2 ) noexcept {
    return unicode::utf16::join( byte2, byte1 );
}

/**
 * Byte joining for U32 little endian
 * @param byte1 First byte in order
 * @param byte2 Second byte in order
 * @param byte3 Third byte in order
 * @param byte4 Fourth byte in order
 * @return U32 code unit
 */
char32_t Transcode::joinU32LE( char8_t byte1, char8_t byte2, char8_t byte3, char8_t byte4 ) noexcept {
    return unicode::utf32::join( byte4, byte3, byte2, byte1 );
}

/**
 * Converts content of a UTF-32 byte stream to a collection of u32 characters
 * @param pre_buffer Buffered input bytes
 * @param src Source (UTF-16 LE/BE format must be set prior)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U16toU32( std::deque<char8_t> &pre_buffer, Source &src, std::vector<char32_t> &out ) {
    const auto join = ( src.format() == Format::UTF16_LE ? Transcode::joinU16LE : unicode::utf16::join );
    auto &     in   = src.stream();
    auto &     pos  = src.position();
    char32_t   prev = 0x00;

    if( !pre_buffer.empty() ) {
        while( !pre_buffer.empty() ) {
            if( pre_buffer.size() == 1 && ( fetchCodeUnit( in, pre_buffer, 1 ) != 1 ) ) {
                logging::ParserLog::log( src.path(),
                                         specs::Context::NATIVE,
                                         specs::native::ErrorCode::INCOMPLETE_UTF16_HIGH_SURROGATE_IN_INPUT_STREAM,
                                         pos
                );

                LOG_ERROR( "[parser::encoding::Transcode::U16toU32( std::deque<char8_t> &, Source &, std::vector<char32_t> & )] "
                           "Failed to complete high-surrogate: ", unicode::utf8::toxunicode( pre_buffer[0], "0x" ), " 0x??" );

                return false; //EARLY RETURN
            }

            char32_t   codepoint      = 0x00;
            auto       high_surrogate = join( pre_buffer[0], pre_buffer[1] );
            auto       byte_length    = unicode::utf16::codeunits( high_surrogate ) * 2;
            const auto missing_bytes  = ( byte_length > pre_buffer.size() ? ( byte_length - pre_buffer.size() ) : 0 );

            if( fetchCodeUnit( in, pre_buffer, missing_bytes ) != missing_bytes ) {
                logging::ParserLog::log( src.path(),
                                         specs::Context::NATIVE,
                                         specs::native::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                         pos
                );

                LOG_ERROR( "[parser::encoding::Transcode::U16toU32( std::deque<char8_t> &, Source &, std::vector<char32_t> & )] "
                           "Failed to complete codepoint: ", unicode::utf8::toxunicode( high_surrogate, "0x" ), " 0x????" );

                return false; //EARLY RETURN
            };

            if( byte_length == 2 ) { //i.e. high surrogate code unit
                codepoint = unicode::utf16::toU32( high_surrogate );
                Transcode::addCodePoint( src, prev, codepoint, out );
                prev = codepoint;

            } else if( byte_length == 4 ) { //i.e. high-low surrogate pair
                auto low_surrogate  = join( pre_buffer[2], pre_buffer[3] );

                if( unicode::utf16::islowsurrogate( low_surrogate ) ) {
                    codepoint = unicode::utf16::toU32( high_surrogate, low_surrogate );
                    Transcode::addCodePoint( src, prev, codepoint, out );
                    prev = codepoint;

                } else {
                    logging::ParserLog::log( src.path(),
                                             specs::Context::NATIVE,
                                             specs::native::ErrorCode::INVALID_UTF16_SURROGATE_PAIR,
                                             pos );

                    LOG_WARNING( "[parser::encoding::Transcode::U16toU32( std::deque<char8_t> &, Source &, std::vector<char32_t> & )] "
                                 "Invalid low surrogate: ", unicode::utf8::toxunicode( high_surrogate, "0x" ), " ",
                                 unicode::utf8::toxunicode( low_surrogate, "0x" ) );
                }

            } else {
                logging::ParserLog::log( src.path(),
                                         specs::Context::HTML5,
                                         specs::infra::ErrorCode::SURROGATE_IN_INPUT_STREAM,
                                         pos );

                LOG_WARNING( "[parser::encoding::Transcode::U16toU32( std::deque<char8_t> &, Source &, std::vector<char32_t> & )] "
                             "High surrogate in low surrogate range: ", unicode::utf8::toxunicode( high_surrogate, "0x" ) );

                codepoint = unicode::utf16::toU32( high_surrogate ); //should be dealt with by the tokeniser
                Transcode::addCodePoint( src, prev, codepoint, out );
                prev        = codepoint;
                byte_length = ( byte_length == 0 ? 2 : byte_length ); //discard the code unit
            }

            for( auto i = 0; i < byte_length; ++i ) {
                pre_buffer.pop_front();
            }
        }
    }

    return U16toU32( src, out );
}

/**
 * Converts an UTF-16 encoded byte stream to UTF-32
 * @param src Source  UTF-16 LE/BE format must be set prior)
 * @param out UTF-32 collection
 * @param endianness Endianness of the code units
 * @return Success
 */
bool Transcode::U16toU32( Source &src, std::vector<char32_t> &out ) {
    const auto join  = ( src.format() == Format::UTF16_LE ? Transcode::joinU16LE : unicode::utf16::join );
    auto &     in    = src.stream();
    auto &     pos   = src.position();
    char32_t   prev  = 0x00;
    bool       eof   = ( in.bad() || in.eof() );
    bool       error = false;

    char8_t high_surrogate_bytes[2] = { 0x00, 0x00 };
    char8_t low_surrogate_bytes [2] = { 0x00, 0x00 };

    while( !eof ) {
        char32_t codepoint = 0x00;
        size_t   bytes     = fetchCodeUnit( in, high_surrogate_bytes, 2 );

        if( bytes == 2 ) {
            char16_t high_surrogate = join( high_surrogate_bytes[0], high_surrogate_bytes[1] );
            auto     code_units     = unicode::utf16::codeunits( high_surrogate );

            switch( code_units ) {
                case 1: {
                    codepoint = unicode::utf16::toU32( high_surrogate );
                    Transcode::addCodePoint( src, prev, codepoint, out );
                    prev = codepoint;
                } break;

                case 2: {
                    if( fetchCodeUnit( in, low_surrogate_bytes, 2 ) != 2 ) {
                        logging::ParserLog::log( src.path(),
                                                 specs::Context::NATIVE,
                                                 specs::native::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                                 pos
                        );

                        LOG_ERROR( "[parser::encoding::Transcode::U16toU32( Source &, std::vector<char32_t> & )] "
                                   "Failed to complete codepoint: ", unicode::utf8::toxunicode( high_surrogate, "0x" ), " 0x????" );

                        return false; //EARLY RETURN
                    }

                    const char16_t low_surrogate = join( low_surrogate_bytes[0], low_surrogate_bytes[1] );

                    if( unicode::utf16::islowsurrogate( low_surrogate ) ) {
                        codepoint = unicode::utf16::toU32( high_surrogate, low_surrogate );
                        Transcode::addCodePoint( src, prev, codepoint, out );
                        prev = codepoint;

                    } else {
                        logging::ParserLog::log( src.path(),
                                                 specs::Context::NATIVE,
                                                 specs::native::ErrorCode::INVALID_UTF16_SURROGATE_PAIR,
                                                 pos );

                        LOG_WARNING( "[parser::encoding::Transcode::U16toU32( Source &, std::vector<char32_t> & )] "
                                     "Invalid low surrogate: ", unicode::utf8::toxunicode( high_surrogate, "0x" ), " ",
                                     unicode::utf8::toxunicode( low_surrogate, "0x" ) );
                    }
                } break;

                default: {
                    logging::ParserLog::log( src.path(),
                                             specs::Context::HTML5,
                                             specs::infra::ErrorCode::SURROGATE_IN_INPUT_STREAM,
                                             pos );

                    LOG_WARNING( "[parser::encoding::Transcode::U16toU32( Source &, std::vector<char32_t> & )] "
                                 "High surrogate in low surrogate range: ", unicode::utf8::toxunicode( high_surrogate, "0x" ) );

                    codepoint = unicode::utf16::toU32( high_surrogate ); //should be dealt with by the tokeniser
                    Transcode::addCodePoint( src, prev, codepoint, out );
                    prev = codepoint;
                }
            }

        } else { //EOF and error-control cases
            if( ( error = ( bytes != 0 ) ) ) {
                logging::ParserLog::log( src.path(),
                                         specs::Context::NATIVE,
                                         specs::native::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                         pos
                );
            }

            eof = true;
        }
    }

    return !( error );
}

/**
 * Converts content of a UTF-32 byte stream to a collection of u32 characters
 * @param pre_buffer Buffered input bytes
 * @param src Source (UTF-32 LE/BE format must be set prior)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32toU32( std::deque<char8_t> &pre_buffer, Source &src, std::vector<char32_t> &out ) {
    const auto hasIncompleteCodePoint = []( std::deque<char8_t> &buffer ) { return ( ( buffer.size() % 4 ) > 0 ); };

    const auto join = ( src.format() == Format::UTF32_LE ? Transcode::joinU32LE : unicode::utf32::join );
    auto &     in   = src.stream();
    auto &     pos  = src.position();
    char32_t   prev = 0x00;

    if( !pre_buffer.empty() ) {
        if( hasIncompleteCodePoint( pre_buffer ) ) {
            size_t remainder = ( pre_buffer.size() % 4 );
            size_t required  = ( 4 - remainder );

            if( fetchCodeUnit( in, pre_buffer, required ) != required ) {
                logging::ParserLog::log( src.path(),
                                         specs::Context::NATIVE,
                                         specs::native::ErrorCode::INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM,
                                         pos
                );

                return false; //EARLY RETURN
            }
        }

        const size_t code_point_count = ( pre_buffer.size() / 4 );

        for( int i = 0, byte_i = 0; i < code_point_count; ++i, byte_i += 4 ) {
            char32_t codepoint = join( pre_buffer.at( byte_i ),
                                       pre_buffer.at( byte_i + 1 ),
                                       pre_buffer.at( byte_i + 2 ),
                                       pre_buffer.at( byte_i + 3 ) );

            Transcode::addCodePoint( src, prev, codepoint, out );
            prev = codepoint;
        }
    }

    return Transcode::U32toU32( src, out );
}

/**
 * Converts content of a UTF-32 byte stream to a collection of u32 characters
 * @param src Source (UTF-32 LE/BE format must be set prior)
 * @param out UTF-32 collection
 * @return Success
 */
bool Transcode::U32toU32( Source &src, std::vector<char32_t> &out ) {
    const auto join     = ( src.format() == Format::UTF32_LE ? Transcode::joinU32LE : unicode::utf32::join );
    auto &     in       = src.stream();
    auto &     pos      = src.position();
    char32_t   prev     = 0x00;
    char8_t    bytes[4] = { 0x00, 0x00 };

    in.read( ( char * ) &bytes, 4 );

    while( in.good() && !in.eof() ) {
        char32_t codepoint = join( bytes[0], bytes[1], bytes[2], bytes[3] );

        Transcode::addCodePoint( src, prev, codepoint, out );
        prev = codepoint;
        in.read( ( char * ) &bytes, 4 );
    }

    return true;
}

/**
 * Gets a string representation of an parser::encoding::Endianness enum
 * @param e Endianness enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::encoding::Endianness e ) {
    std::stringstream ss;
    ss << e;
    return ss.str();
}
