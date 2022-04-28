#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/Element.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

class parser_dom_node_Attr_Tests : public ::testing::Test {
  protected:
    static const DOMString_t ns;
    static const DOMString_t prefix;
    static const DOMString_t name;
    static const DOMString_t value;
    static const node::Attr test_attr;
};

const DOMString_t parser_dom_node_Attr_Tests::ns        = DOMString_t( U"namespace/test" );
const DOMString_t parser_dom_node_Attr_Tests::prefix    = DOMString_t( U"test-prefix" );
const DOMString_t parser_dom_node_Attr_Tests::name      = DOMString_t( U"test-name" );
const DOMString_t parser_dom_node_Attr_Tests::value     = DOMString_t( U"test-value" );
const node::Attr  parser_dom_node_Attr_Tests::test_attr = node::Attr( ns, prefix, name, value );

TEST_F( parser_dom_node_Attr_Tests, namespaceURI ) {
    const auto & ns   = parser_dom_node_Attr_Tests::ns;
    const auto & node = parser_dom_node_Attr_Tests::test_attr;

    ASSERT_EQ( node.namespaceURI(), ns );
}

TEST_F( parser_dom_node_Attr_Tests, prefix ) {
    const auto & prefix = parser_dom_node_Attr_Tests::prefix;
    const auto & node   = parser_dom_node_Attr_Tests::test_attr;

    ASSERT_EQ( node.prefix(), prefix );
}

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

    ASSERT_NE( node.value(), nullptr );
    ASSERT_EQ( *node.value(), value );
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
    const auto & name   = parser_dom_node_Attr_Tests::name;
    const auto & value  = parser_dom_node_Attr_Tests::value;
    auto         node   = node::Attr( name, value );

    ASSERT_EQ( node.ownerElement(), nullptr );
}

TEST_F( parser_dom_node_Attr_Tests, swap_0 ) {
    const auto ns1     = DOMString_t( U"namespace/1" );
    const auto prefix1 = DOMString_t( U"a" );
    const auto name1   = DOMString_t( U"test-name1" );
    const auto value1  = DOMString_t( U"test-value1" );
    const auto ns2     = DOMString_t( U"namespace/2" );
    const auto prefix2 = DOMString_t( U"b" );
    const auto name2   = DOMString_t( U"test-name2" );
    auto attr1 = node::Attr( ns1, prefix1, name1, value1 );
    auto attr2 = node::Attr( ns2, prefix2, name2 );

    attr1.swap( attr2 );

    ASSERT_FALSE( attr1.hasValue() );
    ASSERT_EQ( attr1.namespaceURI(), ns2 );
    ASSERT_EQ( attr1.prefix(), prefix2 );
    ASSERT_EQ( attr1.name(), name2 );
    ASSERT_EQ( attr1.value(), nullptr );

    ASSERT_TRUE( attr2.hasValue() );
    ASSERT_EQ( attr2.namespaceURI(), ns1 );
    ASSERT_EQ( attr2.prefix(), prefix1 );
    ASSERT_EQ( attr2.name(), name1 );
    ASSERT_NE( attr2.value(), nullptr );
    ASSERT_EQ( *attr2.value(), value1 );
}

TEST_F( parser_dom_node_Attr_Tests, output_stream ) {
    const auto & attr = parser_dom_node_Attr_Tests::test_attr;

    std::stringstream expected_ss;
    expected_ss << "test-prefix:test-name=\"test-value\"";

    std::stringstream received_ss;
    received_ss << attr;

    ASSERT_EQ( received_ss.str(), expected_ss.str() );
}