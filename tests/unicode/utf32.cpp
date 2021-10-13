#include "gtest/gtest.h"
#include "../../src/unicode/utf32.h"

using namespace blogator::unicode::utf32;

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
