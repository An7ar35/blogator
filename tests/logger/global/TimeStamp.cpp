#include "gtest/gtest.h"
#include "../../../src/logger/global/TimeStamp.h"

class logger_TimeStamp_Tests : public testing::Test {
  protected:
    static void SetUpTestSuite() {
        logger_TimeStamp_Tests::_sys_ts = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
    }

    static std::time_t _sys_ts;
};

std::time_t logger_TimeStamp_Tests::_sys_ts = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );


TEST_F( logger_TimeStamp_Tests, ostream_operator ) {
    using blogator::logger::TimeStamp;

    auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_ts );

    std::stringstream expected;
    expected << std::put_time( std::gmtime( &logger_TimeStamp_Tests::_sys_ts ), "%FT%TZ" );

    std::stringstream returned;
    returned << ts;

    ASSERT_EQ( returned.str(), expected.str() );
}

TEST_F( logger_TimeStamp_Tests, getTime ) {
    FAIL();
}

TEST_F( logger_TimeStamp_Tests, getDate ) {
    FAIL();
}

TEST_F( logger_TimeStamp_Tests, getTimeStamp ) {
    FAIL();
}

TEST_F( logger_TimeStamp_Tests, getUTC ) {
    using blogator::logger::TimeStamp;

    auto ts = TimeStamp( logger_TimeStamp_Tests::_sys_ts );

    std::stringstream expected;
    expected << std::put_time( std::gmtime( &logger_TimeStamp_Tests::_sys_ts ), "%FT%TZ" );

    ASSERT_EQ( ts.getUTC(), expected.str() );
}