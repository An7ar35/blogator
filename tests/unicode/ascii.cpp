#include "gtest/gtest.h"
#include "../../src/unicode/ascii.h"

TEST( unicode_ascii, isalpha ) {
    for( uint32_t i = 0; i <= 0x7F; ++i ) {
        if( ( i >= 0x41 && i <= 0x5A ) || ( i >= 0x61 && i <= 0x7A ) ) {
            ASSERT_TRUE( blogator::unicode::ascii::isalpha( i ) ) << "Expected true: '" << i << "'";
        } else {
            ASSERT_FALSE( blogator::unicode::ascii::isalpha( i ) )  << "Expected false: '" << i << "'";
        }
    }
    ASSERT_FALSE( blogator::unicode::ascii::isalpha( 0x80 ) )  << "Expected false: '" << 0x80 << "'";
}

TEST( unicode_ascii, isdigit ) {
    for( uint32_t i = 0; i <= 0x7F; ++i ) {
        if( ( i >= 0x30 && i <= 0x39 ) ) {
            ASSERT_TRUE( blogator::unicode::ascii::isdigit( i ) ) << "Expected true: '" << i << "'";
        } else {
            ASSERT_FALSE( blogator::unicode::ascii::isdigit( i ) )  << "Expected false: '" << i << "'";
        }
    }
    ASSERT_FALSE( blogator::unicode::ascii::isdigit( 0x80 ) )  << "Expected false: '" << 0x80 << "'";
}