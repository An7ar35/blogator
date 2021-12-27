#include "gtest/gtest.h"
#include "../../../src/parser/specs/Context.h"

using namespace blogator::parser::specs;

TEST( parser_specs_Tests, output_stream_operator ) {
    for( auto i = 0; i <= static_cast<int>( Context::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<Context>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "Context '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<Context>( i - 1 ) << "\", "
            << "next: \"" << static_cast<Context>( i + 1 ) << "\""
            << ").";
    }
}