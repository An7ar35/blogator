#include "gtest/gtest.h"
#include "../../src/dom/parser/Tokeniser.h"

//#include <filesystem>
#include <fstream>

namespace Tokeniser_Tests {
//    blogator::dom::dto::Text importFile( std::filesystem::path path ) {
//        auto text = blogator::dom::dto::Text();
//        std::string   line;
//        std::ifstream file( path );
//
//        if( !file.is_open() )
//            throw std::filesystem::filesystem_error( "Could not open: " + path.string(), std::error_code() );
//
//        while( getline( file, line ) ) {
//            text._lines.emplace_back( blogator::encoding::encodeToUTF32( line ) );
//        }
//
//        file.close();
//        return text;
//    }

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
    ASSERT_EQ( TokenType::START_TAG, tokeniser.cbegin()->type );
}

TEST( Tokeniser_Tests, valid_tag1 ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"</div >" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::END_TAG, tokeniser.cbegin()->type );
}

TEST( Tokeniser_Tests, valid_tag2_comment_open ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"<!--" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::START_TAG, tokeniser.cbegin()->type );
}

TEST( Tokeniser_Tests, valid_tag2_comment_line ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"<!--some comment ... -->" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 3, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::START_TAG, tokeniser.cbegin()->type );
    ASSERT_EQ( U"<!--", tokeniser.cbegin()->content );
    ASSERT_EQ( TokenType::TEXT, std::next( tokeniser.cbegin() )->type );
    ASSERT_EQ( U"some comment ... ", std::next( tokeniser.cbegin() )->content );
    ASSERT_EQ( TokenType::END_TAG, std::next( tokeniser.cbegin(), 2 )->type );
    ASSERT_EQ( U"-->", std::next( tokeniser.cbegin(), 2 )->content );
}