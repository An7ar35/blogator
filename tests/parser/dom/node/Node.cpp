#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Node.h"
#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/CDATASection.h"
#include "../../../src/parser/dom/node/Comment.h"
#include "../../../src/parser/dom/node/Document.h"
#include "../../../src/parser/dom/node/DocumentType.h"
#include "../../../src/parser/dom/node/DocumentFragment.h"
#include "../../../src/parser/dom/node/Element.h"
#include "../../../src/parser/dom/node/Text.h"
#include "../../../src/exception/failed_expectation.h"
#include "../../../../src/unicode/unicode.h"
#include "../../../TestHelpers/TestDOMs.h"

#include <memory>
#include <vector>

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;

TEST( parser_dom_node_Node_Tests, Constructor ) { //NodeType only / 1 arg constructor
    auto node = node::Node( NodeType::DOCUMENT_NODE );

    ASSERT_EQ( NodeType::DOCUMENT_NODE, node.nodeType() );
    ASSERT_EQ( node.parentNode(), nullptr );
    ASSERT_EQ( node.previousSibling(), nullptr );
    ASSERT_EQ( node.nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, Constructor_w_parent_0 ) { //3 args constructor
    auto parent = node::Node( NodeType::UNDEFINED );
    auto child  = node::Node( NodeType::UNDEFINED, &parent, nullptr );

    ASSERT_EQ( parent.parentNode(), nullptr );
    ASSERT_NE( child.parentNode(), nullptr );
    ASSERT_EQ( *child.parentNode(), parent );
}

TEST( parser_dom_node_Node_Tests, Constructor_prev_only_0 ) { //lone detached Node
    auto node = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr );

    ASSERT_EQ( NodeType::UNDEFINED, node->nodeType() );
    ASSERT_EQ( node->previousSibling(), nullptr );
    ASSERT_EQ( node->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, Constructor_prev_only_1 ) { //2 sequential nodes
    auto node1 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr );
    auto node2 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get() );

    ASSERT_EQ( node1->previousSibling(), nullptr );
    ASSERT_EQ( node1->nextSibling(), node2.get() );

    ASSERT_EQ( node2->nextSibling(), nullptr );
    ASSERT_EQ( node2->previousSibling(), node1.get() );
}

TEST( parser_dom_node_Node_Tests, Constructor_prev_only_2 ) { //2 sequential node and a third inserted between
    auto node1 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr );     //{ n1 }
    auto node2 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get() ); //{ n1, n2 }
    auto node3 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get() ); //{ n1, n3, n2 }

    ASSERT_EQ( node1->previousSibling(), nullptr );
    ASSERT_EQ( node1->nextSibling(), node3.get() );

    ASSERT_EQ( node3->previousSibling(), node1.get());
    ASSERT_EQ( node3->nextSibling(), node2.get() );

    ASSERT_EQ( node2->previousSibling(), node3.get() );
    ASSERT_EQ( node2->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, Constructor_w_parent_1 ) { //4 args constructor
    auto parent = node::Node( NodeType::UNDEFINED );
    auto child  = node::Node( NodeType::UNDEFINED, &parent, nullptr, nullptr );

    ASSERT_EQ( parent.parentNode(), nullptr );
    ASSERT_NE( child.parentNode(), nullptr );
    ASSERT_EQ( child.parentNode(), &parent );
}

TEST( parser_dom_node_Node_Tests, Constructor_prev_and_next_0 ) { //2 sequential nodes
    auto node = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );

    ASSERT_EQ( NodeType::UNDEFINED, node->nodeType() );
    ASSERT_EQ( node->previousSibling(), nullptr );
    ASSERT_EQ( node->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, Constructor_prev_and_next_1 ) { //lone detached node
    auto node1 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );
    auto node2 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get(), nullptr );

    ASSERT_EQ( node1->previousSibling(), nullptr );
    ASSERT_EQ( node1->nextSibling(), node2.get() );

    ASSERT_EQ( node2->nextSibling(), nullptr );
    ASSERT_EQ( node2->previousSibling(), node1.get() );
}

TEST( parser_dom_node_Node_Tests, Constructor_prev_and_next_2 ) { //2 sequential node and a third inserted between
    auto node1 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );
    auto node2 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get(), nullptr );
    auto node3 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get(), node2.get() );

    ASSERT_EQ( node1->previousSibling(), nullptr );
    ASSERT_EQ( node1->nextSibling(), node3.get() );

    ASSERT_EQ( node3->previousSibling(), node1.get() );
    ASSERT_EQ( node3->nextSibling(), node2.get() );

    ASSERT_EQ( node2->previousSibling(), node3.get() );
    ASSERT_EQ( node2->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, Desctructor_w_siblings ) { //checks that when a node is destroyed the immediate sibling references are updated correctly
    auto node1 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );
    auto node2 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get(), nullptr );
    auto node3 = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, node1.get(), node2.get() );

    ASSERT_EQ( node3->previousSibling(), node1.get() );
    ASSERT_EQ( node3->nextSibling(), node2.get() );

    node3.reset(); //destroy node3

    ASSERT_EQ( node1->previousSibling(), nullptr );
    ASSERT_EQ( node1->nextSibling(), node2.get() );

    ASSERT_EQ( node2->previousSibling(), node1.get() );
    ASSERT_EQ( node2->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_0 ) {
    auto node = node::Element( blogator::parser::specs::infra::Element::MATH );
    ASSERT_EQ( node::Node::getNamespaceEnum( node.namespaceID() ), blogator::parser::specs::infra::Namespace::MATHML );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_1 ) {
    auto node = node::Element( U"namespace/uri", U"tag" );
    ASSERT_EQ( node::Node::getNamespaceEnum( node.namespaceID() ), blogator::parser::specs::infra::Namespace::OTHER );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_fail_0 ) {
    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    ASSERT_THROW( node::Node::getNamespaceEnum( -1 ), blogator::exception::failed_expectation );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_fail_1 ) {
    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    ASSERT_THROW( node::Node::getNamespaceEnum( 100 ), blogator::exception::failed_expectation );
}

TEST( parser_dom_node_Node_Tests, getNamespaceURI_0 ) {
    auto node = node::Element( blogator::parser::specs::infra::Element::MATH );
    ASSERT_EQ( node.getNamespaceURI( node.namespaceID() ),
               blogator::parser::specs::infra::to_namespaceURI( blogator::parser::specs::infra::Namespace::MATHML ) );
}

TEST( parser_dom_node_Node_Tests, getNamespaceURI_1 ) {
    auto node = node::Element( U"namespace/uri", U"tag" );
    ASSERT_EQ( node.getNamespaceURI( node.namespaceID() ), U"namespace/uri" );
}

TEST( parser_dom_node_Node_Tests, getNamespaceURI_fail_0 ) {
    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    ASSERT_THROW( node.getNamespaceURI( -1 ), blogator::exception::failed_expectation );
}

TEST( parser_dom_node_Node_Tests, getNamespaceURI_fail_1 ) {
    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    ASSERT_THROW( node.getNamespaceURI( 100 ), blogator::exception::failed_expectation );
}

TEST( parser_dom_node_Node_Tests, namespaceMap ) {
    using blogator::parser::specs::infra::resolveNamespace;

    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_HTML );
    ASSERT_NE( node.namespaceMap().getID( resolveNamespace( blogator::parser::specs::infra::Element::HTML5_HTML ) ), NamespaceMap::INVALID );
}

TEST( parser_dom_node_Node_Tests, nodeType_0 ) { //default
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->nodeType(), NodeType::UNDEFINED );
}

TEST( parser_dom_node_Node_Tests, nodeType_1 ) { //with constructor arg set
    auto node = std::make_unique<node::Node>( NodeType::COMMENT_NODE );
    ASSERT_EQ( node->nodeType(), NodeType::COMMENT_NODE );
}

TEST( parser_dom_node_Node_Tests, nodeName_0 ) { //generic
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->nodeName(), U"" );
}

TEST( parser_dom_node_Node_Tests, nodeName_1 ) { //CDATASection
    auto node = node::CDATASection( U"" );
    ASSERT_EQ( node.nodeName(), U"#cdata-section" );
}

TEST( parser_dom_node_Node_Tests, nodeName_2 ) { //Document
    auto node = node::Document();
    ASSERT_EQ( node.nodeName(), U"#document" );
}

TEST( parser_dom_node_Node_Tests, nodeName_3 ) { //Text
    auto node = node::Text( U"" );
    ASSERT_EQ( node.nodeName(), U"#text" );
}

TEST( parser_dom_node_Node_Tests, nodeName_4 ) { //DocumentType
    const auto name = DOMString_t( U"testing-name" );
    const auto node = node::DocumentType( name, U"", U"", nullptr, nullptr );
    ASSERT_EQ( node.nodeName(), name );
}

TEST( parser_dom_node_Node_Tests, nodeName_5 ) { //Comment
    auto node = node::Comment( U"" );
    ASSERT_EQ( node.nodeName(), U"#comment" );
}

TEST( parser_dom_node_Node_Tests, nodeName_6 ) { //Element orphaned
    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_SECTION );
    ASSERT_EQ( node.nodeName(), U"section" );
}

TEST( parser_dom_node_Node_Tests, nodeName_7 ) { //Element attached to HTML Document
    auto doc = node::Document();
    auto * node = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SECTION ) );
    ASSERT_EQ( node->nodeName(), U"SECTION" );
}

TEST( parser_dom_node_Node_Tests, baseURI_0 ) { //Document, self
    auto doc = node::Document( U"UTF-8",
                               blogator::parser::specs::infra::ContentType::APPLICATION_XHTML_XML,
                               U"url",
                               "/filepath/to/origin",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.baseURI(), U"url" );
}

TEST( parser_dom_node_Node_Tests, baseURI_1 ) { //self, orphaned
    auto node = node::Text( U"text" );
    ASSERT_EQ( node.baseURI(), U"" );
}

TEST( parser_dom_node_Node_Tests, baseURI_2 ) { //Connected tree
    auto doc = node::Document( U"UTF-8",
                               blogator::parser::specs::infra::ContentType::APPLICATION_XHTML_XML,
                               U"url",
                               "/filepath/to/origin",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );
    auto * element = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_BODY ) );

    ASSERT_EQ( element->baseURI(), U"url" );
}

TEST( parser_dom_node_Node_Tests, isConnected_0 ) { //connected to self
    auto doc = node::Document();
    ASSERT_TRUE( doc.isConnected() );
}

TEST( parser_dom_node_Node_Tests, isConnected_1 ) { //connected to ancestor
    auto   doc   = node::Document();
    auto * child = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_BODY ) );
    ASSERT_TRUE( child->isConnected() );
}

TEST( parser_dom_node_Node_Tests, isConnected_2 ) { //not connected, orphaned
    auto element = node::Element( blogator::parser::specs::infra::Element::HTML5_BODY );
    ASSERT_FALSE( element.isConnected() );
}

TEST( parser_dom_node_Node_Tests, isConnected_3 ) { //not connected, has parent
    auto   element = node::Element( blogator::parser::specs::infra::Element::HTML5_BODY );
    auto * text    = element.appendChild( std::make_unique<node::Text>( U"text" ) );
    ASSERT_FALSE( text->isConnected() );
}

