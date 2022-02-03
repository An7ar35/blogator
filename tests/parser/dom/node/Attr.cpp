#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/Element.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

class parser_dom_node_Attr_Tests : public ::testing::Test {
  protected:
    static const DOMString_t prefix;
    static const DOMString_t name;
    static const DOMString_t value;
    static const node::Attr test_attr;
};

const DOMString_t parser_dom_node_Attr_Tests::prefix    = DOMString_t( U"test-prefix" );
const DOMString_t parser_dom_node_Attr_Tests::name      = DOMString_t( U"test-name" );
const DOMString_t parser_dom_node_Attr_Tests::value     = DOMString_t( U"test-value" );
const node::Attr  parser_dom_node_Attr_Tests::test_attr = node::Attr( prefix, name, value );


TEST_F( parser_dom_node_Attr_Tests, localName ) {
    const auto & name = parser_dom_node_Attr_Tests::name;
    const auto & node = parser_dom_node_Attr_Tests::test_attr;

    ASSERT_EQ( node.localName(), name );
}

TEST_F( parser_dom_node_Attr_Tests, name ) {
    const auto & name = parser_dom_node_Attr_Tests::name;
    const auto & node = parser_dom_node_Attr_Tests::test_attr;

    ASSERT_EQ( node.name(), name );
}

TEST_F( parser_dom_node_Attr_Tests, value ) {
    const auto & value = parser_dom_node_Attr_Tests::value;
    const auto & node  = parser_dom_node_Attr_Tests::test_attr;

    ASSERT_EQ( node.value(), value );
}

TEST_F( parser_dom_node_Attr_Tests, hasValue_false ) {
    const auto & node = parser_dom_node_Attr_Tests::test_attr;

    ASSERT_TRUE( node.hasValue() );
}

TEST_F( parser_dom_node_Attr_Tests, hasValue_true ) {
    const auto name  = DOMString_t( U"test-name" );
    const auto node  = node::Attr( name );

    ASSERT_FALSE( node.hasValue() );
}

TEST_F( parser_dom_node_Attr_Tests, ownerElement ) {
    const auto name   = DOMString_t( U"test-name" );
    const auto value  = DOMString_t( U"test-value" );
    auto       parent = node::Element( U"", U"div" );
    auto       node   = node::Attr( name, value, &parent, nullptr );

    ASSERT_EQ( node.ownerElement(), &parent );
}

TEST_F( parser_dom_node_Attr_Tests, ownerElement_none ) {
    const auto name   = DOMString_t( U"test-name" );
    const auto value  = DOMString_t( U"test-value" );
    auto       node   = node::Attr( name, value );

    ASSERT_EQ( node.ownerElement(), nullptr );
}