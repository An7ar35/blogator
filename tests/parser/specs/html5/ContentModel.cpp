#include "gtest/gtest.h"
#include "../../../src/parser/specs/html5/ContentModel.h"

using namespace blogator::parser::specs::html5;


TEST( parser_specs_html5_ContentModel_Tests, output_stream_operator ) {
    for( auto i = 0; i <= static_cast<int>( ContentModel::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<ContentModel>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "ContentModel '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<ContentModel>( i - 1 ) << "\", "
            << "next: \"" << static_cast<ContentModel>( i + 1 ) << "\""
            << ").";
    }
}