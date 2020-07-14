#include "gtest/gtest.h"
#include "../../src/dom/parser/Tokeniser.h"

#include <filesystem>
#include <fstream>

namespace Tokeniser_Tests {
    blogator::dto::Text importFile( std::filesystem::path path ) {
        auto text = blogator::dto::Text();
        std::string   line;
        std::ifstream file( path );

        if( !file.is_open() )
            throw std::filesystem::filesystem_error( "Could not open: " + path.string(), std::error_code() );

        while( getline( file, line ) ) {
            text._lines.emplace_back( blogator::encoding::encodeToUTF32( line ) );
        }

        file.close();
        return text;
    }

    blogator::dto::Text createTextObject( std::u32string line ) {
        auto text = blogator::dto::Text();
        text._lines.emplace_back( std::move( line ) );
        return text;
    }
}

TEST( Tokeniser_Tests, init ) {
    auto text = Tokeniser_Tests::importFile( "../tests/dom/parser/text1.html" );

//    for( auto &l : text._lines )
//        std::cout << blogator::encoding::encodeToUTF8( l ) << std::endl;

    auto tokeniser = blogator::dom::parser::Tokeniser( text );
}

TEST( Tokeniser_Tests, empty_str ) {
    auto text      = Tokeniser_Tests::createTextObject( U"" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );
    ASSERT_EQ( 0, tokeniser.tokenCount() );
}

TEST( Tokeniser_Tests, invalid_tag_as_text0 ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"< div>" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::TEXT, tokeniser.cbegin()->type );
}

TEST( Tokeniser_Tests, invalid_tag_as_text1 ) {
    using blogator::dom::parser::TokenType;

    auto text      = Tokeniser_Tests::createTextObject( U"</ div>" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );

    ASSERT_EQ( 1, tokeniser.tokenCount() );
    ASSERT_EQ( TokenType::TEXT, tokeniser.cbegin()->type );
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




TEST( Tokeniser_Tests, bad_closing_tag0 ) {
    auto text      = Tokeniser_Tests::createTextObject( U"</>" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );
    std::cout << "tokens = " << tokeniser.tokenCount() << std::endl;
    std::cout << tokeniser;
}

TEST( Tokeniser_Tests, closing_tag_ ) {
    auto text      = Tokeniser_Tests::createTextObject( U"< a/>" );
    auto tokeniser = blogator::dom::parser::Tokeniser( text );
}