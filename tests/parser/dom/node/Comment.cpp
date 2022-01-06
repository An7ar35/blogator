#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Comment.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

TEST( parser_dom_node_Comment_Tests, nodeName ) {
    auto node = node::Comment( U"", nullptr, nullptr );
    ASSERT_EQ( node.nodeName(), U"#comment" );
}