#include "gtest/gtest.h"
#include "../../../src/logger/global/LogLevel.h"

#include <vector>

class logger_LogLevel_Tests : public testing::Test {
  public:
    struct Test {
        blogator::logger::LogLevel level;
        std::string                expected;
    };

  protected:
    static void SetUpTestSuite() {
        logger_LogLevel_Tests::tests = loadTests();
    }

    static std::vector<Test> loadTests() {
        return {
            { static_cast<blogator::logger::LogLevel>( -1 ), "UNKNOWN" },
            { blogator::logger::LogLevel::EMERGENCY, "EMERGENCY" },
            { blogator::logger::LogLevel::ALERT, "ALERT" },
            { blogator::logger::LogLevel::CRITICAL, "CRITICAL" },
            { blogator::logger::LogLevel::ERROR, "ERROR" },
            { blogator::logger::LogLevel::WARNING, "WARNING" },
            { blogator::logger::LogLevel::NOTICE, "NOTICE" },
            { blogator::logger::LogLevel::INFO, "INFO" },
            { blogator::logger::LogLevel::DEBUG, "DEBUG" },
            { blogator::logger::LogLevel::TRACE, "TRACE" },
            { static_cast<blogator::logger::LogLevel>( 9 ), "UNKNOWN" },
        };
    }

    static std::vector<Test> tests;
};

std::vector<logger_LogLevel_Tests::Test> logger_LogLevel_Tests::tests = logger_LogLevel_Tests::loadTests();


TEST_F( logger_LogLevel_Tests, ostream_operator ) {
    const auto & tests = logger_LogLevel_Tests::tests;

    for( size_t i = 0; i < tests.size(); ++i ) {
        std::stringstream ss;
        ss << tests[i].level;

        ASSERT_EQ( ss.str(), tests[i].expected )
            << "Failed on test case #" << i << ": '"<< tests[i].expected << "'";
    }
}

TEST_F( logger_LogLevel_Tests, to_string ) {
    const auto & tests = logger_LogLevel_Tests::tests;

    for( size_t i = 0; i < tests.size(); ++i ) {
        ASSERT_EQ( blogator::logger::to_string( tests[i].level ), tests[i].expected )
            << "Failed on test case #" << i << ": '"<< tests[i].expected << "'";
    }
}