#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Element.h"
#include "../../../src/parser/dom/node/Document.h"
#include "../../../src/parser/dom/node/Text.h"
#include "../../../src/parser/dom/node/CDATASection.h"
#include "../../../src/parser/dom/node/Comment.h"

#include "../../../TestHelpers/TestDOMs.h"

using namespace blogator::parser::dom::node;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;
using           blogator::parser::dom::exception::DOMException;
using           blogator::parser::dom::exception::DOMExceptionType;

TEST( parser_dom_node_Element_Tests, constructor_0 ) { //Element( specs::infra::Element element )
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    ASSERT_EQ( node.nodeName(), U"html" );
    ASSERT_EQ( node.prefix(), U"" );
    ASSERT_EQ( node.namespaceURI(), U"http://www.w3.org/1999/xhtml" );
}

TEST( parser_dom_node_Element_Tests, constructor_1 ) { //Element( specs::infra::Element element ) w/ HTML owner document
    auto   doc  = Document();
    auto * node = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    ASSERT_EQ( node->nodeName(), U"HTML" );
    ASSERT_EQ( dynamic_cast<Element *>( node )->prefix(), U"" );
    ASSERT_EQ( dynamic_cast<Element *>( node )->namespaceURI(), U"http://www.w3.org/1999/xhtml" );
}

TEST( parser_dom_node_Element_Tests, constructor_2 ) { //Element( DOMString_t ns, DOMString_t name )
    auto node = Element( U"namespace", U"name" );
    ASSERT_EQ( node.nodeName(), U"name" );
    ASSERT_EQ( node.prefix(), U"" );
    ASSERT_EQ( node.namespaceURI(), U"namespace" );
}

TEST( parser_dom_node_Element_Tests, constructor_3 ) { //Element( DOMString_t ns, DOMString_t prefix, DOMString_t name )
    auto node = Element( U"namespace", U"prefix", U"name" );
    ASSERT_EQ( node.nodeName(), U"prefix:name" );
    ASSERT_EQ( node.prefix(), U"prefix" );
    ASSERT_EQ( node.namespaceURI(), U"namespace" );
}

TEST( parser_dom_node_Element_Tests, constructor_4 ) { //Element::Element( NamespaceMap::id_t ns_id, DOMString_t name )
    auto node = Element( 0, U"constructor_4" ); //ns_id: 0 for NONE otherwise `nodeName()` would detect NamespaceMap inconsistency and throw
    ASSERT_EQ( node.nodeName(), U"constructor_4" );
    ASSERT_EQ( node.prefix(), U"" );
    ASSERT_EQ( node.namespaceID(), 0 );
}

TEST( parser_dom_node_Element_Tests, constructor_5 ) { //Element::Element( Document * document, NamespaceMap::id_t ns_id, DOMString_t name )
    auto doc  = Document();
    auto node = Element( &doc, 666, U"constructor_5" );
    ASSERT_EQ( node.ownerDocument(), &doc );
    ASSERT_EQ( node.localName(), U"constructor_5" );
    ASSERT_EQ( node.prefix(), U"" );
    ASSERT_EQ( node.namespaceID(), 666 );
}

TEST( parser_dom_node_Element_Tests, constructor_6 ) { //Element::Element( Document * document, NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t name )
    auto doc  = Document();
    auto node = Element( &doc, 666, U"parser_dom_node_Element_Tests", U"constructor_6" );
    ASSERT_EQ( node.ownerDocument(), &doc );
    ASSERT_EQ( node.localName(), U"constructor_6" );
    ASSERT_EQ( node.prefix(), U"parser_dom_node_Element_Tests" );
    ASSERT_EQ( node.namespaceID(), 666 );
}

