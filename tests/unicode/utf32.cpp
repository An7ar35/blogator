#include "gtest/gtest.h"
#include "../../src/unicode/utf32.h"

using namespace blogator::unicode::utf32;

TEST( unicode_utf32, join ) {
    ASSERT_EQ( 0b00000000'00000000'00000000'00000000, join( 0b00000000, 0b00000000, 0b00000000, 0b00000000 ) );
    ASSERT_EQ( 0b11111111'11111111'11111111'11111111, join( 0b11111111, 0b11111111, 0b11111111, 0b11111111 ) );
    ASSERT_EQ( 0b11111111'00000000'11111111'00000000, join( 0b11111111, 0b00000000, 0b11111111, 0b00000000 ) );
    ASSERT_EQ( 0b00000000'11111111'00000000'11111111, join( 0b00000000, 0b11111111, 0b00000000, 0b11111111 ) );
    ASSERT_EQ( 0b11110000'00111100'00011110'00001111, join( 0b11110000, 0b00111100, 0b00011110, 0b00001111 ) );
}

TEST( unicode_utf32, toxunicode ) {
    ASSERT_EQ( toxunicode( 0, U"U+" ), U"U+0000" );
    ASSERT_EQ( toxunicode( 1, U"U+" ), U"U+0001" );
    ASSERT_EQ( toxunicode( 162, U"U+" ), U"U+00A2" );
    ASSERT_EQ( toxunicode( 4095, U"U+" ), U"U+0FFF" );
    ASSERT_EQ( toxunicode( 59418, U"U+" ), U"U+E81A" );
    ASSERT_EQ( toxunicode( 65535, U"U+" ), U"U+FFFF" );
    ASSERT_EQ( toxunicode( 131071, U"U+" ), U"U+1FFFF" );
    ASSERT_EQ( toxunicode( 6946816, U"U+" ), U"U+6A0000" );
}
