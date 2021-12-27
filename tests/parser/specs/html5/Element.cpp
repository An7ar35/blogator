#include "gtest/gtest.h"
#include "../../../src/parser/specs/html5/specifications.h"

using namespace blogator::parser::specs::html5;

TEST( parser_specs_html5_Element_Tests, output_stream_operator ) {
    for( auto i = 0; i <= static_cast<int>( Element::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<Element>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "Element '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<Element>( i - 1 ) << "\", "
            << "next: \"" << static_cast<Element>( i + 1 ) << "\""
            << ").";
    }
}