TEST( parser_dom_node_Element_Tests, copy_constructor ) {
    auto   document     = Document();
    auto * parent       = document.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * prev_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HEAD ) );
    auto * element      = dynamic_cast<Element *>( parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) ) );
    auto * next_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   attr         = Attr( U"", U"p", U"name" , U"value" );
    element->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    element->setAttributeNode( attr );

    //sanity check
    ASSERT_EQ( element->ownerDocument(), &document );
    ASSERT_EQ( element->parentNode(), parent );
    ASSERT_EQ( element->previousSibling(), prev_sibling );
    ASSERT_EQ( element->nextSibling(), next_sibling );
    ASSERT_EQ( element->childNodes().size(), 1 );
    ASSERT_EQ( element->attributes().length(), 1 );
    ASSERT_TRUE( element->attributes().list()[0]->equivalent( attr ) );

    auto copy = Element( *element );

    //test
    ASSERT_EQ( copy.ownerDocument(), nullptr );
    ASSERT_EQ( copy.parentNode(), nullptr );
    ASSERT_EQ( copy.previousSibling(), nullptr );
    ASSERT_EQ( copy.nextSibling(), nullptr );
    ASSERT_EQ( copy.namespaceID(), element->namespaceID() );
    ASSERT_EQ( copy.prefix(), element->prefix() );
    ASSERT_EQ( copy.nodeName(), U"h1" ); //lowercase as copy is orphaned from HTML document
    ASSERT_EQ( copy.childNodes().size(), 1 );
    ASSERT_EQ( copy.childNodes()[0]->nodeName(), U"span" ); //lowercase as copy is orphaned from HTML document
    ASSERT_EQ( copy.attributes().length(), 1 );
    ASSERT_TRUE( copy.attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, move_constructor ) {
    auto   document     = Document();
    auto * parent       = document.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * prev_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HEAD ) );
    auto * element      = dynamic_cast<Element *>( parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) ) );
    auto * next_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   attr         = Attr( U"p", U"name" , U"value" );
    element->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    element->setAttributeNode( attr );

    //sanity check
    ASSERT_EQ( element->ownerDocument(), &document );
    ASSERT_EQ( element->parentNode(), parent );
    ASSERT_EQ( element->previousSibling(), prev_sibling );
    ASSERT_EQ( element->nextSibling(), next_sibling );
    ASSERT_EQ( element->childNodes().size(), 1 );
    ASSERT_EQ( element->attributes().length(), 1 );
    ASSERT_TRUE( element->attributes().list()[0]->equivalent( attr ) );

    auto ns_id  = element->namespaceID();
    auto prefix = element->prefix();
    auto moved  = Element( std::move( *element ) );

    //test
    ASSERT_EQ( moved.ownerDocument(), &document );
    ASSERT_EQ( moved.parentNode(), parent );
    ASSERT_EQ( moved.previousSibling(), prev_sibling );
    ASSERT_EQ( moved.nextSibling(), next_sibling );
    ASSERT_EQ( moved.prefix(), prefix );
    ASSERT_EQ( moved.nodeName(), U"H1" );
    ASSERT_EQ( moved.namespaceID(), ns_id );
    ASSERT_EQ( moved.childNodes().size(), 1 );
    ASSERT_EQ( moved.childNodes()[0]->nodeName(), U"SPAN" );
    ASSERT_EQ( moved.attributes().length(), 1 );
    ASSERT_TRUE( moved.attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, copy_operator ) {
    auto   document     = Document();
    auto * parent       = document.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * prev_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HEAD ) );
    auto * element      = dynamic_cast<Element *>( parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) ) );
    auto * next_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   attr         = Attr( U"", U"p", U"name" , U"value" );
    element->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    element->setAttributeNode( attr );

    //sanity check
    ASSERT_EQ( element->ownerDocument(), &document );
    ASSERT_EQ( element->parentNode(), parent );
    ASSERT_EQ( element->previousSibling(), prev_sibling );
    ASSERT_EQ( element->nextSibling(), next_sibling );
    ASSERT_EQ( element->childNodes().size(), 1 );
    ASSERT_EQ( element->attributes().length(), 1 );
    ASSERT_TRUE( element->attributes().list()[0]->equivalent( attr ) );

    Element copy = Element( blogator::parser::specs::infra::Element::SVG ); //temp assigned Element will be discarded
    copy = *element;

    //test
    ASSERT_EQ( copy.ownerDocument(), nullptr );
    ASSERT_EQ( copy.parentNode(), nullptr );
    ASSERT_EQ( copy.previousSibling(), nullptr );
    ASSERT_EQ( copy.nextSibling(), nullptr );
    ASSERT_EQ( copy.namespaceID(), element->namespaceID() );
    ASSERT_EQ( copy.prefix(), element->prefix() );
    ASSERT_EQ( copy.nodeName(), U"h1" ); //lowercase as copy is orphaned from HTML document
    ASSERT_EQ( copy.childNodes().size(), 1 );
    ASSERT_EQ( copy.childNodes()[0]->nodeName(), U"span" ); //lowercase as copy is orphaned from HTML document
    ASSERT_EQ( copy.attributes().length(), 1 );
    ASSERT_TRUE( copy.attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, move_operator ) {
    auto   document     = Document();
    auto * parent       = document.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * prev_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HEAD ) );
    auto * element      = dynamic_cast<Element *>( parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) ) );
    auto * next_sibling = parent->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   attr         = Attr( U"p", U"name" , U"value" );
    element->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    element->setAttributeNode( attr );

    //sanity check
    ASSERT_EQ( element->ownerDocument(), &document );
    ASSERT_EQ( element->parentNode(), parent );
    ASSERT_EQ( element->previousSibling(), prev_sibling );
    ASSERT_EQ( element->nextSibling(), next_sibling );
    ASSERT_EQ( element->childNodes().size(), 1 );
    ASSERT_EQ( element->attributes().length(), 1 );
    ASSERT_TRUE( element->attributes().list()[0]->equivalent( attr ) );

    auto ns_id  = element->namespaceID();
    auto prefix = element->prefix();
    Element moved = Element( blogator::parser::specs::infra::Element::SVG ); //temp assigned Element will be discarded
    moved = std::move( *element );

    //test
    ASSERT_EQ( moved.ownerDocument(), &document );
    ASSERT_EQ( moved.parentNode(), parent );
    ASSERT_EQ( moved.previousSibling(), prev_sibling );
    ASSERT_EQ( moved.nextSibling(), next_sibling );
    ASSERT_EQ( moved.prefix(), prefix );
    ASSERT_EQ( moved.tagName(), U"H1" );
    ASSERT_EQ( moved.namespaceID(), ns_id );
    ASSERT_EQ( moved.childNodes().size(), 1 );
    ASSERT_EQ( moved.childNodes()[0]->nodeName(), U"SPAN" );
    ASSERT_EQ( moved.attributes().length(), 1 );
    ASSERT_TRUE( moved.attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, swap_0 ) { //as Node
    auto   doc1   = Document();
    auto * node1  = dynamic_cast<Element *>( doc1.appendChild( std::make_unique<Element>( U"ns-A", U"x", U"name1" ) ) );
    auto   attr1a = Attr( U"x", U"id", U"attr1" );
    auto   attr1b = Attr( U"x", U"class", U"class1" );
    auto * ptr1a  = node1->attributes().setNamedItem( attr1a );
    auto * ptr1b  = node1->attributes().setNamedItem( attr1b );
    auto * child1 = node1->appendChild( std::make_unique<Text>( U"text" )  );

    auto   doc2   = Document();
    auto   node2  = dynamic_cast<Element *>( doc2.appendChild( std::make_unique<Element>( U"ns-B", U"y", U"name2" ) ) );
    auto   attr2a = Attr( U"y", U"id", U"attr2" );
    auto   attr2b = Attr( U"y", U"class", U"class2" );
    auto * ptr2a  = node2->attributes().setNamedItem( attr2a );
    auto * ptr2b  = node2->attributes().setNamedItem( attr2b );

    blogator::parser::dom::node::swap( dynamic_cast<Node &>( *node1 ), dynamic_cast<Node &>( *node2 ) );

    auto * doc1_element = doc1.documentElement();
    ASSERT_EQ( doc1_element, dynamic_cast<Element *>( node1 ) );
    ASSERT_EQ( doc1_element->namespaceURI(), U"ns-B" );
    ASSERT_EQ( doc1_element->nodeName(), U"y:name2" );
    ASSERT_EQ( doc1_element->attributes().length(), 2 );
    ASSERT_EQ( doc1_element->attributes().ownerElement(), node1 );
    ASSERT_EQ( doc1_element->ownerDocument(), &doc1 );
    ASSERT_EQ( doc1_element->parentNode(), &doc1 );
    ASSERT_TRUE( doc1_element->attributes().list()[0]->equivalent( attr2a ) );
    ASSERT_TRUE( doc1_element->attributes().list()[1]->equivalent( attr2b ) );
    ASSERT_EQ( doc1_element->attributes().list()[0]->ownerElement(), doc1_element );
    ASSERT_EQ( doc1_element->attributes().list()[0]->ownerDocument(), &doc1 );
    ASSERT_EQ( doc1_element->attributes().list()[0]->parentNode(), node1 );
    ASSERT_EQ( doc1_element->attributes().list()[1]->ownerElement(), doc1_element );
    ASSERT_EQ( doc1_element->attributes().list()[1]->ownerDocument(), &doc1 );
    ASSERT_EQ( doc1_element->attributes().list()[1]->parentNode(), node1 );
    ASSERT_EQ( doc1_element->childNodes().size(), 1 );
    ASSERT_EQ( doc1_element->childNodes()[0].get(), child1 );

    auto * doc2_element = doc2.documentElement();
    ASSERT_EQ( doc2_element, dynamic_cast<Element *>( node2 ) );
    ASSERT_EQ( doc2_element->namespaceURI(), U"ns-A" );
    ASSERT_EQ( doc2_element->nodeName(), U"x:name1" );
    ASSERT_EQ( doc2_element->attributes().length(), 2 );
    ASSERT_EQ( doc2_element->attributes().ownerElement(), node2 );
    ASSERT_EQ( doc2_element->ownerDocument(), &doc2 );
    ASSERT_EQ( doc2_element->parentNode(), &doc2 );
    ASSERT_TRUE( doc2_element->attributes().list()[0]->equivalent( attr1a ) );
    ASSERT_TRUE( doc2_element->attributes().list()[1]->equivalent( attr1b ) );
    ASSERT_EQ( doc2_element->attributes().list()[0]->ownerElement(), doc2_element );
    ASSERT_EQ( doc2_element->attributes().list()[0]->ownerDocument(), &doc2 );
    ASSERT_EQ( doc2_element->attributes().list()[0]->parentNode(), node2 );
    ASSERT_EQ( doc2_element->attributes().list()[1]->ownerElement(), doc2_element );
    ASSERT_EQ( doc2_element->attributes().list()[1]->ownerDocument(), &doc2 );
    ASSERT_EQ( doc2_element->attributes().list()[1]->parentNode(), node2 );
    ASSERT_EQ( doc2_element->childNodes().size(), 0 );
}