TEST( parser_dom_node_Node_Tests, ownerDocument_0 ) { //has owner Document
    auto v   = std::vector<node::Node *>();
    auto doc = node::Document();

    v.emplace_back( &doc );
    v.emplace_back( doc.appendChild( std::make_unique<node::Comment>( U"comment" ) ) );
    v.emplace_back( doc.appendChild( std::make_unique<node::CDATASection>( U"data" ) ) );
    v.emplace_back( doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_BODY ) ) );
    auto * body = v.back();
    v.emplace_back( body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    v.emplace_back( body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    v.emplace_back( dynamic_cast<node::Element *>( v.back() )->createAttribute( U"attr-name", U"attr-value" ) );

    //             [Document]
    //            /     |     \
    //           /      |      \
    //          /       |       \
    //  [Comment] [CDATASection] [Element:BODY]
    //                            /         \
    //                    [Element:DIV] [Element:DIV]
    //                                        |
    //                               (Attr: "prefix:name")


    for( auto p : v ) {
        ASSERT_EQ( p->ownerDocument(), &doc ) << "Failed on '" << blogator::unicode::utf8::convert( p->nodeName() ) << "'.";
    }
}

TEST( parser_dom_node_Node_Tests, ownerDocument_1 ) { //no owner Document
    auto v      = std::vector<node::Node *>();
    auto parent = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    v.emplace_back( parent.get() );
    v.emplace_back( parent->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) ) );
    auto * div = v.back();
    v.emplace_back( div->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) ) );
    v.emplace_back( div->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_IMG ) ) );
    v.emplace_back( dynamic_cast<node::Element *>( v.back() )->createAttribute( U"prefix", U"name" ) );

    //          [Element:DIV]
    //                |
    //          [Element:DIV]
    //            /      \
    // [Element:SPAN]  [Element:IMG]
    //                       |
    //               (Attr: "prefix:name")


    for( auto p : v ) {
        ASSERT_EQ( p->ownerDocument(), nullptr );
    }
}

TEST( parser_dom_node_Node_Tests, getRootNode_0 ) { //self
    auto root = std::make_unique<node::Node>();
    ASSERT_EQ( root->getRootNode(), root.get() );
}

TEST( parser_dom_node_Node_Tests, getRootNode_2 ) { //parent
    auto   root  = std::make_unique<node::Node>();
    auto * child = root->appendChild( std::make_unique<node::Node>() );
    ASSERT_EQ( child->getRootNode(), root.get() );
}

TEST( parser_dom_node_Node_Tests, getRootNode_3 ) { //grand-parent
    auto   root   = std::make_unique<node::Node>();
    auto * child  = root->appendChild( std::make_unique<node::Node>() );
    auto * gchild = child->appendChild( std::make_unique<node::Node>() );
    ASSERT_EQ( gchild->getRootNode(), root.get() );
}

TEST( parser_dom_node_Node_Tests, parentNode_null ) {
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->parentNode(), nullptr );
}

TEST( parser_dom_node_Node_Tests, parentNode_not_null ) {
    auto parent = std::make_unique<node::Node>();
    auto node   = std::make_unique<node::Node>( NodeType::UNDEFINED, parent.get(), nullptr );
    ASSERT_EQ( node->parentNode(), parent.get() );
}

TEST( parser_dom_node_Node_Tests, parentElement_0 ) { //Element parent
    auto   element = node::Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * attr    = element.createAttribute( U"name" );
    ASSERT_EQ( attr->parentElement(), &element );
}

TEST( parser_dom_node_Node_Tests, parentElement_1 ) { //non Element parent
    auto document = node::Document();
    auto element  = document.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    ASSERT_EQ( element->parentElement(), nullptr );
}

TEST( parser_dom_node_Node_Tests, parentElement_2 ) { //orphaned node
    auto element = node::Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_EQ( element.parentElement(), nullptr );
}

TEST( parser_dom_node_Node_Tests, hasChildNodes ) {
    auto node = std::make_unique<node::Node>();
    ASSERT_FALSE( node->hasChildNodes() );
    node->appendChild( std::make_unique<node::Node>() );
    ASSERT_TRUE( node->hasChildNodes() );
}

TEST( parser_dom_node_Node_Tests, childNodes ) {
    auto    node = std::make_unique<node::Node>();
    auto * child = node->appendChild( std::make_unique<node::Node>() );
    ASSERT_EQ( node->childNodes().size(), 1 );
    ASSERT_NE( node->childNodes()[0], nullptr );
    ASSERT_EQ( node->childNodes()[0].get(), child );
}

TEST( parser_dom_node_Node_Tests, firstChild ) {
    auto    node  = std::make_unique<node::Node>();
    ASSERT_EQ( node->firstChild(), nullptr );
    auto * child1 = node->appendChild( std::make_unique<node::Node>() );
    ASSERT_NE( node->firstChild(), nullptr );
    ASSERT_EQ( node->firstChild(), child1 );
    std::ignore = node->appendChild( std::make_unique<node::Node>() );
    ASSERT_NE( node->firstChild(), nullptr );
    ASSERT_EQ( node->firstChild(), child1 );
}

TEST( parser_dom_node_Node_Tests, lastChild ) {
    auto    node  = std::make_unique<node::Node>();
    ASSERT_EQ( node->lastChild(), nullptr );
    auto * child1 = node->appendChild( std::make_unique<node::Node>() );
    ASSERT_NE( node->lastChild(), nullptr );
    ASSERT_EQ( node->lastChild(), child1 );
    auto * child2 = node->appendChild( std::make_unique<node::Node>() );
    ASSERT_NE( node->lastChild(), nullptr );
    ASSERT_EQ( node->lastChild(), child2 );
}

TEST( parser_dom_node_Node_Tests, previousSibling_null ) {
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->previousSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, previousSibling_not_null ) {
    auto sibling = std::make_unique<node::Node>();
    auto node    = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, sibling.get() );
    ASSERT_EQ( node->previousSibling(), sibling.get() );
}

TEST( parser_dom_node_Node_Tests, nextSibling_null ) {
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, nextSibling_not_null ) {
    auto sibling = std::make_unique<node::Node>();
    auto node    = std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr, sibling.get() );
    ASSERT_EQ( node->nextSibling(), sibling.get() );
}

TEST( parser_dom_node_Node_Tests, nodeValue ) {
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->nodeValue(), nullptr );
}

TEST( parser_dom_node_Node_Tests, textContent_0 ) { //Attr node
    auto node = std::make_unique<node::Attr>( U"", U"prefix", U"name", U"value" );
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"value" );
}

TEST( parser_dom_node_Node_Tests, textContent_1 ) { //CDATASection node
    auto node = std::make_unique<node::CDATASection>( U"cdata" );
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"cdata" );
}

TEST( parser_dom_node_Node_Tests, textContent_2 ) { //Comment node
    auto node = std::make_unique<node::Comment>( U"comment" );
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"comment" );
}

TEST( parser_dom_node_Node_Tests, textContent_3 ) { //Document node
    auto node = std::make_unique<node::Document>();
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"" );
}

TEST( parser_dom_node_Node_Tests, textContent_4 ) { //DocumentType node
    auto node = std::make_unique<node::DocumentType>( U"name", U"public-id", U"system-id" );
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"" );
}

TEST( parser_dom_node_Node_Tests, textContent_5 ) { //Text node
    auto node = std::make_unique<node::Text>( U"text" );
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"text" );
}

TEST( parser_dom_node_Node_Tests, textContent_6 ) { //Element node, empty
    auto node = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"" );
}

TEST( parser_dom_node_Node_Tests, textContent_7 ) { //Element node with text content
    auto node = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    node->appendChild( std::make_unique<node::Text>( U"Hello, " ) );
    node->appendChild( std::make_unique<node::Text>( U"world!" ) );
    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"Hello, world!" );
}

TEST( parser_dom_node_Node_Tests, textContent_8 ) { //Element node with hierarchy of text content
    auto   node   = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    node->appendChild( std::make_unique<node::Text>( U"H" ) );
    auto * el1 = node->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    el1->appendChild( std::make_unique<node::Text>( U"ello" ) );
    el1->appendChild( std::make_unique<node::Text>( U", " ) );
    node->appendChild( std::make_unique<node::Text>( U"w" ) );
    auto * el2 = node->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    el2->appendChild( std::make_unique<node::Text>( U"orld" ) );
    el2->appendChild( std::make_unique<node::Text>( U"!" ) );

    ASSERT_EQ( dynamic_cast<node::Node *>( node.get() )->textContent(), U"Hello, world!" );
}

TEST( parser_dom_node_Node_Tests, textContent_9 ) { //DOM tree with various nodes
    auto doc = node::Document();
    auto * comment = doc.appendChild( std::make_unique<node::Comment>( U"comment" ) );
    auto * cdata   = doc.appendChild( std::make_unique<node::CDATASection>( U"cdata" ) );
    auto * body    = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_BODY ) );
    auto * div1    = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    div1->appendChild( std::make_unique<node::Text>( U"A" ) );
    div1->appendChild( std::make_unique<node::Text>( U"B" ) );
    auto * div2    = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * attr    = dynamic_cast<node::Element *>( div2 )->createAttribute( U"name", U"value" );
    div2->appendChild( std::make_unique<node::Text>( U"-a-" ) );
    auto * div3    = div2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * span    = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    span->appendChild( std::make_unique<node::Text>( U"span" ) );
    div3->appendChild( std::make_unique<node::Text>( U"div3" ) );
    div2->appendChild( std::make_unique<node::Text>( U"" ) );
    div2->appendChild( std::make_unique<node::Text>( U"-b-" ) );

    //  [Document] ---> [Comment]
    //              |-> [CDATASection]
    //              `-> [Element:BODY] ---> [Element:DIV] ---> [Text:'A']
    //                                  |      (*div1)     `-> [Text:'B']
    //                                  `-> [Element:DIV] ---> (Attr: "name: value")
    //                                         (*div2)     |-> [Text:'-a-']
    //                                                     |-> [Element:DIV] ---> [Element:SPAN] --> [Text:'span']
    //                                                     |      (*div3)     `-> [Text:'div3']
    //                                                     |-> [Text:'']
    //                                                     `-> [Text:'-b-']

    ASSERT_EQ( doc.textContent(), U"" );
    ASSERT_EQ( comment->textContent(), U"comment" );
    ASSERT_EQ( cdata->textContent(), U"cdata" );
    ASSERT_EQ( div1->textContent(), U"AB" );;
    ASSERT_EQ( attr->textContent(), U"value" );
    ASSERT_EQ( div3->textContent(), U"spandiv3" );
    ASSERT_EQ( div2->textContent(), U"-a-spandiv3-b-" );
}

TEST( parser_dom_node_Node_Tests, length_0 ) { //generic
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->length(), 0 );
    node->appendChild( std::make_unique<node::Node>() );
    ASSERT_EQ( node->length(), 1 );
}

TEST( parser_dom_node_Node_Tests, length_1 ) { //Element
    auto node = blogator::parser::dom::node::Element(
        blogator::parser::specs::infra::to_namespaceURI(
            blogator::parser::specs::infra::Namespace::HTML5
        ),
        U"div"
    );
    ASSERT_EQ( node.length(), node.childNodes().size() );
}

