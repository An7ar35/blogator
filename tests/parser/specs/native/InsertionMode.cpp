#include "gtest/gtest.h"
#include "../../../src/parser/specs/native/InsertionMode.h"

using namespace blogator::parser::specs::native;

TEST( parser_specs_native_InsertionMode_Tests, output_stream_operator ) {
    for( auto i = 0; i <= static_cast<int>( InsertionMode::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<InsertionMode>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "Element '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<InsertionMode>( i - 1 ) << "\", "
            << "next: \"" << static_cast<InsertionMode>( i + 1 ) << "\""
            << ").";
    }
}