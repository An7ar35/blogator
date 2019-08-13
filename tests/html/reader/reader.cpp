#include "gtest/gtest.h"
#include "../../../src/html/reader/reader.h"

TEST( html_reader_tests, getContentBetween_string_0 ) {
    using blogator::html::reader::getContentBetween;
    ASSERT_EQ( "", getContentBetween( "<h1>", "</h1>", "" ) );
}

TEST( html_reader_tests, getContentBetween_string_1 ) {
    using blogator::html::reader::getContentBetween;
    ASSERT_EQ( "", getContentBetween( "<h1>", "</h1>", "<h1></h1>" ) );
    ASSERT_EQ( "", getContentBetween( "<time datetime=\"", "\"", "<time>5 Juin 2019</time>" ) );
}

TEST( html_reader_tests, getContentBetween_string_2 ) {
    using blogator::html::reader::getContentBetween;
    ASSERT_EQ( "", getContentBetween( "<h1>", "</h1>", "<h1> some tests...." ) );
    ASSERT_EQ( "", getContentBetween( "<h1>", "</h1>", "some tests....<h1>" ) );
}

TEST( html_reader_tests, getContentBetween_string_3 ) {
    using blogator::html::reader::getContentBetween;
    ASSERT_EQ( "heading", getContentBetween( "<h1>", "</h1>", "<h1>heading</h1>" ) );
    ASSERT_EQ( "heading", getContentBetween( "<h1>", "</h1>", "some text <h1>heading</h1>" ) );
    ASSERT_EQ( "heading", getContentBetween( "<h1>", "</h1>", "<h1>heading</h1> some text" ) );
    ASSERT_EQ( "2019-05-05", getContentBetween( "<time datetime=\"", "\"", "<time datetime=\"2019-05-05\">5 Juin 2019</time>" ) );
}

TEST( html_reader_tests, getContentBetween_string_4 ) {
    using blogator::html::reader::getContentBetween;
    ASSERT_EQ( "     heading      ", getContentBetween( "<h1>", "</h1>", "<h1>     heading      </h1>" ) );
    ASSERT_EQ( "\n\t  heading h1 \n", getContentBetween( "<h1>", "</h1>", "some text <h1>\n\t  heading h1 \n</h1> some text" ) );
}

TEST( html_reader_tests, getContentBetween_HTML_0 ) {
    using blogator::dto::HTML;
    using blogator::html::reader::getContentBetween;
    auto html = HTML();
    html._lines.emplace_back( "some text <p>text to grab</p> some text" );
    auto expected = HTML();
    expected._lines.emplace_back( "text to grab" );
    ASSERT_EQ( expected, getContentBetween( "<p>", "</p>", html ) );
}

TEST( html_reader_tests, getContentBetween_HTML_1 ) {
    using blogator::dto::HTML;
    using blogator::html::reader::getContentBetween;
    auto html = HTML();
    html._lines.emplace_back( "some text" );
    html._lines.emplace_back( "    <p>" );
    html._lines.emplace_back( "        text to grab line1" );
    html._lines.emplace_back( "        text to grab line2" );
    html._lines.emplace_back( "        text to grab line3" );
    html._lines.emplace_back( "     </p>" );
    html._lines.emplace_back( "some text" );
    auto expected = HTML();
    expected._lines.emplace_back( "" );
    expected._lines.emplace_back( "        text to grab line1" );
    expected._lines.emplace_back( "        text to grab line2" );
    expected._lines.emplace_back( "        text to grab line3" );
    expected._lines.emplace_back( "     " );
    ASSERT_EQ( expected, getContentBetween( "<p>", "</p>", html ) );
}

TEST( html_reader_tests, getContentBetween_HTML_2 ) {
    using blogator::dto::HTML;
    using blogator::html::reader::getContentBetween;
    auto html = HTML();
    html._lines.emplace_back( "some text <p>" );
    html._lines.emplace_back( "        text to grab line1" );
    html._lines.emplace_back( "        text to grab line2" );
    html._lines.emplace_back( "        text to grab line3" );
    html._lines.emplace_back( "     </p> some text" );
    auto expected = HTML();
    expected._lines.emplace_back( "" );
    expected._lines.emplace_back( "        text to grab line1" );
    expected._lines.emplace_back( "        text to grab line2" );
    expected._lines.emplace_back( "        text to grab line3" );
    expected._lines.emplace_back( "     " );
    ASSERT_EQ( expected, getContentBetween( "<p>", "</p>", html ) );
}

