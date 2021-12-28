#include "gtest/gtest.h"
#include "../../../src/parser/specs/html5/Namespace.h"

using namespace blogator::parser::specs::html5;

TEST( parser_specs_html5_Namespace_Tests, output_stream_operator ) {
    for( auto i = 0; i <= static_cast<int>( Namespace::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<Namespace>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "Namespace '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<Namespace>( i - 1 ) << "\", "
            << "next: \"" << static_cast<Namespace>( i + 1 ) << "\""
            << ").";
    }
}