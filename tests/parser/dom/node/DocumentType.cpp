#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/DocumentType.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;
using           blogator::parser::specs::infra::Namespace;

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

TEST( parser_dom_node_DocumentType_Tests, output_stream ) {
    const auto name = DOMString_t( U"testing-name" );
    const auto pid  = DOMString_t( U"testing-public-id" );
    const auto sid  = DOMString_t( U"testing-system-id" );
    const auto node = node::DocumentType( name, pid, sid );

    std::stringstream expected_ss;
    expected_ss << R"(<!DOCTYPE testing-name PUBLIC "testing-public-id" SYSTEM "testing-system-id">)";

    std::stringstream received_ss;
    received_ss << node;

    ASSERT_EQ( received_ss.str(), expected_ss.str() );
}