TEST( parser_dom_node_Element_Tests, swap_1 ) { //as Element
    auto   doc1   = Document();
    auto * node1  = dynamic_cast<Element *>( doc1.appendChild( std::make_unique<Element>( U"ns-A", U"x", U"name1" ) ) );
    auto   attr1a = Attr( U"x", U"id", U"attr1" );
    auto   attr1b = Attr( U"x", U"class", U"class1" );
    auto * ptr1a  = node1->attributes().setNamedItem( attr1a );
    auto * ptr1b  = node1->attributes().setNamedItem( attr1b );
    auto * child1 = node1->appendChild( std::make_unique<Text>( U"text" )  );

    auto   doc2   = Document();
    auto   node2  = dynamic_cast<Element *>( doc2.appendChild( std::make_unique<Element>( U"ns-B", U"y", U"name2" ) ) );
    auto   attr2a = Attr( U"y", U"id", U"attr2" );
    auto   attr2b = Attr( U"y", U"class", U"class2" );
    auto * ptr2a  = node2->attributes().setNamedItem( attr2a );
    auto * ptr2b  = node2->attributes().setNamedItem( attr2b );

    blogator::parser::dom::node::swap( *node1, *node2 ); //swap only element properties and attributes

    auto * doc1_element = doc1.documentElement();
    ASSERT_EQ( doc1_element, dynamic_cast<Element *>( node1 ) );
    ASSERT_EQ( doc1_element->namespaceURI(), U"ns-B" );
    ASSERT_EQ( doc1_element->nodeName(), U"y:name2" );
    ASSERT_EQ( doc1_element->attributes().length(), 2 );
    ASSERT_EQ( doc1_element->attributes().ownerElement(), node1 );
    ASSERT_EQ( doc1_element->ownerDocument(), &doc1 );
    ASSERT_EQ( doc1_element->parentNode(), &doc1 );
    ASSERT_TRUE( doc1_element->attributes().list()[0]->equivalent( attr2a ) );
    ASSERT_TRUE( doc1_element->attributes().list()[1]->equivalent( attr2b ) );
    ASSERT_EQ( doc1_element->attributes().list()[0]->ownerElement(), doc1_element );
    ASSERT_EQ( doc1_element->attributes().list()[0]->ownerDocument(), &doc1 );
    ASSERT_EQ( doc1_element->attributes().list()[0]->parentNode(), node1 );
    ASSERT_EQ( doc1_element->attributes().list()[1]->ownerElement(), doc1_element );
    ASSERT_EQ( doc1_element->attributes().list()[1]->ownerDocument(), &doc1 );
    ASSERT_EQ( doc1_element->attributes().list()[1]->parentNode(), node1 );
    ASSERT_EQ( doc1_element->childNodes().size(), 1 );
    ASSERT_EQ( doc1_element->childNodes()[0].get(), child1 );

    auto * doc2_element = doc2.documentElement();
    ASSERT_EQ( doc2_element, dynamic_cast<Element *>( node2 ) );
    ASSERT_EQ( doc2_element->namespaceURI(), U"ns-A" );
    ASSERT_EQ( doc2_element->nodeName(), U"x:name1" );
    ASSERT_EQ( doc2_element->attributes().length(), 2 );
    ASSERT_EQ( doc2_element->attributes().ownerElement(), node2 );
    ASSERT_EQ( doc2_element->ownerDocument(), &doc2 );
    ASSERT_EQ( doc2_element->parentNode(), &doc2 );
    ASSERT_TRUE( doc2_element->attributes().list()[0]->equivalent( attr1a ) );
    ASSERT_TRUE( doc2_element->attributes().list()[1]->equivalent( attr1b ) );
    ASSERT_EQ( doc2_element->attributes().list()[0]->ownerElement(), doc2_element );
    ASSERT_EQ( doc2_element->attributes().list()[0]->ownerDocument(), &doc2 );
    ASSERT_EQ( doc2_element->attributes().list()[0]->parentNode(), node2 );
    ASSERT_EQ( doc2_element->attributes().list()[1]->ownerElement(), doc2_element );
    ASSERT_EQ( doc2_element->attributes().list()[1]->ownerDocument(), &doc2 );
    ASSERT_EQ( doc2_element->attributes().list()[1]->parentNode(), node2 );
    ASSERT_EQ( doc2_element->childNodes().size(), 0 );
}

TEST( parser_dom_node_Element_Tests, elementType_0 ) {
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_EQ( node.elementType(), blogator::parser::specs::infra::Element::HTML5_DIV );
}

TEST( parser_dom_node_Element_Tests, elementType_1 ) {
    auto node = Element( U"ns", U"name" );
    ASSERT_EQ( node.elementType(), blogator::parser::specs::infra::Element::OTHER );
}

TEST( parser_dom_node_Element_Tests, qualifiedName_0 ) {
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_EQ( node.qualifiedName(), U"div" );
}

TEST( parser_dom_node_Element_Tests, qualifiedName_1 ) {
    auto node = Element( U"ns", U"name" );
    ASSERT_EQ( node.qualifiedName(), U"name" );
}

TEST( parser_dom_node_Element_Tests, qualifiedName_2 ) {
    auto node = Element( U"ns", U"", U"name" );
    ASSERT_EQ( node.qualifiedName(), U"name" );
}

TEST( parser_dom_node_Element_Tests, qualifiedName_3 ) {
    auto node = Element( U"ns", U"prefix", U"name" );
    ASSERT_EQ( node.qualifiedName(), U"prefix:name" );
}

TEST( parser_dom_node_Element_Tests, namespaceID ) {
    auto node = Element( U"ns", U"prefix", U"name" );
    ASSERT_NE( node.namespaceID(), blogator::parser::dom::NamespaceMap::INVALID );
}

TEST( parser_dom_node_Element_Tests, isHTML_0 ) { //orphaned, non-HTML Element
    auto node = Element( blogator::parser::specs::infra::Element::SVG );
    ASSERT_FALSE( node.isHtmlNative() );
}

TEST( parser_dom_node_Element_Tests, isHTML_1 ) { //orphaned, HTML Element
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_FALSE( node.isHtmlNative() );
}

TEST( parser_dom_node_Element_Tests, isHTML_2 ) { //XML Document, HTML Element
    using blogator::parser::specs::infra::ContentType;
    auto   doc  = Document( U"UTF-8", ContentType::APPLICATION_XML, U"", "/", Document::Type::XML, blogator::parser::dom::QuirksMode::QUIRKS );
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    ASSERT_FALSE( node->isHtmlNative() );
}

TEST( parser_dom_node_Element_Tests, isHTML_3 ) { //HTML Document, non-HTML Element
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::SVG ) ) );
    ASSERT_FALSE( node->isHtmlNative() );
}

TEST( parser_dom_node_Element_Tests, isHTML_4 ) { //HTML Document, HTML Element
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    ASSERT_TRUE( node->isHtmlNative() );
}

