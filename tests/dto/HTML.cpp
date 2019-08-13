#include "gtest/gtest.h"
#include "../../src/dto/HTML.h"

TEST( HTML_tests, findLineOfTag0 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 0, html.findLineOfTag( "</h1>" ) );
}

TEST( HTML_tests, findLineOfTag1 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 1, html.findLineOfTag( "<p>" ) );
}

TEST( HTML_tests, findLineOfTag2 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 2, html.findLineOfTag( "</p>" ) );
}

TEST( HTML_tests, findLineOfTag_fail0 ) {
    auto html = blogator::dto::HTML();
    ASSERT_THROW( html.findLineOfTag( "<head>" ), std::out_of_range );

}

TEST( HTML_tests, findLineOfTag_fail1 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "lorem ipsum..." );
    html._lines.emplace_back( "some more text" );
    ASSERT_THROW( html.findLineOfTag( "" ), std::invalid_argument );
}