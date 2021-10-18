#include "gtest/gtest.h"
#include "../../../src/parser/encoding/Encoding.h"

#include <fstream>
#include <locale>
#include <codecvt>

#include "../../helper.h"

using namespace blogator::parser::encoding;

TEST( parser_encoding_Encoding, U32toU32_string ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    std::vector<uint32_t> src = { 0x0000210c, 0x0001d522, 0x0001d529, 0x0001d529, 0x0001d52c, 0x0000002c, 0x00000020,
                                  0x0001d534, 0x0001d52c, 0x0001d52f, 0x0001d529, 0x0001d521, 0x00000021 };

    std::u32string        src_u32 = std::u32string( src.begin(), src.end() );
    std::stringstream     char_stream;
    std::vector<uint32_t> buffer_out;

    Encoding::U32toU32( src_u32, char_stream );
    Encoding::U32toU32( char_stream, buffer_out );

    ASSERT_EQ( src_u32, std::u32string( buffer_out.begin(), buffer_out.end() ) );
}

TEST( parser_encoding_Encoding, U32toU32_file ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    auto src_u8  = blogator::tests::loadFile( source );
    auto src_u32 = converter.from_bytes( src_u8 );

    std::stringstream     char_stream;
    std::vector<uint32_t> buffer_out;

    Encoding::U32toU32( src_u32, char_stream );
    Encoding::U32toU32( char_stream, buffer_out );

    ASSERT_EQ( src_u32, std::u32string( buffer_out.begin(), buffer_out.end() ) );
}


TEST( parser_encoding_Encoding, U8toU32 ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;

    file.open( source, std::ios::binary | std::ios::in );

    if( file ) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::vector<uint32_t>                                       output_buffer;

        Encoding::U8toU32( file, output_buffer );
        file.close();

        auto src_u8  = blogator::tests::loadFile( source );
        auto src_u32 = converter.from_bytes( src_u8 ); //basically testing against STL converter's output
        auto out_u32  = std::u32string( output_buffer.begin(), output_buffer.end() );

        ASSERT_EQ( src_u32, out_u32 );
    } else {
        FAIL() << "Could not open test file: " << source;
    }
}