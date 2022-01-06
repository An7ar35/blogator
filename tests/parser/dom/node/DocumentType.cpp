#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/DocumentType.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;
using           blogator::parser::specs::html5::Namespace;

TEST( parser_dom_node_DocumentType_Tests, nodeName ) {
    const auto name = DOMString_t( U"testing-name" );
    const auto node = node::DocumentType( name, U"", U"", nullptr, nullptr );
    ASSERT_EQ( node.nodeName(), name );
}

TEST( parser_dom_node_DocumentType_Tests, length ) {
    const auto node = node::DocumentType( U"name", U"pid", U"sid", nullptr, nullptr );
    ASSERT_EQ( node.length(), 0 );
}

TEST( parser_dom_node_DocumentType_Tests, name ) {
    const auto name = DOMString_t( U"testing-name" );
    const auto node = node::DocumentType( name, U"", U"", nullptr, nullptr );
    ASSERT_EQ( node.name(), name );
}

TEST( parser_dom_node_DocumentType_Tests, publicId ) {
    const auto str = DOMString_t( U"testing-id" );
    const auto node = node::DocumentType( U"", str, U"", nullptr, nullptr );
    ASSERT_EQ( node.publicId(), str );
}

TEST( parser_dom_node_DocumentType_Tests, systemId ) {
    const auto str = DOMString_t( U"testing-id" );
    const auto node = node::DocumentType( U"", U"", str, nullptr, nullptr );
    ASSERT_EQ( node.systemId(), str );
}