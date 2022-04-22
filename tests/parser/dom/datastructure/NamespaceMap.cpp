#include "gtest/gtest.h"
#include "../../../src/parser/dom/datastructure/NamespaceMap.h"

using namespace blogator::parser::dom;
using           blogator::parser::specs::infra::Namespace;

TEST( parser_dom_NamespaceMap_Tests, constructor ) {
    auto nsmap = NamespaceMap();
    ASSERT_NO_THROW( ASSERT_EQ( nsmap.getNamespaceEnum( NamespaceMap::NONE ), Namespace::NONE ) );
    ASSERT_NO_THROW( ASSERT_EQ( nsmap.getNamespaceURI( NamespaceMap::NONE ), U"" ) );
    ASSERT_NO_THROW( ASSERT_EQ( nsmap.getNamespacePrefix( NamespaceMap::NONE ), U"" ) );
    ASSERT_NO_THROW( ASSERT_EQ( nsmap.setNamespace( U"" ), NamespaceMap::NONE ) );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_0 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( U"new_namespace" );
    ASSERT_EQ( id, 1 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_1 ) { //recall
    auto nsmap = NamespaceMap();
    ASSERT_EQ( nsmap.setNamespace( U"new_namespace" ), 1 );
    ASSERT_EQ( nsmap.setNamespace( U"new_namespace" ), 1 );
    ASSERT_EQ( nsmap.size(), 2 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_2 ) {
    auto nsmap = NamespaceMap();
    auto id    = nsmap.setNamespace( Namespace::MATHML );
    ASSERT_EQ( id, 1 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_3 ) { //recall
    auto nsmap = NamespaceMap();
    ASSERT_EQ( nsmap.setNamespace( Namespace::MATHML ), 1 );
    ASSERT_EQ( nsmap.setNamespace( Namespace::MATHML ), 1 );
    ASSERT_EQ( nsmap.size(), 2 );
}

TEST( parser_dom_NamespaceMap_Tests, setNamespace_4 ) { //URI range convertible to Namespace enums
    auto nsmap = NamespaceMap();

    for( auto e = static_cast<int>( Namespace::URI_DEFS_BEGIN ), i = 1; e <= static_cast<int>( Namespace::URI_DEFS_END ); ++e, ++i  ) {
        ASSERT_EQ( nsmap.setNamespace( blogator::parser::specs::infra::to_namespaceURI( static_cast<Namespace>( e ) ) ), i )
            << "Failed on '" << static_cast<Namespace>( e ) << "'.";

        ASSERT_EQ( nsmap.getID( static_cast<Namespace>( e ) ), i )
            << "Failed on '" << static_cast<Namespace>( e ) << "'."; //checking enum map is correct
    }
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
    ASSERT_EQ( nsmap.size(), 1 );
    nsmap.setNamespace( U"new_namespace" );
    ASSERT_EQ( nsmap.size(), 2 );
}

TEST( parser_dom_NamespaceMap_Tests, empty ) {
    auto nsmap = NamespaceMap();
    ASSERT_FALSE( nsmap.empty() ); //since 'NONE' is auto-added
}

TEST( parser_dom_NamespaceMap_Tests, getID_0 ) {
    auto nsmap  = NamespaceMap();
    ASSERT_EQ( nsmap.getID( Namespace::HTML5 ), NamespaceMap::INVALID );
}

TEST( parser_dom_NamespaceMap_Tests, getID_1 ) {
    auto nsmap  = NamespaceMap();
    auto id     = nsmap.setNamespace( Namespace::MATHML );
    ASSERT_EQ( nsmap.getID( Namespace::MATHML ), id );
}

TEST( parser_dom_NamespaceMap_Tests, getID_2 ) {
    auto nsmap  = NamespaceMap();
    ASSERT_EQ( nsmap.getID( U"other/uri" ), NamespaceMap::INVALID );
}

TEST( parser_dom_NamespaceMap_Tests, getID_3 ) {
    auto nsmap  = NamespaceMap();
    auto uri    = DOMString_t( U"namespace/uri" );
    auto id     = nsmap.setNamespace( uri );
    ASSERT_EQ( nsmap.getID( uri ), id );
}