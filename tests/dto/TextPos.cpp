#include "gtest/gtest.h"
#include "../../../src/dto/TextPos.h"

using blogator::TextPos;

TEST( parser_dto_TextPos, operator_equal ) {
    ASSERT_TRUE( TextPos( 5, 10 ) == TextPos( 5, 10 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) == TextPos( 5, 9 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) == TextPos( 5, 11 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) == TextPos( 4, 10 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) == TextPos( 6, 10 ) );
}

TEST( parser_dto_TextPos, operator_not_equal ) {
    ASSERT_FALSE( TextPos( 5, 10 ) != TextPos( 5, 10 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) != TextPos( 5, 9 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) != TextPos( 5, 11 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) != TextPos( 4, 10 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) != TextPos( 6, 10 ) );
}

TEST( parser_dto_TextPos, operator_greater ) {
    ASSERT_FALSE( TextPos( 5, 10 ) > TextPos( 5, 10 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) > TextPos( 5, 9 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) > TextPos( 5, 11 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) > TextPos( 4, 10 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) > TextPos( 6, 10 ) );
}

TEST( parser_dto_TextPos, operator_greater_equal ) {
    ASSERT_TRUE( TextPos( 5, 10 ) >= TextPos( 5, 10 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) >= TextPos( 5, 9 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) >= TextPos( 5, 11 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) >= TextPos( 4, 10 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) >= TextPos( 6, 10 ) );
}

TEST( parser_dto_TextPos, operator_lesser ) {
    ASSERT_FALSE( TextPos( 5, 10 ) < TextPos( 5, 10 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) < TextPos( 5, 9 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) < TextPos( 5, 11 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) < TextPos( 4, 10 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) < TextPos( 6, 10 ) );
}

TEST( parser_dto_TextPos, operator_lesser_equal ) {
    ASSERT_TRUE( TextPos( 5, 10 ) <= TextPos( 5, 10 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) <= TextPos( 5, 9 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) <= TextPos( 5, 11 ) );
    ASSERT_FALSE( TextPos( 5, 10 ) <= TextPos( 4, 10 ) );
    ASSERT_TRUE( TextPos( 5, 10 ) <= TextPos( 6, 10 ) );
}