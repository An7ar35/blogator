#include "gtest/gtest.h"
#include "../../../src/parser/dom/datastructure/NamespaceMap.h"

using namespace blogator::parser::dom;
using           blogator::parser::specs::infra::Namespace;

TEST( parser_dom_NamespaceMap_Tests, setNamespace_0 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( U"new_namespace" );
    ASSERT_EQ( id, 0 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_1 ) { //recall
    auto nsmap = NamespaceMap();
    ASSERT_EQ( nsmap.setNamespace( U"new_namespace" ), 0 );
    ASSERT_EQ( nsmap.setNamespace( U"new_namespace" ), 0 );
    ASSERT_EQ( nsmap.size(), 1 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_2 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( Namespace::MATHML );
    ASSERT_EQ( id, 0 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_3 ) { //recall
    auto nsmap = NamespaceMap();
    ASSERT_EQ( nsmap.setNamespace( Namespace::MATHML ), 0 );
    ASSERT_EQ( nsmap.setNamespace( Namespace::MATHML ), 0 );
    ASSERT_EQ( nsmap.size(), 1 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_fail_0 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( Namespace::OTHER );
    ASSERT_EQ( id, NamespaceMap::INVALID );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_fail_1 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( Namespace::UNKNOWN );
    ASSERT_EQ( id, NamespaceMap::INVALID );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceEnum_0 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( U"new_namespace" );
    ASSERT_EQ( nsmap.getNamespaceEnum( id ), Namespace::OTHER );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceEnum ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( Namespace::HTML5 );
    ASSERT_EQ( nsmap.getNamespaceEnum( id ), blogator::parser::specs::infra::Namespace::HTML5 );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceEnum_fail_0 ) { //invalid id
    auto nsmap = NamespaceMap();
    ASSERT_THROW( std::ignore = nsmap.getNamespaceEnum( 100 ), std::out_of_range );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceEnum_fail_1 ) {
    auto nsmap = NamespaceMap();
    ASSERT_THROW( std::ignore = nsmap.getNamespaceEnum( -1 ), std::out_of_range );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceURI_0 ) {
    auto nsmap = NamespaceMap();
    auto uri   = DOMString_t( U"namespace/uri" );
    auto id    = nsmap.setNamespace( uri );
    ASSERT_EQ( nsmap.getNamespaceURI( id ), uri );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceURI_1 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( Namespace::HTML5 );
    ASSERT_EQ( nsmap.getNamespaceURI( id ), U"http://www.w3.org/1999/xhtml" );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceURI_fail_0 ) {
    auto nsmap = NamespaceMap();
    ASSERT_THROW( std::ignore = nsmap.getNamespaceURI( 100 ), std::out_of_range );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespaceURI_fail_1 ) {
    auto nsmap = NamespaceMap();
    ASSERT_THROW( std::ignore = nsmap.getNamespaceURI( -1 ), std::out_of_range );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespacePrefix_0 ) {
    auto nsmap  = NamespaceMap();
    auto uri    = DOMString_t( U"namespace/uri" );
    auto prefix = DOMString_t( U"prefix" );
    auto id     = nsmap.setNamespace( uri, prefix );
    ASSERT_EQ( nsmap.getNamespacePrefix( id ), prefix );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespacePrefix_1 ) {
    auto nsmap  = NamespaceMap();
    auto id     = nsmap.setNamespace( Namespace::MATHML );
    ASSERT_EQ( nsmap.getNamespacePrefix( id ), U"math" );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespacePrefix_fail_0 ) {
    auto nsmap = NamespaceMap();
    ASSERT_THROW( std::ignore = nsmap.getNamespacePrefix( 100 ), std::out_of_range );
}

TEST( parser_dom_NamespaceMap_Tests, getNamespacePrefix_fail_1 ) {
    auto nsmap = NamespaceMap();
    ASSERT_THROW( std::ignore = nsmap.getNamespacePrefix( -1 ), std::out_of_range );
}

TEST( parser_dom_NamespaceMap_Tests, size ) {
    auto nsmap = NamespaceMap();
    ASSERT_EQ( nsmap.size(), 0 );
    nsmap.setNamespace( U"new_namespace" );
    ASSERT_EQ( nsmap.size(), 1 );
}

TEST( parser_dom_NamespaceMap_Tests, empty ) {
    auto nsmap = NamespaceMap();
    ASSERT_TRUE( nsmap.empty() );
    nsmap.setNamespace( U"new_namespace" );
    ASSERT_FALSE( nsmap.empty() );
}