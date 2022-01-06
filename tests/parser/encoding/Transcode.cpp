#include "gtest/gtest.h"
#include "../../../src/parser/encoding/Transcode.h"

#include <fstream>
#include <locale>
#include <codecvt>
#include <bit>

#include "../../helper.h"
#include "../../TestStrings.h"
#include "../../../src/parser/logging/ParserLog.h"

using namespace blogator::parser;
using namespace blogator::parser::encoding;
using           blogator::parser::Source;
using           blogator::parser::specs::Context;

/**
 * Parsing log catcher
 */
class ParserLogCatcher {
  public:
    void log( const logging::ErrorObject & e ) {
        _errors.push_back( e );
    }

    [[nodiscard]] const std::vector<logging::ErrorObject> & getErrors() const {
        return _errors;
    };

  private:
    std::vector<blogator::parser::logging::ErrorObject> _errors;
};

//===========================================[ TESTS ] =============================================

TEST( parser_encoding_Transcode, sniffBOM_utf8 ) {
    std::deque<uint8_t> buffer = { 0xEF, 0xBB, 0xBF, 0x74, 0x65, 0x73, 0x74 };
    ASSERT_EQ( Format::UTF8, Transcode::sniffBOM( buffer ) );
    ASSERT_EQ( 4, buffer.size() );
}

TEST( parser_encoding_Transcode, sniffBOM_utf16be ) {
    std::deque<uint8_t> buffer = { 0xFE, 0xFF, 0x74, 0x65, 0x73, 0x74 };
    ASSERT_EQ( Format::UTF16_BE, Transcode::sniffBOM( buffer ) );
    ASSERT_EQ( 4, buffer.size() );
}

TEST( parser_encoding_Transcode, sniffBOM_utf16le ) {
    std::deque<uint8_t> buffer = { 0xFF, 0xFE, 0x74, 0x65, 0x73, 0x74 };
    ASSERT_EQ( Format::UTF16_LE, Transcode::sniffBOM( buffer ) );
    ASSERT_EQ( 4, buffer.size() );
}

TEST( parser_encoding_Transcode, sniffBOM_utf32le ) {
    std::deque<uint8_t> buffer = { 0x00, 0x00, 0xFE, 0xFF, 0x74, 0x65, 0x73, 0x74 };
    ASSERT_EQ( Format::UTF32_LE, Transcode::sniffBOM( buffer ) );
    ASSERT_EQ( 4, buffer.size() );
}

TEST( parser_encoding_Transcode, sniffBOM_utf32be ) {
    std::deque<uint8_t> buffer = { 0xFF, 0xFE, 0x00, 0x00, 0x74, 0x65, 0x73, 0x74 };
    ASSERT_EQ( Format::UTF32_BE, Transcode::sniffBOM( buffer ) );
    ASSERT_EQ( 4, buffer.size() );
}

TEST( parser_encoding_Transcode, sniffBOM_unknown ) {
    std::deque<uint8_t> buffer = { 0xEF, 0x74, 0x65, 0x73, 0x74 };
    ASSERT_EQ( Format::UNKNOWN, Transcode::sniffBOM( buffer ) );
    ASSERT_EQ( 5, buffer.size() );
}

TEST( parser_encoding_Transcode, U32toByteStream_u32string_U32LE ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        const auto &      in_str = blogator::tests::TestStrings::string( str_id );
        std::stringstream out_stream;
        std::stringstream expected_stream;

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32LE, expected_stream );

        ASSERT_TRUE( Transcode::U32toByteStream( in_str, out_stream, Endianness::LE ) );
        ASSERT_EQ( out_stream.str(), expected_stream.str() );
    }
}

TEST( parser_encoding_Transcode, U32toByteStream_u32string_U32BE ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        const auto &      in_str = blogator::tests::TestStrings::string( str_id );
        std::stringstream out_stream;
        std::stringstream expected_stream;

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32BE, expected_stream );

        ASSERT_TRUE( Transcode::U32toByteStream( in_str, out_stream, Endianness::BE ) );
        ASSERT_EQ( out_stream.str(), expected_stream.str() );
    }
}

