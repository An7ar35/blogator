#include "gtest/gtest.h"
#include "../../../src/parser/encoding/Transcode.h"

#include <fstream>
#include <locale>
#include <codecvt>

#include "../../helper.h"

using namespace blogator::parser::encoding;

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

TEST( parser_encoding_Transcode, sniffBOM_unknown ) {
    std::deque<uint8_t> buffer = { 0xEF, 0x74, 0x65, 0x73, 0x74 };
    ASSERT_EQ( Format::UNKNOWN, Transcode::sniffBOM( buffer ) );
    ASSERT_EQ( 5, buffer.size() );
}

TEST( parser_encoding_Transcode, U32toU32_string0 ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    std::vector<uint32_t> src = { 0x0000210c, 0x0001d522, 0x0001d529, 0x0001d529, 0x0001d52c, 0x0000002c, 0x00000020,
                                  0x0001d534, 0x0001d52c, 0x0001d52f, 0x0001d529, 0x0001d521, 0x00000021 };

    std::u32string        src_u32 = std::u32string( src.begin(), src.end() );
    std::stringstream     char_stream;
    std::vector<uint32_t> buffer_out;

    Transcode::U32toByteStream( src_u32, char_stream );
    Transcode::U32toU32( char_stream, buffer_out );

    auto output = std::u32string( buffer_out.begin(), buffer_out.end() );
    ASSERT_EQ( src_u32, output );
}

TEST( parser_encoding_Transcode, U32toU32_string1 ) { //with pre-buffered bytes
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    std::deque<uint8_t>   buffer   = { 0x0, 0x01, 0xFA, 0xD6 };
    std::vector<uint32_t> src      = { 0x0001FAD6, 0x0000210c, 0x0001d522, 0x0001d529, 0x0001d529, 0x0001d52c, 0x0000002c, 0x00000020,
                                       0x0001d534, 0x0001d52c, 0x0001d52f, 0x0001d529, 0x0001d521, 0x00000021 };
    std::u32string        src_u32  = std::u32string( std::next( src.begin(), 1 ), src.end() );
    std::u32string        expected = std::u32string( src.begin(), src.end() );

    std::stringstream     char_stream;
    std::vector<uint32_t> buffer_out;

    Transcode::U32toByteStream( src_u32, char_stream );
    Transcode::U32toU32( buffer, char_stream, buffer_out );

    auto output = std::u32string( buffer_out.begin(), buffer_out.end() );
    ASSERT_EQ( expected, output );
}

TEST( parser_encoding_Transcode, U32toU32_file ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    auto src_u8  = blogator::tests::loadFile( source );
    auto src_u32 = converter.from_bytes( src_u8 );

    std::stringstream     char_stream;
    std::vector<uint32_t> buffer_out;

    Transcode::U32toByteStream( src_u32, char_stream );
    Transcode::U32toU32( char_stream, buffer_out );

    auto output = std::u32string( buffer_out.begin(), buffer_out.end() );
    ASSERT_EQ( src_u32, output );
}


TEST( parser_encoding_Transcode, U8toU32 ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;

    file.open( source, std::ios::binary | std::ios::in );

    if( file ) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::vector<uint32_t>                                       output_buffer;

        Transcode::U8toU32( file, output_buffer );
        file.close();

        auto src_u8  = blogator::tests::loadFile( source );
        auto src_u32 = converter.from_bytes( src_u8 ); //basically testing against STL converter's output
        auto out_u32  = std::u32string( output_buffer.begin(), output_buffer.end() );

        ASSERT_EQ( src_u32, out_u32 );
    } else {
        FAIL() << "Could not open test file: " << source;
    }
}