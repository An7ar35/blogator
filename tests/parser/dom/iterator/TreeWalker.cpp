#include "gtest/gtest.h"
#include "../../../src/parser/dom/iterator/TreeWalker.h"

#include "../../../TestHelpers/TestDOMs.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::TreeWalker;

TEST( parser_dom_TreeWalker_Tests, constructor_0 ) { //no filter specified
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get() );

    ASSERT_EQ( tree_walker.root(), document.get() );
    ASSERT_EQ( tree_walker.currentNode(), document.get() );
    ASSERT_FALSE( tree_walker.filter() );
    ASSERT_EQ( tree_walker.whatToShow(), NodeFilter::SHOW_ALL );
}

TEST( parser_dom_TreeWalker_Tests, constructor_1 ) { //with a filter specified
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_ELEMENT ) );

    ASSERT_EQ( tree_walker.root(), document.get() );
    ASSERT_EQ( tree_walker.currentNode(), document.get() );
    ASSERT_TRUE( tree_walker.filter() );
    ASSERT_EQ( tree_walker.whatToShow(), NodeFilter::SHOW_ELEMENT );
}

TEST( parser_dom_TreeWalker_Tests, constructor_2 ) { //with a filter pointer specified
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto filter      = std::make_shared<NodeFilter>( NodeFilter::SHOW_COMMENT );
    auto tree_walker = TreeWalker( document.get(), filter );

    ASSERT_EQ( tree_walker.root(), document.get() );
    ASSERT_EQ( tree_walker.currentNode(), document.get() );
    ASSERT_TRUE( tree_walker.filter() );
    ASSERT_EQ( tree_walker.whatToShow(), NodeFilter::SHOW_COMMENT );
}