TEST( parser_encoding_Transcode, U32toByteStream_u32string_fail_1 ) { //bad output stream
    const size_t      test_string_id = 0;
    const auto &      in_str = blogator::tests::TestStrings::string( test_string_id );
    std::stringstream out_stream;

    out_stream.setstate( std::ios::badbit );
    ASSERT_FALSE( Transcode::U32toByteStream( in_str, out_stream, Endianness::BE ) );
}

TEST( parser_encoding_Transcode, U32toByteStream_u32collection_U32LE ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        const auto &          in_str = blogator::tests::TestStrings::string( str_id );
        std::vector<uint32_t> in_vec( in_str.begin(), in_str.end() );
        std::stringstream     out_stream;
        std::stringstream     expected_stream;

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32LE, expected_stream );

        ASSERT_TRUE( Transcode::U32toByteStream( in_vec, out_stream, Endianness::LE ) );
        ASSERT_EQ( out_stream.str(), expected_stream.str() );
    }
}

TEST( parser_encoding_Transcode, U32toByteStream_u32collection_U32BE ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        const auto &          in_str = blogator::tests::TestStrings::string( str_id );
        std::vector<uint32_t> in_vec( in_str.begin(), in_str.end() );
        std::stringstream     out_stream;
        std::stringstream     expected_stream;

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32BE, expected_stream );

        ASSERT_TRUE( Transcode::U32toByteStream( in_vec, out_stream, Endianness::BE ) );
        ASSERT_EQ( out_stream.str(), expected_stream.str() );
    }
}

TEST( parser_encoding_Transcode, U32toByteStream_u32collection_fail_1 ) { //bad output stream
    const size_t          test_string_id = 0;
    const auto &          in_str = blogator::tests::TestStrings::string( test_string_id );
    std::vector<uint32_t> in_vec( in_str.begin(), in_str.end() );
    std::stringstream     out_stream;

    out_stream.setstate( std::ios::badbit );
    ASSERT_FALSE( Transcode::U32toByteStream( in_vec, out_stream, Endianness::BE ) );
}

TEST( parser_encoding_Transcode, fetchCodeUnit_array_2bytes ) {
    uint8_t           buffer[3] = { 0, 0, 0 };
    std::stringstream ss;

    ss << (char) 0x63 << (char) 0x64 << (char) 0x65 << (char) 0x66;

    ASSERT_EQ( 2, Transcode::fetchCodeUnit( ss, buffer, 2 ) );
    ASSERT_EQ( 0x63, buffer[0] );
    ASSERT_EQ( 0x64, buffer[1] );
    ASSERT_EQ( 0x00, buffer[2] );
}

TEST( parser_encoding_Transcode, fetchCodeUnit_array_4bytes ) {
    uint8_t           buffer[4];
    std::stringstream ss;

    ss << (char) 0x63 << (char) 0x64 << (char) 0x65 << (char) 0x66;

    ASSERT_EQ( 4, Transcode::fetchCodeUnit( ss, buffer, 4 ) );
    ASSERT_EQ( 0x63, buffer[0] );
    ASSERT_EQ( 0x64, buffer[1] );
    ASSERT_EQ( 0x65, buffer[2] );
    ASSERT_EQ( 0x66, buffer[3] );
}

TEST( parser_encoding_Transcode, fetchCodeUnit_array_greedy ) {
    uint8_t           buffer[4] = { 0x00, 0x00, 0x00, 0x00 };
    std::stringstream ss;

    ss << (char) 0x63 << (char) 0x64;

    ASSERT_EQ( 2, Transcode::fetchCodeUnit( ss, buffer, 3 ) );
    ASSERT_EQ( 0x63, buffer[0] );
    ASSERT_EQ( 0x64, buffer[1] );
}

