#include "gtest/gtest.h"
#include "../../src/dto/DateStamp.h"

TEST( DateStamp_tests, compare1 ) {
    using blogator::dto::DateStamp;
    auto day1 = DateStamp( 2000, 01, 01 );
    auto day2 = DateStamp( 2000, 01, 02 );
    ASSERT_EQ( -1, DateStamp::compare( day1, day2 ) );
    ASSERT_EQ(  0, DateStamp::compare( day1, day1 ) );
    ASSERT_EQ(  1, DateStamp::compare( day2, day1 ) );
}

TEST( DateStamp_tests, compare2 ) {
    using blogator::dto::DateStamp;
    auto month1 = DateStamp( 2000, 01, 01 );
    auto month2 = DateStamp( 2000, 02, 01 );
    ASSERT_EQ( -1, DateStamp::compare( month1, month2 ) );
    ASSERT_EQ(  0, DateStamp::compare( month1, month1 ) );
    ASSERT_EQ(  1, DateStamp::compare( month2, month1 ) );
}

TEST( DateStamp_tests, compare3 ) {
    using blogator::dto::DateStamp;
    auto year1 = DateStamp( 2000, 01, 01 );
    auto year2 = DateStamp( 2001, 01, 01 );
    ASSERT_EQ( -1, DateStamp::compare( year1, year2 ) );
    ASSERT_EQ(  0, DateStamp::compare( year1, year1 ) );
    ASSERT_EQ(  1, DateStamp::compare( year2, year1 ) );
}

TEST( DateStamp_tests, concatYYYYMM1 ) {
    using blogator::dto::DateStamp;
    auto a = DateStamp( 2000, 01, 01 );
    ASSERT_EQ( 200001, DateStamp::concatYYYYMM( a ) );
}

TEST( DateStamp_tests, concatYYYYMM2 ) {
    using blogator::dto::DateStamp;
    auto a = DateStamp( 1999, 12, 30 );
    ASSERT_EQ( 199912, DateStamp::concatYYYYMM( a ) );
}