#include "gtest/gtest.h"
#include "../../../src/parser/specs/html5/Attribute.h"

using namespace blogator::parser::specs::html5;

TEST( parser_specs_html5_Attribute_Tests, output_stream_operator ) {
    for( auto i = 0; i <= static_cast<int>( Attribute::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<Attribute>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "Attribute '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<Attribute>( i - 1 ) << "\", "
            << "next: \"" << static_cast<Attribute>( i + 1 ) << "\""
            << ").";
    }
}