#include "gtest/gtest.h"
#include "../../../src/parser/encoding/Transcode.h"

#include <fstream>
#include <locale>
#include <codecvt>

#include "../../helper.h"

using namespace blogator::parser::encoding;
using           blogator::parser::Source;

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

TEST( parser_encoding_Transcode, U32LEtoU32_string0 ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF32_LE );

    in_stream << (char) 0x0C << (char) 0x21 << (char) 0x00 << (char) 0x00 //0x0000210c
              << (char) 0x22 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d522
              << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
              << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
              << (char) 0x2C << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52c
              << (char) 0x2C << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x0000002c
              << (char) 0x20 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x00000020
              << (char) 0x34 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d534
              << (char) 0x2C << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52c
              << (char) 0x2F << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52f
              << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
              << (char) 0x21 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d521
              << (char) 0x21 << (char) 0x00 << (char) 0x00 << (char) 0x00;//0x00000021


    std::vector<uint32_t> expected_src = { 0x0000210c, 0x0001d522, 0x0001d529, 0x0001d529,
                                           0x0001d52c, 0x0000002c, 0x00000020, 0x0001d534,
                                           0x0001d52c, 0x0001d52f, 0x0001d529, 0x0001d521,
                                           0x00000021 };
    std::u32string        expected_str = std::u32string( expected_src.begin(), expected_src.end() );
    std::vector<uint32_t> out_buffer;

    Transcode::U32LEtoU32( in_source, out_buffer );

    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U32LEtoU32_string1 ) { //with pre-buffered bytes
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    std::deque<uint8_t>   in_buffer    = { 0xD6, 0xFA, 0x01, 0x00 }; //LE 0x0001FAD6
    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF32_LE );

    in_stream << (char) 0x0C << (char) 0x21 << (char) 0x00 << (char) 0x00 //0x0000210c
              << (char) 0x22 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d522
              << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
              << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
              << (char) 0x2C << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52c
              << (char) 0x2C << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x0000002c
              << (char) 0x20 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x00000020
              << (char) 0x34 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d534
              << (char) 0x2C << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52c
              << (char) 0x2F << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52f
              << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
              << (char) 0x21 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d521
              << (char) 0x21 << (char) 0x00 << (char) 0x00 << (char) 0x00;//0x00000021

    std::vector<uint32_t> expected_src = { 0x0001FAD6, 0x0000210c, 0x0001d522, 0x0001d529,
                                           0x0001d529, 0x0001d52c, 0x0000002c, 0x00000020,
                                           0x0001d534, 0x0001d52c, 0x0001d52f, 0x0001d529,
                                           0x0001d521, 0x00000021 };
    std::u32string        expected_str = std::u32string( expected_src.begin(), expected_src.end() );
    std::vector<uint32_t> out_buffer;

    Transcode::U32LEtoU32( in_buffer, in_source, out_buffer );

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

    Transcode::U32toByteStream( src_u32, in_stream );
    Transcode::U32LEtoU32( in_source, out_buffer );

    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( src_u32, output );
}

TEST( parser_encoding_Transcode, U32BEtoU32_string0 ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    std::stringstream     in_stream;
    Source                in_source = Source( in_stream, "", Format::UTF32_BE );

    in_stream << (char) 0x00 << (char) 0x00 << (char) 0x21 << (char) 0x0C //0x0000210c
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x22 //0x0001d522
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2C //0x0001d52c
              << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x2C //0x0000002c
              << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x20 //0x00000020
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x34 //0x0001d534
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2C //0x0001d52c
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2F //0x0001d52f
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x21 //0x0001d521
              << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x21;//0x00000021


    std::vector<uint32_t> expected_src = { 0x0000210c, 0x0001d522, 0x0001d529, 0x0001d529,
                                           0x0001d52c, 0x0000002c, 0x00000020, 0x0001d534,
                                           0x0001d52c, 0x0001d52f, 0x0001d529, 0x0001d521,
                                           0x00000021 };
    std::u32string        expected_str = std::u32string( expected_src.begin(), expected_src.end() );
    std::vector<uint32_t> out_buffer;

    Transcode::U32BEtoU32( in_source, out_buffer );

    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U32BEtoU32_string1 ) { //with pre-buffered bytes
    std::deque<uint8_t> in_buffer = { 0x00, 0x01, 0xFA, 0xD6 }; //BE 0x0001FAD6
    std::stringstream   in_stream;
    Source              in_source = Source( in_stream, "", Format::UTF32_BE );

    in_stream << (char) 0x00 << (char) 0x00 << (char) 0x21 << (char) 0x0C //0x0000210c
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x22 //0x0001d522
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2C //0x0001d52c
              << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x2C //0x0000002c
              << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x20 //0x00000020
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x34 //0x0001d534
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2C //0x0001d52c
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2F //0x0001d52f
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
              << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x21 //0x0001d521
              << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x21;//0x00000021

    std::vector<uint32_t> expected_src = { 0x0001FAD6, 0x0000210c, 0x0001d522, 0x0001d529,
                                           0x0001d529, 0x0001d52c, 0x0000002c, 0x00000020,
                                           0x0001d534, 0x0001d52c, 0x0001d52f, 0x0001d529,
                                           0x0001d521, 0x00000021 };
    std::u32string        expected_str = std::u32string( expected_src.begin(), expected_src.end() );
    std::vector<uint32_t> out_buffer;

    Transcode::U32BEtoU32( in_buffer, in_source, out_buffer );

    auto output = std::u32string( out_buffer.begin(), out_buffer.end() );
    ASSERT_EQ( expected_str, output );
}

TEST( parser_encoding_Transcode, U8toU32 ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;
    Source                input_src = Source( file, "", Format::UTF8 );

    file.open( source, std::ios::binary | std::ios::in );

    if( file ) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::vector<uint32_t>                                       output_buffer;

        Transcode::U8toU32( input_src, output_buffer );
        file.close();

        auto src_u8  = blogator::tests::loadFile( source );
        auto src_u32 = converter.from_bytes( src_u8 ); //basically testing against STL converter's output
        auto out_u32  = std::u32string( output_buffer.begin(), output_buffer.end() );

        ASSERT_EQ( src_u32, out_u32 );
    } else {
        FAIL() << "Could not open test file: " << source;
    }
}