TEST( parser_dom_node_Element_Tests, createAttribute_0 ) {
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr = Attr( U"name" );
    auto * ptr  = node->createAttribute( attr.name() );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 1 );
    ASSERT_EQ( node->attributes().list()[0].get(), ptr );
    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, createAttribute_1 ) { //html doc + element
    auto   doc   = Document();
    auto * node  = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    auto   attr1 = Attr( U"NAME" );
    auto   attr2 = Attr( U"name" );
    auto * ptr   = node->createAttribute( attr1.name() );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 1 );
    ASSERT_EQ( node->attributes().list()[0].get(), ptr );
    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr2 ) );
}

TEST( parser_dom_node_Element_Tests, createAttribute_2 ) {
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr = Attr( U"name", U"value" );
    auto * ptr  = node->createAttribute( attr.name(), *attr.value() );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 1 );
    ASSERT_EQ( node->attributes().list()[0].get(), ptr );
    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, createAttribute_3 ) { //html doc + element
    auto   doc   = Document();
    auto * node  = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    auto   attr1 = Attr( U"NAME", U"value" );
    auto   attr2 = Attr( U"name", U"value" );
    auto * ptr   = node->createAttribute( attr1.name(), *attr1.value() );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 1 );
    ASSERT_EQ( node->attributes().list()[0].get(), ptr );
    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr2 ) );
}

TEST( parser_dom_node_Element_Tests, createAttribute_fail_0 ) { //name only + invalid name
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );

    try {
        node.createAttribute( U"n a m e" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Element_Tests, createAttribute_fail_1 ) { //name & value + invalid name
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );

    try {
        node.createAttribute( U"n a m e", U"value" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Element_Tests, createAttributeNS_0 ) {
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr = Attr( U"parser_dom_node_Element_Tests_createAttributeNS_0", U"ns", U"name" );
    auto * ptr  = node->createAttributeNS( attr.namespaceURI(), attr.prefix() + U":" + attr.name(), U"" );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 1 );
    ASSERT_EQ( node->attributes().list()[0].get(), ptr );
    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, createAttributeNS_1 ) { //html doc + element
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    auto   attr = Attr( U"parser_dom_node_Element_Tests_createAttributeNS_1", U"ns", U"NAME", U"value" );
    auto * ptr  = node->createAttributeNS( attr.namespaceURI(), attr.prefix() + U":" + attr.name(), *attr.value() );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 1 );
    ASSERT_EQ( node->attributes().list()[0].get(), ptr );
    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, createCDATASection ) {
    auto   node  = Element( U"ns", U"name" );
    auto   text  = DOMString_t( U"test" );
    auto * child = node.createCDATASection( text );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->nodeType(), NodeType::CDATA_SECTION_NODE );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_NE( node.childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( *node.childNodes()[0]->nodeValue(), text );
}

TEST( parser_dom_node_Element_Tests, createComment ) {
    auto   node  = Element( U"ns", U"name" );
    auto   text  = DOMString_t( U"test" );
    auto * child = node.createComment( text );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->nodeType(), NodeType::COMMENT_NODE );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_NE( node.childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( *node.childNodes()[0]->nodeValue(), text );
}

TEST( parser_dom_node_Element_Tests, createElement_0 ) {
    auto   node  = Element( U"name1" );
    auto * child = node.createElement( U"name2" );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->nodeType(), NodeType::ELEMENT_NODE );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_EQ( node.childNodes()[0]->nodeName(), U"name2" );
}

TEST( parser_dom_node_Element_Tests, createElement_1 ) {
    auto   node  = Element( U"ns1", U"name1" );
    auto * child = node.createElement( blogator::parser::specs::infra::Element::HTML5_SPAN );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->nodeType(), NodeType::ELEMENT_NODE );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_EQ( node.childNodes()[0]->nodeName(), U"span" );
}

TEST( parser_dom_node_Element_Tests, createElement_2 ) { //html + doc
    auto   doc   = Document();
    auto * node  = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns1", U"name1" ) ) );
    auto * child = node->createElement( U"MyElement" );

    ASSERT_EQ( node->childNodes().size(), 1 );
    ASSERT_EQ( node->childNodes()[0].get(), child );
    ASSERT_EQ( node->childNodes()[0]->nodeType(), NodeType::ELEMENT_NODE );
    ASSERT_EQ( node->childNodes()[0]->parentNode(), node );
    ASSERT_EQ( node->childNodes()[0]->nodeName(), U"MYELEMENT" );
}

TEST( parser_dom_node_Element_Tests, createElement_fail_0 ) {
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );

    try {
        node.createElement( U"n@me" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Element_Tests, createElementNS_0 ) { //no prefix
    auto   node  = Element( U"name1" );
    auto * child = node.createElementNS( U"parser_dom_node_Element_Tests_createElementNS_0", U"NoPrefixName" );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->nodeType(), NodeType::ELEMENT_NODE );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_EQ( dynamic_cast<Element *>( node.childNodes()[0].get() )->namespaceURI(), U"parser_dom_node_Element_Tests_createElementNS_0" );
    ASSERT_EQ( node.childNodes()[0]->nodeName(), U"NoPrefixName" );
}

TEST( parser_dom_node_Element_Tests, createElementNS_1 ) { //with prefix
    auto   node  = Element( U"name1" );
    auto * child = node.createElementNS( U"parser_dom_node_Element_Tests_createElementNS_1", U"ns:name2" );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->nodeType(), NodeType::ELEMENT_NODE );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_EQ( dynamic_cast<Element *>( node.childNodes()[0].get() )->namespaceURI(), U"parser_dom_node_Element_Tests_createElementNS_1" );
    ASSERT_EQ( node.childNodes()[0]->nodeName(), U"ns:name2" );
}

TEST( parser_dom_node_Element_Tests, createElementNS_fail_0 ) { //invalid qualified name
    auto   node  = Element( U"name1" );

    try {
        node.createElementNS( U"parser_dom_node_Element_Tests_createElementNS_fail_0", U"ns:n@me" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Element_Tests, createText ) {
    auto   node  = Element( U"ns", U"name" );
    auto   text  = DOMString_t( U"test" );
    auto * child = node.createText( text );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->nodeType(), NodeType::TEXT_NODE );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_NE( node.childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( *node.childNodes()[0]->nodeValue(), text );
}

TEST( parser_dom_node_Element_Tests, namespaceURI_0 ) {
    auto node = Element( blogator::parser::specs::infra::Element::MATH );
    ASSERT_EQ( node.namespaceURI(), U"http://www.w3.org/1998/Math/MathML" );
}

TEST( parser_dom_node_Element_Tests, namespaceURI_1 ) {
    auto node = Element( U"namespace", U"name" );
    ASSERT_EQ( node.namespaceURI(), U"namespace" );
}

TEST( parser_dom_node_Element_Tests, namespaceURI_fail_0 ) {
    auto node = Element( 666, U"name" );
    ASSERT_THROW( std::ignore = node.namespaceURI(), blogator::exception::failed_expectation );
}

TEST( parser_dom_node_Element_Tests, prefix_0 ) {
    auto node = Element( U"ns", U"name" );
    ASSERT_EQ( node.prefix(), U"" );
}

TEST( parser_dom_node_Element_Tests, prefix_1 ) {
    auto node = Element( U"ns", U"prefix", U"name" );
    ASSERT_EQ( node.prefix(), U"prefix" );
}

TEST( parser_dom_node_Element_Tests, localName ) {
    auto node = Element( U"ns", U"prefix", U"name" );
    ASSERT_EQ( node.localName(), U"name" );
}

TEST( parser_dom_node_Element_Tests, tagName ) {
    auto node = Element( U"ns", U"prefix", U"name" );
    ASSERT_EQ( node.tagName(), U"PREFIX:NAME" );
}

TEST( parser_dom_node_Element_Tests, id_0 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"name", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( node.id(), U"" );
}

TEST( parser_dom_node_Element_Tests, id_1 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"id", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( node.id(), U"value" );
}

TEST( parser_dom_node_Element_Tests, className_0 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"name", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( node.className(), U"" );
}

TEST( parser_dom_node_Element_Tests, className_1 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"class", U"name1 name2" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( node.className(), U"name1 name2" );
}

TEST( parser_dom_node_Element_Tests, classList_0 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"name", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );
    auto   tks  = node.classList();

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( tks.size(), 0 );
}

TEST( parser_dom_node_Element_Tests, classList_1 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"class", U"name" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );
    auto   tks  = node.classList();

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( tks.size(), 1 );
    ASSERT_EQ( tks[0], U"name" );
}