TEST( html_reader_tests, getContentBetween_HTML_3 ) {
    using blogator::dto::HTML;
    using blogator::html::reader::getContentBetween;
    auto html = HTML();
    html._lines.emplace_back( "some </p> text <p>" );
    html._lines.emplace_back( "        text to grab line1" );
    html._lines.emplace_back( "        text to grab line2" );
    html._lines.emplace_back( "        text to grab line3" );
    html._lines.emplace_back( "     </p> some <p> text" );
    auto expected = HTML();
    expected._lines.emplace_back( "" );
    expected._lines.emplace_back( "        text to grab line1" );
    expected._lines.emplace_back( "        text to grab line2" );
    expected._lines.emplace_back( "        text to grab line3" );
    expected._lines.emplace_back( "     " );
    ASSERT_EQ( expected, getContentBetween( "<p>", "</p>", html ) );
}

TEST( html_reader_tests, getContentsBetween_0 ) {
    using blogator::dto::HTML;
    using blogator::html::reader::getContentsBetween;
    auto expected = std::vector<std::string>( { "some text" } );
    auto text     = "text text text <p class=\"author\">some text</p> more...";
    ASSERT_EQ( expected, getContentsBetween( "<p class=\"author\">", "</p>", text ) );
}

TEST( html_reader_tests, getContentsBetween_1 ) {
    using blogator::dto::HTML;
    using blogator::html::reader::getContentsBetween;
    auto expected = std::vector<std::string>( { "some text" , "", "text3" } );
    auto text     = R"(text text text <p class="author">some text</p> more...<p class="author"></p>  <p class="author">text3</p>)";
    ASSERT_EQ( expected, getContentsBetween( "<p class=\"author\">", "</p>", text ) );
}

TEST( html_reader_tests, getTags0 ) {
    auto result = blogator::html::reader::getTags( "" );
    ASSERT_EQ( 0, result.size() );
}

TEST( html_reader_tests, getTags1 ) {
    auto result = blogator::html::reader::getTags( "<span>First Tag</span>" );
    ASSERT_EQ( 0, result.size() );
}

TEST( html_reader_tests, getTags2 ) {
    auto result = blogator::html::reader::getTags( "<span class=\"tag\">First Tag</span>" );
    ASSERT_EQ( 1, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
}

TEST( html_reader_tests, getTags3 ) {
    auto result = blogator::html::reader::getTags( "    <span class=\"tag\">First Tag</span>" );
    ASSERT_EQ( 1, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
}

TEST( html_reader_tests, getTags4 ) {
    auto result = blogator::html::reader::getTags(
        "<span class=\"tag\">First Tag, Second Tag, Third Tag, nthTag</span>" );
    ASSERT_EQ( 4, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
    ASSERT_EQ( "Second Tag", result.at( 1 ) );
    ASSERT_EQ( "Third Tag", result.at( 2 ) );
    ASSERT_EQ( "nthTag", result.at( 3 ) );
}

TEST( html_reader_tests, getTags5 ) {
    auto result = blogator::html::reader::getTags( "<span class=\"tag\">First Tag, Second Tag</span>\n"
                                            "<span class=\"tag\">Third Tag</span>\n"
                                            "<span class=\"tag\">nthTag</span>" );
    ASSERT_EQ( 4, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
    ASSERT_EQ( "Second Tag", result.at( 1 ) );
    ASSERT_EQ( "Third Tag", result.at( 2 ) );
    ASSERT_EQ( "nthTag", result.at( 3 ) );
}

TEST( html_reader_tests, getIndent ) {
    using blogator::html::reader::getIndent;
    ASSERT_EQ( "", getIndent( "some text") );
    ASSERT_EQ( "  ", getIndent( "  some text") );
    ASSERT_EQ( "\t", getIndent( "\tsome text") );
    ASSERT_EQ( "\t  ", getIndent( "\t  some text") );
}