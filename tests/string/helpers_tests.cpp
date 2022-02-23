#include "gtest/gtest.h"
#include "../../src/string/helpers.h"

TEST( string_Tests, split0 ) {
    using blogator::string::split;

    std::string str = "";
    char        sep = ' ';

    auto res = split<char>( str, sep );
    ASSERT_TRUE( res.empty() );
}

TEST( string_Tests, split1 ) {
    using blogator::string::split;

    std::string str = "hello_world!";
    char        sep = ' ';

    auto res = split<char>( str, sep );
    ASSERT_EQ( 1, res.size() );
    ASSERT_EQ( "hello_world!", res.at( 0 ) );
}

TEST( string_Tests, split2 ) {
    using blogator::string::split;

    std::string str = "hello, world!";
    char        sep = ' ';

    auto res = split<char>( str, sep );
    ASSERT_EQ( 2, res.size() );
    ASSERT_EQ( "hello,", res.at( 0 ) );
    ASSERT_EQ( "world!", res.at( 1 ) );
}

TEST( string_Tests, split3 ) {
    using blogator::string::split;

    std::string str = " one two three four five six ";
    char        sep = ' ';

    auto res = split<char>( str, sep );
    ASSERT_EQ( 8,       res.size() );
    ASSERT_EQ( "",      res.at( 0 ) );
    ASSERT_EQ( "one",   res.at( 1 ) );
    ASSERT_EQ( "two",   res.at( 2 ) );
    ASSERT_EQ( "three", res.at( 3 ) );
    ASSERT_EQ( "four",  res.at( 4 ) );
    ASSERT_EQ( "five",  res.at( 5 ) );
    ASSERT_EQ( "six",   res.at( 6 ) );
    ASSERT_EQ( "",      res.at( 7 ) );
}

TEST( string_Tests, split4 ) {
    using blogator::string::split;

    std::string str = " one two three four five six ";
    char        sep = ' ';

    auto res = split<char>( str, sep, true );
    ASSERT_EQ( 6,       res.size() );
    ASSERT_EQ( "one",   res.at( 0 ) );
    ASSERT_EQ( "two",   res.at( 1 ) );
    ASSERT_EQ( "three", res.at( 2 ) );
    ASSERT_EQ( "four",  res.at( 3 ) );
    ASSERT_EQ( "five",  res.at( 4 ) );
    ASSERT_EQ( "six",   res.at( 5 ) );
}

TEST( string_Tests, split5 ) {
    using blogator::string::split;

    std::string str = "  three  five ";
    char        sep = ' ';

    auto res = split<char>( str, sep );
    ASSERT_EQ( 6,       res.size() );
    ASSERT_EQ( "",      res.at( 0 ) );
    ASSERT_EQ( "",      res.at( 1 ) );
    ASSERT_EQ( "three", res.at( 2 ) );
    ASSERT_EQ( "",      res.at( 3 ) );
    ASSERT_EQ( "five",  res.at( 4 ) );
    ASSERT_EQ( "",  res.at( 5 ) );
}

TEST( string_Tests, split6 ) {
    using blogator::string::split;

    std::string str = "  three  five ";
    char        sep = ' ';

    auto res = split<char>( str, sep, true );
    ASSERT_EQ( 2,       res.size() );
    ASSERT_EQ( "three", res.at( 0 ) );
    ASSERT_EQ( "five",  res.at( 1 ) );
}

TEST( string_Tests, split7 ) {
    using blogator::string::split;

    std::string str = "1,2,3,,,6,,";
    char        sep = ',';

    auto res = split<char>( str, sep );
    ASSERT_EQ( 8,   res.size() );
    ASSERT_EQ( "1", res.at( 0 ) );
    ASSERT_EQ( "2", res.at( 1 ) );
    ASSERT_EQ( "3", res.at( 2 ) );
    ASSERT_EQ( "",  res.at( 3 ) );
    ASSERT_EQ( "",  res.at( 4 ) );
    ASSERT_EQ( "6", res.at( 5 ) );
    ASSERT_EQ( "",  res.at( 6 ) );
    ASSERT_EQ( "",  res.at( 7 ) );
}

TEST( string_Tests, split8 ) {
    using blogator::string::split;

    std::string str = "1,2,3,,,6,,";
    char        sep = ',';

    auto res = split<char>( str, sep, true );
    ASSERT_EQ( 4,   res.size() );
    ASSERT_EQ( "1", res.at( 0 ) );
    ASSERT_EQ( "2", res.at( 1 ) );
    ASSERT_EQ( "3", res.at( 2 ) );
    ASSERT_EQ( "6", res.at( 3 ) );
}

TEST( string_Tests, skipWhitespace1 ) {
    using blogator::string::skipWhitespace;
    std::basic_string<char> str = "    ";
    auto it = str.cbegin();
    it = skipWhitespace<char>( str, it );
    ASSERT_EQ( it, str.cend() );
}

TEST( string_Tests, skipWhitespace2 ) {
    using blogator::string::skipWhitespace;
    std::basic_string<char> str = "hello world";
    auto it = str.cbegin();
    it = skipWhitespace<char>( str, it );
    ASSERT_TRUE( it != str.cend() && *it == 'h' );
}

TEST( string_Tests, skipWhitespace3 ) {
    using blogator::string::skipWhitespace;
    std::basic_string<char> str = "hello \t  world";
    auto it = std::next( str.cbegin(), 5 );
    it = skipWhitespace<char>( str, it );
    ASSERT_TRUE( it != str.cend() && *it == 'w' );
}