TEST( parser_encoding_Transcode, fetchCodeUnit_prebuffer_2bytes ) {
    std::deque<uint8_t> buffer;
    std::stringstream   ss;

    ss << (char) 0x63 << (char) 0x64 << (char) 0x65 << (char) 0x66;

    ASSERT_EQ( 2, Transcode::fetchCodeUnit( ss, buffer, 2 ) );
    ASSERT_EQ( 0x63, buffer[0] );
    ASSERT_EQ( 0x64, buffer[1] );
}

TEST( parser_encoding_Transcode, fetchCodeUnit_prebuffer_4bytes ) {
    std::deque<uint8_t> buffer;
    std::stringstream   ss;

    ss << (char) 0x63 << (char) 0x64 << (char) 0x65 << (char) 0x66;

    ASSERT_EQ( 4, Transcode::fetchCodeUnit( ss, buffer, 4 ) );
    ASSERT_EQ( 0x63, buffer[0] );
    ASSERT_EQ( 0x64, buffer[1] );
    ASSERT_EQ( 0x65, buffer[2] );
    ASSERT_EQ( 0x66, buffer[3] );
}

TEST( parser_encoding_Transcode, fetchCodeUnit_prebuffer_greedy ) {
    std::deque<uint8_t> buffer;
    std::stringstream   ss;

    ss << (char) 0x63 << (char) 0x64;

    ASSERT_EQ( 2, Transcode::fetchCodeUnit( ss, buffer, 3 ) );
    ASSERT_EQ( 0x63, buffer[0] );
    ASSERT_EQ( 0x64, buffer[1] );
}

TEST( parser_encoding_Transcode, addCodePoint ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::string           test_str = "abcde12345";
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;

    for( auto c : test_str ) {
        Transcode::addCodePoint( in_source, 0x00, (uint32_t) c, out_buffer );
    }

    ASSERT_EQ( 10, out_buffer.size() );
    ASSERT_EQ( U"abcde12345", std::u32string( out_buffer.begin(), out_buffer.end() ) );
    ASSERT_TRUE( log_catcher.getErrors().empty() );
}

TEST( parser_encoding_Transcode, addCodePoint_nonchar ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;
    uint32_t              test_char = 0x00FDD0;

    Transcode::addCodePoint( in_source, 0x00, test_char, out_buffer );

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::HTML5,
                                                                specs::html5::ErrorCode::NONCHARACTER_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_EQ( 1, out_buffer.size() );
    ASSERT_EQ( test_char, out_buffer.back() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().back() );
}

TEST( parser_encoding_Transcode, addCodePoint_ctrlchar_BELL ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;
    uint32_t              test_char = 0x07; //BELL

    Transcode::addCodePoint( in_source, 0x00, test_char, out_buffer );

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::HTML5,
                                                                specs::html5::ErrorCode::CONTROL_CHARACTER_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_EQ( 1, out_buffer.size() );
    ASSERT_EQ( test_char, out_buffer.at( 0 ) );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, addCodePoint_ctrlchar_DEL ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;
    uint32_t              test_char = 0x7F; //DEL

    Transcode::addCodePoint( in_source, 0x00, test_char, out_buffer );

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::HTML5,
                                                                specs::html5::ErrorCode::CONTROL_CHARACTER_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_EQ( 1, out_buffer.size() );
    ASSERT_EQ( test_char, out_buffer.at( 0 ) );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, addCodePoint_newline_CRLF ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;

    Transcode::addCodePoint( in_source, 0x00, blogator::unicode::CR, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::CR, blogator::unicode::LF, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::LF, U'A', out_buffer );

    ASSERT_EQ( 2, out_buffer.size() );
    ASSERT_EQ( blogator::unicode::LF, out_buffer.at( 0 ) );
    ASSERT_EQ( U'A', out_buffer.at( 1 ) );
    ASSERT_EQ( 0, log_catcher.getErrors().size() );
}

TEST( parser_encoding_Transcode, addCodePoint_newline_LFLF ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;

    Transcode::addCodePoint( in_source, 0x00, blogator::unicode::LF, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::LF, blogator::unicode::LF, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::LF, U'A', out_buffer );

    ASSERT_EQ( 3, out_buffer.size() );
    ASSERT_EQ( blogator::unicode::LF, out_buffer.at( 0 ) );
    ASSERT_EQ( blogator::unicode::LF, out_buffer.at( 1 ) );
    ASSERT_EQ( U'A', out_buffer.at( 2 ) );
    ASSERT_EQ( 0, log_catcher.getErrors().size() );
}

