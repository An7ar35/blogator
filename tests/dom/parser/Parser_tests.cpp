#include "gtest/gtest.h"
#include "../../../src/dom/parser/Parser.h"

class Parser_Tests : public testing::Test {
  protected:
    void SetUp() override {
        node      = std::make_unique<blogator::dom::DOTNode>( blogator::dom::html5::Tag::NONE );
        attr_maps = std::make_unique<blogator::dom::dto::GlobalMaps>();
    }
    void TearDown() override {}
  public:
    std::unique_ptr<blogator::dom::DOTNode>         node;
    std::unique_ptr<blogator::dom::dto::GlobalMaps> attr_maps;
};

TEST_F( Parser_Tests, parseToken_parseText ) {
    FAIL();
}

TEST_F( Parser_Tests, parseToken_parseOpeningTag ) {
    FAIL();
}

TEST_F( Parser_Tests, parseToken_parseOpeningTag_attr_fail0 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE } );
    auto str    = U"<div id='bad\x05id'>";
    auto token  = Token( 1, 0, TokenType::START_TAG, str );

    EXPECT_THROW( {
        try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Invalid attribute(s) format: Invalid character in value for named attribute \"id\".", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail0 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str    = U"";
    auto token  = Token( 2, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
        try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Closing tag is empty.", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail1 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str = U"</>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
        try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: No name found in closing tag \"</>\".", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail2 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str = U"<tagname/>";
    auto token = Token( 1, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
        try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 2nd character of an end tag must be U+002F ('/').", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail3 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str    = U"</tagname>";
    auto token  = Token( 1, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
        try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Tag name \"tagname\" is not valid.", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail4 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str    = U"<a/>";
    auto token  = Token( 1, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
        try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 2nd character of an end tag must be U+002F ('/').", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail5 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str    = U"< /a>";
    auto token  = Token( 1, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
        try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 2nd character of an end tag must be U+002F ('/').", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail6 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::A } );
    auto str    = U"</ a>";
    auto token  = Token( 1, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
    try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: No name detected after \"</\" in closing tag.", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_fail7 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;

    auto parser = blogator::dom::parser::Parser( { Tag::NONE, Tag::A } );
    auto str    = U"</div>";
    auto token  = Token( 1, 0, TokenType::END_TAG, str );

    EXPECT_THROW( {
    try {
            parser.parseToken( token, Parser_Tests::node.get(), *attr_maps );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "ValidationError: Closing tag (div) does not match last opened tag (a).", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_pass0 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    using blogator::dom::DOTNode;

    auto parser    = blogator::dom::parser::Parser( { Tag::NONE, Tag::A } );
    auto str       = U"</a>";
    auto root_node = std::make_unique<DOTNode>( Tag::NONE );
    auto div_node  = std::make_unique<DOTNode>( Tag::A );
    auto curr_node = root_node->addChild( std::move( div_node ) ); //(root)->(a)
    auto token     = Token( 1, 0, TokenType::END_TAG, str );

    curr_node = parser.parseToken( token, curr_node, *attr_maps );
    ASSERT_EQ( *curr_node, *root_node );
    ASSERT_FALSE( curr_node->parent() );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_pass1 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    using blogator::dom::DOTNode;

    auto parser    = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str       = U"</div>";
    auto root_node = std::make_unique<DOTNode>( Tag::NONE );
    auto div_node  = std::make_unique<DOTNode>( Tag::DIV );
    auto curr_node = root_node->addChild( std::move( div_node ) ); //(root)->(div)
    auto token     = Token( 1, 0, TokenType::END_TAG, str );

    curr_node = parser.parseToken( token, curr_node, *attr_maps ); //(root)
    ASSERT_EQ( *curr_node, *root_node );
    ASSERT_FALSE( curr_node->parent() );
}

TEST_F( Parser_Tests, parseToken_parseClosingTag_pass2 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::parser::Token;
    using blogator::dom::parser::TokenType;
    using blogator::dom::DOTNode;

    auto parser    = blogator::dom::parser::Parser( { Tag::NONE, Tag::DIV } );
    auto str       = U"</div  >";
    auto root_node = std::make_unique<DOTNode>( Tag::NONE );
    auto div_node  = std::make_unique<DOTNode>( Tag::DIV );
    auto curr_node = root_node->addChild( std::move( div_node ) ); //(root)->(div)
    auto token     = Token( 1, 0, TokenType::END_TAG, str );

    curr_node = parser.parseToken( token, curr_node, *attr_maps ); //(root)
    ASSERT_EQ( *curr_node, *root_node );
    ASSERT_FALSE( curr_node->parent() );
}