TEST( parser_dom_node_Element_Tests, classList_2 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"class", U"name1 name2   name3" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );
    auto   tks  = node.classList();

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( tks.size(), 3 );
    ASSERT_EQ( tks[0], U"name1" );
    ASSERT_EQ( tks[1], U"name2" );
    ASSERT_EQ( tks[2], U"name3" );
}

TEST( parser_dom_node_Element_Tests, slot_0 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"name", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( node.slot(), U"" );
}

TEST( parser_dom_node_Element_Tests, slot_1 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"slot", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_EQ( node.slot(), U"value" );
}

TEST( parser_dom_node_Element_Tests, hasAttributes ) {
    auto node = Element( U"ns", U"element" );

    ASSERT_FALSE( node.hasAttributes() );
    node.createAttribute( U"test" );
    ASSERT_TRUE( node.hasAttributes() );
}

TEST( parser_dom_node_Element_Tests, attributes ) {
    auto   node       = Element( U"ns", U"element" );
    auto & attributes = node.attributes();
    node.createAttribute( U"test" );

    ASSERT_EQ( attributes.list().size(), 1 );
}

TEST( parser_dom_node_Element_Tests, getAttributeNames ) {
    auto node = Element( U"ns", U"element" );
    node.createAttribute( U"name1", U"value1" );
    node.createAttribute( U"name1", U"value2" );
    node.createAttribute( U"name2" );
    node.createAttribute( U"name3", U"value3" );

    auto names = node.getAttributeNames();
    ASSERT_EQ( names.size(), 3 );
    ASSERT_EQ( names[0], U"name1" );
    ASSERT_EQ( names[1], U"name2" );
    ASSERT_EQ( names[2], U"name3" );
}

TEST( parser_dom_node_Element_Tests, getAttribute ) {
    auto node = Element( U"ns", U"element" );

    ASSERT_EQ( node.getAttribute( U"y:name" ), nullptr );

    node.createAttribute( U"name1", U"value1" );
    node.createAttribute( U"name2", U"value2" );

    ASSERT_NE( node.getAttribute( U"name1" ), nullptr );
    ASSERT_EQ( *node.getAttribute( U"name1" ), U"value1" );
}

TEST( parser_dom_node_Element_Tests, getAttributeNS_0 ) {
    auto node   = Element( U"ns", U"element" );
    auto ns     = DOMString_t( U"parser_dom_node_Element_Tests_getAttributeNS_0" );
    auto prefix = DOMString_t( U"prefix" );
    auto name   = DOMString_t( U"name" );
    auto value  = DOMString_t( U"ns_value" );

    ASSERT_EQ( node.getAttributeNS( ns, name ), nullptr );

    auto * attr = node.createAttributeNS( ns, prefix + U":" + name, value );

    ASSERT_NE( node.getAttributeNS( ns, name ), nullptr );
    ASSERT_EQ( *node.getAttributeNS( ns, name ), value );
}

TEST( parser_dom_node_Element_Tests, setAttribute_0 ) { //add/name only
    auto   doc    = Document();
    auto * node   = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr1  = Attr( U"name" );
    auto   attr2  = Attr( U"", U"p", U"name" );
    auto   qname1 = DOMString_t( attr1.name() );
    auto   qname2 = DOMString_t( attr2.prefix() + U":" +  attr2.name() );

    ASSERT_TRUE( node->setAttribute( qname1 ) );
    ASSERT_TRUE( node->setAttribute( qname2 ) );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 2 );

    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr1 ) );

    ASSERT_EQ( node->attributes().list()[1]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[1]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_node_Element_Tests, setAttribute_1 ) { //add/name+value
    auto   doc    = Document();
    auto * node   = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr1  = Attr( U"name", U"value" );
    auto   attr2  = Attr( U"", U"p", U"name", U"value" );
    auto   qname1 = DOMString_t( attr1.name() );
    auto   qname2 = DOMString_t( attr2.prefix() + U":" +  attr2.name() );

    ASSERT_TRUE( node->setAttribute( qname1, *attr1.value() ) );
    ASSERT_TRUE( node->setAttribute( qname2, *attr2.value() ) );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 2 );

    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr1 ) );

    ASSERT_EQ( node->attributes().list()[1]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[1]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_node_Element_Tests, setAttribute_2 ) { //update
    auto   doc   = Document();
    auto * node  = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr  = Attr( U"", U"p", U"name", U"value" );
    auto   qname = DOMString_t( attr.prefix() + U":" +  attr.name() );

    ASSERT_TRUE( node->setAttribute( qname ) );
    ASSERT_TRUE( node->setAttribute( qname, *attr.value() ) );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 1 );
    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr ) );
}

TEST( parser_dom_node_Element_Tests, setAttribute_fail_0 ) { //invalid qualified name
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );

    ASSERT_THROW( node->setAttribute( U":p:name" ), blogator::parser::dom::exception::DOMException );
    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, setAttribute_fail_1 ) { //invalid qualified name
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );

    ASSERT_THROW( node->setAttribute( U"p:na%me" ), blogator::parser::dom::exception::DOMException );
    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, setAttribute_fail_2 ) { //invalid qualified name + value
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );

    ASSERT_THROW( node->setAttribute( U":p:name", U"value" ), blogator::parser::dom::exception::DOMException );
    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, setAttribute_fail_3 ) { //invalid qualified name + value
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );

    ASSERT_THROW( node->setAttribute( U"p:n%ame", U"value" ), blogator::parser::dom::exception::DOMException );
    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, setAttributeNS_0 ) { //prefixed
    auto   doc    = Document();
    auto * node   = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr1  = Attr( U"parser_dom_node_Element_Tests_setAttributeNS_0", U"ns", U"name1" );
    auto   attr2  = Attr( U"parser_dom_node_Element_Tests_setAttributeNS_0", U"ns", U"name2", U"value" );
    auto   qname1 = DOMString_t( attr1.prefix() + U":" +  attr1.name() );
    auto   qname2 = DOMString_t( attr2.prefix() + U":" +  attr2.name() );

    ASSERT_TRUE( node->setAttributeNS( attr1.namespaceURI(), qname1, U"" ) );
    ASSERT_TRUE( node->setAttributeNS( attr2.namespaceURI(), qname2, *attr2.value() ) );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 2 );

    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr1 ) );

    ASSERT_EQ( node->attributes().list()[1]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[1]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_node_Element_Tests, setAttributeNS_1 ) { //no prefix
    auto   doc    = Document();
    auto * node   = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );
    auto   attr1  = Attr( U"parser_dom_node_Element_Tests_setAttributeNS_1", U"", U"name1" );
    auto   attr2  = Attr( U"parser_dom_node_Element_Tests_setAttributeNS_1", U"", U"name2", U"value" );
    auto   qname1 = DOMString_t( attr1.prefix() + U":" +  attr1.name() );
    auto   qname2 = DOMString_t( attr2.prefix() + U":" +  attr2.name() );

    ASSERT_TRUE( node->setAttributeNS( attr1.namespaceURI(), qname1, U"" ) );
    ASSERT_TRUE( node->setAttributeNS( attr2.namespaceURI(), qname2, *attr2.value() ) );

    ASSERT_EQ( node->childNodes().size(), 0 );
    ASSERT_EQ( node->attributes().length(), 2 );

    ASSERT_EQ( node->attributes().list()[0]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[0]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[0]->equivalent( attr1 ) );

    ASSERT_EQ( node->attributes().list()[1]->ownerDocument(), &doc );
    ASSERT_EQ( node->attributes().list()[1]->ownerElement(), node );
    ASSERT_TRUE( node->attributes().list()[1]->equivalent( attr2 ) );
}

