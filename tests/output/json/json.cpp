#include "gtest/gtest.h"
#include "../../../src/output/json/JSON.h"

TEST( json_Tests, escapeStr1 ) {
    using blogator::output::json::escapeStr;
    auto str = R"(string 'with' "quotations".)";
    auto expected = R"(string 'with' \"quotations\".)";
    ASSERT_EQ( expected, escapeStr( str ) );
}

TEST( json_Tests, escapeStr2 ) {
    using blogator::output::json::escapeStr;
    auto str = "string\t with tab and on\n multiple\nlines.";
    auto expected = R"(string\t with tab and on\n multiple\nlines.)";
    ASSERT_EQ( expected, escapeStr( str ) );
}

TEST( json_Tests, escapeStr3 ) {
    using blogator::output::json::escapeStr;
    auto str = R"(string with \reverse \ solidus.)";
    auto expected = R"(string with \\reverse \\ solidus.)";
    ASSERT_EQ( expected, escapeStr( str ) );
}