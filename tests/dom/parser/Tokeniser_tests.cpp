#include "gtest/gtest.h"
#include "../../src/dom/parser/Tokeniser.h"

#include <fstream>

namespace Tokeniser_Tests {
    blogator::dom::dto::Text createTextObject( std::u32string line ) {
        auto text = blogator::dom::dto::Text();
        text._lines.emplace_back( std::move( line ) );
        return text;
    }
}

TEST( Tokeniser_Tests, empty_str ) {
    auto text      = Tokeniser_Tests::createTextObject( U"" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );
    ASSERT_EQ( 0, tokeniser.tokenCount() );
}

TEST( Tokeniser_Tests, valid_tag0 ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"<div >" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 0 ).type );
}

TEST( Tokeniser_Tests, valid_tag1 ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"</div >" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 0 ).type );
}

TEST( Tokeniser_Tests, valid_tag2_comment_open ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"<!--" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 0 ).type );
}

TEST( Tokeniser_Tests, valid_tag2_comment_line ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"<!--some comment ... -->" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 3, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 0 ).type );
    ASSERT_EQ( U"<!--", tokeniser.at( 0 ).content );
    ASSERT_EQ( TokenType::TEXT, tokeniser.at( 1 ).type );
    ASSERT_EQ( U"some comment ... ", tokeniser.at( 1 ).content );
    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 2 ).type );
    ASSERT_EQ( U"-->", tokeniser.at( 2 ).content );
}

TEST( Tokeniser_Tests, valid_line1 ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"<div><h1>Title</h1><p>Paragraph text.</p></div>" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 8, tokeniser.tokenCount() );

    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 0 ).type );
    ASSERT_EQ( U"<div>", tokeniser.at( 0 ).content );

    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 1 ).type );
    ASSERT_EQ( U"<h1>", tokeniser.at( 1 ).content );

    ASSERT_EQ( TokenType::TEXT, tokeniser.at( 2 ).type );
    ASSERT_EQ( U"Title", tokeniser.at( 2 ).content );

    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 3 ).type );
    ASSERT_EQ( U"</h1>", tokeniser.at( 3 ).content );

    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 4 ).type );
    ASSERT_EQ( U"<p>", tokeniser.at( 4 ).content );

    ASSERT_EQ( TokenType::TEXT, tokeniser.at( 5 ).type );
    ASSERT_EQ( U"Paragraph text.", tokeniser.at( 5 ).content );

    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 6 ).type );
    ASSERT_EQ( U"</p>", tokeniser.at( 6 ).content );

    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 7 ).type );
    ASSERT_EQ( U"</div>", tokeniser.at( 7 ).content );
}


TEST( Tokeniser_Tests, valid_line2 ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject(
        U"<div class=class1><h1 id='title' >Title</h1 >  <p class=\"id001\">Paragraph text.</p>   </div>"
    );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 10, tokeniser.tokenCount() );

    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 0 ).type );
    ASSERT_EQ( U"<div class=class1>", tokeniser.at( 0 ).content );

    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 1 ).type );
    ASSERT_EQ( U"<h1 id='title' >", tokeniser.at( 1 ).content );

    ASSERT_EQ( TokenType::TEXT, tokeniser.at( 2 ).type );
    ASSERT_EQ( U"Title", tokeniser.at( 2 ).content );

    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 3 ).type );
    ASSERT_EQ( U"</h1 >", tokeniser.at( 3 ).content );

    ASSERT_EQ( TokenType::TEXT, tokeniser.at( 4 ).type );
    ASSERT_EQ( U"  ", tokeniser.at( 4 ).content );

    ASSERT_EQ( TokenType::START_TAG, tokeniser.at( 5 ).type );
    ASSERT_EQ( U"<p class=\"id001\">", tokeniser.at( 5 ).content );

    ASSERT_EQ( TokenType::TEXT, tokeniser.at( 6 ).type );
    ASSERT_EQ( U"Paragraph text.", tokeniser.at( 6 ).content );

    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 7 ).type );
    ASSERT_EQ( U"</p>", tokeniser.at( 7 ).content );

    ASSERT_EQ( TokenType::TEXT, tokeniser.at( 8 ).type );
    ASSERT_EQ( U"   ", tokeniser.at( 8 ).content );

    ASSERT_EQ( TokenType::END_TAG, tokeniser.at( 9 ).type );
    ASSERT_EQ( U"</div>", tokeniser.at( 9 ).content );
}


TEST( Tokeniser_Tests, valid_line3 ) {
    using blogator::dom::parser::TokenType;

    auto text = blogator::dom::dto::Text(
        U"<div class=class1>",
        U"<h1 id='title' >",
        U"Title",
        U"</h1 >",
        U"  <p class=\"id001\">",
        U"Paragraph text.",
        U"</p>",
        U"   </div>"
        "<div id='id002'>",
        " some more text... ",
        "</div>"
    );

    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    for( const auto & t : tokeniser )
        std::cout << t << std::endl;

}