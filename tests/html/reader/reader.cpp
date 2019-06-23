#include "gtest/gtest.h"
#include "../../../src/html/reader/reader.h"
#include "../../../src/html/reader/reader.cpp"

TEST( html_reader_tests, stripDate0 ) {
    auto result = blogator::html::reader::getDate( "" );
    ASSERT_TRUE( result.empty() );
}

TEST( html_reader_tests, stripDate1 ) {
    auto result = blogator::html::reader::getDate( "<time>5 Juin 2019</time>" );
    ASSERT_TRUE( result.empty() );
}

TEST( html_reader_tests, stripDate2 ) {
    auto result = blogator::html::reader::getDate( "<time datetime=\"2019-05-05\">5 Juin 2019</time>" );
    ASSERT_EQ( "2019-05-05", result );
}

TEST( html_reader_tests, stripTags0 ) {
    auto result = blogator::html::reader::getTags( "" );
    ASSERT_EQ( 0, result.size() );
}

TEST( html_reader_tests, stripTags1 ) {
    auto result = blogator::html::reader::getTags( "<span>First Tag</span>" );
    ASSERT_EQ( 0, result.size() );
}

TEST( html_reader_tests, stripTags2 ) {
    auto result = blogator::html::reader::getTags( "<span class=\"tag\">First Tag</span>" );
    ASSERT_EQ( 1, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
}

TEST( html_reader_tests, stripTags3 ) {
    auto result = blogator::html::reader::getTags( "    <span class=\"tag\">First Tag</span>" );
    ASSERT_EQ( 1, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
}

TEST( html_reader_tests, stripTags4 ) {
    auto result = blogator::html::reader::getTags(
        "<span class=\"tag\">First Tag, Second Tag, Third Tag, nthTag</span>" );
    ASSERT_EQ( 4, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
    ASSERT_EQ( "Second Tag", result.at( 1 ) );
    ASSERT_EQ( "Third Tag", result.at( 2 ) );
    ASSERT_EQ( "nthTag", result.at( 3 ) );
}

TEST( html_reader_tests, stripTags5 ) {
    auto result = blogator::html::reader::getTags( "<span class=\"tag\">First Tag, Second Tag</span>\n"
                                            "<span class=\"tag\">Third Tag</span>\n"
                                            "<span class=\"tag\">nthTag</span>" );
    ASSERT_EQ( 4, result.size() );
    ASSERT_EQ( "First Tag", result.at( 0 ) );
    ASSERT_EQ( "Second Tag", result.at( 1 ) );
    ASSERT_EQ( "Third Tag", result.at( 2 ) );
    ASSERT_EQ( "nthTag", result.at( 3 ) );
}

TEST( html_reader_tests, findLineOfTag0 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 0, blogator::html::reader::findLineOfTag( "</h1>", html ) );
}

TEST( html_reader_tests, findLineOfTag1 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 1, blogator::html::reader::findLineOfTag( "<p>", html ) );
}

TEST( html_reader_tests, findLineOfTag2 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "<h1>lorem ipsum</h1>" );
    html._lines.emplace_back( "\t<p>some more text" );
    html._lines.emplace_back( "\ttext text text text</p>" );
    ASSERT_EQ( 2, blogator::html::reader::findLineOfTag( "</p>", html ) );
}

TEST( html_reader_tests, findLineOfTag_fail0 ) {
    auto html = blogator::dto::HTML();
    ASSERT_THROW( blogator::html::reader::findLineOfTag( "<head>", html ), std::out_of_range );

}

TEST( html_reader_tests, findLineOfTag_fail1 ) {
    auto html = blogator::dto::HTML();
    html._lines.emplace_back( "lorem ipsum..." );
    html._lines.emplace_back( "some more text" );
    ASSERT_THROW( blogator::html::reader::findLineOfTag( "", html ), std::invalid_argument );
}

TEST( html_reader_tests, findInsertPositions1 ) { //empty line
    using blogator::dto::HTML;
    using blogator::dto::Template;
    using blogator::html::reader::findInsertPositions;

    HTML html;
    html._lines.emplace_back( "" );
    Template::DivWritePositions_t collection = {
        { "page_index",   std::vector<Template::InsertPosition_t>() },
        { "post_content", std::vector<Template::InsertPosition_t>() },
        { "index_pane",   std::vector<Template::InsertPosition_t>() }
    };

    findInsertPositions( html, collection );

    for( const auto &div : collection )
        ASSERT_TRUE( div.second.empty() );
}

TEST( html_reader_tests, findInsertPositions2 ) { //1x occurrence
    using blogator::dto::HTML;
    using blogator::dto::Template;
    using blogator::html::reader::findInsertPositions;

    HTML html;
    html._lines.emplace_back( "\t  <div class=\"index_pane\"></div><div class\"a\">some text</div><div class=\"index_pane\"></div>" );
    Template::DivWritePositions_t collection = {
        { "page_index",   std::vector<Template::InsertPosition_t>() },
        { "post_content", std::vector<Template::InsertPosition_t>() },
        { "index_pane",   std::vector<Template::InsertPosition_t>() }
    };

    findInsertPositions( html, collection );

    for( const auto &div : collection ) {
        std::cout << div.first << " = {" << std::endl;
        for( auto e : div.second ) {
            std::cout << "\t{ " << e.first << ", " << e.second << " }" << std::endl;
        }
        std::cout << "}" << std::endl;
    }

    FAIL(); //TODO
}