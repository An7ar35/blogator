#include "gtest/gtest.h"
#include "../../../src/parser/specs/markdown/InsertionMode.h"

using namespace blogator::parser::specs::markdown;

TEST( parser_specs_markdown_InsertionMode, output_stream_operator ) {
    for( auto i = static_cast<int>( InsertionMode::ENUM_BEGIN ); i <= static_cast<int>( InsertionMode::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<InsertionMode>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "InsertionMode '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<InsertionMode>( i - 1 ) << "\", "
            << "next: \"" << static_cast<InsertionMode>( i + 1 ) << "\""
            << ").";
    }
}