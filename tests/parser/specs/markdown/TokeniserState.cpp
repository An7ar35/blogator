#include "gtest/gtest.h"
#include "../../../src/parser/specs/markdown/TokeniserState.h"

using namespace blogator::parser::specs::markdown;

TEST( parser_specs_markdown_TokeniserState, output_stream_operator ) {
    for( auto i = static_cast<int>( TokeniserState::ENUM_BEGIN ); i <= static_cast<int>( TokeniserState::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<TokeniserState>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "InsertionMode '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<TokeniserState>( i - 1 ) << "\", "
            << "next: \"" << static_cast<TokeniserState>( i + 1 ) << "\""
            << ").";
    }
}