#include "gtest/gtest.h"
#include "../../../src/dom/parser/Parser.h"
#include "../../../src/dom/dot/DOTNode.h"

class Parser_Tests : public testing::Test {
  protected:
    void SetUp() override {
        node = std::make_unique<blogator::dom::DOTNode>( blogator::dom::html5::Tag::NONE );
    }
    void TearDown() override {}
  public:
    std::unique_ptr<blogator::dom::DOTNode> node;
};

//TEST_F( DOT_Tests, getSpecs ) {
//    using blogator::dom::html5::Tag;
//    using blogator::dom::html5::TagStructure;
//
//    auto p1 = dot->getSpecs( blogator::html::dom::HtmlTag::Type::A );
//    ASSERT_EQ( Structure::PAIRED, p1->structure );
//    ASSERT_EQ( "<a" ,p1->opening_begin );
//    ASSERT_EQ( ">" ,p1->opening_end );
//    ASSERT_EQ( "</a>" ,p1->closing );
//    auto p2 = dot->getSpecs( blogator::html::dom::HtmlTag::Type::A );
//    ASSERT_EQ( p1, p2 );
//}

TEST_F( Parser_Tests, parseText ) {
    FAIL();
}

TEST_F( Parser_Tests, parseTag ) {
    FAIL();
}

TEST_F( Parser_Tests, parseTag_parseOpeningTag ) {

}

TEST_F( Parser_Tests, parseTag_parseClosingTag_fail0 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    ASSERT_THROW( blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node ), std::invalid_argument );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_fail1 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"</>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    ASSERT_THROW( blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node ), std::invalid_argument );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_fail2 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"<tagname/>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    ASSERT_THROW( blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node ), std::invalid_argument );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_fail3 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"</tagname>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    ASSERT_THROW( blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node ), std::invalid_argument );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_fail4 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"<a/>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    ASSERT_THROW( blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node ), std::invalid_argument );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_fail5 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"< /a>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    ASSERT_THROW( blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node ), std::invalid_argument );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_fail6 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"</ a>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    ASSERT_THROW( blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node ), std::invalid_argument );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_pass0 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"</a>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    auto &child = blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_pass1 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"</div>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    auto &child = blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node );
}

TEST_F( Parser_Tests, parseTag_parseClosingTag_pass2 ) {
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    auto str = U"</div  >";
    auto token = Token( 1, 0, TokenType::END_TAG, str );
    auto &child = blogator::dom::parser::Parser::parseTag( token, *Parser_Tests::node );
}