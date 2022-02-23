#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Comment.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

TEST( parser_dom_node_Comment_Tests, x ) {
}

TEST( parser_dom_node_Comment_Tests, swap_0 ) {
    auto comment1 = node::Comment( U"comment1" );
    auto comment2 = node::Comment( U"comment2" );

    blogator::parser::dom::node::swap( dynamic_cast<node::Node &>( comment1 ), dynamic_cast<node::Node &>( comment2 ) );

    ASSERT_EQ( comment1.data(), U"comment2" );
    ASSERT_EQ( comment2.data(), U"comment1" );
}

TEST( parser_dom_node_Comment_Tests, swap_1 ) {
    auto comment1 = node::Comment( U"comment1" );
    auto comment2 = node::Comment( U"comment2" );

    blogator::parser::dom::node::swap( comment1, comment2 );

    ASSERT_EQ( comment1.data(), U"comment2" );
    ASSERT_EQ( comment2.data(), U"comment1" );
}