TEST( parser_dom_TreeWalker_Tests, parentNode_0 ) { //no filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get() );

    ASSERT_EQ( tree_walker.parentNode(), nullptr );

    while( tree_walker.nextNode() ); //get to the last pre-order node in tree

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"#text" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"B",
        U"P",
        U"DIV",
        U"BODY",
        U"HTML",
        U"#document",
    } );

    node::Node * parent = nullptr;
    size_t       count  = 0;

    while( ( parent = tree_walker.parentNode() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( parent->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, parentNode_1 ) { //with filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_ELEMENT ) );

    ASSERT_EQ( tree_walker.parentNode(), nullptr );

    while( tree_walker.nextNode() ); //get to the last pre-order node in tree

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"B" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"P",
        U"DIV",
        U"BODY",
        U"HTML",
    } );

    node::Node * parent = nullptr;
    size_t       count  = 0;

    while( ( parent = tree_walker.parentNode() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( parent->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, firstChild_0 ) { //no filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get() );

    { //get to the HTML element in the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"HTML" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"HTML" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"HEAD",
        U"DIV",
        U"H1",
        U"#text",
    } );

    node::Node * first_child = nullptr;
    size_t       count       = 0;

    while( ( first_child = tree_walker.firstChild() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( first_child->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, firstChild_1 ) { //with filter
    auto filter_fn = []( const node::Node &node ) {
        return ( node.nodeName() == U"DIV"
                 ? blogator::parser::dom::NodeFilter::Result::FILTER_SKIP
                 : blogator::parser::dom::NodeFilter::Result::FILTER_ACCEPT );
    };

    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_ELEMENT, filter_fn ) );

    { //get to the HTML element in the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"HTML" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"HTML" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"HEAD",
        U"H1",
    } );

    node::Node * first_child = nullptr;
    size_t       count       = 0;

    while( ( first_child = tree_walker.firstChild() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( first_child->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, lastChild_0 ) { //no filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get() );

    { //get to the HTML element in the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"HTML" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"HTML" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"BODY",
        U"DIV",
        U"P",
        U"B",
        U"#text",
    } );

    node::Node * last_child = nullptr;
    size_t       count      = 0;

    while( ( last_child = tree_walker.lastChild() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( last_child->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, lastChild_1 ) { //with filter
    auto filter_fn = []( const node::Node &node ) {
        return ( node.nodeName() == U"DIV"
                 ? blogator::parser::dom::NodeFilter::Result::FILTER_SKIP
                 : blogator::parser::dom::NodeFilter::Result::FILTER_ACCEPT );
    };

    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_ELEMENT, filter_fn ) );

    { //get to the HTML element in the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"HTML" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"HTML" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"BODY",
        U"P",
        U"B",
    } );

    node::Node * last_child = nullptr;
    size_t       count      = 0;

    while( ( last_child = tree_walker.lastChild() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( last_child->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, previousSibling_0 ) { //no filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get() );

    { //get to the H4 element in the HTML element of the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"H4" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"H4" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"DIV",
        U"A",
        U"#text",
        U"DIV",
    } );

    node::Node * prev_sibling = nullptr;
    size_t       count        = 0;

    while( ( prev_sibling = tree_walker.previousSibling() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( prev_sibling->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, previousSibling_1 ) { //with filter
    auto filter_fn = []( const node::Node &node ) {
        if( node.nodeName() == U"A" ) {
            return blogator::parser::dom::NodeFilter::Result::FILTER_SKIP;
        } else if( node.nodeName() == U"#text" ) {
            return blogator::parser::dom::NodeFilter::Result::FILTER_REJECT;
        } else {
            return blogator::parser::dom::NodeFilter::Result::FILTER_ACCEPT;
        }
    };

    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_ALL, filter_fn ) );

    { //get to the first DIV in the HTML element of the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"H4" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"H4" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"DIV",
        U"SPAN",
        U"DIV",
    } );

    node::Node * prev_sibling = nullptr;
    size_t       count        = 0;

    while( ( prev_sibling = tree_walker.previousSibling() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( prev_sibling->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, nextSibling_0 ) { //no filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get() );

    { //get to the first DIV in the HTML element of the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"DIV" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"DIV" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"#text",
        U"A",
        U"DIV",
        U"H4",
    } );

    node::Node * next_sibling = nullptr;
    size_t       count        = 0;

    while( ( next_sibling = tree_walker.nextSibling() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( next_sibling->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, nextSibling_1 ) { //with filter
    auto filter_fn = []( const node::Node &node ) {
        return ( node.nodeName() == U"A"
                 ? blogator::parser::dom::NodeFilter::Result::FILTER_SKIP
                 : blogator::parser::dom::NodeFilter::Result::FILTER_ACCEPT );
    };

    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_ALL, filter_fn ) );

    { //get to the first DIV in the HTML element of the tree
        node::Node * curr = nullptr;
        while( ( curr = tree_walker.nextNode() ) && curr->nodeName() != U"DIV" );
    }

    //sanity check
    ASSERT_NE( tree_walker.currentNode(), nullptr );
    ASSERT_EQ( tree_walker.currentNode()->nodeName(), U"DIV" );

    auto expected_nodes = std::vector<DOMString_t>( {
        U"#text",
        U"SPAN",
        U"DIV",
        U"H4",
    } );

    node::Node * next_sibling = nullptr;
    size_t       count        = 0;

    while( ( next_sibling = tree_walker.nextSibling() ) ) {
        ASSERT_TRUE( count < expected_nodes.size() ) << "Unexpected node: " << tree_walker;
        ASSERT_EQ( next_sibling->nodeName(), expected_nodes[count] );
        ++count;
    }

    ASSERT_EQ( count, expected_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, nextNode_0 ) { //no filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get() );

    auto i = 0;

    for( auto * curr = tree_walker.currentNode(); curr != nullptr; curr = tree_walker.nextNode(), ++i ) {
        ASSERT_EQ( curr, pre_order[i] );
    }

    ASSERT_EQ( i, pre_order.size() );
}

TEST( parser_dom_TreeWalker_Tests, nextNode_1 ) { //with filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_TEXT ) );
    auto text_nodes  = std::vector<node::Node *>();

    for( auto * node : pre_order ) {
        if( node->nodeType() == NodeType::TEXT_NODE ) {
            text_nodes.emplace_back( node );
        }
    }

    auto i = 0;

    for( auto * curr = tree_walker.nextNode(); curr != nullptr; curr = tree_walker.nextNode(), ++i ) {
        ASSERT_EQ( curr, text_nodes[i] );
    }

    ASSERT_EQ( i, text_nodes.size() );
}

TEST( parser_dom_TreeWalker_Tests, previousNode_0 ) { //no filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto reverse     = std::vector<node::Node *>( pre_order.rbegin(), pre_order.rend() );
    auto tree_walker = TreeWalker( document.get() );

    while( tree_walker.nextNode() ); //get to the last pre-order node in tree

    auto i = 0;

    for( auto * curr = tree_walker.currentNode(); curr != nullptr; curr = tree_walker.previousNode(), ++i ) {
        ASSERT_EQ( curr, reverse[i] );
    }

    ASSERT_EQ( i, reverse.size() )  << "Did not transverse all expected nodes in tree - ignore if 'nextNode' tests fail.";
}

TEST( parser_dom_TreeWalker_Tests, previousNode_1 ) { //with filter
    std::unique_ptr<node::Node> document;
    auto pre_order   = blogator::tests::createTestDOM_1( document );
    auto tree_walker = TreeWalker( document.get(), NodeFilter( NodeFilter::SHOW_TEXT ) );
    auto reverse     = std::vector<node::Node *>( pre_order.rbegin(), pre_order.rend() );
    auto text_nodes  = std::vector<node::Node *>();

    for( auto * node : reverse ) {
        if( node->nodeType() == NodeType::TEXT_NODE ) {
            text_nodes.emplace_back( node );
        }
    }

    while( tree_walker.nextNode() ); //get to the last pre-order node in tree

    auto i = 0;

    for( auto * curr = tree_walker.currentNode(); curr != nullptr; curr = tree_walker.previousNode(), ++i ) {
        ASSERT_EQ( curr, text_nodes[i] );
    }

    ASSERT_EQ( i, text_nodes.size() ) << "Did not transverse all expected nodes in tree - ignore if 'nextNode' tests fail.";
}