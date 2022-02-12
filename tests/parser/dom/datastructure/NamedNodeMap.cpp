#include "gtest/gtest.h"
#include "../../../src/parser/dom/datastructure/NamedNodeMap.h"
#include "../../../src/parser/dom/node/Node.h"
#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/Document.h"
#include "../../../src/parser/dom/node/Element.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NamedNodeMap;

TEST( parser_dom_NamedNodeMap_Tests, constructor ) {
    auto parent = node::Node();
    auto nnm    = NamedNodeMap( &parent );
    ASSERT_EQ( nnm.ownerNode(), &parent );
}

TEST( parser_dom_NamedNodeMap_Tests, copy_constructor ) {
    auto parent = node::Node();
    auto attr1  = node::Attr( U"p", U"name1", U"value" );
    auto attr2  = node::Attr( U"q", U"name2" );
    auto nnm1   = NamedNodeMap( &parent );

    nnm1.setNamedItem( attr1 );
    nnm1.setNamedItem( attr2 );
    //sanity check
    ASSERT_EQ( nnm1.list().size(), 2 );
    ASSERT_TRUE( nnm1.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm1.list()[1]->equivalent( attr2 ) );

    //testing
    auto nnm2 = NamedNodeMap( nnm1 );
    ASSERT_EQ( nnm2.ownerNode(), &parent );
    ASSERT_EQ( nnm2.list().size(), 2 );
    ASSERT_TRUE( nnm2.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm2.list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_NamedNodeMap_Tests, move_constructor ) {
    auto parent = node::Node();
    auto attr1  = node::Attr( U"p", U"name1", U"value" );
    auto attr2  = node::Attr( U"q", U"name2" );
    auto nnm1   = NamedNodeMap( &parent );

    nnm1.setNamedItem( attr1 );
    nnm1.setNamedItem( attr2 );
    //sanity check
    ASSERT_EQ( nnm1.list().size(), 2 );
    ASSERT_TRUE( nnm1.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm1.list()[1]->equivalent( attr2 ) );

    //testing
    auto nnm2 = NamedNodeMap( std::move( nnm1 ) );
    ASSERT_EQ( nnm2.ownerNode(), &parent );
    ASSERT_EQ( nnm2.list().size(), 2 );
    ASSERT_TRUE( nnm2.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm2.list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_NamedNodeMap_Tests, copy_operator ) {
    auto parent = node::Node();
    auto attr1  = node::Attr( U"p", U"name1", U"value" );
    auto attr2  = node::Attr( U"q", U"name2" );
    auto nnm1   = NamedNodeMap( &parent );

    nnm1.setNamedItem( attr1 );
    nnm1.setNamedItem( attr2 );
    //sanity check
    ASSERT_EQ( nnm1.list().size(), 2 );
    ASSERT_TRUE( nnm1.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm1.list()[1]->equivalent( attr2 ) );

    //testing
    auto nnm2 = NamedNodeMap( nullptr );
    nnm2 = nnm1;
    ASSERT_EQ( nnm2.ownerNode(), &parent );
    ASSERT_EQ( nnm2.list().size(), 2 );
    ASSERT_TRUE( nnm2.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm2.list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_NamedNodeMap_Tests, move_operator ) {
    auto parent = node::Node();
    auto attr1  = node::Attr( U"p", U"name1", U"value" );
    auto attr2  = node::Attr( U"q", U"name2" );
    auto nnm1   = NamedNodeMap( &parent );

    nnm1.setNamedItem( attr1 );
    nnm1.setNamedItem( attr2 );
    //sanity check
    ASSERT_EQ( nnm1.list().size(), 2 );
    ASSERT_TRUE( nnm1.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm1.list()[1]->equivalent( attr2 ) );

    //testing
    auto nnm2 = NamedNodeMap( nullptr );
    nnm2 = std::move( nnm1 );
    ASSERT_EQ( nnm2.ownerNode(), &parent );
    ASSERT_EQ( nnm2.list().size(), 2 );
    ASSERT_TRUE( nnm2.list()[0]->equivalent( attr1 ) );
    ASSERT_TRUE( nnm2.list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_NamedNodeMap_Tests, empty_0 ) {
    auto nnm = NamedNodeMap( nullptr );
    ASSERT_TRUE( nnm.empty() );
}

TEST( parser_dom_NamedNodeMap_Tests, empty_1 ) {
    auto nnm = NamedNodeMap( nullptr );
    nnm.setNamedItem( node::Attr( U"name" ) );
    ASSERT_FALSE( nnm.empty() );
}

TEST( parser_dom_NamedNodeMap_Tests, length_0 ) {
    auto nnm = NamedNodeMap( nullptr );
    ASSERT_EQ( nnm.length(), 0 );
    ASSERT_EQ( nnm.length(), nnm.list().size() );
}

TEST( parser_dom_NamedNodeMap_Tests, length_1 ) {
    auto nnm = NamedNodeMap( nullptr );
    nnm.setNamedItem( node::Attr( U"name" ) );
    ASSERT_EQ( nnm.length(), 1 );
    ASSERT_EQ( nnm.length(), nnm.list().size() );
}

TEST( parser_dom_NamedNodeMap_Tests, item_0 ) {
    auto nnm  = NamedNodeMap( nullptr );
    ASSERT_EQ( nnm.item( 0 ), nullptr );
}

TEST( parser_dom_NamedNodeMap_Tests, item_1 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );
    ASSERT_NE( nnm.item( 0 ), nullptr );
    ASSERT_TRUE( nnm.item( 0 )->equivalent( attr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, attributeExists_0 ) {
    auto nnm  = NamedNodeMap( nullptr );
    ASSERT_FALSE( nnm.attributeExists( U"name" ) );
}

TEST( parser_dom_NamedNodeMap_Tests, attributeExists_1 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );
    ASSERT_FALSE( nnm.attributeExists( U"name" ) );
    ASSERT_TRUE( nnm.attributeExists( U"p:name" ) );
}

TEST( parser_dom_NamedNodeMap_Tests, getNamedItem_0 ) {
    auto nnm  = NamedNodeMap( nullptr );
    ASSERT_EQ( nnm.getNamedItem( U"name" ), nullptr );
}

TEST( parser_dom_NamedNodeMap_Tests, getNamedItem_1 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );

    auto * returned = nnm.getNamedItem( U"name" );
    ASSERT_NE( returned, nullptr );
    ASSERT_TRUE( returned->equivalent( attr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, getNamedItem_2 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );

    auto * returned = nnm.getNamedItem( U"p:name" );
    ASSERT_NE( returned, nullptr );
    ASSERT_TRUE( returned->equivalent( attr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, getNamedItem_3 ) { //html doc+element
    auto   doc  = node::Document();
    auto * node = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto   nnm  = NamedNodeMap( node );
    auto   attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );

    auto * returned = nnm.getNamedItem( U"P:NAME" );
    ASSERT_NE( returned, nullptr );
    ASSERT_TRUE( returned->equivalent( attr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, setNamedItem_0 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );

    auto * returned = nnm.setNamedItem( attr );
    ASSERT_NE( returned, nullptr );
    ASSERT_TRUE( attr.equivalent( *returned ) );
}

TEST( parser_dom_NamedNodeMap_Tests, setNamedItem_1 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr1 = node::Attr( U"p", U"name", U"value1" );
    auto attr2 = node::Attr( U"p", U"name", U"value2" );

    //added attribute - sanity check
    auto * returned1 = nnm.setNamedItem( attr1 );
    ASSERT_TRUE( returned1->equivalent( attr1 ) );
    ASSERT_EQ( nnm.list().size(), 1 );

    //updated attribute
    auto * returned2 = nnm.setNamedItem( attr2 );
    ASSERT_NE( returned1, nullptr );
    ASSERT_EQ( returned1, returned2 );
    ASSERT_TRUE( returned2->equivalent( attr2 ) );
    ASSERT_EQ( nnm.list().size(), 1 );
}

TEST( parser_dom_NamedNodeMap_Tests, setNamedItem_fail_0 ) { //invalid name
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"na%me", U"value" );

    ASSERT_THROW( std::ignore = nnm.setNamedItem( attr ), blogator::parser::dom::exception::DOMException );
}

TEST( parser_dom_NamedNodeMap_Tests, setNode_0 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );

    std::unique_ptr<node::Attr> ptr;

    auto * returned = nnm.setNode( std::move( ptr ) );
    ASSERT_EQ( returned, nullptr );
}

TEST( parser_dom_NamedNodeMap_Tests, setNode_1 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );

    auto * returned = nnm.setNode( std::make_unique<node::Attr>( attr ) );
    ASSERT_NE( returned, nullptr );
    ASSERT_TRUE( attr.equivalent( *returned ) );
}

TEST( parser_dom_NamedNodeMap_Tests, setNode_2 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr1 = node::Attr( U"p", U"name", U"value1" );
    auto attr2 = node::Attr( U"p", U"name", U"value2" );

    //added attribute - sanity check
    auto * returned1 = nnm.setNode( std::make_unique<node::Attr>( attr1 ) );
    ASSERT_TRUE( returned1->equivalent( attr1 ) );
    ASSERT_EQ( nnm.list().size(), 1 );

    //updated attribute
    auto * returned2 = nnm.setNode( std::make_unique<node::Attr>( attr2 ) );
    ASSERT_NE( returned1, nullptr );
    ASSERT_EQ( returned1, returned2 );
    ASSERT_TRUE( returned2->equivalent( attr2 ) );
    ASSERT_EQ( nnm.list().size(), 1 );
}

TEST( parser_dom_NamedNodeMap_Tests, setNode_fail_0 ) { //invalid name
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"na%me", U"value" );

    ASSERT_THROW( std::ignore = nnm.setNode( std::make_unique<node::Attr>( attr ) ),
                  blogator::parser::dom::exception::DOMException );
}

TEST( parser_dom_NamedNodeMap_Tests, removeItem_0 ) {
    auto nnm  = NamedNodeMap( nullptr );
    ASSERT_FALSE( nnm.removeItem( 0 ) );
}

TEST( parser_dom_NamedNodeMap_Tests, removeItem_1 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );

    ASSERT_EQ( nnm.list().size(), 1 );
    auto ptr = nnm.removeItem( 0 );
    ASSERT_TRUE( ptr );
    ASSERT_TRUE( attr.equivalent( *ptr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, removeNode_0 ) {
    auto nnm = NamedNodeMap( nullptr );
    auto ptr = std::make_unique<node::Attr>( U"" );
    ASSERT_FALSE( nnm.removeNode( ptr.get() ) );
}

TEST( parser_dom_NamedNodeMap_Tests, removeNode_1 ) {
    auto nnm = NamedNodeMap( nullptr );
    std::unique_ptr<node::Attr> ptr;
    ASSERT_FALSE( nnm.removeNode( ptr.get() ) );
}

TEST( parser_dom_NamedNodeMap_Tests, removeNode_2 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );

    ASSERT_EQ( nnm.list().size(), 1 );
    auto ptr = nnm.removeNode( nnm.list()[0].get() );
    ASSERT_TRUE( ptr );
    ASSERT_TRUE( attr.equivalent( *ptr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, removeNamedItem_0 ) {
    auto nnm = NamedNodeMap( nullptr );
    ASSERT_FALSE( nnm.removeNamedItem( U"invalid" ) );
}

TEST( parser_dom_NamedNodeMap_Tests, removeNamedItem_1 ) {
    auto nnm  = NamedNodeMap( nullptr );
    auto attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );

    ASSERT_EQ( nnm.list().size(), 1 );
    auto ptr = nnm.removeNamedItem( attr.nodeName() );
    ASSERT_TRUE( ptr );
    ASSERT_TRUE( attr.equivalent( *ptr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, removeNamedItem_2 ) { //html doc+element
    auto   doc  = node::Document();
    auto * node = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto   nnm  = NamedNodeMap( node );
    auto   attr = node::Attr( U"p", U"name", U"value" );
    nnm.setNode( std::make_unique<node::Attr>( attr ) );

    ASSERT_EQ( nnm.list().size(), 1 );
    auto ptr = nnm.removeNamedItem( U"P:Name" );
    ASSERT_TRUE( ptr );
    ASSERT_TRUE( attr.equivalent( *ptr ) );
}

TEST( parser_dom_NamedNodeMap_Tests, swap ) {
    auto   doc1  = node::Document();
    auto   doc2  = node::Document();
    auto * node1 = doc1.appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto * node2 = doc2.appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto   attr1 = node::Attr( U"p", U"name1", U"value1" );
    auto   attr2 = node::Attr( U"p", U"name2", U"value2" );

    auto   nnm1 = NamedNodeMap( node1 );
    auto   nnm2 = NamedNodeMap( node2 );

    nnm1.setNamedItem( attr1 );
    nnm2.setNamedItem( attr2 );

    swap( nnm1, nnm2 );

    ASSERT_EQ( nnm1.ownerNode(), node1 );
    ASSERT_EQ( nnm1.list().size(), 1 );
    ASSERT_EQ( nnm1.list()[0]->parentNode(), node1 );
    ASSERT_EQ( nnm1.list()[0]->ownerElement(), node1 );
    ASSERT_EQ( nnm1.list()[0]->ownerDocument(), &doc1 );
    ASSERT_TRUE( nnm1.list()[0]->equivalent( attr2 ) );

    ASSERT_EQ( nnm2.ownerNode(), node2 );
    ASSERT_EQ( nnm2.list().size(), 1 );
    ASSERT_EQ( nnm2.list()[0]->parentNode(), node2 );
    ASSERT_EQ( nnm2.list()[0]->ownerElement(), node2 );
    ASSERT_EQ( nnm2.list()[0]->ownerDocument(), &doc2 );
    ASSERT_TRUE( nnm2.list()[0]->equivalent( attr1 ) );
}

TEST( parser_dom_NamedNodeMap_Tests, ownerElement_0 ) {
    auto node = node::Node();
    auto nnm  = NamedNodeMap( &node );
    ASSERT_EQ( nnm.ownerElement(), nullptr );
}

TEST( parser_dom_NamedNodeMap_Tests, ownerElement_1 ) {
    auto node = node::Element( blogator::parser::specs::html5::Element::HTML5_DIV );
    auto nnm  = NamedNodeMap( &node );
    ASSERT_EQ( nnm.ownerElement(), &node );
}