TEST( parser_encoding_Transcode, addCodePoint_newline_CRCR ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;

    Transcode::addCodePoint( in_source, 0x00, blogator::unicode::CR, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::CR, blogator::unicode::CR, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::CR, U'A', out_buffer );

    ASSERT_EQ( 3, out_buffer.size() );
    ASSERT_EQ( blogator::unicode::LF, out_buffer.at( 0 ) );
    ASSERT_EQ( blogator::unicode::LF, out_buffer.at( 1 ) );
    ASSERT_EQ( U'A', out_buffer.at( 2 ) );
    ASSERT_EQ( 0, log_catcher.getErrors().size() );
}

TEST( parser_encoding_Transcode, addCodePoint_newline_LFCR ) {
    ParserLogCatcher      log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );
    std::stringstream     in_stream; //unused
    Source                in_source = Source( in_stream, "", Format::UNKNOWN );
    std::vector<uint32_t> out_buffer;

    Transcode::addCodePoint( in_source, 0x00, blogator::unicode::LF, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::LF, blogator::unicode::CR, out_buffer );
    Transcode::addCodePoint( in_source, blogator::unicode::CR, U'A', out_buffer );

    ASSERT_EQ( 3, out_buffer.size() );
    ASSERT_EQ( blogator::unicode::LF, out_buffer.at( 0 ) );
    ASSERT_EQ( blogator::unicode::LF, out_buffer.at( 1 ) );
    ASSERT_EQ( U'A', out_buffer.at( 2 ) );
    ASSERT_EQ( 0, log_catcher.getErrors().size() );
}

TEST( parser_encoding_Transcode, U32LEtoU32_string_stream ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::stringstream      in_stream;
        Source                 in_source = Source( in_stream, "", Format::UTF32_LE );
        std::vector<uint32_t>  out_buffer;
        const std::u32string & expected_str = blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32LE, in_stream );

        ASSERT_TRUE( Transcode::U32LEtoU32( in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U32LEtoU32_string_prebuffered_0 ) { //with pre-buffered bytes
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t> in_buffer = { 0xD6, 0xFA, 0x01, 0x00 }; //LE 0x0001FAD6 (TEAPOT)
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF32_LE );
        std::vector<uint32_t> out_buffer;
        std::u32string expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32LE, in_stream );

        ASSERT_TRUE( Transcode::U32LEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U32LEtoU32_string_prebuffered_1 ) { //with pre-buffered bytes (1/4 byte)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t> in_buffer = { 0xD6 }; //LE 0x0001FAD6 (TEAPOT) - 1/4 byte
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF32_LE );
        std::vector<uint32_t> out_buffer;
        std::u32string expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        in_stream << (char) 0xFA << (char) 0x01 << (char) 0x00; //3/4 bytes
        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32LE, in_stream );

        ASSERT_TRUE( Transcode::U32LEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U32LEtoU32_string_prebuffered_2 ) { //with pre-buffered bytes (3/4 byte)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t> in_buffer = { 0xD6, 0xFA, 0x01 }; //LE 0x0001FAD6 (TEAPOT) - 3/4 byte
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF32_LE );
        std::vector<uint32_t> out_buffer;
        std::u32string expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        in_stream << (char) 0x00; //1/4 bytes
        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32LE, in_stream );

        ASSERT_TRUE( Transcode::U32LEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U32LEtoU32_string_prebuffered_fail ) { //with pre-buffered bytes (incomplete + fail fetch
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t> in_buffer = { 0xD6 }; //LE 0x0001FAD6 (TEAPOT) - 1/4 byte
    std::stringstream in_stream;
    Source in_source = Source( in_stream, "", Format::UTF32_LE );
    std::vector<uint32_t> out_buffer;

    in_stream << (char) 0xFA << (char) 0xFA; //2/4 bytes w/ missing last 0x00

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U32LEtoU32( in_buffer, in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U32LEtoU32_file ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter; //to check against

    auto src_u8  = blogator::tests::loadFile( source );
    auto src_u32 = converter.from_bytes( src_u8 );

    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF32_LE );
    std::vector<uint32_t> out_buffer;

    if constexpr( std::endian::native == std::endian::little ) {
        Transcode::U32toByteStream( src_u32, in_stream, Endianness::LE );
    } else { //std::endian::big
        Transcode::U32toByteStream( src_u32, in_stream, Endianness::BE );
    }

    ASSERT_TRUE( Transcode::U32LEtoU32( in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( src_u32, output );
}

TEST( parser_encoding_Transcode, U32BEtoU32_string_stream ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF32_BE );
        std::vector<uint32_t> out_buffer;
        const std::u32string &expected_str = blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32BE, in_stream );

        ASSERT_TRUE( Transcode::U32BEtoU32( in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U32BEtoU32_string_prebuffered ) { //with pre-buffered bytes
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>  in_buffer = { 0x00, 0x01, 0xFA, 0xD6 }; //BE 0x0001FAD6 (TEAPOT)
        std::stringstream    in_stream;
        Source               in_source = Source( in_stream, "", Format::UTF32_BE );
        std::vector<uint32_t> out_buffer;
        std::u32string        expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF32BE, in_stream );

        ASSERT_TRUE( Transcode::U32BEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U8toU32_u8_file ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;
    Source                in_source = Source( file, "", Format::UTF8 );

    file.open( source, std::ios::binary | std::ios::in );

    if( file ) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::vector<uint32_t>                                       out_buffer;

        ASSERT_TRUE( Transcode::U8toU32( in_source, out_buffer ) );
        file.close();

        auto expected_src = blogator::tests::loadFile( source );
        auto expected_str = converter.from_bytes( expected_src ); //basically testing against STL converter's output

        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output );
    } else {
        FAIL() << "Could not open test file: " << source;
    }
}

