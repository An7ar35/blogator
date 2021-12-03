#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Node.h"

#include <memory>
#include <vector>

using namespace blogator::parser::dom::node;
using           blogator::parser::dom::NodeType;

TEST( parser_dom_node_Node, Constructor ) { //NodeType only / 1 arg constructor
    auto node = Node( NodeType::DOCUMENT_NODE );

    ASSERT_EQ( NodeType::DOCUMENT_NODE, node.nodeType() );
    ASSERT_FALSE( node.hasParent() );
    ASSERT_FALSE( node.hasSiblingPrev() );
    ASSERT_FALSE( node.hasSiblingNext() );
}

TEST( parser_dom_node_Node, Constructor_w_parent_0 ) { //3 args constructor
    auto parent = Node( NodeType::UNDEFINED );
    auto child  = Node( NodeType::UNDEFINED, &parent, nullptr );

    ASSERT_FALSE( parent.hasParent() );
    ASSERT_TRUE( child.hasParent() );
    ASSERT_EQ( child.parentNode(), parent );
}

TEST( parser_dom_node_Node, Constructor_prev_only_0 ) { //lone detached Node
    auto node = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, nullptr );

    ASSERT_EQ( NodeType::UNDEFINED, node->nodeType() );
    ASSERT_FALSE( node->hasSiblingPrev() );
    ASSERT_FALSE( node->hasSiblingNext() );
}

TEST( parser_dom_node_Node, Constructor_prev_only_1 ) { //2 sequential nodes
    auto node1 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, nullptr );
    auto node2 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get() );

    ASSERT_FALSE( node1->hasSiblingPrev() );
    ASSERT_TRUE( node1->hasSiblingNext() );
    ASSERT_EQ( node1->nextSibling(), *node2 );

    ASSERT_TRUE( node2->hasSiblingPrev() );
    ASSERT_FALSE( node2->hasSiblingNext() );
    ASSERT_EQ( node2->previousSibling(), *node1 );
}

TEST( parser_dom_node_Node, Constructor_prev_only_2 ) { //2 sequential node and a third inserted between
    auto node1 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, nullptr );     //{ n1 }
    auto node2 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get() ); //{ n1, n2 }
    auto node3 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get() ); //{ n1, n3, n2 }

    ASSERT_FALSE( node1->hasSiblingPrev() );
    ASSERT_TRUE( node1->hasSiblingNext() );
    ASSERT_EQ( node1->nextSibling(), *node3 );

    ASSERT_TRUE( node3->hasSiblingPrev() );
    ASSERT_EQ( node3->previousSibling(), *node1 );
    ASSERT_TRUE( node3->hasSiblingNext() );
    ASSERT_EQ( node3->nextSibling(), *node2 );

    ASSERT_TRUE( node2->hasSiblingPrev() );
    ASSERT_EQ( node2->previousSibling(), *node3 );
    ASSERT_FALSE( node2->hasSiblingNext() );
}

TEST( parser_dom_node_Node, Constructor_w_parent_1 ) { //4 args constructor
    auto parent = Node( NodeType::UNDEFINED );
    auto child  = Node( NodeType::UNDEFINED, &parent, nullptr, nullptr );

    ASSERT_FALSE( parent.hasParent() );
    ASSERT_TRUE( child.hasParent() );
    ASSERT_EQ( child.parentNode(), parent );
}

TEST( parser_dom_node_Node, Constructor_prev_and_next_0 ) { //2 sequential nodes
    auto node = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );

    ASSERT_EQ( NodeType::UNDEFINED, node->nodeType() );
    ASSERT_FALSE( node->hasSiblingPrev() );
    ASSERT_FALSE( node->hasSiblingNext() );
}

TEST( parser_dom_node_Node, Constructor_prev_and_next_1 ) { //lone detached node
    auto node1 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );
    auto node2 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get(), nullptr );

    ASSERT_FALSE( node1->hasSiblingPrev() );
    ASSERT_TRUE( node1->hasSiblingNext() );
    ASSERT_EQ( node1->nextSibling(), *node2 );

    ASSERT_TRUE( node2->hasSiblingPrev() );
    ASSERT_FALSE( node2->hasSiblingNext() );
    ASSERT_EQ( node2->previousSibling(), *node1 );
}

TEST( parser_dom_node_Node, Constructor_prev_and_next_2 ) { //2 sequential node and a third inserted between
    auto node1 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );
    auto node2 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get(), nullptr );
    auto node3 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get(), node2.get() );

    ASSERT_FALSE( node1->hasSiblingPrev() );
    ASSERT_TRUE( node1->hasSiblingNext() );
    ASSERT_EQ( node1->nextSibling(), *node3 );

    ASSERT_TRUE( node3->hasSiblingPrev() );
    ASSERT_EQ( node3->previousSibling(), *node1 );
    ASSERT_TRUE( node3->hasSiblingNext() );
    ASSERT_EQ( node3->nextSibling(), *node2 );

    ASSERT_TRUE( node2->hasSiblingPrev() );
    ASSERT_EQ( node2->previousSibling(), *node3 );
    ASSERT_FALSE( node2->hasSiblingNext() );
}

TEST( parser_dom_node_Node, Desctructor_w_siblings ) { //checks that when a node is destroyed the immediate sibling references are updated correctly
    auto node1 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, nullptr, nullptr );
    auto node2 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get(), nullptr );
    auto node3 = std::make_unique<Node>( NodeType::UNDEFINED, nullptr, node1.get(), node2.get() );

    ASSERT_TRUE( node3->hasSiblingPrev() );
    ASSERT_EQ( node3->previousSibling(), *node1 );
    ASSERT_TRUE( node3->hasSiblingNext() );
    ASSERT_EQ( node3->nextSibling(), *node2 );

    node3.reset(); //destroy node3

    ASSERT_FALSE( node1->hasSiblingPrev() );
    ASSERT_TRUE( node1->hasSiblingNext() );
    ASSERT_EQ( node1->nextSibling(), *node2 );

    ASSERT_TRUE( node2->hasSiblingPrev() );
    ASSERT_FALSE( node2->hasSiblingNext() );
    ASSERT_EQ( node2->previousSibling(), *node1 );
}

TEST( parser_dom_node_Node, isSameNode_null ) { //null unique pointer to compare to
    Node                  node1 = Node( NodeType::UNDEFINED );
    std::unique_ptr<Node> node2;

    ASSERT_FALSE( node1.isSameNode( node2 ) );
}

TEST( parser_dom_node_Node, isSameNode_not_same ) { //valid but not same node to comare to
    auto node1 = Node( NodeType::UNDEFINED );
    auto node2 = std::make_unique<Node>( NodeType::UNDEFINED );

    ASSERT_FALSE( node1.isSameNode( node2 ) );
}

TEST( parser_dom_node_Node, isSameNode_same ) { //valid but not same node to compare to
    auto   node1 = std::make_unique<Node>( NodeType::UNDEFINED );
    auto * node2 = node1.get();

    ASSERT_TRUE( node2->isSameNode( node1 ) );
}