TEST( parser_dom_node_Element_Tests, setAttributeNS_fail_0 ) { //invalid qualified name
    auto   doc    = Document();
    auto * node   = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( U"ns", U"element" ) ) );

    try {
        node->setAttributeNS( U"parser_dom_node_Element_Tests_setAttributeNS_fail_0", U"ns:n@me", U"value" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Element_Tests, removeAttribute_0 ) { //simple attribute
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_TRUE( node.setAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_TRUE( node.removeAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, removeAttribute_1 ) { //prefixed attribute
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_TRUE( node.setAttribute( U"p:attribute" ) );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_FALSE( node.removeAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_TRUE( node.removeAttribute( U"p:attribute" ) );
    ASSERT_EQ( node.attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, removeAttributeNS_0 ) {
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto attr = Attr( U"parser_dom_node_Element_Tests_removeAttributeNS_0", U"prefix", U"name", U"value" );
    ASSERT_TRUE( node.setAttributeNS( attr.namespaceURI(), attr.prefix() + U":" + attr.localName(), *attr.value() ) );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_FALSE( node.removeAttributeNS( U"invalid", attr.localName() ) );
    ASSERT_FALSE( node.removeAttributeNS( attr.namespaceURI(), U"invalid" ) );
    ASSERT_EQ( node.attributes().length(), 1 );
    ASSERT_TRUE( node.removeAttributeNS( attr.namespaceURI(), attr.localName() ) );
    ASSERT_EQ( node.attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_0 ) { //non-existing attribute
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_TRUE( node.toggleAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 1 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_1 ) { //existing attribute
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_TRUE( node.setAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 1 );

    ASSERT_FALSE( node.toggleAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_2 ) { //non-existing attribute + html doc
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    ASSERT_TRUE( node->toggleAttribute( U"attribute" ) );
    ASSERT_EQ( node->attributes().length(), 1 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_3 ) { //existing attribute + html doc
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    ASSERT_TRUE( node->setAttribute( U"attribute" ) );
    ASSERT_EQ( node->attributes().length(), 1 );

    ASSERT_FALSE( node->toggleAttribute( U"ATTRIBUTE" ) );
    ASSERT_EQ( node->attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_4 ) { //non-existing attribute + force=false
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_FALSE( node.toggleAttribute( U"attribute", false ) );
    ASSERT_EQ( node.attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_5 ) { //non-existing attribute + force=true
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_TRUE( node.toggleAttribute( U"attribute", true ) );
    ASSERT_EQ( node.attributes().length(), 1 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_6 ) { //existing attribute + force=false
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_TRUE( node.setAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 1 );

    ASSERT_FALSE( node.toggleAttribute( U"attribute", false ) );
    ASSERT_EQ( node.attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_7 ) { //existing attribute + force=true
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_TRUE( node.setAttribute( U"attribute" ) );
    ASSERT_EQ( node.attributes().length(), 1 );

    ASSERT_TRUE( node.toggleAttribute( U"attribute", true ) );
    ASSERT_EQ( node.attributes().length(), 1 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_8 ) { //non-existing attribute + html doc + force=false
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );

    ASSERT_FALSE( node->toggleAttribute( U"ATTRIBUTE", false ) );
    ASSERT_EQ( node->attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_9 ) { //non-existing attribute + html doc + force=true
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );

    ASSERT_TRUE( node->toggleAttribute( U"ATTRIBUTE", true ) );
    ASSERT_EQ( node->attributes().length(), 1 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_10 ) { //existing attribute + html doc + force=false
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    ASSERT_TRUE( node->setAttribute( U"attribute" ) );
    ASSERT_EQ( node->attributes().length(), 1 );

    ASSERT_FALSE( node->toggleAttribute( U"ATTRIBUTE", false ) );
    ASSERT_EQ( node->attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_11 ) { //existing attribute + html doc + force=true
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    ASSERT_TRUE( node->setAttribute( U"attribute" ) );
    ASSERT_EQ( node->attributes().length(), 1 );

    ASSERT_TRUE( node->toggleAttribute( U"ATTRIBUTE", true ) );
    ASSERT_EQ( node->attributes().length(), 1 );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_fail_0 ) { //invalid qualified name
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_THROW( node.toggleAttribute( U"attr¬" ), blogator::parser::dom::exception::DOMException );
}

TEST( parser_dom_node_Element_Tests, toggleAttribute_fail_1 ) { //force + invalid qualified name
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_THROW( node.toggleAttribute( U"a%ttr", true ), blogator::parser::dom::exception::DOMException );
}

TEST( parser_dom_node_Element_Tests, hasAttribute_0 ) {
    auto node = Element( U"ns", U"element" );
    node.createAttribute( U"name" );

    ASSERT_TRUE( node.hasAttribute( U"name" ) );
}

TEST( parser_dom_node_Element_Tests, hasAttribute_1 ) {
    auto node = Element( U"ns", U"element" );
    node.createAttribute( U"name", U"value1" );

    ASSERT_FALSE( node.hasAttribute( U"p:name" ) );
    ASSERT_TRUE( node.hasAttribute( U"name" ) );
}

TEST( parser_dom_node_Element_Tests, hasAttribute_2 ) {
    auto node = Element( U"ns", U"element" );
    node.createAttribute( U"name", U"value" );

    ASSERT_FALSE( node.hasAttribute( U"p:name" ) );
    ASSERT_TRUE( node.hasAttribute( U"name" ) );
}

TEST( parser_dom_node_Element_Tests, hasAttribute_3 ) { //HTML doc
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    node->createAttribute( U"name", U"value1" );

    ASSERT_TRUE( node->hasAttribute( U"name" ) );
    ASSERT_TRUE( node->hasAttribute( U"NAME" ) );
}

TEST( parser_dom_node_Element_Tests, hasAttributeNS_0 )  {
    auto node = Element( U"ns", U"element" );

    ASSERT_FALSE( node.hasAttributeNS( U"parser_dom_node_Element_Tests_hasAttributeNS_0", U"name" ) );

    node.createAttributeNS( U"parser_dom_node_Element_Tests_hasAttributeNS_0", U"ns:name", U"value" );

    ASSERT_TRUE( node.hasAttributeNS( U"parser_dom_node_Element_Tests_hasAttributeNS_0", U"name" ) );
}

TEST( parser_dom_node_Element_Tests, getAttributeNode_0 ) {
    auto   node = Element( U"ns", U"element" );
    auto * attr = node.createAttribute( U"name", U"value" );

    ASSERT_EQ( node.getAttributeNode( U"NAME" ), nullptr );
    ASSERT_EQ( node.getAttributeNode( U"p:Name" ), nullptr );
    ASSERT_EQ( node.getAttributeNode( U"P:NAME" ), nullptr );
    ASSERT_EQ( node.getAttributeNode( U"name" ), attr );
}

TEST( parser_dom_node_Element_Tests, getAttributeNode_1 ) { //HTML doc
    auto   doc  = Document();
    auto * node = dynamic_cast<Element *>( doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    auto * attr = node->createAttribute( U"name", U"value1" );

    ASSERT_EQ( node->getAttributeNode( U"p:name" ), nullptr );
    ASSERT_EQ( node->getAttributeNode( U"Name" ), attr );
    ASSERT_EQ( node->getAttributeNode( U"name" ), attr );
    ASSERT_EQ( node->getAttributeNode( U"NAME" ), attr );
}

TEST( parser_dom_node_Element_Tests, getAttributeNodeNS_0 ) {
    auto   node   = Element( U"ns", U"element" );
    auto   ns     = DOMString_t( U"parser_dom_node_Element_Tests_getAttributeNodeNS_0" );
    auto   prefix = DOMString_t( U"ns" );
    auto   name   = DOMString_t( U"name" );
    auto * attr = node.createAttributeNS( ns, prefix + U":" + name, U"value" );

    ASSERT_EQ( node.getAttributeNodeNS( ns, U"invalid" ), nullptr );
    ASSERT_EQ( node.getAttributeNodeNS( U"invalid", name ), nullptr );
    ASSERT_EQ( node.getAttributeNodeNS( ns, name ), attr );
}

TEST( parser_dom_node_Element_Tests, setAttributeNode_0 ) { //new
    auto node = Element( U"ns", U"element" );
    auto attr = Attr( U"p", U"name", U"value" );

    auto * returned = node.setAttributeNode( attr );
    ASSERT_NE( returned, nullptr );
    ASSERT_TRUE( attr.equivalent( *returned ) );
}

TEST( parser_dom_node_Element_Tests, setAttributeNode_1 ) { //existing
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"", U"", U"name", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    auto * returned = node.setAttributeNode( attr );
    ASSERT_NE( returned, nullptr );
    ASSERT_EQ( returned, ptr );
    ASSERT_TRUE( attr.equivalent( *returned ) );
}

TEST( parser_dom_node_Element_Tests, setAttributeNodeNS_0 ) { //new
    auto node = Element( U"ns", U"element" );
    auto attr = Attr( U"parser_dom_node_Element_Tests_setAttributeNodeNS_0", U"prefix", U"name", U"value" );

    auto * returned = node.setAttributeNodeNS( attr );
    ASSERT_NE( returned, nullptr );
    ASSERT_TRUE( attr.equivalent( *returned ) );
}

TEST( parser_dom_node_Element_Tests, setAttributeNodeNS_1 ) { //existing
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"parser_dom_node_Element_Tests_setAttributeNodeNS_1", U"prefix", U"name", U"value" );
    auto * ptr  = node.createAttributeNS( attr.namespaceURI(), attr.prefix() + U":" + attr.localName(), *attr.value() );

    auto * returned = node.setAttributeNodeNS( attr );
    ASSERT_NE( returned, nullptr );
    ASSERT_EQ( returned, ptr );
    ASSERT_TRUE( attr.equivalent( *returned ) );
}

TEST( parser_dom_node_Element_Tests, removeAttributeNode_0 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"", U"p", U"name", U"value" );

    ASSERT_FALSE( node.removeAttributeNode( &attr ) );
}

TEST( parser_dom_node_Element_Tests, removeAttributeNode_1 ) {
    auto   node = Element( U"ns", U"element" );
    auto   attr = Attr( U"", U"", U"name", U"value" );
    auto * ptr  = node.createAttribute( attr.name(), *attr.value() );

    //sanity check
    ASSERT_NE( ptr, nullptr );
    ASSERT_EQ( node.attributes().length(), 1 );

    auto returned = node.removeAttributeNode( ptr );
    ASSERT_TRUE( returned );
    ASSERT_TRUE( dynamic_cast<Attr *>( returned.get() )->equivalent( attr ) );
    ASSERT_EQ( node.attributes().length(), 0 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_0 ) { //single child of same tag + named
    auto   node   = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * child1 = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = node.getElementsByTagName( U"div" );

    ASSERT_EQ( list.size(), 1 );
    ASSERT_EQ( list[0], child1 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_1 ) { //single child + named
    auto   node   = Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    auto * child1 = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = node.getElementsByTagName( U"div" );

    ASSERT_EQ( list.size(), 1 );
    ASSERT_EQ( list[0], child1 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_2 ) { //multi-child + named
    auto   node   = Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    auto * child1 = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    auto * child2 = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * child3 = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    auto * child4 = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = node.getElementsByTagName( U"div" );

    ASSERT_EQ( list.size(), 2 );
    ASSERT_EQ( list[0], child2 );
    ASSERT_EQ( list[1], child4 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_3 ) { //single child of same tag + named + HTML doc
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = dynamic_cast<Element *>( node )->getElementsByTagName( U"Div" );

    ASSERT_EQ( list.size(), 1 );
    ASSERT_EQ( list[0], child1 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_4 ) { //single child + named + HTML doc
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = dynamic_cast<Element *>( node )->getElementsByTagName( U"Div" );

    ASSERT_EQ( list.size(), 1 );
    ASSERT_EQ( list[0], child1 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_5 ) { //multi-child + named + HTML doc
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    auto * child2 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * child3 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    auto * child4 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = dynamic_cast<Element *>( node )->getElementsByTagName( U"Div" );

    ASSERT_EQ( list.size(), 2 );
    ASSERT_EQ( list[0], child2 );
    ASSERT_EQ( list[1], child4 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_6 ) { //* of same tag + named + HTML doc
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = dynamic_cast<Element *>( node )->getElementsByTagName( U"*" );

    ASSERT_EQ( list.size(), 1 );
    ASSERT_EQ( list[0], child1 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_7 ) { //* + named + HTML doc
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = dynamic_cast<Element *>( node )->getElementsByTagName( U"*" );

    ASSERT_EQ( list.size(), 1 );
    ASSERT_EQ( list[0], child1 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagName_8 ) { //* + named + HTML doc
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    auto * child2 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * child3 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    auto * child4 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto   list   = dynamic_cast<Element *>( node )->getElementsByTagName( U"*" );

    ASSERT_EQ( list.size(), 4 );
    ASSERT_EQ( list[0], child1 );
    ASSERT_EQ( list[1], child2 );
    ASSERT_EQ( list[2], child3 );
    ASSERT_EQ( list[3], child4 );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagNameNS_0 ) { //namespace and local name
    std::unique_ptr<Node> root;
    auto   v           = blogator::tests::createTestDOM_3( root );
    auto * doc         = dynamic_cast<Document *>( root.get() );
    auto * doc_element = doc->documentElement();

    auto elements = doc_element->getElementsByTagNameNS( U"http://www.site.com/xml/postgrad", U"surname" );

    ASSERT_EQ( elements.size(), 1 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"pg:surname" );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagNameNS_1 ) { //any namespaces with local name
    std::unique_ptr<Node> root;
    auto   v           = blogator::tests::createTestDOM_3( root );
    auto * doc         = dynamic_cast<Document *>( root.get() );
    auto * doc_element = doc->documentElement();

    auto elements = doc_element->getElementsByTagNameNS( U"*", U"surname" );

    ASSERT_EQ( elements.size(), 2 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"ug:surname" );
    ASSERT_EQ( elements[1]->qualifiedName(), U"pg:surname" );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagNameNS_2 ) { //any elements in a namespace
    std::unique_ptr<Node> root;
    auto   v           = blogator::tests::createTestDOM_3( root );
    auto * doc         = dynamic_cast<Document *>( root.get() );
    auto * doc_element = doc->documentElement();

    auto elements = doc_element->getElementsByTagNameNS( U"http://www.site.com/xml/postgrad", U"*" );

    ASSERT_EQ( elements.size(), 4 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"pg:student" );
    ASSERT_EQ( elements[1]->qualifiedName(), U"pg:name" );
    ASSERT_EQ( elements[2]->qualifiedName(), U"pg:surname" );
    ASSERT_EQ( elements[3]->qualifiedName(), U"pg:score" );
}

TEST( parser_dom_node_Element_Tests, getElementsByTagNameNS_3 ) { //any elements in any namespaces
    std::unique_ptr<Node> root;
    auto   v           = blogator::tests::createTestDOM_3( root );
    auto * doc         = dynamic_cast<Document *>( root.get() );
    auto * doc_element = doc->documentElement();

    auto elements = doc_element->getElementsByTagNameNS( U"*", U"*" );

    ASSERT_EQ( elements.size(), 16 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"ug:student" );
    ASSERT_EQ( elements[15]->qualifiedName(), U"pg:score" );
}

TEST( parser_dom_node_Element_Tests, getElementsByClassName_0 ) { //empty arg
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    dynamic_cast<Element *>( node )->createAttribute( U"class", U"class2" );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    dynamic_cast<Element *>( child1 )->createAttribute( U"class", U"class1" );
    auto * child2 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    dynamic_cast<Element *>( child2 )->createAttribute( U"class", U"class2" );
    auto * child3 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    dynamic_cast<Element *>( child3 )->createAttribute( U"class", U"class1" );
    auto * child4 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    dynamic_cast<Element *>( child4 )->createAttribute( U"class", U"class3" );

    auto list = dynamic_cast<Element *>( node )->getElementsByClassName( {} );
    ASSERT_EQ( list.size(), 0 );
}

TEST( parser_dom_node_Element_Tests, getElementsByClassName_1 ) { //no-quirks document
    auto   doc    = Document();
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    dynamic_cast<Element *>( node )->createAttribute( U"class", U"class2" );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    dynamic_cast<Element *>( child1 )->createAttribute( U"class", U"class1" );
    auto * child2 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    dynamic_cast<Element *>( child2 )->createAttribute( U"class", U"class2" );
    auto * child3 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    dynamic_cast<Element *>( child3 )->createAttribute( U"class", U"class1 class4" );
    auto * child4 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    dynamic_cast<Element *>( child4 )->createAttribute( U"class", U"class3" );

    auto list1 = dynamic_cast<Element *>( node )->getElementsByClassName( { U"class1" } );
    ASSERT_EQ( list1.size(), 2 );
    ASSERT_EQ( list1[0], child1 );
    ASSERT_EQ( list1[1], child3 );

    auto list2 = dynamic_cast<Element *>( node )->getElementsByClassName( { U"class2" } );
    ASSERT_EQ( list2.size(), 2 );
    ASSERT_EQ( list2[0], node );
    ASSERT_EQ( list2[1], child2 );

    auto list3 = dynamic_cast<Element *>( node )->getElementsByClassName( { U"class3" } );
    ASSERT_EQ( list3.size(), 1 );
    ASSERT_EQ( list3[0], child4 );

    auto list4 = dynamic_cast<Element *>( node )->getElementsByClassName( { U"class4" } );
    ASSERT_EQ( list4.size(), 1 );
    ASSERT_EQ( list4[0], child3 );
}

TEST( parser_dom_node_Element_Tests, getElementsByClassName_2 ) { //'quirks' document
    auto   doc    = Document();
    doc.setQuirks( blogator::parser::dom::QuirksMode::QUIRKS );
    auto * node   = doc.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );
    dynamic_cast<Element *>( node )->createAttribute( U"class", U"claSS2" );
    auto * child1 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    dynamic_cast<Element *>( child1 )->createAttribute( U"class", U"class1" );
    auto * child2 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    dynamic_cast<Element *>( child2 )->createAttribute( U"class", U"class2" );
    auto * child3 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    dynamic_cast<Element *>( child3 )->createAttribute( U"class", U"CLASS1 class4" );
    auto * child4 = node->appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    dynamic_cast<Element *>( child4 )->createAttribute( U"class", U"class3" );

    auto list1 = dynamic_cast<Element *>( node )->getElementsByClassName( { U"CLass1" } );
    ASSERT_EQ( list1.size(), 2 );
    ASSERT_EQ( list1[0], child1 );
    ASSERT_EQ( list1[1], child3 );

    auto list2 = dynamic_cast<Element *>( node )->getElementsByClassName( { U"class2", U"class3" } );
    ASSERT_EQ( list2.size(), 3 );
    ASSERT_EQ( list2[0], node );
    ASSERT_EQ( list2[1], child2 );
    ASSERT_EQ( list2[2], child4 );

    auto list3 = dynamic_cast<Element *>( node )->getElementsByClassName( { U"CLASS4" } );
    ASSERT_EQ( list3.size(), 1 );
    ASSERT_EQ( list3[0], child3 );
}

TEST( parser_dom_node_Element_Tests, output_stream_0 ) {
    auto node = Element( U"option" );
    node.createAttribute( U"value", U"test" );
    node.createAttribute( U"selected" );

    std::stringstream expected_ss;
    expected_ss << "<option value=\"test\" selected></option>";

    std::stringstream received_ss;
    received_ss << node;

    ASSERT_EQ( received_ss.str(), expected_ss.str() );
}

TEST( parser_dom_node_Element_Tests, output_stream_1 ) {
    auto node = Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    node.createAttribute( U"class", U"main" );
    auto * h1     = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    auto * h1_txt = h1->appendChild( std::make_unique<Text>( U"Heading 1" ) );
    auto * img    = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_IMG ) );
    dynamic_cast<Element *>( img )->createAttribute( U"src", U"image.jpg" );
    dynamic_cast<Element *>( img )->createAttribute( U"alt", U"kitteh" );
    auto * p      = node.appendChild( std::make_unique<Element>( blogator::parser::specs::infra::Element::HTML5_P ) );
    auto * p_txt  = p->appendChild( std::make_unique<Text>( U"paragraph text" ) );

    std::stringstream expected_ss;
    expected_ss << R"(<div class="main"><h1>Heading 1</h1><img src="image.jpg" alt="kitteh"><p>paragraph text</p></div>)";

    std::stringstream received_ss;
    received_ss << node;

    ASSERT_EQ( received_ss.str(), expected_ss.str() );
}