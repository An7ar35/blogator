#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/CDATASection.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

TEST( parser_dom_node_CDATASection_Tests, x ) {
}

TEST( parser_dom_node_CDATASection_Tests, swap_0 ) {
    auto cdata1 = node::CDATASection( U"cdata1" );
    auto cdata2 = node::CDATASection( U"cdata2" );

    blogator::parser::dom::node::swap( dynamic_cast<node::Node &>( cdata1 ), dynamic_cast<node::Node &>( cdata2 ) );

    ASSERT_EQ( cdata1.data(), U"cdata2" );
    ASSERT_EQ( cdata2.data(), U"cdata1" );
}

TEST( parser_dom_node_CDATASection_Tests, swap_1 ) {
    auto cdata1 = node::CDATASection( U"cdata1" );
    auto cdata2 = node::CDATASection( U"cdata2" );

    blogator::parser::dom::node::swap( cdata1, cdata2 );

    ASSERT_EQ( cdata1.data(), U"cdata2" );
    ASSERT_EQ( cdata2.data(), U"cdata1" );
}