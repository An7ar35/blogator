#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/CDATASection.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

TEST( parser_dom_node_CDATASection_Tests, nodeName ) {
    auto node = node::CDATASection( U"", nullptr, nullptr );
    ASSERT_EQ( node.nodeName(), U"#cdata-section" );
}