TEST( parser_encoding_Transcode, U8toU32_string_stream ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF8 );
        std::vector<uint32_t> out_buffer;
        const std::u32string &expected_str = blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF8, in_stream );

        ASSERT_TRUE( Transcode::U8toU32( in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U8toU32_string_stream_fail_0 ) { //incomplete codepoint in stream (2/4 bytes)
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;

    in_stream << (char) 0xf0 << (char) 0x9f; //U+1FAD6 (TEAPOT) - 2/4 bytes

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U8toU32( in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U8toU32_string_stream_fail_1 ) { //invalid codepoint in stream
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;

    in_stream << (char) 0xA0 << (char) 0x61; //invalid byte and 'a'
    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U8toU32( in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U8toU32_string_prebuffered_0 ) { //with pre-buffered bytes
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t> in_buffer = { 0xf0, 0x9f, 0xab, 0x96 }; //U+1FAD6 (TEAPOT)
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF8 );
        std::vector<uint32_t> out_buffer;
        std::u32string expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF8, in_stream );

        ASSERT_TRUE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U8toU32_string_prebuffered_1 ) { //incomplete codepoint buffered (1/4 bytes)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t> in_buffer = { 0xf0 }; //U+1FAD6 (TEAPOT) - first byte
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF8 );
        std::vector<uint32_t> out_buffer;
        std::u32string expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        in_stream << ( char ) 0x9f << ( char ) 0xab << ( char ) 0x96; //remainder of TEAPOT
        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF8, in_stream );

        ASSERT_TRUE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U8toU32_string_prebuffered_2 ) { //incomplete codepoint buffered (3/4 bytes)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>   in_buffer = { 0xf0, 0x9f, 0xab }; //U+1FAD6 (TEAPOT) - first 3 byte
        std::stringstream     in_stream;
        Source                in_source = Source( in_stream, "", Format::UTF8 );
        std::vector<uint32_t> out_buffer;
        std::u32string        expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        in_stream << ( char ) 0x96; //remainder of TEAPOT
        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF8, in_stream );

        ASSERT_TRUE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U8toU32_string_prebuffered_3 ) { //pre-buffered with variable width codepoints
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>   in_buffer = { 0xf0, 0x9f, 0xab, 0x96, //U+0001FAD6 (TEAPOT)
                                            0xC3, 0x84,             //U+000000C4 ('Ä')
                                            0xeb, 0x86, 0xa2,       //U+0000B1A2 ('놢')
                                            0x61 };                 //U+00000061 ('a')
        std::stringstream     in_stream;
        Source                in_source = Source( in_stream, "", Format::UTF8 );
        std::vector<uint32_t> out_buffer;
        std::u32string        expected_str = U"\U0001FAD6\U000000C4\U0000B1A2a" + blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF8, in_stream );

        ASSERT_TRUE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U8toU32_string_prebuffered_fail_0 ) { //incomplete codepoint buffered (2/4 bytes)
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>   in_buffer = { 0xf0, 0x9f }; //U+1FAD6 (TEAPOT) - 2/4 bytes
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U8toU32_string_prebuffered_fail_1 ) { //invalid codepoint buffered
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>   in_buffer = { 0xA0, 0x61 }; //invalid byte and 'a'
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16LEtoU32_string_stream ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF16_LE );
        std::vector<uint32_t> out_buffer;
        const std::u32string &expected_str = blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF16LE, in_stream );

        ASSERT_TRUE( Transcode::U16LEtoU32( in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U16LEtoU32_string_prebuffered_0 ) { //with pre-buffered bytes (complete)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>    in_buffer = { 0x3E, 0xD8, 0xD6, 0xDE }; //U+1FAD6 (TEAPOT)
        std::stringstream      in_stream;
        Source                 in_source = Source( in_stream, "", Format::UTF16_LE );
        std::vector<uint32_t>  out_buffer;
        const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF16LE, in_stream );

        ASSERT_TRUE( Transcode::U16LEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_stream ) {
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::stringstream in_stream;
        Source in_source = Source( in_stream, "", Format::UTF16_BE );
        std::vector<uint32_t> out_buffer;
        const std::u32string &expected_str = blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF16BE, in_stream );

        ASSERT_TRUE( Transcode::U16BEtoU32( in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_stream_fail_1 ) { //1 bytes available in stream
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    in_stream << (char) 0xD8;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U16BEtoU32( in_source, out_buffer ) );
    ASSERT_EQ( 0, out_buffer.size() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_stream_fail_2 ) { //3 bytes available in stream (incomplete surrogate pair)
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    in_stream << (char) 0xD8 << (char) 0x3E << (char) 0xDE; //U+1FAD6 (TEAPOT) - 3/4 bytes

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U16BEtoU32( in_source, out_buffer ) );
    ASSERT_EQ( 0, out_buffer.size() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_stream_fail_3 ) { //valid high surrogate w/ invalid low surrogate
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    in_stream << (char) 0xD8 << (char) 0x3E  //pair: valid high surrogate
              << (char) 0xDB << (char) 0xFF  //pair: invalid low surrogate
              << (char) 0x00 << (char) 0x61; //single: valid codepoint ('a')

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INVALID_UTF16_SURROGATE_PAIR,
                                                                { 1, 1 } );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_source, out_buffer ) );
    ASSERT_EQ( 1, out_buffer.size() );
    ASSERT_EQ( U'a', out_buffer[0] );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_stream_fail_4 ) { //invalid high surrogate
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    in_stream << (char) 0x00 << (char) 0x61  //single/valid codepoint ('a')
              << (char) 0xDC << (char) 0x00  //invalid high surrogate (low surrogate range)
              << (char) 0x00 << (char) 0x62; //single/valid codepoint ('b')

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::HTML5,
                                                                specs::html5::ErrorCode::SURROGATE_IN_INPUT_STREAM,
                                                                { 1, 2 } );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_source, out_buffer ) );
    ASSERT_EQ( 3, out_buffer.size() );
    ASSERT_EQ( 0x0061, out_buffer[0] );
    ASSERT_EQ( 0xDC00, out_buffer[1] );
    ASSERT_EQ( 0x0062, out_buffer[2] );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_prebuffered_0 ) { //with pre-buffered bytes (complete)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>    in_buffer = { 0xD8, 0x3E, 0xDE, 0xD6 }; //U+1FAD6 (TEAPOT)
        std::stringstream      in_stream;
        Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
        std::vector<uint32_t>  out_buffer;
        const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF16BE, in_stream );

        ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_prebuffered_1 ) { //incomplete codepoint buffered (1/4 bytes)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>    in_buffer = { 0xD8 }; //U+1FAD6 (TEAPOT) - 1/4 bytes
        std::stringstream      in_stream;
        Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
        std::vector<uint32_t>  out_buffer;
        const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        in_stream << (char) 0x3E << (char) 0xDE << (char) 0xD6;
        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF16BE, in_stream );

        ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_prebuffered_2 ) { //incomplete codepoint buffered (2/4 bytes)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>    in_buffer = { 0xD8, 0x3E }; //U+1FAD6 (TEAPOT) - 2/4 bytes
        std::stringstream      in_stream;
        Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
        std::vector<uint32_t>  out_buffer;
        const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::string( str_id );

        in_stream << (char) 0xDE << (char) 0xD6;
        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF16BE, in_stream );

        ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U16BEtoU32_string_prebuffered_3 ) { //incomplete codepoint buffered (2/2 + 1/2 bytes)
    for( auto str_id = 0; str_id < blogator::tests::TestStrings::count(); ++str_id ) {
        std::deque<uint8_t>    in_buffer = { 0x01, 0xA3, //U+01A3 ('ƣ')
                                             0xB1 };     //U+B1A2 ('놢') - 1/2 bytes;
        std::stringstream      in_stream;
        Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
        std::vector<uint32_t>  out_buffer;
        const std::u32string & expected_str = U"\U000001A3\U0000B1A2" + blogator::tests::TestStrings::string( str_id );

        in_stream << (char) 0xA2;
        blogator::tests::TestStrings::write( str_id, blogator::tests::EncodingFmt::UTF16BE, in_stream );

        ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) ) << "Failed transcoding with str #" << str_id;
        auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
        ASSERT_EQ( expected_str, output ) << "Expected != output for str #" << str_id;
    }
}