TEST( parser_dom_node_Node_Tests, length_2 ) { //DocumentType
    const auto node = node::DocumentType( U"name", U"pid", U"sid", nullptr, nullptr );
    ASSERT_EQ( node.length(), 0 );
}

TEST( parser_dom_node_Node_Tests, length_3 ) { //CharacterData
    auto txt  = DOMString_t( U"comment string" );
    auto node = node::CharacterData( NodeType::TEXT_NODE, txt );

    ASSERT_EQ( node.length(), txt.length() );
}

TEST( parser_dom_node_Node_Tests, length_4 ) { //Attr
    const auto name  = DOMString_t( U"test-name" );
    const auto value = DOMString_t( U"test-value" );
    const auto node  = node::Attr( name, value );

    ASSERT_EQ( node.length(), 0 );
}

TEST( parser_dom_node_Node_Tests, length_5 ) { //Document
    auto node = node::Document();
    ASSERT_EQ( node.length(), node.childNodes().size() );
}

TEST( parser_dom_node_Node_Tests, normalize_0 ) { //contiguous Comment with length > 0
    auto doc = node::Document();
    doc.appendChild( std::make_unique<node::Comment>( U"a" ) );
    doc.appendChild( std::make_unique<node::Comment>( U"b" ) );
    doc.appendChild( std::make_unique<node::Comment>( U"c" ) );
    ASSERT_EQ( doc.childNodes().size(), 3 );

    doc.normalize();
    ASSERT_EQ( doc.childNodes().size(), 1 );
    ASSERT_NE( doc.childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( *doc.childNodes()[0]->nodeValue(), U"abc" );
}

TEST( parser_dom_node_Node_Tests, normalize_1 ) { //contiguous Comment with mix of empty and length > 0
    auto element = node::Document();
    element.appendChild( std::make_unique<node::Comment>( U"" ) );
    element.appendChild( std::make_unique<node::Comment>( U"a" ) );
    element.appendChild( std::make_unique<node::Comment>( U"" ) );
    element.appendChild( std::make_unique<node::Comment>( U"b" ) );
    element.appendChild( std::make_unique<node::Comment>( U"" ) );
    element.appendChild( std::make_unique<node::Comment>( U"c" ) );
    element.appendChild( std::make_unique<node::Comment>( U"" ) );
    ASSERT_EQ( element.childNodes().size(), 7 );

    element.normalize();
    ASSERT_EQ( element.childNodes().size(), 1 );
    ASSERT_NE( element.childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( *element.childNodes()[0]->nodeValue(), U"abc" );
}

TEST( parser_dom_node_Node_Tests, normalize_2 ) { //Mix of contiguous Comment and Text with length > 0
    auto element = node::Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    element.appendChild( std::make_unique<node::Comment>( U"a" ) );
    element.appendChild( std::make_unique<node::Comment>( U"b" ) );
    element.appendChild( std::make_unique<node::Text>( U"A" ) );
    element.appendChild( std::make_unique<node::Text>( U"B" ) );
    element.appendChild( std::make_unique<node::Comment>( U"c" ) );
    element.appendChild( std::make_unique<node::Text>( U"C" ) );
    element.appendChild( std::make_unique<node::Text>( U"D" ) );
    ASSERT_EQ( element.childNodes().size(), 7 );

    element.normalize();
    ASSERT_EQ( element.childNodes().size(), 4 );
    ASSERT_NE( element.childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( element.childNodes()[0]->nodeType(), NodeType::COMMENT_NODE );
    ASSERT_EQ( *element.childNodes()[0]->nodeValue(), U"ab" );
    ASSERT_NE( element.childNodes()[1]->nodeValue(), nullptr );
    ASSERT_EQ( element.childNodes()[1]->nodeType(), NodeType::TEXT_NODE );
    ASSERT_EQ( *element.childNodes()[1]->nodeValue(), U"AB" );
    ASSERT_NE( element.childNodes()[2]->nodeValue(), nullptr );
    ASSERT_EQ( element.childNodes()[2]->nodeType(), NodeType::COMMENT_NODE );
    ASSERT_EQ( *element.childNodes()[2]->nodeValue(), U"c" );
    ASSERT_NE( element.childNodes()[3]->nodeValue(), nullptr );
    ASSERT_EQ( element.childNodes()[3]->nodeType(), NodeType::TEXT_NODE );
    ASSERT_EQ( *element.childNodes()[3]->nodeValue(), U"CD" );
}

TEST( parser_dom_node_Node_Tests, normalize_3 ) { //Tree with mix of nodes
    auto doc = node::Document();
    doc.appendChild( std::make_unique<node::CDATASection>( U"CDATA A" ) );
    doc.appendChild( std::make_unique<node::CDATASection>( U"CDATA B" ) );
    doc.appendChild( std::make_unique<node::Comment>( U"doc" ) );
    doc.appendChild( std::make_unique<node::Comment>( U"-" ) );
    doc.appendChild( std::make_unique<node::Comment>( U"comment" ) );
    auto * element = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    element->appendChild( std::make_unique<node::Text>( U"te" ) );
    element->appendChild( std::make_unique<node::Text>( U"xt" ) );
    auto * child = element->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    child->appendChild( std::make_unique<node::Text>( U"Hello, " ) );
    child->appendChild( std::make_unique<node::Text>( U"" ) );
    child->appendChild( std::make_unique<node::Text>( U"world!" ) );
    child->appendChild( std::make_unique<node::Comment>( U"comment" ) );

    //sanity pre-checks
    ASSERT_EQ( doc.childNodes().size(), 6 );
    ASSERT_EQ( element->childNodes().size(), 3 );
    ASSERT_EQ( child->childNodes().size(), 4 );

    doc.normalize();

    //post-checks
    ASSERT_EQ( doc.childNodes().size(), 4 );
    ASSERT_NE( doc.childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( doc.childNodes()[0]->nodeType(), NodeType::CDATA_SECTION_NODE );
    ASSERT_EQ( *doc.childNodes()[0]->nodeValue(), U"CDATA A" );
    ASSERT_NE( doc.childNodes()[1]->nodeValue(), nullptr );
    ASSERT_EQ( doc.childNodes()[1]->nodeType(), NodeType::CDATA_SECTION_NODE );
    ASSERT_EQ( *doc.childNodes()[1]->nodeValue(), U"CDATA B" );
    ASSERT_NE( doc.childNodes()[2]->nodeValue(), nullptr );
    ASSERT_EQ( doc.childNodes()[2]->nodeType(), NodeType::COMMENT_NODE );
    ASSERT_EQ( *doc.childNodes()[2]->nodeValue(), U"doc-comment" );
    ASSERT_EQ( doc.childNodes()[3]->nodeValue(), nullptr );
    ASSERT_EQ( doc.childNodes()[3]->nodeType(), NodeType::ELEMENT_NODE );

    ASSERT_EQ( element->childNodes().size(), 2 );
    ASSERT_NE( element->childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( element->childNodes()[0]->nodeType(), NodeType::TEXT_NODE );
    ASSERT_EQ( *element->childNodes()[0]->nodeValue(), U"text" );
    ASSERT_EQ( element->childNodes()[1]->nodeValue(), nullptr );
    ASSERT_EQ( element->childNodes()[1]->nodeType(), NodeType::ELEMENT_NODE );

    ASSERT_EQ( child->childNodes().size(), 2 );
    ASSERT_NE( child->childNodes()[0]->nodeValue(), nullptr );
    ASSERT_EQ( child->childNodes()[0]->nodeType(), NodeType::TEXT_NODE );
    ASSERT_EQ( *child->childNodes()[0]->nodeValue(), U"Hello, world!" );
    ASSERT_NE( child->childNodes()[1]->nodeValue(), nullptr );
    ASSERT_EQ( child->childNodes()[1]->nodeType(), NodeType::COMMENT_NODE );
    ASSERT_EQ( *child->childNodes()[1]->nodeValue(), U"comment" );
}

TEST( parser_dom_node_Node_Tests, normalize_fail_0 ) { //contiguous CDATASection with length > 0
    auto doc = node::Document();
    doc.appendChild( std::make_unique<node::CDATASection>( U"a" ) );
    doc.appendChild( std::make_unique<node::CDATASection>( U"b" ) );
    doc.appendChild( std::make_unique<node::CDATASection>( U"c" ) );
    ASSERT_EQ( doc.childNodes().size(), 3 );

    doc.normalize(); //no effect as CDATASection is not an 'exclusive Text node'
    ASSERT_EQ( doc.childNodes().size(), 3 );
}

TEST( parser_dom_node_Node_Tests, copy_constructor ) {
    auto original = node::Node( NodeType::DOCUMENT_NODE );
    original.appendChild( std::make_unique<node::Node>( NodeType::DOCUMENT_TYPE_NODE ) );
    original.appendChild( std::make_unique<node::Node>( NodeType::COMMENT_NODE ) );
    auto * child = original.appendChild( std::make_unique<node::Node>( NodeType::ELEMENT_NODE ) );
    child->appendChild( std::make_unique<node::Node>( NodeType::TEXT_NODE ) );

    //[Document]
    //  |-[DocumentType]
    //  |-[Comment]
    //  |-[Element]
    //      |-[Text]

    auto copy = original; //copy-constructor

    ASSERT_EQ( original.length(), 3 );
    ASSERT_EQ( copy.length(), original.length() );

    auto * origi_child_it = original.firstChild();
    auto * copy_child_it = copy.firstChild();

    for( size_t i = 0; i < original.length(); ++i ) {
        ASSERT_EQ( origi_child_it->nodeType(), copy_child_it->nodeType() );
        ASSERT_NE( origi_child_it, copy_child_it ); //make sure ptr addresses are different

        if( i < original.length() - 1 ) {
            origi_child_it = origi_child_it->nextSibling();
            copy_child_it = copy_child_it->nextSibling();
        }
    }

    ASSERT_EQ( origi_child_it->length(), 1 );
    ASSERT_EQ( origi_child_it->length(), copy_child_it->length() );
    ASSERT_EQ( origi_child_it->firstChild()->nodeType(), copy_child_it->firstChild()->nodeType() );
}

TEST( parser_dom_node_Node_Tests, move_constructor ) {
    auto original = node::Node( NodeType::DOCUMENT_NODE );
    original.appendChild( std::make_unique<node::Node>( NodeType::DOCUMENT_TYPE_NODE ) );
    original.appendChild( std::make_unique<node::Node>( NodeType::COMMENT_NODE ) );
    auto * child = original.appendChild( std::make_unique<node::Node>( NodeType::ELEMENT_NODE ) );
    child->appendChild( std::make_unique<node::Node>( NodeType::TEXT_NODE ) );

    //[Document]
    //  |-[DocumentType]
    //  |-[Comment]
    //  |-[Element]
    //      |-[Text]

    auto length = original.length();
    auto moved  = std::move( original ); //move-constructor

    ASSERT_EQ( moved.length(), length );
    ASSERT_EQ( moved.childNodes().size(), 3 );

    ASSERT_EQ( moved.childNodes()[0]->nodeType(), NodeType::DOCUMENT_TYPE_NODE );
    ASSERT_EQ( moved.childNodes()[0]->parentNode(), &moved );
    ASSERT_EQ( moved.childNodes()[0]->childNodes().size(), 0 );
    ASSERT_EQ( moved.childNodes()[1]->nodeType(), NodeType::COMMENT_NODE );
    ASSERT_EQ( moved.childNodes()[1]->parentNode(), &moved );
    ASSERT_EQ( moved.childNodes()[1]->childNodes().size(), 0 );
    ASSERT_EQ( moved.childNodes()[2]->nodeType(), NodeType::ELEMENT_NODE );
    ASSERT_EQ( moved.childNodes()[2]->parentNode(), &moved );
    ASSERT_EQ( moved.childNodes()[2]->childNodes().size(), 1 );
    ASSERT_EQ( moved.childNodes()[2].get(), child );
}

TEST( parser_dom_node_Node_Tests, copy_assignment ) {
    auto original = node::Node( NodeType::DOCUMENT_NODE );
    original.appendChild( std::make_unique<node::Node>( NodeType::DOCUMENT_TYPE_NODE ) );
    original.appendChild( std::make_unique<node::Node>( NodeType::COMMENT_NODE ) );
    auto * child = original.appendChild( std::make_unique<node::Node>( NodeType::ELEMENT_NODE ) );
    child->appendChild( std::make_unique<node::Node>( NodeType::TEXT_NODE ) );

    //[Document]
    //  |-[DocumentType]
    //  |-[Comment]
    //  |-[Element]
    //      |-[Text]

    node::Node copy;
    copy = original; //copy-operator

    ASSERT_EQ( original.length(), 3 );
    ASSERT_EQ( copy.length(), original.length() );

    auto * origi_child_it = original.firstChild();
    auto * copy_child_it = copy.firstChild();

    for( size_t i = 0; i < original.length(); ++i ) {
        ASSERT_EQ( origi_child_it->nodeType(), copy_child_it->nodeType() );
        ASSERT_NE( origi_child_it, copy_child_it ); //make sure ptr addresses are different

        if( i < original.length() - 1 ) {
            origi_child_it = origi_child_it->nextSibling();
            copy_child_it = copy_child_it->nextSibling();
        }
    }

    ASSERT_EQ( origi_child_it->length(), 1 );
    ASSERT_EQ( origi_child_it->length(), copy_child_it->length() );
    ASSERT_EQ( origi_child_it->firstChild()->nodeType(), copy_child_it->firstChild()->nodeType() );
}

TEST( parser_dom_node_Node_Tests, move_assignment ) {
    auto original = node::Node( NodeType::DOCUMENT_NODE );
    original.appendChild( std::make_unique<node::Node>( NodeType::DOCUMENT_TYPE_NODE ) );
    original.appendChild( std::make_unique<node::Node>( NodeType::COMMENT_NODE ) );
    auto * child = original.appendChild( std::make_unique<node::Node>( NodeType::ELEMENT_NODE ) );
    child->appendChild( std::make_unique<node::Node>( NodeType::TEXT_NODE ) );

    //[Document]
    //  |-[DocumentType]
    //  |-[Comment]
    //  |-[Element]
    //      |-[Text]

    auto length = original.length();
    node::Node moved;
    moved = std::move( original ); //move-operator

    ASSERT_EQ( moved.length(), length );
    ASSERT_EQ( moved.childNodes().size(), 3 );

    ASSERT_EQ( moved.childNodes()[0]->nodeType(), NodeType::DOCUMENT_TYPE_NODE );
    ASSERT_EQ( moved.childNodes()[0]->parentNode(), &moved );
    ASSERT_EQ( moved.childNodes()[0]->childNodes().size(), 0 );
    ASSERT_EQ( moved.childNodes()[1]->nodeType(), NodeType::COMMENT_NODE );
    ASSERT_EQ( moved.childNodes()[1]->parentNode(), &moved );
    ASSERT_EQ( moved.childNodes()[1]->childNodes().size(), 0 );
    ASSERT_EQ( moved.childNodes()[2]->nodeType(), NodeType::ELEMENT_NODE );
    ASSERT_EQ( moved.childNodes()[2]->parentNode(), &moved );
    ASSERT_EQ( moved.childNodes()[2]->childNodes().size(), 1 );
    ASSERT_EQ( moved.childNodes()[2].get(), child );
}

TEST( parser_dom_node_Node_Tests, cloneNode_0 ) { //Element with Attr nodes
    const auto attr1_name   = DOMString_t( U"attr1-name" );
    const auto attr1_value  = DOMString_t( U"attr1-value" );
    const auto attr2_name   = DOMString_t( U"attr2-name" );
    const auto attr2_value  = DOMString_t( U"attr2-value" );
    auto       element      = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto *     attr1        = dynamic_cast<node::Element *>( element.get() )->createAttribute( attr1_name, attr1_value );
    auto *     attr2        = dynamic_cast<node::Element *>( element.get() )->createAttribute( attr2_name, attr2_value );

    ASSERT_EQ( element->attributes().length(), 2 );

    auto clone = element->cloneNode( true );

    ASSERT_EQ( clone->nodeType(), element->nodeType() );
    const auto * recast_clone = dynamic_cast<node::Element *>( clone.get() );
    ASSERT_EQ( recast_clone->tagName(), U"DIV" );
    ASSERT_EQ( recast_clone->attributes().length(), element->attributes().length() );

    const auto * cloned_attr1 = recast_clone->attributes().list()[0].get();
    ASSERT_NE( cloned_attr1, nullptr );
    ASSERT_EQ( cloned_attr1->nodeType(), attr1->nodeType() );
    ASSERT_EQ( cloned_attr1->parentNode(), clone.get() );
    ASSERT_EQ( cloned_attr1->prefix(), attr1->prefix() );
    ASSERT_EQ( cloned_attr1->name(), attr1->name() );
    ASSERT_EQ( cloned_attr1->hasValue(), attr1->hasValue() );
    ASSERT_EQ( *cloned_attr1->value(), *attr1->value() );

    const auto * cloned_attr2 = recast_clone->attributes().list()[1].get();
    ASSERT_NE( cloned_attr2, nullptr );
    ASSERT_EQ( cloned_attr2->nodeType(), attr2->nodeType() );
    ASSERT_EQ( cloned_attr2->parentNode(), clone.get() );
    ASSERT_EQ( cloned_attr2->prefix(), attr2->prefix() );
    ASSERT_EQ( cloned_attr2->name(), attr2->name() );
    ASSERT_EQ( cloned_attr2->hasValue(), attr2->hasValue() );
    ASSERT_EQ( *cloned_attr2->value(), *attr2->value() );
}

TEST( parser_dom_node_Node_Tests, cloneNode_1 ) { //Element with CharacterData nodes
    const auto cdata_str    = DOMString_t( U"cdata" );
    const auto text_str     = DOMString_t( U"text" );
    const auto comment_str  = DOMString_t( U"comment" );
    const auto span_txt_str = DOMString_t( U"span-txt" );

    auto   element  = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * cdata    = element->appendChild( std::make_unique<node::CDATASection>( cdata_str ) );
    auto * text     = element->appendChild( std::make_unique<node::Text>( text_str ) );
    auto * comment  = element->appendChild( std::make_unique<node::Comment>( comment_str ) );
    auto * span     = element->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    auto * span_txt = span->appendChild( std::make_unique<node::Text>( span_txt_str ) );

    ASSERT_EQ( element->childNodes().size(), 4 );

    //[Element:DIV]
    //  |-[CDATASection]
    //  |-[Text]
    //  |-[Comment]
    //  |-[Element:SPAN]
    //      |-[Text]

    auto clone = element->cloneNode( true );

    ASSERT_EQ( clone->nodeType(), element->nodeType() );
    const auto * recast_clone = dynamic_cast<node::Element *>( clone.get() );
    ASSERT_EQ( recast_clone->tagName(), U"DIV" );

    ASSERT_EQ( recast_clone->childNodes().size(), element->childNodes().size() );

    for( size_t i = 0; i < element->childNodes().size(); ++i ) {
        ASSERT_EQ( recast_clone->childNodes()[i]->nodeType(), element->childNodes()[i]->nodeType() );

        if( element->childNodes()[i]->nodeType() == blogator::parser::dom::NodeType::CDATA_SECTION_NODE ||
            element->childNodes()[i]->nodeType() == blogator::parser::dom::NodeType::COMMENT_NODE ||
            element->childNodes()[i]->nodeType() == blogator::parser::dom::NodeType::TEXT_NODE )
        {
            ASSERT_NE( recast_clone->childNodes()[i]->nodeValue(), element->childNodes()[i]->nodeValue() ); //ptr to data should not be the same
            ASSERT_EQ( *recast_clone->childNodes()[i]->nodeValue(), *element->childNodes()[i]->nodeValue() );
        }
    }

    ASSERT_EQ( recast_clone->childNodes()[3]->nodeType(), span->nodeType() );
    auto * cloned_span = dynamic_cast<node::Element *>( recast_clone->childNodes()[3].get() );
    ASSERT_EQ( cloned_span->tagName(), U"SPAN" );
    ASSERT_EQ( span->childNodes().size(), 1 );
    ASSERT_EQ( span->childNodes().size(), cloned_span->childNodes().size() );
    ASSERT_EQ( *span->childNodes()[0]->nodeValue(), *cloned_span->childNodes()[0]->nodeValue() );
}

TEST( parser_dom_node_Node_Tests, cloneNode_2 ) { //Mixed tree from Document root
    auto original = node::Node( NodeType::DOCUMENT_NODE );
    original.appendChild( std::make_unique<node::Node>( NodeType::DOCUMENT_TYPE_NODE ) );
    original.appendChild( std::make_unique<node::Node>( NodeType::COMMENT_NODE ) );
    auto * child = original.appendChild( std::make_unique<node::Node>( NodeType::ELEMENT_NODE ) );
    child->appendChild( std::make_unique<node::Node>( NodeType::ATTRIBUTE_NODE ) );

    //[Document]
    //  |-[DocumentType]
    //  |-[Comment]
    //  |-[Element]
    //      |-[Attr]

    auto clone = original.cloneNode( true );

    ASSERT_EQ( original.length(), 3 );
    ASSERT_EQ( clone->length(), original.length() );

    auto * origi_child_it = original.firstChild();
    auto * clone_child_it = clone->firstChild();

    for( size_t i = 0; i < original.length(); ++i ) {
        ASSERT_EQ( origi_child_it->nodeType(), clone_child_it->nodeType() );
        ASSERT_NE( origi_child_it, clone_child_it ); //make sure ptr addresses are different

        if( i < original.length() - 1 ) {
            origi_child_it = origi_child_it->nextSibling();
            clone_child_it = clone_child_it->nextSibling();
        }
    }

    ASSERT_EQ( origi_child_it->length(), 1 );
    ASSERT_EQ( origi_child_it->length(), clone_child_it->length() );
    ASSERT_EQ( origi_child_it->firstChild()->nodeType(), clone_child_it->firstChild()->nodeType() );
}

TEST( parser_dom_node_Node_Tests, cloneNode_3 ) { //full DOM tree
    std::unique_ptr<node::Node> root;
    auto v = blogator::tests::createTestDOM_1( root );

    auto clone = root->cloneNode( true );

    auto it1     = root->cbegin();
    auto it2     = clone->cbegin();
    auto counter = 0;

    while( it1 != root->cend() && it2 != clone->cend() ) {
        ASSERT_EQ( it1->nodeType(), it2->nodeType() );
        ASSERT_EQ( it1->nodeName(), it2->nodeName() );

        if( it1->nodeValue() ) {
            ASSERT_NE( it2->nodeValue(), nullptr );
            ASSERT_EQ( *it1->nodeValue(), *it2->nodeValue() );
        } else {
            ASSERT_EQ( it2->nodeValue(), nullptr );
        }

        ASSERT_EQ( it1->ownerDocument(), root.get() );
        ASSERT_EQ( it2->ownerDocument(), clone.get() );

        ++it1; ++it2; ++counter;
    }

    ASSERT_EQ( it1, root->cend() );
    ASSERT_EQ( it2, clone->cend() );
    ASSERT_EQ( counter, v.size() );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_0 ) { //invalid (null)
    node::Node                  node1 = node::Node( NodeType::UNDEFINED );
    std::unique_ptr<node::Node> node2;

    ASSERT_FALSE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_1 ) { //valid/different interface implementations
    auto                        node1 = node::Node( NodeType::COMMENT_NODE );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Node>( NodeType::DOCUMENT_NODE );

    ASSERT_FALSE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_2 ) { //valid/different number of children
    auto                        node1 = node::Node( NodeType::UNDEFINED );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Node>( NodeType::UNDEFINED );
    node1.appendChild( std::make_unique<node::Node>() );

    ASSERT_FALSE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_3 ) { //valid/different attribute nodes
    auto                        node1 = node::Attr( U"test-name", U"test-value1" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Attr>( U"test-name", U"test-value2" );

    ASSERT_FALSE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_4 ) { //valid/different CDATASection nodes
    auto                        node1 = node::CDATASection( U"test1" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::CDATASection>( U"test2" );

    ASSERT_FALSE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_5 ) { //valid/different Comment nodes
    auto                        node1 = node::Comment( U"test1" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Comment>( U"test2" );

    ASSERT_FALSE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_6 ) { //valid/different Text nodes
    auto                        node1 = node::Text( U"test1" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Text>( U"test2" );

    ASSERT_FALSE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_7 ) { //valid/different DocumentType nodes
    auto                        node1 = node::DocumentType( U"test-name", U"public-id", U"system-id_1" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::DocumentType>( U"test-name", U"public-id", U"system-id_2" );

    ASSERT_FALSE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_8 ) { //valid/different Element nodes
    auto                        node1 = node::Element( U"test-namespace", U"test-prefix", U"name_1" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Element>( U"test-namespace", U"test-prefix", U"name_2" );

    ASSERT_FALSE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_9 ) { //valid/equivalent Element nodes with different children
    auto node1 = node::Element( U"test-namespace", U"test-prefix", U"name" );
    auto node2 = node::Element( U"test-namespace", U"test-prefix", U"name" );

    node1.createElement( blogator::parser::specs::infra::Element::HTML5_SPAN );
    node2.createElement( blogator::parser::specs::infra::Element::HTML5_H1 );

    ASSERT_FALSE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_fail_10 ) { //valid/equivalent Document nodes with different children
    auto node1 = node::Document();
    node1.appendChild( std::make_unique<node::CDATASection>( U"data" ) );
    node1.appendChild( std::make_unique<node::Element>( U"namespace", U"prefix-a", U"name") );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Document>();
    node2->appendChild( std::make_unique<node::CDATASection>( U"data" ) );
    node2->appendChild( std::make_unique<node::Element>( U"namespace", U"prefix-b", U"name") );

    ASSERT_FALSE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_0 ) { //valid and same node
    std::unique_ptr<node::Node> node1 = std::make_unique<node::Node>( NodeType::UNDEFINED );
    node::Node *                node2 = node1.get();

    ASSERT_TRUE( node2->isEqualNode( node1 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_1 ) { //valid/equivalent Attr
    auto                        node1 = node::Attr( U"test-name", U"test-value" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Attr>( U"test-name", U"test-value" );

    ASSERT_TRUE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_2 ) { //valid/equivalent CDATASection nodes
    auto                        node1 = node::CDATASection( U"test" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::CDATASection>( U"test" );

    ASSERT_TRUE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_3 ) { //valid/equivalent Comment nodes
    auto                        node1 = node::Comment( U"test" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Comment>( U"test" );

    ASSERT_TRUE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_4 ) { //valid/equivalent Text nodes
    auto                        node1 = node::Text( U"test" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Text>( U"test" );

    ASSERT_TRUE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_5 ) { //valid/equivalent DocumentType nodes
    auto                        node1 = node::DocumentType( U"test-name", U"public-id", U"system-id" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::DocumentType>( U"test-name", U"public-id", U"system-id" );

    ASSERT_TRUE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_6 ) { //valid/equivalent Element nodes
    auto                        node1 = node::Element( U"test-namespace", U"test-prefix", U"name" );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Element>( U"test-namespace", U"test-prefix", U"name" );

    ASSERT_TRUE( node1.isEqualNode( *node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_7 ) { //valid/equivalent Element nodes with equivalent children
    auto node1 = node::Element( U"test-namespace", U"test-prefix", U"name" );
    auto node2 = node::Element( U"test-namespace", U"test-prefix", U"name" );

    node1.createElement( blogator::parser::specs::infra::Element::HTML5_SPAN );
    node2.createElement( blogator::parser::specs::infra::Element::HTML5_SPAN );

    ASSERT_TRUE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_8 ) { //valid/equivalent Element nodes with equivalent attributes
    auto node1 = node::Element( U"test-namespace", U"test-prefix", U"name" );
    auto node2 = node::Element( U"test-namespace", U"test-prefix", U"name" );

    node1.createAttribute( U"name", U"value" );
    node2.createAttribute( U"name", U"value" );

    ASSERT_TRUE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_9 ) { //valid/equivalent Element nodes with different attributes
    auto node1 = node::Element( U"test-namespace", U"test-prefix", U"name" );
    auto node2 = node::Element( U"test-namespace", U"test-prefix", U"name" );

    node1.createAttribute( U"name", U"value1" );
    node2.createAttribute( U"name", U"value2" );

    ASSERT_TRUE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isEqualNode_same_10 ) { //valid/equivalent Document nodes with equivalent children
    auto node1 = node::Document();
    node1.appendChild( std::make_unique<node::CDATASection>( U"data" ) );
    node1.appendChild( std::make_unique<node::Element>( U"namespace", U"prefix", U"name") );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Document>();
    node2->appendChild( std::make_unique<node::CDATASection>( U"data" ) );
    node2->appendChild( std::make_unique<node::Element>( U"namespace", U"prefix", U"name") );

    ASSERT_TRUE( node1.isEqualNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isSameNode_null ) { //invalid
    node::Node                  node1 = node::Node( NodeType::UNDEFINED );
    std::unique_ptr<node::Node> node2;

    ASSERT_FALSE( node1.isSameNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isSameNode_not_same ) { //valid/not same
    auto                        node1 = node::Node( NodeType::UNDEFINED );
    std::unique_ptr<node::Node> node2 = std::make_unique<node::Node>( NodeType::UNDEFINED );

    ASSERT_FALSE( node1.isSameNode( node2 ) );
}

TEST( parser_dom_node_Node_Tests, isSameNode_same ) { //valid/same
    std::unique_ptr<node::Node> node1 = std::make_unique<node::Node>( NodeType::UNDEFINED );
    node::Node *                node2 = node1.get();

    ASSERT_TRUE( node2->isSameNode( node1 ) );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_0 ) { //same node
    auto node = std::make_unique<node::Node>();
    ASSERT_EQ( node->compareDocumentPosition( *node ), 0 );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_1 ) { //same node, same attribute
    auto   e1   = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * e1a1 = e1->createAttribute( U"e1a1" );
    ASSERT_EQ( e1a1->compareDocumentPosition( *e1a1 ), 0 );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_2 ) { //same node, attribute comparison preceding
    auto   e1   = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * e1a1 = e1->createAttribute( U"e1a1" );
    auto * e1a2 = e1->createAttribute( U"e1a2" );
    auto * e1a3 = e1->createAttribute( U"e1a3" );

    ASSERT_EQ( e1a3->compareDocumentPosition( *e1a1 ),
               node::Node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC +
               node::Node::DOCUMENT_POSITION_PRECEDING );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_3 ) { //same node, attribute comparison following
    auto   e1   = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * e1a1 = e1->createAttribute( U"e1a1" );
    auto * e1a2 = e1->createAttribute( U"e1a2" );
    auto * e1a3 = e1->createAttribute( U"e1a3" );

    ASSERT_EQ( e1a1->compareDocumentPosition( *e1a3 ),
               node::Node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC +
               node::Node::DOCUMENT_POSITION_FOLLOWING );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_4 ) { //compare against a null node
    auto element   = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto attribute = std::make_unique<node::Attr>( U"prefix", U"name" ); //orphan (i.e.: parent is null)

    ASSERT_EQ( element->compareDocumentPosition( *attribute ),
               node::Node::DOCUMENT_POSITION_DISCONNECTED +
               node::Node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC +
               node::Node::DOCUMENT_POSITION_PRECEDING );

    ASSERT_EQ( attribute->compareDocumentPosition( *element ),
               node::Node::DOCUMENT_POSITION_DISCONNECTED +
               node::Node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC +
               node::Node::DOCUMENT_POSITION_PRECEDING );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_5 ) { //descendant
    auto   e1   = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * e2   = e1->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );

    //[e1] --> [e2]

    ASSERT_EQ( e1->compareDocumentPosition( *e2 ),
               node::Node::DOCUMENT_POSITION_CONTAINED_BY +
               node::Node::DOCUMENT_POSITION_FOLLOWING );

    ASSERT_EQ( e2->compareDocumentPosition( *e1 ),
               node::Node::DOCUMENT_POSITION_CONTAINS +
               node::Node::DOCUMENT_POSITION_PRECEDING );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_6 ) { //descendant via attributes
    auto   e1  = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * e1a = e1->createAttribute( U"e1a1" );
    auto * e2  = e1->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * e2a = dynamic_cast<node::Element *>( e2 )->createAttribute( U"e2a1" );

    //  [e1] --> [e2]
    //   |        |
    // (e1a1)   (e2a1)

    ASSERT_EQ( e1a->compareDocumentPosition( *e2a ),
               node::Node::DOCUMENT_POSITION_CONTAINED_BY +
               node::Node::DOCUMENT_POSITION_FOLLOWING );
    ASSERT_EQ( e2a->compareDocumentPosition( *e1a ),
               node::Node::DOCUMENT_POSITION_CONTAINS +
               node::Node::DOCUMENT_POSITION_PRECEDING );
}

TEST( parser_dom_node_Node_Tests, compareDocumentPosition_7 ) { // both children of same parent
    auto   parent = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * c1     = parent->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * c2     = parent->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * c3     = parent->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );

    ASSERT_EQ( c1->compareDocumentPosition( *c3 ), node::Node::DOCUMENT_POSITION_FOLLOWING );
    ASSERT_EQ( c3->compareDocumentPosition( *c1 ), node::Node::DOCUMENT_POSITION_PRECEDING );
}

TEST( parser_dom_node_Node_Tests, contains_0 ) { //self
    auto node = node::Node();
    ASSERT_TRUE( node.contains( node ) );
}

TEST( parser_dom_node_Node_Tests, contains_1 ) { //child
    auto   node  = node::Node();
    auto * child = node.appendChild( std::make_unique<node::Node>() );
    ASSERT_TRUE( node.contains( *child ) );
}

TEST( parser_dom_node_Node_Tests, contains_2 ) { //grand-child
    auto   node   = node::Node();
    auto * child1 = node.appendChild( std::make_unique<node::Node>() );
    auto * child2 = node.appendChild( std::make_unique<node::Node>() );
    auto * gchild = child2->appendChild( std::make_unique<node::Node>() );
    ASSERT_TRUE( node.contains( *gchild ) );
}

TEST( parser_dom_node_Node_Tests, contains_fail ) {
    auto   node     = node::Node();
    auto * child1   = node.appendChild( std::make_unique<node::Node>() );
    auto * child2   = node.appendChild( std::make_unique<node::Node>() );
    auto * gchild   = child2->appendChild( std::make_unique<node::Node>() );
    auto   look_for = node::Node();

    ASSERT_FALSE( node.contains( look_for ) );
}

TEST( parser_dom_node_Node_Tests, lookupPrefix_0 ) { //Element with declaration in Attribute
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto * r   = doc->documentElement(); //<root>

    ASSERT_EQ( r->lookupPrefix( U"http://www.site.com/xml/undergrad" ), U"ug" );
}

TEST( parser_dom_node_Node_Tests, lookupPrefix_1 ) { //namespaced Element
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto   ug  = doc->documentElement()->firstChild(); //<ug:student>

    ASSERT_EQ( ug->lookupPrefix( U"http://www.site.com/xml/undergrad" ), U"ug" );
}

TEST( parser_dom_node_Node_Tests, lookupPrefix_2 ) { //nested child Element in namespaced Element
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto   tr  = doc->documentElement()->firstChild()->firstChild(); //<tr>

    ASSERT_EQ( tr->lookupPrefix( U"http://www.site.com/xml/undergrad" ), U"ug" );
}

TEST( parser_dom_node_Node_Tests, lookupPrefix_3 ) { //Attribute of nested child Element in namespaced Element
    std::unique_ptr<node::Node> root;
    auto   v    = blogator::tests::createTestDOM_3( root );
    auto * doc  = dynamic_cast<node::Document *>( root.get() );
    auto * attr = dynamic_cast<node::Element *>( doc->documentElement()->firstChild()->firstChild() )->attributes().list()[0].get(); //{class='table'} (i.e.: <tr> attribute)

    ASSERT_EQ( attr->lookupPrefix( U"http://www.site.com/xml/undergrad" ), U"ug" );
}

TEST( parser_dom_node_Node_Tests, lookupPrefix_4 ) { //no namespace prefixed Element
    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_DIV );

    ASSERT_EQ( node.lookupPrefix( U"http://www.site.com/xml/undergrad" ), U"" );
}

TEST( parser_dom_node_Node_Tests, lookupPrefix_5 ) { //Attribute of nested child Element in namespaced Element
    std::unique_ptr<node::Node> root;
    auto   v       = blogator::tests::createTestDOM_3( root );
    auto * doc     = dynamic_cast<node::Document *>( root.get() );
    auto * doctype = doc->doctype();

    ASSERT_EQ( doctype->lookupPrefix( U"http://www.site.com/xml/undergrad" ), U"" );
}

TEST( parser_dom_node_Node_Tests, lookupNamespaceURI_0 ) { //get default NS
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto   tr  = doc->documentElement()->firstChild()->firstChild(); //<tr>

    ASSERT_EQ( tr->lookupNamespaceURI( U"" ), U"http://www.site.com/xml/default" );
}

TEST( parser_dom_node_Node_Tests, lookupNamespaceURI_2 ) { //namespace found via parent Element NS
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto   tr  = doc->documentElement()->firstChild()->firstChild(); //<tr>

    ASSERT_EQ( tr->lookupNamespaceURI( U"ug" ), U"http://www.site.com/xml/undergrad" );
}

TEST( parser_dom_node_Node_Tests, lookupNamespaceURI_3 ) { //URI found via parent attribute ns declaration
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto   el  = doc->documentElement()->firstChild(); //<ug:student>

    ASSERT_EQ( el->lookupNamespaceURI( U"pg" ), U"http://www.site.com/xml/default" ); //since 'xlmns' > 'xmlnx:pg'
}

TEST( parser_dom_node_Node_Tests, isDefaultNamespace_0 ) {
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto   el  = doc->documentElement()->firstChild(); //<ug:student>

    ASSERT_TRUE( el->isDefaultNamespace( U"http://www.site.com/xml/default" ) );
}

TEST( parser_dom_node_Node_Tests, isDefaultNamespace_1 ) {
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );
    auto   el  = doc->documentElement()->firstChild(); //<ug:student>

    ASSERT_FALSE( el->isDefaultNamespace( U"http://www.site.com/xml/postgrad" ) );
}

TEST( parser_dom_node_Node_Tests, insertBefore_0 ) { //insert at end
    auto node = node::Node();
    auto * child = node.insertBefore( std::make_unique<node::Node>(), nullptr );
    ASSERT_FALSE( node.childNodes().empty() );
    ASSERT_EQ( node.childNodes()[0].get(), child );
    ASSERT_EQ( node.childNodes()[0]->parentNode(), &node );
    ASSERT_EQ( node.childNodes()[0]->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, insertBefore_1 ) { //insert before a child
    auto   node       = node::Node();
    auto * child      = node.appendChild( std::make_unique<node::Node>() );
    auto   insert     = std::make_unique<node::Node>();
    auto * insert_ptr = insert.get();
    ASSERT_FALSE( node.childNodes().empty() );
    ASSERT_TRUE( node.childNodes()[0] );
    ASSERT_EQ( node.insertBefore( insert, node.childNodes()[0].get() ), insert_ptr );
    ASSERT_EQ( node.childNodes().size(), 2 );
    ASSERT_EQ( insert_ptr->parentNode(), &node );
    ASSERT_EQ( insert_ptr->previousSibling(), nullptr );
    ASSERT_EQ( insert_ptr->nextSibling(), child );
    ASSERT_EQ( child->previousSibling(), insert_ptr );
    ASSERT_EQ( child->nextSibling(), nullptr);
}

TEST( parser_dom_node_Node_Tests, insertBefore_2 ) { //insert between CDATA and Element in a Document node
    auto   node   = node::Document();
    auto * child1 = node.appendChild( std::make_unique<node::CDATASection>( U"CDATASection" ) );
    auto * child2 = node.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_HEAD ) );

    std::unique_ptr<node::Node> insert     = std::make_unique<node::DocumentType>( U"name", U"public-id", U"system-id" );
    node::Node *                insert_ptr = insert.get();

    ASSERT_EQ( node.childNodes().size(), 2 );
    ASSERT_EQ( node.childNodes()[0].get(), child1 );
    ASSERT_EQ( node.childNodes()[1].get(), child2 );
    ASSERT_EQ( node.insertBefore( insert, node.childNodes()[1].get() ), insert_ptr );
    ASSERT_EQ( node.childNodes().size(), 3 );

    ASSERT_EQ( node.childNodes()[0]->previousSibling(), nullptr );
    ASSERT_EQ( node.childNodes()[0]->nextSibling(), insert_ptr );

    ASSERT_EQ( node.childNodes()[1]->parentNode(), &node );
    ASSERT_EQ( node.childNodes()[1]->nextSibling(), child2 );
    ASSERT_EQ( node.childNodes()[1]->previousSibling(), child1 );

    ASSERT_EQ( node.childNodes()[2]->previousSibling(), insert_ptr );
    ASSERT_EQ( node.childNodes()[2]->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, insertBefore_3 ) { //DocumentFragment elements append at end
    NodePtr_t doc_fragment = std::make_unique<node::DocumentFragment>();
    auto *    frag_child_1 = doc_fragment->appendChild( std::make_unique<node::Element>( U"frag1" ) );
    auto *    frag_child_2 = doc_fragment->appendChild( std::make_unique<node::Element>( U"frag2" ) );

    auto      node         = node::Element( U"parent" );
    auto *    first_child  = node.appendChild( std::make_unique<node::Text>( U"first child" ) );

    //sanity check
    ASSERT_EQ( doc_fragment->childNodes().size(), 2 );
    ASSERT_EQ( node.childNodes().size(), 1 );

    //test
    ASSERT_EQ( node.insertBefore( doc_fragment, nullptr ), frag_child_1 );
    ASSERT_FALSE( doc_fragment );
    ASSERT_EQ( node.childNodes().size(), 3 );

    ASSERT_EQ( first_child->previousSibling(), nullptr );
    ASSERT_EQ( first_child->nextSibling(), frag_child_1 );

    ASSERT_EQ( frag_child_1->parentNode(), &node );
    ASSERT_EQ( frag_child_1->previousSibling(), first_child );
    ASSERT_EQ( frag_child_1->nextSibling(), frag_child_2 );

    ASSERT_EQ( frag_child_2->parentNode(), &node );
    ASSERT_EQ( frag_child_2->previousSibling(), frag_child_1 );
    ASSERT_EQ( frag_child_2->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, insertBefore_4 ) { //DocumentFragment elements between other nodes
    NodePtr_t doc_fragment = std::make_unique<node::DocumentFragment>();
    auto *    frag_child_1 = doc_fragment->appendChild( std::make_unique<node::Element>( U"frag1" ) );
    auto *    frag_child_2 = doc_fragment->appendChild( std::make_unique<node::Element>( U"frag2" ) );

    auto      node         = node::Element( U"parent" );
    auto *    first_child  = node.appendChild( std::make_unique<node::Text>( U"first child" ) );
    auto *    last_child   = node.appendChild( std::make_unique<node::Text>( U"last child" ) );

    //sanity check
    ASSERT_EQ( doc_fragment->childNodes().size(), 2 );
    ASSERT_EQ( node.childNodes().size(), 2 );

    //test
    ASSERT_EQ( node.insertBefore( doc_fragment, last_child ), frag_child_1 );
    ASSERT_FALSE( doc_fragment );
    ASSERT_EQ( node.childNodes().size(), 4 );

    ASSERT_EQ( first_child->previousSibling(), nullptr );
    ASSERT_EQ( first_child->nextSibling(), frag_child_1 );

    ASSERT_EQ( frag_child_1->parentNode(), &node );
    ASSERT_EQ( frag_child_1->previousSibling(), first_child );
    ASSERT_EQ( frag_child_1->nextSibling(), frag_child_2 );

    ASSERT_EQ( frag_child_2->parentNode(), &node );
    ASSERT_EQ( frag_child_2->previousSibling(), frag_child_1 );
    ASSERT_EQ( frag_child_2->nextSibling(), last_child );

    ASSERT_EQ( last_child->previousSibling(), frag_child_2 );
    ASSERT_EQ( last_child->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_0 ) { //null arg
    auto parent = node::Node();
    ASSERT_THROW( parent.insertBefore( nullptr, nullptr ), std::invalid_argument );
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_1 ) { //null unique_ptr
    auto parent       = node::Node();
    std::unique_ptr<node::Node> node;;

    ASSERT_THROW( parent.insertBefore( node, nullptr ), std::invalid_argument );
    ASSERT_THROW( parent.insertBefore( std::move( node ), nullptr ), std::invalid_argument );
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_2 ) { //node is a host-including inclusive ancestor of parent (i.e. already in parent tree)
    auto   parent = node::Node();
    auto * child  = parent.appendChild( std::make_unique<node::Node>() );
    auto * gchild = child->appendChild( std::make_unique<node::Node>() );

    ASSERT_FALSE( child->childNodes().empty() );
    ASSERT_TRUE( child->childNodes()[0] );

    try {
        parent.insertBefore( child->childNodes()[0], nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_3 ) { //child parent in different tree
    auto   parent1          = node::Node();
    auto * legitimate_child = parent1.appendChild( std::make_unique<node::Node>() );
    auto   parent2          = node::Node();
    auto * alien_child      = parent2.appendChild( std::make_unique<node::Node>() );

    try {
        parent1.insertBefore( std::make_unique<node::Node>(), alien_child );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NotFoundError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_4 ) { //insert Attr node into Document
    auto parent = node::Document();

    try {
        parent.insertBefore( std::make_unique<node::Attr>( U"name" ), nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_5 ) { //insert Attr node into Element
    auto parent = node::Element( blogator::parser::specs::infra::Element::HTML5_SPAN );

    try {
        parent.insertBefore( std::make_unique<node::Attr>( U"name" ), nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_6 ) { //Insert into Attr
    auto parent = std::make_unique<node::Attr>( U"prefix", U"name" );
    auto insert = std::make_unique<node::Node>();

    try {
        dynamic_cast<node::Node *>( parent.get() )->insertBefore( insert, nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_7 ) { //insert into CDATASection
    auto parent = std::make_unique<node::CDATASection>( U"data" );
    auto insert = std::make_unique<node::Node>();

    try {
        dynamic_cast<node::Node *>( parent.get() )->insertBefore( insert, nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_8 ) { //insert into Comment
    auto parent = std::make_unique<node::Comment>( U"data" );
    auto insert = std::make_unique<node::Node>();

    try {
        dynamic_cast<node::Node *>( parent.get() )->insertBefore( insert, nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_9 ) { //insert into DocumentType
    auto parent = std::make_unique<node::DocumentType>( U"name", U"public-id", U"system-id" );
    auto insert = std::make_unique<node::Node>();

    try {
        dynamic_cast<node::Node *>( parent.get() )->insertBefore( insert, nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, insertBefore_fail_10 ) { //insert into Text
    auto parent = std::make_unique<node::Text>( U"data" );
    auto insert = std::make_unique<node::Node>();

    try {
        dynamic_cast<node::Node *>( parent.get() )->insertBefore( insert, nullptr );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError )
                            << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, appendChild_0 ) { //[]
    node::Node real_parent = node::Node();
    node::Node * child     = real_parent.appendChild( std::make_unique<node::Node>( NodeType::UNDEFINED ) );

    auto & children = real_parent.childNodes();
    ASSERT_EQ( children.size(), 1 );
    ASSERT_EQ( children.back().get(), child );
    ASSERT_EQ( *children.back(), *child );
    ASSERT_EQ( child->parentNode(), &real_parent );
    ASSERT_EQ( child->previousSibling(), nullptr );
    ASSERT_EQ( child->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, appendChild_1 ) { //[][]
    auto parent = node::Node();
    node::Node * child1 = parent.appendChild( std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr) );
    node::Node * child2 = parent.appendChild( std::make_unique<node::Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr) );

    auto & children = parent.childNodes();
    ASSERT_EQ( children.size(), 2 );
    ASSERT_EQ( children.back().get(), child2 );
    ASSERT_EQ( *children.back(), *child2 );

    ASSERT_EQ( child1->parentNode(), &parent );
    ASSERT_EQ( child1->previousSibling(), nullptr );
    ASSERT_EQ( child1->nextSibling(), child2 );

    ASSERT_EQ( child2->parentNode(), &parent );
    ASSERT_EQ( child2->previousSibling(), child1 );
    ASSERT_EQ( child2->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, appendChild_2 ) { //[][][]
    auto parent = node::Node( NodeType::UNDEFINED );

    ASSERT_EQ( parent.length(), 0 ); //no children

    parent.appendChild( std::make_unique<node::Node>( NodeType::ATTRIBUTE_NODE ) );
    ASSERT_EQ( parent.length(), 1 );
    ASSERT_EQ( parent.lastChild()->nodeType(), NodeType::ATTRIBUTE_NODE );

    parent.appendChild( std::make_unique<node::Node>( NodeType::DOCUMENT_NODE ) );
    ASSERT_EQ( parent.length(), 2 );
    ASSERT_EQ( parent.lastChild()->nodeType(), NodeType::DOCUMENT_NODE );

    parent.appendChild( std::make_unique<node::Node>( NodeType::COMMENT_NODE ) );
    ASSERT_EQ( parent.length(), 3 );
    ASSERT_EQ( parent.lastChild()->nodeType(), NodeType::COMMENT_NODE );

    auto & children = parent.childNodes();
    ASSERT_EQ( children.size(), 3 );

    auto & child1 = children.front();
    ASSERT_EQ( child1->parentNode(), &parent );
    ASSERT_EQ( child1->previousSibling(), nullptr );
    ASSERT_NE( child1->nextSibling(), nullptr );

    auto * child2 = child1->nextSibling();
    auto & child3 = children.back();

    ASSERT_EQ( child2->parentNode(), &parent );
    ASSERT_EQ( child2->previousSibling(), child1.get() );
    ASSERT_EQ( child2->nextSibling(), child3.get() );

    ASSERT_EQ( child3->parentNode(), &parent );
    ASSERT_EQ( child3->previousSibling(), child2 );
    ASSERT_EQ( child3->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, appendChild_3 )  { //append tree
    auto   doc = node::Document();
    auto * el  = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );

    auto   root  = std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto * attr1 = root->createAttribute( U"name1" );
    auto * child = root->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    auto * attr2 = dynamic_cast<node::Element *>( child )->createAttribute( U"name2" );

    //sanity check
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), &doc );
    ASSERT_EQ( root->ownerDocument(), nullptr );
    ASSERT_EQ( root->parentNode(), nullptr );

    auto * ptr = el->appendChild( std::move( root ) );

    ASSERT_EQ( ptr->parentNode(), el );
    ASSERT_EQ( attr1->ownerDocument(), &doc ); //'ptr' attribute
    ASSERT_EQ( attr2->ownerDocument(), &doc ); //'child' attribute

    size_t counter = 0;
    for( auto it = doc.cbegin(); it != doc.cend(); ++it, ++counter ) {
        ASSERT_EQ( it->ownerDocument(), &doc );
    }

    ASSERT_EQ( counter, 4 ); //make sure all nodes were checked
}

TEST( parser_dom_node_Node_Tests, appendChild_fail_0 ) { //null unique_ptr
    auto parent       = node::Node();
    std::unique_ptr<node::Node> node;;

    ASSERT_THROW( parent.appendChild( node ), std::invalid_argument );
    ASSERT_THROW( parent.appendChild( std::move( node ) ), std::invalid_argument );
}

TEST( parser_dom_node_Node_Tests, appendChild_fail_1 ) { //append to Attr
    auto parent = std::make_unique<node::Attr>( U"prefix", U"name" );
    auto node   = std::make_unique<node::Text>( U"text");

    try {
        dynamic_cast<node::Node *>( parent.get() )->appendChild( std::move( node ) );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError )
                            << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, appendChild_fail_2 ) { //append to Text
    auto parent = std::make_unique<node::Text>( U"a" );
    auto node   = std::make_unique<node::Text>( U"b");

    try {
        dynamic_cast<node::Node *>( parent.get() )->appendChild( std::move( node ) );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError )
                            << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, replaceChild_0 ) { //by unique_ptr reference
    auto parent          = node::Node();
    auto * child         = parent.appendChild( std::make_unique<node::Node>() );
    auto replacement     = std::make_unique<node::Node>();
    auto replacement_ptr = replacement.get();

    //sanity check
    ASSERT_EQ( parent.childNodes().size(), 1 );
    ASSERT_EQ( parent.childNodes()[0].get(), child );

    auto removed = parent.replaceChild( replacement, parent.childNodes()[0] );
    ASSERT_TRUE( removed );
    ASSERT_EQ( removed.get(), child );
    ASSERT_FALSE( replacement );
    ASSERT_EQ( parent.childNodes().size(), 1 );
    ASSERT_EQ( parent.childNodes()[0].get(), replacement_ptr );

    ASSERT_EQ( removed->ownerDocument(), nullptr );
    ASSERT_EQ( removed->parentNode(), nullptr );
    ASSERT_EQ( removed->previousSibling(), nullptr );
    ASSERT_EQ( removed->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, replaceChild_1 ) { //by index
    auto node            = node::Node();
    auto * child         = node.appendChild( std::make_unique<node::Node>() );
    auto replacement     = std::make_unique<node::Node>();
    auto replacement_ptr = replacement.get();

    //sanity check
    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );

    auto removed = node.replaceChild( replacement, 0 );
    ASSERT_TRUE( removed );
    ASSERT_EQ( removed.get(), child );
    ASSERT_FALSE( replacement );
    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), replacement_ptr );

    ASSERT_EQ( removed->ownerDocument(), nullptr );
    ASSERT_EQ( removed->parentNode(), nullptr );
    ASSERT_EQ( removed->previousSibling(), nullptr );
    ASSERT_EQ( removed->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, replaceChild_fail_1 ) { //by index
    auto node        = node::Node();
    auto * child     = node.appendChild( std::make_unique<node::Node>() );
    auto replacement = std::make_unique<node::Node>();

    try {
        std::ignore = node.replaceChild( replacement, 1 );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NotFoundError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }

    ASSERT_TRUE( replacement ); //checks it hasn't been touched
}

TEST( parser_dom_node_Node_Tests, replaceChild_fail_2 ) { //null child
    auto parent      = node::Node();
    auto replacement = std::make_unique<node::Node>();
    std::unique_ptr<node::Node> child;

    ASSERT_THROW( std::ignore = parent.replaceChild( replacement, child ), std::invalid_argument );
    ASSERT_TRUE( replacement ); //checks it hasn't been touched
}

TEST( parser_dom_node_Node_Tests, replaceChild_fail_3 ) { //node is a host-including inclusive ancestor of parent (i.e. already in parent tree)
    auto parent  = node::Node();
    auto * child = parent.appendChild( std::make_unique<node::Node>() );

    ASSERT_EQ( parent.childNodes().size(), 1 );

    try {
        std::ignore = parent.replaceChild( parent.childNodes()[0], parent.childNodes()[0] );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, replaceChild_fail_4 ) { //child not in tree
    auto   parent           = node::Node();
    auto * legitimate_child = parent.appendChild( std::make_unique<node::Node>() );
    auto   alien_child      = std::make_unique<node::Node>();
    auto   replacement      = std::make_unique<node::Node>();

    try {
        std::ignore = parent.replaceChild( replacement, alien_child );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NotFoundError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }

    ASSERT_TRUE( replacement ); //checks it hasn't been touched
}

TEST( parser_dom_node_Node_Tests, replaceChild_fail_5 ) { //replace w/ an Attr node
    auto      parent      = node::Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto *    child       = parent.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    NodePtr_t replacement = std::make_unique<node::Attr>( U"prefix", U"name" );

    ASSERT_EQ( parent.childNodes().size(), 1 ); //sanity check

    try {
        std::ignore = parent.replaceChild( replacement, parent.childNodes()[0] );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }

    ASSERT_TRUE( replacement ); //checks it hasn't been touched
}

TEST( parser_dom_node_Node_Tests, replaceChild_fail_6 ) { //replace w/ an Document node
    auto      parent      = node::Element( blogator::parser::specs::infra::Element::HTML5_DIV );
    auto *    child       = parent.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    NodePtr_t replacement = std::make_unique<node::Document>();

    ASSERT_EQ( parent.childNodes().size(), 1 ); //sanity check

    try {
        std::ignore = parent.replaceChild( replacement, parent.childNodes()[0] );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }

    ASSERT_TRUE( replacement ); //checks it hasn't been touched
}

TEST( parser_dom_node_Node_Tests, removeChild_0 ) { //by unique_ptr reference
    auto   node  = node::Node();
    auto * child = node.appendChild( std::make_unique<node::Node>() );

    ASSERT_EQ( node.childNodes().size(), 1 );
    ASSERT_EQ( node.childNodes()[0].get(), child );

    auto removed = node.removeChild( node.childNodes()[0] );
    ASSERT_EQ( node.childNodes().size(), 0 );
    ASSERT_EQ( removed.get(), child );
}

TEST( parser_dom_node_Node_Tests, removeChild_1 ) { //by index
    auto    node = node::Node();
    auto * child = node.appendChild( std::make_unique<node::Node>() );
    auto removed = node.removeChild( 0 );

    ASSERT_FALSE( node.hasChildNodes() );
    ASSERT_EQ( child, removed.get() );
}

TEST( parser_dom_node_Node_Tests, removeChild_2 ) { //[][x][] by unique_ptr reference, check neighbours
    auto   node   = node::Node();
    auto * child1 = node.appendChild( std::make_unique<node::Node>() );
    auto * child2 = node.appendChild( std::make_unique<node::Node>() );
    auto * child3 = node.appendChild( std::make_unique<node::Node>() );

    //sanity check
    ASSERT_EQ( node.childNodes().size(), 3 );
    ASSERT_EQ( child1->previousSibling(), nullptr );
    ASSERT_EQ( child1->nextSibling(), child2 );
    ASSERT_EQ( child2->previousSibling(), child1 );
    ASSERT_EQ( child2->nextSibling(), child3 );
    ASSERT_EQ( child3->previousSibling(), child2 );
    ASSERT_EQ( child3->nextSibling(), nullptr );

    auto removed = node.removeChild( node.childNodes()[1] ); //i.e.: child2
    ASSERT_EQ( node.childNodes().size(), 2 );
    ASSERT_EQ( removed.get(), child2 );
    ASSERT_EQ( child1->previousSibling(), nullptr );
    ASSERT_EQ( child1->nextSibling(), child3 );
    ASSERT_EQ( child2->previousSibling(), nullptr );
    ASSERT_EQ( child2->nextSibling(), nullptr );
    ASSERT_EQ( child3->previousSibling(), child1 );
    ASSERT_EQ( child3->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, removeChild_3 ) { //[][x][] by index, check neighbours
    auto   node   = node::Node();
    auto * child1 = node.appendChild( std::make_unique<node::Node>() );
    auto * child2 = node.appendChild( std::make_unique<node::Node>() );
    auto * child3 = node.appendChild( std::make_unique<node::Node>() );

    //sanity check
    ASSERT_EQ( node.childNodes().size(), 3 );
    ASSERT_EQ( child1->previousSibling(), nullptr );
    ASSERT_EQ( child1->nextSibling(), child2 );
    ASSERT_EQ( child2->previousSibling(), child1 );
    ASSERT_EQ( child2->nextSibling(), child3 );
    ASSERT_EQ( child3->previousSibling(), child2 );
    ASSERT_EQ( child3->nextSibling(), nullptr );

    auto removed = node.removeChild( 1 ); //i.e.: child2
    ASSERT_EQ( node.childNodes().size(), 2 );
    ASSERT_EQ( removed.get(), child2 );
    ASSERT_EQ( child1->previousSibling(), nullptr );
    ASSERT_EQ( child1->nextSibling(), child3 );
    ASSERT_EQ( child2->previousSibling(), nullptr );
    ASSERT_EQ( child2->nextSibling(), nullptr );
    ASSERT_EQ( child3->previousSibling(), child1 );
    ASSERT_EQ( child3->nextSibling(), nullptr );
}

TEST( parser_dom_node_Node_Tests, removeChild_fail_0 ) { //by unique_ptr reference
    auto node         = node::Node();
    auto disconnected = std::make_unique<node::Node>();

    try {
        std::ignore = node.removeChild( disconnected );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NotFoundError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, removeChild_fail_1 ) { //by index
    auto   node   = node::Node();
    auto * child1 = node.appendChild( std::make_unique<node::Node>() );

    try {
        std::ignore = node.removeChild( 1 );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NotFoundError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Node_Tests, swap_0 ) { //parent
    auto parent1 = node::Node();
    auto parent2 = node::Node();
    auto node1   = node::Node( NodeType::UNDEFINED, &parent1, nullptr );
    auto node2   = node::Node( NodeType::UNDEFINED, &parent2, nullptr );

    blogator::parser::dom::node::swap( node1, node2 );

    ASSERT_EQ( node1.parentNode(), &parent1 );
    ASSERT_EQ( node2.parentNode(), &parent2 );
}

TEST( parser_dom_node_Node_Tests, swap_1 ) { //previous sibling
    auto prev_sibling_1 = node::Node();
    auto prev_sibling_2 = node::Node();
    auto node1          = node::Node( NodeType::UNDEFINED, nullptr, &prev_sibling_1 );
    auto node2          = node::Node( NodeType::UNDEFINED, nullptr, &prev_sibling_2 );

    blogator::parser::dom::node::swap( node1, node2 );

    ASSERT_EQ( node1.previousSibling(), &prev_sibling_1 );
    ASSERT_EQ( node2.previousSibling(), &prev_sibling_2 );
}

TEST( parser_dom_node_Node_Tests, swap_2 ) { //next sibling
    auto next_sibling_1 = node::Node();
    auto next_sibling_2 = node::Node();
    auto node1          = node::Node( NodeType::UNDEFINED, nullptr, nullptr, &next_sibling_1 );
    auto node2          = node::Node( NodeType::UNDEFINED, nullptr, nullptr, &next_sibling_2 );

    blogator::parser::dom::node::swap( node1, node2 );

    ASSERT_EQ( node1.nextSibling(), &next_sibling_1 );
    ASSERT_EQ( node2.nextSibling(), &next_sibling_2 );
}

TEST( parser_dom_node_Node_Tests, swap_3 ) { //children + owner doc
    auto parent1 = node::Document();
    auto node1   = parent1.appendChild( std::make_unique<node::Node>( NodeType::UNDEFINED ) );
    auto child1  = node1->appendChild( std::make_unique<node::Node>() );

    auto parent2 = node::Document();
    auto node2   = parent2.appendChild( std::make_unique<node::Node>( NodeType::UNDEFINED ) );
    auto child2  = node2->appendChild( std::make_unique<node::Node>() );
    auto child3  = node2->appendChild( std::make_unique<node::Node>() );

    //sanity check
    ASSERT_EQ( node1->childNodes().size(), 1 );
    ASSERT_EQ( node1->childNodes()[0]->ownerDocument(), &parent1 );
    ASSERT_EQ( node1->childNodes()[0]->parentNode(), node1 );

    ASSERT_EQ( node2->childNodes().size(), 2 );
    ASSERT_EQ( node2->childNodes()[0]->ownerDocument(), &parent2 );
    ASSERT_EQ( node2->childNodes()[0]->parentNode(), node2 );
    ASSERT_EQ( node2->childNodes()[1]->ownerDocument(), &parent2 );
    ASSERT_EQ( node2->childNodes()[1]->parentNode(), node2 );

    blogator::parser::dom::node::swap( *node1, *node2 );

    //assert result
    ASSERT_EQ( node1->ownerDocument(), &parent1 );
    ASSERT_EQ( node1->childNodes().size(), 1 );
    ASSERT_EQ( node1->childNodes()[0]->ownerDocument(), &parent1 );
    ASSERT_EQ( node1->childNodes()[0]->parentNode(), node1 );

    ASSERT_EQ( node2->ownerDocument(), &parent2 );
    ASSERT_EQ( node2->childNodes().size(), 2 );
    ASSERT_EQ( node2->childNodes()[0]->ownerDocument(), &parent2 );
    ASSERT_EQ( node2->childNodes()[0]->parentNode(), node2 );
    ASSERT_EQ( node2->childNodes()[1]->ownerDocument(), &parent2 );
    ASSERT_EQ( node2->childNodes()[1]->parentNode(), node2 );
}

TEST( parser_dom_node_Node_Tests, swap_fail_0 ) { //not same types
    auto node1   = node::Node( NodeType::TEXT_NODE );
    auto node2   = node::Node( NodeType::COMMENT_NODE );

    ASSERT_THROW( node1.swap( node2 ), blogator::parser::dom::exception::DOMException );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_local_0 ) { //generic
    auto node = node::Node();
    ASSERT_EQ( node.getNamespaceEnum(), blogator::parser::specs::infra::Namespace::NONE );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_local_1 ) { //Attr (HTML5 namespace)
    auto ns_id = node::Node::namespaceMap().getID( blogator::parser::specs::infra::Namespace::HTML );
    auto node  = node::Attr( ns_id, U"name", U"value" );

    ASSERT_EQ( node.getNamespaceEnum(), blogator::parser::specs::infra::Namespace::HTML );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_local_2 ) { //Attr (other namespace)
    auto ns_id = node::Node::namespaceMap().setNamespace( U"parser_dom_node_Node_Tests_getNamespaceEnum_local_2", U"prefix" );
    auto node  = node::Attr( ns_id, U"name", U"value" );

    ASSERT_EQ( node.getNamespaceEnum(), blogator::parser::specs::infra::Namespace::OTHER );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_local_3 ) { //Element (HTML5 namespace)
    auto node = node::Element( blogator::parser::specs::infra::Element::HTML5_HTML );

    ASSERT_EQ( node.getNamespaceEnum(), blogator::parser::specs::infra::Namespace::HTML );
}

TEST( parser_dom_node_Node_Tests, getNamespaceEnum_local_4 ) { //Element (other namespace)
    auto ns_id = node::Node::namespaceMap().setNamespace( U"parser_dom_node_Node_Tests_getNamespaceEnum_local_4", U"prefix" );
    auto node  = node::Element( ns_id, U"tag" );

    ASSERT_EQ( node.getNamespaceEnum(), blogator::parser::specs::infra::Namespace::OTHER );
}