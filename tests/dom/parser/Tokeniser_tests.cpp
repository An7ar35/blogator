#include "gtest/gtest.h"
#include "../../src/dom/parser/Tokeniser.h"

TEST( Tokeniser_Tests, empty_str ) {
    auto text      = blogator::dom::dto::Text( U"" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );
    ASSERT_EQ( 0, tokeniser.tokenCount() );
}

TEST( Tokeniser_Tests, valid_tag0 ) {
    using blogator::dom::parser::TokenType;
    using blogator::dom::parser::Token;

    auto text      = blogator::dom::dto::Text( U"<div >" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( Token( { 1, 1, TokenType::START_TAG, U"<div >" } ), tokeniser.at( 0 ) );
}

TEST( Tokeniser_Tests, valid_tag1 ) {
    using blogator::dom::parser::TokenType;
    using blogator::dom::parser::Token;

    auto text      = blogator::dom::dto::Text( U"</div >" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( Token( { 1, 1, TokenType::END_TAG, U"</div >" } ), tokeniser.at( 0 ) );
}

TEST( Tokeniser_Tests, valid_tag2_comment_open ) {
    using blogator::dom::parser::TokenType;
    using blogator::dom::parser::Token;

    auto text      = blogator::dom::dto::Text( U"<!--" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( Token( { 1, 1, TokenType::START_TAG, U"<!--" } ), tokeniser.at( 0 ) );
}

TEST( Tokeniser_Tests, valid_tag2_comment_line ) {
    using blogator::dom::parser::TokenType;
    using blogator::dom::parser::Token;

    auto text      = blogator::dom::dto::Text( U"<!--some comment ... -->" );
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
    using blogator::dom::parser::Token;

    auto text      = blogator::dom::dto::Text( U"<div><h1>Title</h1><p>Paragraph text.</p></div>" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 8, tokeniser.tokenCount() );
    ASSERT_EQ( Token( { 1, 1, TokenType::START_TAG, U"<div>" } ), tokeniser.at( 0 ) );
    ASSERT_EQ( Token( { 1, 6, TokenType::START_TAG, U"<h1>" } ), tokeniser.at( 1 ) );
    ASSERT_EQ( Token( { 1, 10, TokenType::TEXT, U"Title" } ), tokeniser.at( 2 ) );
    ASSERT_EQ( Token( { 1, 15, TokenType::END_TAG, U"</h1>" } ), tokeniser.at( 3 ) );
    ASSERT_EQ( Token( { 1, 20, TokenType::START_TAG, U"<p>" } ), tokeniser.at( 4 ) );
    ASSERT_EQ( Token( { 1, 23, TokenType::TEXT, U"Paragraph text." } ), tokeniser.at( 5 ) );
    ASSERT_EQ( Token( { 1, 38, TokenType::END_TAG, U"</p>" } ), tokeniser.at( 6 ) );
    ASSERT_EQ( Token( { 1, 42, TokenType::END_TAG, U"</div>" } ), tokeniser.at( 7 ) );
}


TEST( Tokeniser_Tests, valid_line2 ) {
    using blogator::dom::parser::TokenType;
    using blogator::dom::parser::Token;

    auto text = blogator::dom::dto::Text(
        U"<div class=class1><h1 id='title' >Title</h1 >  <p class=\"id001\">Paragraph text.</p>   </div>"
    );

    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 10, tokeniser.tokenCount() );
    ASSERT_EQ( Token( { 1, 1, TokenType::START_TAG, U"<div class=class1>" } ), tokeniser.at( 0 ) );
    ASSERT_EQ( Token( { 1, 19, TokenType::START_TAG, U"<h1 id='title' >" } ), tokeniser.at( 1 ) );
    ASSERT_EQ( Token( { 1, 35, TokenType::TEXT, U"Title" } ), tokeniser.at( 2 ) );
    ASSERT_EQ( Token( { 1, 40, TokenType::END_TAG, U"</h1 >" } ), tokeniser.at( 3 ) );
    ASSERT_EQ( Token( { 1, 46, TokenType::TEXT, U"  " } ), tokeniser.at( 4 ) );
    ASSERT_EQ( Token( { 1, 48, TokenType::START_TAG, U"<p class=\"id001\">" } ), tokeniser.at( 5 ) );
    ASSERT_EQ( Token( { 1, 65, TokenType::TEXT, U"Paragraph text." } ), tokeniser.at( 6 ) );
    ASSERT_EQ( Token( { 1, 80, TokenType::END_TAG, U"</p>" } ), tokeniser.at( 7 ) );
    ASSERT_EQ( Token( { 1, 84, TokenType::TEXT, U"   " } ), tokeniser.at( 8 ) );
    ASSERT_EQ( Token( { 1, 87, TokenType::END_TAG, U"</div>" } ), tokeniser.at( 9 ) );
}


TEST( Tokeniser_Tests, valid_multi_lines1 ) {
    using blogator::dom::parser::TokenType;
    using blogator::dom::parser::Token;

    auto text = blogator::dom::dto::Text(
        U"<div class=class1>",
        U"<h1 id='title' >",
        U"Title",
        U"</h1 >",
        U"  <p class=\"id001\">",
        U"Paragraph text.",
        U"</p>",
        U"   </div>",
        "<div id='id002'>",
        " some more text... ",
        "</div>"
    );

    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 13, tokeniser.tokenCount() );
    ASSERT_EQ( Token ( { 1, 1, TokenType::START_TAG, U"<div class=class1>" } ), tokeniser.at( 0 ) );
    ASSERT_EQ( Token ( { 2, 1, TokenType::START_TAG, U"<h1 id='title' >" } ), tokeniser.at( 1 ) );
    ASSERT_EQ( Token ( { 3, 1, TokenType::TEXT, U"Title" } ), tokeniser.at( 2 ) );
    ASSERT_EQ( Token ( { 4, 1, TokenType::END_TAG, U"</h1 >" } ), tokeniser.at( 3 ) );
    ASSERT_EQ( Token ( { 5, 1, TokenType::TEXT, U"  " } ), tokeniser.at( 4 ) );
    ASSERT_EQ( Token ( { 5, 3, TokenType::START_TAG, U"<p class=\"id001\">" } ), tokeniser.at( 5 ) );
    ASSERT_EQ( Token ( { 6, 1, TokenType::TEXT, U"Paragraph text." } ), tokeniser.at( 6 ) );
    ASSERT_EQ( Token ( { 7, 1, TokenType::END_TAG, U"</p>" } ), tokeniser.at( 7 ) );
    ASSERT_EQ( Token ( { 8, 1, TokenType::TEXT, U"   " } ), tokeniser.at( 8 ) );
    ASSERT_EQ( Token ( { 8, 4, TokenType::END_TAG, U"</div>" } ), tokeniser.at( 9 ) );
    ASSERT_EQ( Token ( { 9, 1, TokenType::START_TAG, U"<div id='id002'>" } ), tokeniser.at( 10 ) );
    ASSERT_EQ( Token ( { 10, 1, TokenType::TEXT, U" some more text... " } ), tokeniser.at( 11 ) );
    ASSERT_EQ( Token ( { 11, 1, TokenType::END_TAG, U"</div>" } ), tokeniser.at( 12 ) );
}

TEST( Tokeniser_Tests, valid_multi_lines2 ) {
    using blogator::dom::parser::TokenType;
    using blogator::dom::parser::Token;

    auto text = blogator::dom::dto::Text(
        U"<div paragraph text </div> everything is going to",
        " break<p> oh fml...</p"
    );

    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    for( const auto & t : tokeniser )
        std::cout << t << std::endl;

}