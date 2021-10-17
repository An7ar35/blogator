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

TEST( unicode_utf8, toU32_ASCII ) {
    const uint8_t low   [4] = { 0, 0, 0, 0 }; //NUL
    const uint8_t char_A[4] = { 0x41, 0, 0, 0 }; //'A'
    const uint8_t high  [4] = { 0x7F, 0, 0, 0 }; //DEL

    ASSERT_EQ( 0, blogator::unicode::utf8::toU32( low, 1 ) );
    ASSERT_EQ( U'A', blogator::unicode::utf8::toU32( char_A, 1 ) );
    ASSERT_EQ( 0x7F, blogator::unicode::utf8::toU32( high, 1 ) );
}

TEST( unicode_utf8, toU32_2Bytes ) {
    const uint8_t low [4] = { 0xC2, 0x80, 0, 0 }; //€
    const uint8_t mid [4] = { 0xD2, 0xA2, 0, 0 }; //CYRILLIC CAPITAL LETTER EN WITH DESCENDER
    const uint8_t high[4] = { 0xDF, 0xBF, 0, 0 }; //NKO TAMAN SIGN

    ASSERT_EQ( 0x080, blogator::unicode::utf8::toU32( low, 2 ) );
    ASSERT_EQ( 0x4A2, blogator::unicode::utf8::toU32( mid, 2 ) );
    ASSERT_EQ( 0x7FF, blogator::unicode::utf8::toU32( high, 2 ) );
}

TEST( unicode_utf8, toU32_3Bytes ) {
    const uint8_t low [4] = { 0xE0, 0xA0, 0x80, 0 }; //SAMARITAN LETTER ALAF
    const uint8_t mid [4] = { 0xE1, 0x96, 0xA3, 0 }; //CANADIAN SYLLABICS LHOO
    const uint8_t high[4] = { 0xEF, 0xBF, 0xBF, 0 }; //￿

    ASSERT_EQ( 0x0800, blogator::unicode::utf8::toU32( low, 3 ) );
    ASSERT_EQ( 0x15A3, blogator::unicode::utf8::toU32( mid, 3 ) );
    ASSERT_EQ( 0xFFFF, blogator::unicode::utf8::toU32( high, 3 ) );
}

TEST( unicode_utf8, toU32_4Bytes ) {
    const uint8_t low [4] = { 0xF0, 0x90, 0x80, 0x80 }; //LINEAR B SYLLABLE B008 A
    const uint8_t mid [4] = { 0xF3, 0x85, 0x88, 0xBC }; //󅈼
    const uint8_t high[4] = { 0xF4, 0x8F, 0xBF, 0xBF }; //􏿿

    ASSERT_EQ( 0x010000, blogator::unicode::utf8::toU32( low, 4 ) );
    ASSERT_EQ( 0x0C523C, blogator::unicode::utf8::toU32( mid, 4 ) );
    ASSERT_EQ( 0x10FFFF, blogator::unicode::utf8::toU32( high, 4 ) );
}

TEST( unicode_utf8, toU32_invalid ) {
    const uint8_t a [4] = { 0, 0, 0, 0 };
    ASSERT_THROW( blogator::unicode::utf8::toU32( a, 0 ), std::invalid_argument );
    ASSERT_THROW( blogator::unicode::utf8::toU32( a, 5 ), std::invalid_argument );
}