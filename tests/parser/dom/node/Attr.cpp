#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/Element.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

TEST( parser_dom_node_Attr_Tests, localName ) {
    const auto name  = DOMString_t( U"test-name" );
    const auto value = DOMString_t( U"test-value" );
    const auto node  = node::Attr( name, value );

    ASSERT_EQ( node.localName(), name );
}

TEST( parser_dom_node_Attr_Tests, name ) {
    const auto name  = DOMString_t( U"test-name" );
    const auto value = DOMString_t( U"test-value" );
    const auto node  = node::Attr( name, value );

    ASSERT_EQ( node.name(), name );
}

TEST( parser_dom_node_Attr_Tests, value ) {
    const auto name  = DOMString_t( U"test-name" );
    const auto value = DOMString_t( U"test-value" );
    const auto node  = node::Attr( name, value );

    ASSERT_EQ( node.value(), value );
}

TEST( parser_dom_node_Attr_Tests, hasValue_false ) {
    const auto name  = DOMString_t( U"test-name" );
    const auto value = DOMString_t( U"test-value" );
    const auto node  = node::Attr( name, value );

    ASSERT_TRUE( node.hasValue() );
}

TEST( parser_dom_node_Attr_Tests, hasValue_true ) {
    const auto name  = DOMString_t( U"test-name" );
    const auto node  = node::Attr( name );

    ASSERT_FALSE( node.hasValue() );
}

TEST( parser_dom_node_Attr_Tests, ownerElement ) {
    const auto name   = DOMString_t( U"test-name" );
    const auto value  = DOMString_t( U"test-value" );
    auto       parent = node::Element( U"", U"div" );
    auto       node   = node::Attr( name, value, &parent, nullptr );

    ASSERT_EQ( node.ownerElement(), &parent );
}

TEST( parser_dom_node_Attr_Tests, ownerElement_none ) {
    const auto name   = DOMString_t( U"test-name" );
    const auto value  = DOMString_t( U"test-value" );
    auto       node   = node::Attr( name, value );

    ASSERT_EQ( node.ownerElement(), nullptr );
}