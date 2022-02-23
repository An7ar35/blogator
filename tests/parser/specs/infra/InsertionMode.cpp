#include "gtest/gtest.h"
#include "../../../src/parser/specs/infra/InsertionMode.h"

using namespace blogator::parser::specs::infra;

TEST( parser_specs_html5_InsertionMode_Tests, output_stream_operator ) {
    for( auto i = 0; i <= static_cast<int>( InsertionMode::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<InsertionMode>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "InsertionMode '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<InsertionMode>( i - 1 ) << "\", "
            << "next: \"" << static_cast<InsertionMode>( i + 1 ) << "\""
            << ").";
    }
}