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
    void log( const blogator::parser::logging::ErrorObject & e ) {
        _errors.push_back( e );
    }

    [[nodiscard]] const std::vector<blogator::parser::logging::ErrorObject> & getErrors() const {
        return _errors;
    };

  private:
    std::vector<blogator::parser::logging::ErrorObject> _errors;
};

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
                                                                { 1, 2 } );

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
                                                                { 1, 2 } );

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
                                                                { 1, 2 } );

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

TEST( parser_encoding_Transcode, U32LEtoU32_string0 ) {
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF32_LE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf32LE( in_stream );

    ASSERT_TRUE( Transcode::U32LEtoU32( in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U32LEtoU32_string1 ) { //with pre-buffered bytes
    std::deque<uint8_t>   in_buffer    = { 0xD6, 0xFA, 0x01, 0x00 }; //LE 0x0001FAD6 (TEAPOT)
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF32_LE );
    std::vector<uint32_t> out_buffer;
    std::u32string        expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf32LE( in_stream );

    ASSERT_TRUE( Transcode::U32LEtoU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
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

TEST( parser_encoding_Transcode, U32BEtoU32_string0 ) {
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF32_BE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf32BE( in_stream );

    ASSERT_TRUE( Transcode::U32BEtoU32( in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U32BEtoU32_string1 ) { //with pre-buffered bytes
    std::deque<uint8_t>  in_buffer = { 0x00, 0x01, 0xFA, 0xD6 }; //BE 0x0001FAD6 (TEAPOT)
    std::stringstream    in_stream;
    Source               in_source = Source( in_stream, "", Format::UTF32_BE );
    std::vector<uint32_t> out_buffer;
    std::u32string        expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf32BE( in_stream );

    ASSERT_TRUE( Transcode::U32BEtoU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U8toU32 ) {
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

TEST( parser_encoding_Transcode, U8toU32_string0 ) {
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf8( in_stream );

    ASSERT_TRUE( Transcode::U8toU32( in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U8toU32_string1 ) { //with pre-buffered bytes (complete)
    std::deque<uint8_t>   in_buffer = { 0xf0, 0x9f, 0xab, 0x96 }; //\U1FAD6 (TEAPOT)
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;
    std::u32string        expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf8( in_stream );

    ASSERT_TRUE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U8toU32_string2 ) { //with pre-buffered bytes (incomplete)
    std::deque<uint8_t>   in_buffer = { 0xf0 }; //\U1FAD6 (TEAPOT) - first byte
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;
    std::u32string        expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    in_stream << (char) 0x9f << (char) 0xab << (char) 0x96; //remainder of TEAPOT
    blogator::tests::TestStrings::writeSTR1_utf8( in_stream );

    ASSERT_TRUE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U8toU32_string3 ) { //with pre-buffered bytes (incomplete)
    std::deque<uint8_t>   in_buffer = { 0xf0, 0x9f, 0xab }; //\U1FAD6 (TEAPOT) - first 3 byte
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;
    std::u32string        expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    in_stream << (char) 0x96; //remainder of TEAPOT
    blogator::tests::TestStrings::writeSTR1_utf8( in_stream );

    ASSERT_TRUE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U8toU32_string_fail ) { //with pre-buffered bytes (incomplete)
    std::deque<uint8_t>   in_buffer = { 0xf0, 0x9f }; //\U1FAD6 (TEAPOT) - first 2 byte
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF8 );
    std::vector<uint32_t> out_buffer;

    ASSERT_FALSE( Transcode::U8toU32( in_buffer, in_source, out_buffer ) );
}

TEST( parser_encoding_Transcode, U16LEtoU32_string0 ) {
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_LE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf16LE( in_stream );

    ASSERT_TRUE( Transcode::U16LEtoU32( in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U16LEtoU32_string1 ) { //with pre-buffered bytes (complete)
    std::deque<uint8_t>    in_buffer = { 0x3E, 0xD8, 0xD6, 0xDE }; //\U1FAD6 (TEAPOT)
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_LE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf16LE( in_stream );

    ASSERT_TRUE( Transcode::U16LEtoU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string0 ) {
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf16BE( in_stream );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string1 ) { //with pre-buffered bytes (complete)
    std::deque<uint8_t>    in_buffer = { 0xD8, 0x3E, 0xDE, 0xD6 }; //\U1FAD6 (TEAPOT)
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    blogator::tests::TestStrings::writeSTR1_utf16BE( in_stream );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string2 ) { //with pre-buffered bytes (incomplete)
    std::deque<uint8_t>    in_buffer = { 0xD8 }; //\U1FAD6 (TEAPOT) first byte
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    in_stream << (char) 0x3E << (char) 0xDE << (char) 0xD6;
    blogator::tests::TestStrings::writeSTR1_utf16BE( in_stream );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U16BEtoU32_string3 ) { //with pre-buffered bytes (incomplete)
    std::deque<uint8_t>    in_buffer = { 0xD8, 0x3E }; //\U1FAD6 (TEAPOT) first 2 bytes
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;
    const std::u32string & expected_str = U"\U0001FAD6" + blogator::tests::TestStrings::STR1;

    in_stream << (char) 0xDE << (char) 0xD6;
    blogator::tests::TestStrings::writeSTR1_utf16BE( in_stream );

    ASSERT_TRUE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U16BEtoU32_fail1 ) { //1 byte
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>    in_buffer = { 0xD8 };
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::BLOGATOR,
                                                                specs::blogator::ErrorCode::INCOMPLETE_UTF16_HIGH_SURROGATE_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}

TEST( parser_encoding_Transcode, U16BEtoU32_fail2 ) { //high surrogate for a pair
    ParserLogCatcher       log_catcher;
    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ log_catcher.log( err ); } );

    std::deque<uint8_t>    in_buffer = { 0xD8, 0x3E };
    std::stringstream      in_stream;
    Source                 in_source = Source( in_stream, "", Format::UTF16_BE );
    std::vector<uint32_t>  out_buffer;

    auto expected_err = blogator::parser::logging::ErrorObject( "",
                                                                Context::BLOGATOR,
                                                                specs::blogator::ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
                                                                { 1, 1 } );

    ASSERT_FALSE( Transcode::U16BEtoU32( in_buffer, in_source, out_buffer ) );
    ASSERT_TRUE( out_buffer.empty() );
    ASSERT_EQ( 1, log_catcher.getErrors().size() );
    ASSERT_EQ( expected_err, log_catcher.getErrors().at( 0 ) );
}


//TODO more UTF16 tests
//TODO check all error scenarios - use log catcher