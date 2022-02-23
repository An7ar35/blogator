#include "gtest/gtest.h"
#include "../../../src/parser/specs/infra/ContentType.h"

using namespace blogator::parser::specs::infra;

TEST( parser_specs_html5_ContentType_Tests, to_namespace ) {
    ASSERT_EQ( to_namespace( ContentType::IMAGE_SVG_XML ), Namespace::SVG );
    ASSERT_EQ( to_namespace( ContentType::APPLICATION_XHTML_XML ), Namespace::HTML5 );
    ASSERT_EQ( to_namespace( ContentType::APPLICATION_RSS_XML ), Namespace::XML );
    ASSERT_EQ( to_namespace( ContentType::APPLICATION_XML ), Namespace::XML );
}