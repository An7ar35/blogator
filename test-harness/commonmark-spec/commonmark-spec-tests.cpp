//TODO description and notes
#include "gtest/gtest.h"
#include "../../../src/parser/logging/ParserLog.h"

#include "../helpers/helpers.h"

using blogator::parser::logging::ParserLog;


testing::AssertionResult runMarkdownTest( const test_harness::commonmark_spec_tests::CommonMarkTest &test, const std::filesystem::path &path ) {
    return testing::AssertionFailure(); //TODO
}

  class parser_tokeniser_CommonMarkSpec_Tests : public testing::TestWithParam<std::pair<test_harness::commonmark_spec_tests::CommonMarkTest, std::filesystem::path>> {};

TEST_P( parser_tokeniser_CommonMarkSpec_Tests, commonmark_spec_tests) {
    auto test = GetParam();
    EXPECT_TRUE( runMarkdownTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    MarkdownTokeniserTestInstance,
    parser_tokeniser_CommonMarkSpec_Tests,
    ::testing::ValuesIn( test_harness::commonmark_spec_tests::loadMarkdownTests( test_harness::COMMONMARK_SPEC_TEST_PATH ) )
);