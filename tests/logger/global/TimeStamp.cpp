#include "gtest/gtest.h"
#include "../../../src/logger/global/TimeStamp.h"

class logger_TimeStamp_Tests : public testing::Test {
  protected:
    static void SetUpTestSuite() {
        logger_TimeStamp_Tests::_sys_time = std::chrono::high_resolution_clock::now();
    }

    static blogator::logger::TimeStamp::HiResTimePoint_t _sys_time;
};

blogator::logger::TimeStamp::HiResTimePoint_t logger_TimeStamp_Tests::_sys_time = std::chrono::high_resolution_clock::now();

using blogator::logger::TimeStamp;

TEST_F( logger_TimeStamp_Tests, equal_operator ) {
    auto ts1 = TimeStamp( logger_TimeStamp_Tests::_sys_time );
    auto ts2 = TimeStamp( logger_TimeStamp_Tests::_sys_time );

    ASSERT_EQ( ts1, ts2 );
}

TEST_F( logger_TimeStamp_Tests, not_equal_operator ) {
    auto ts1 = TimeStamp( logger_TimeStamp_Tests::_sys_time );
    auto ts2 = TimeStamp( std::chrono::high_resolution_clock::now() );

    ASSERT_NE( ts1, ts2 );
}

TEST_F( logger_TimeStamp_Tests, ostream_operator ) {
    auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_time );

    std::stringstream expected;
    const auto tp = std::chrono::high_resolution_clock::to_time_t( logger_TimeStamp_Tests::_sys_time );
    expected << std::put_time( std::gmtime( &tp ), "%FT%TZ" );

    std::stringstream returned;
    returned << ts;

    ASSERT_EQ( returned.str(), expected.str() );
}

TEST_F( logger_TimeStamp_Tests, getTime ) {
    const auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_time );
    const auto tp = std::chrono::high_resolution_clock::to_time_t( logger_TimeStamp_Tests::_sys_time );

    std::stringstream expected;
    expected << std::put_time( std::localtime( &tp ), "%H:%M:%S" );

    ASSERT_EQ( ts.getTime(), expected.str() );
}

TEST_F( logger_TimeStamp_Tests, getDate ) {
    const auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_time );
    const auto tp = std::chrono::high_resolution_clock::to_time_t( logger_TimeStamp_Tests::_sys_time );

    std::stringstream expected;
    expected << std::put_time( std::localtime( &tp ), "%d/%m/%Y" );

    ASSERT_EQ( ts.getDate(), expected.str() );
}

TEST_F( logger_TimeStamp_Tests, getTimeStamp ) {
    const auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_time );
    const auto tp = std::chrono::high_resolution_clock::to_time_t( logger_TimeStamp_Tests::_sys_time );
    const auto fmt = "%d-%m-%Y %H:%M:%S";

    std::stringstream expected;
    expected << std::put_time( std::localtime( &tp ), fmt );

    ASSERT_EQ( ts.getTimeStamp( fmt ), expected.str() );
}

TEST_F( logger_TimeStamp_Tests, getUTC_formatter ) {
    const auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_time );
    const auto tp = std::chrono::high_resolution_clock::to_time_t( logger_TimeStamp_Tests::_sys_time );

    std::stringstream expected;
    expected << std::put_time( std::gmtime( &tp ), "%FT%TZ" );

    ASSERT_EQ( ts.getUTC(), expected.str() );
}

TEST_F( logger_TimeStamp_Tests, getUTC_precision ) {
    const auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_time );

    const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>( logger_TimeStamp_Tests::_sys_time );
    const auto fraction = logger_TimeStamp_Tests::_sys_time - seconds;

    std::stringstream expected;
    expected << '.' << std::setfill( '0' ) << std::setw( 9 ) << fraction.count();

    ASSERT_EQ( ts.getUTC( "", 9 ), expected.str() );
}