TEST( parser_encoding_Transcode, U16BEtoU32_prebuffered_fail_1 ) { //1 byte available only
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>    in_buffer = { 0xD8 };
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INCOMPLETE_UTF16_HIGH_SURROGATE_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_prebuffered_fail_2 ) { //high surrogate w/o its low surrogate
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>    in_buffer = { 0xD8, 0x3E };
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_prebuffered_fail_3 ) { //valid high surrogate w/ invalid low surrogate
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>    in_buffer = { 0xD8, 0x3E,   //pair: valid high surrogate
                                         0xDB, 0xFF,   //pair: invalid low surrogate
                                         0x00, 0x61 }; //single: valid codepoint ('a')
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::NATIVE,
                                                                specs::native::ErrorCode::INVALID_UTF16_SURROGATE_PAIR,
                                                                { 1, 1 } );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    ASSERT_EQ( 1, out_buffer.size() );
    ASSERT_EQ( 0x61, out_buffer.back() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_prebuffered_fail_4 ) { //invalid high surrogate
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>    in_buffer = { 0x00, 0x61,   //single/valid codepoint ('a')
                                         0xDC, 0x00,   //invalid high surrogate (low surrogate range)
                                         0x00, 0x62 }; //single/valid codepoint ('b')
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::HTML5,
                                                                specs::html5::ErrorCode::SURROGATE_IN_INPUT_STREAM,
                                                                { 1, 2 } );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    ASSERT_EQ( 3, out_buffer.size() );
    ASSERT_EQ( 0x0061, out_buffer[0] );
    ASSERT_EQ( 0xDC00, out_buffer[1] );
    ASSERT_EQ( 0x0062, out_buffer[2] );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}