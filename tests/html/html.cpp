#include "gtest/gtest.h"
#include "../../src/html/html.h"
#include "../../src/html/html.cpp"

TEST( html_tests, stripDate0 ) {
    auto result = blogator::html::readDate( "" );
    ASSERT_TRUE( result.empty() );
}

TEST( html_tests, stripDate1 ) {
    auto result = blogator::html::readDate( "<time>5 Juin 2019</time>" );
    ASSERT_TRUE( result.empty() );
}

TEST( html_tests, stripDate2 ) {
    auto result = blogator::html::readDate( "<time datetime=\"2019-05-05\">5 Juin 2019</time>" );
    ASSERT_EQ( "2019-05-05", result );
}

TEST( html_tests, stripTags0 ) {
    auto result = blogator::html::readTags( "" );
    ASSERT_EQ( 0, result.size() );
}

TEST( html_tests, stripTags1 ) {
    auto result = blogator::html::readTags( "<span>First Tag</span>" );
    ASSERT_EQ( 0, result.size() );
}

TEST( html_tests, stripTags2 ) {
    auto result = blogator::html::readTags( "<span class=\"tag\">First Tag</span>" );
    ASSERT_EQ( 1, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
}

TEST( html_tests, stripTags3 ) {
    auto result = blogator::html::readTags( "    <span class=\"tag\">First Tag</span>" );
    ASSERT_EQ( 1, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
}

TEST( html_tests, stripTags4 ) {
    auto result = blogator::html::readTags(
        "<span class=\"tag\">First Tag, Second Tag, Third Tag, nthTag</span>" );
    ASSERT_EQ( 4, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
    ASSERT_EQ( "Second Tag", result.at( 1 ) );
    ASSERT_EQ( "Third Tag", result.at( 2 ) );
    ASSERT_EQ( "nthTag", result.at( 3 ) );
}

TEST( html_tests, stripTags5 ) {
    auto result = blogator::html::readTags( "<span class=\"tag\">First Tag, Second Tag</span>\n"
                                            "<span class=\"tag\">Third Tag</span>\n"
                                            "<span class=\"tag\">nthTag</span>" );
    ASSERT_EQ( 4, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
    ASSERT_EQ( "Second Tag", result.at( 1 ) );
    ASSERT_EQ( "Third Tag", result.at( 2 ) );
    ASSERT_EQ( "nthTag", result.at( 3 ) );
}

TEST( html_tests, findLineOfTag0 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 0, blogator::html::findLineOfTag( "</h1>", html ) );
}

TEST( html_tests, findLineOfTag1 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 1, blogator::html::findLineOfTag( "<p>", html ) );
}

TEST( html_tests, findLineOfTag2 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 2, blogator::html::findLineOfTag( "</p>", html ) );
}

TEST( html_tests, findLineOfTag_fail0 ) {
    auto html = blogator::dto::HTML();
    ASSERT_THROW( blogator::html::findLineOfTag( "<head>", html ), std::out_of_range );

}

TEST( html_tests, findLineOfTag_fail1 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "lorem ipsum..." );
    html._lines.emplace_back( "some more text" );
    ASSERT_THROW( blogator::html::findLineOfTag( "", html ), std::invalid_argument );
}