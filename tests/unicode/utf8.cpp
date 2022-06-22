#include "gtest/gtest.h"
#include "../../src/unicode/utf8.h"

using namespace blogator::unicode::utf8;

TEST( unicode_utf8, bytelength ) {
    ASSERT_EQ( 1, bytelength( 0x00 ) ); //0000 0000
    ASSERT_EQ( 1, bytelength( 0x7F ) ); //0111 1111

    ASSERT_EQ( 0, bytelength( 0x8F ) );           //1000 0000
    ASSERT_EQ( 0, bytelength( 0b11000000 - 1 ) ); //1011 1111

    ASSERT_EQ( 2, bytelength( 0b11000000 ) );
    ASSERT_EQ( 2, bytelength( 0b11100000 - 1 ) ); //1101 1111

    ASSERT_EQ( 3, bytelength( 0b11100000 ) );
    ASSERT_EQ( 3, bytelength( 0b11110000 - 1 ) ); //1110 1111

    ASSERT_EQ( 4, bytelength( 0b11110000 ) );
    ASSERT_EQ( 4, bytelength( 0b11110111 ) );

    ASSERT_EQ( 0, bytelength( 0b11111000 ) );
}

TEST( unicode_utf8, toxunicode_uint8_prefix ) {
    ASSERT_EQ( "0x00", blogator::unicode::utf8::toxunicode( (char8_t) 0, "0x" ) );
}

TEST( unicode_utf8, toxunicode_uint8 ) {
    ASSERT_EQ( "21", blogator::unicode::utf8::toxunicode( (char8_t) 33, "" ) );
    ASSERT_EQ( "9F", blogator::unicode::utf8::toxunicode( (char8_t) 159, "" ) );
    ASSERT_EQ( "F0", blogator::unicode::utf8::toxunicode( (char8_t) 240, "" ) );
    ASSERT_EQ( "FF", blogator::unicode::utf8::toxunicode( (char8_t) 255, "" ) );
}

TEST( unicode_utf8, toxunicode_uint16_prefix ) {
    ASSERT_EQ( "0x0000", blogator::unicode::utf8::toxunicode( (char16_t) 0, "0x" ) );
}

TEST( unicode_utf8, toxunicode_uint16 ) {
    ASSERT_EQ( "00A0", blogator::unicode::utf8::toxunicode( (char16_t) 160, "" ) );
    ASSERT_EQ( "01D5", blogator::unicode::utf8::toxunicode( (char16_t) 469, "" ) );
    ASSERT_EQ( "10C0", blogator::unicode::utf8::toxunicode( (char16_t) 4288, "" ) );
    ASSERT_EQ( "FFFF", blogator::unicode::utf8::toxunicode( (char16_t) 65535, "" ) );
}

TEST( unicode_utf8, toxunicode_uint32_prefix ) {
    ASSERT_EQ( "0x00000000", blogator::unicode::utf8::toxunicode( (char32_t) 0, "0x" ) );
}

TEST( unicode_utf8, toxunicode_uint32 ) {
    ASSERT_EQ( "00000099", blogator::unicode::utf8::toxunicode( (char32_t) 153, "" ) );
    ASSERT_EQ( "0000F122", blogator::unicode::utf8::toxunicode( (char32_t) 61730, "" ) );
    ASSERT_EQ( "00A2004C", blogator::unicode::utf8::toxunicode( (char32_t) 10616908, "" ) );
    ASSERT_EQ( "FFFFFFFF", blogator::unicode::utf8::toxunicode( (char32_t) 4294967295, "" ) );
}

TEST( unicode_utf8, toU32_ASCII ) {
    const char8_t low   [4] = { 0, 0, 0, 0 }; //NUL
    const char8_t char_A[4] = { 0x41, 0, 0, 0 }; //'A'
    const char8_t high  [4] = { 0x7F, 0, 0, 0 }; //DEL

    ASSERT_EQ( 0, blogator::unicode::utf8::toU32( low, 1 ) );
    ASSERT_EQ( U'A', blogator::unicode::utf8::toU32( char_A, 1 ) );
    ASSERT_EQ( 0x7F, blogator::unicode::utf8::toU32( high, 1 ) );
}

TEST( unicode_utf8, toU32_2Bytes ) {
    const char8_t low [4] = { 0xC2, 0x80, 0, 0 }; //€
    const char8_t mid [4] = { 0xD2, 0xA2, 0, 0 }; //CYRILLIC CAPITAL LETTER EN WITH DESCENDER
    const char8_t high[4] = { 0xDF, 0xBF, 0, 0 }; //NKO TAMAN SIGN

    ASSERT_EQ( 0x080, blogator::unicode::utf8::toU32( low, 2 ) );
    ASSERT_EQ( 0x4A2, blogator::unicode::utf8::toU32( mid, 2 ) );
    ASSERT_EQ( 0x7FF, blogator::unicode::utf8::toU32( high, 2 ) );
}

TEST( unicode_utf8, toU32_3Bytes ) {
    const char8_t low [4] = { 0xE0, 0xA0, 0x80, 0 }; //SAMARITAN LETTER ALAF
    const char8_t mid [4] = { 0xE1, 0x96, 0xA3, 0 }; //CANADIAN SYLLABICS LHOO
    const char8_t high[4] = { 0xEF, 0xBF, 0xBF, 0 }; //￿

    ASSERT_EQ( 0x0800, blogator::unicode::utf8::toU32( low, 3 ) );
    ASSERT_EQ( 0x15A3, blogator::unicode::utf8::toU32( mid, 3 ) );
    ASSERT_EQ( 0xFFFF, blogator::unicode::utf8::toU32( high, 3 ) );
}

TEST( unicode_utf8, toU32_4Bytes ) {
    const char8_t low [4] = { 0xF0, 0x90, 0x80, 0x80 }; //LINEAR B SYLLABLE B008 A
    const char8_t mid [4] = { 0xF3, 0x85, 0x88, 0xBC }; //󅈼
    const char8_t high[4] = { 0xF4, 0x8F, 0xBF, 0xBF }; //􏿿

    ASSERT_EQ( 0x010000, blogator::unicode::utf8::toU32( low, 4 ) );
    ASSERT_EQ( 0x0C523C, blogator::unicode::utf8::toU32( mid, 4 ) );
    ASSERT_EQ( 0x10FFFF, blogator::unicode::utf8::toU32( high, 4 ) );
}

TEST( unicode_utf8, toU32_invalid ) {
    const char8_t a [4] = { 0, 0, 0, 0 };
    ASSERT_THROW( blogator::unicode::utf8::toU32( a, 0 ), std::invalid_argument );
    ASSERT_THROW( blogator::unicode::utf8::toU32( a, 5 ), std::invalid_argument );
}