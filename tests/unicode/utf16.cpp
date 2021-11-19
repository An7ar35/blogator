#include "gtest/gtest.h"
#include "../../src/unicode/utf16.h"

using namespace blogator::unicode::utf16;

TEST( unicode_utf16, join ) {
    ASSERT_EQ( 0b0000'0000'0000'0000, join( 0b0000'0000, 0b0000'0000 ) );
    ASSERT_EQ( 0b1111'1111'1111'1111, join( 0b1111'1111, 0b1111'1111 ) );
    ASSERT_EQ( 0b0000'1111'1111'0000, join( 0b0000'1111, 0b1111'0000 ) );
    ASSERT_EQ( 0b1111'0000'0000'1111, join( 0b1111'0000, 0b0000'1111 ) );
}

TEST( unicode_utf16, join_LE ) {
    ASSERT_EQ( 0xC21, join( 0xC, 0x21 ) );
}

TEST( unicode_utf16, join_BE ) {
    ASSERT_EQ( 0x210C, join( 0x21, 0x0C ) );
}

TEST( unicode_utf16, codeunits_invalid ) {
    ASSERT_EQ( 0, codeunits( 0xDC00 ) );
    ASSERT_EQ( 0, codeunits( 0xDE00 ) );
    ASSERT_EQ( 0, codeunits( 0xDFFF ) );
}

TEST( unicode_utf16, codeunits_1 ) {
    ASSERT_EQ( 1, codeunits( 0x0000 ) );
    ASSERT_EQ( 1, codeunits( 0x8000 ) );
    ASSERT_EQ( 1, codeunits( 0xD7FF ) );

    ASSERT_EQ( 1, codeunits( 0xE000 ) );
    ASSERT_EQ( 1, codeunits( 0xF000 ) );
    ASSERT_EQ( 1, codeunits( 0xFFFF ) );
}

TEST( unicode_utf16, codeunits_2 ) {
    ASSERT_EQ( 2, codeunits( 0xD800 ) );
    ASSERT_EQ( 2, codeunits( 0xDA00 ) );
    ASSERT_EQ( 2, codeunits( 0xDBC0 ) );
}

TEST( unicode_utf16, toU32_single ) {
    ASSERT_EQ( 0x0000, toU32( 0x0000 ) );
    ASSERT_EQ( 0x0061, toU32( 0x0061 ) );
    ASSERT_EQ( 0xFFFE, toU32( 0xFFFE ) );
    ASSERT_EQ( 0xFFFF, toU32( 0xFFFF ) );
}

TEST( unicode_utf16, toU32_pair ) {
    ASSERT_EQ( 0x010000, toU32( 0xD800, 0xDC00 ) );
    ASSERT_EQ( 0x010001, toU32( 0xD800, 0xDC01 ) );
    ASSERT_EQ( 0x1000FE, toU32( 0xDBC0, 0xDCFE ) );
    ASSERT_EQ( 0x1000FF, toU32( 0xDBC0, 0xDCFF ) );
}