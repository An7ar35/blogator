#include "gtest/gtest.h"
#include "../../../src/parser/dom/iterator/NodeIterator.h"

#include "../../../TestHelpers/TestDOMs.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeIterator;


TEST( parser_dom_NodeIterator_Tests, iteration_0 ) { //simple pre-order forward prefix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = blogator::tests::createTestDOM_1( document );

    size_t i  = 0;
    auto   it = document->begin();

    while( it != document->end() && i < pre_order.size() ) {
        ASSERT_EQ( it.node(), pre_order[i++] );
        ++it;
    }

    ASSERT_EQ( i, pre_order.size() );
    ASSERT_TRUE( it.done() );
}

TEST( parser_dom_NodeIterator_Tests, iteration_1 ) { //simple pre-order forward postfix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = blogator::tests::createTestDOM_1( document );
    auto reverse   = std::vector<node::Node *>( pre_order.rbegin(), pre_order.rend() );

    //increment
    size_t i  = 0;
    auto   it = document->begin();

    while( it != document->end() && i < pre_order.size() ) {
        ASSERT_EQ( ( it++ ).node(), pre_order[i++] );
    }

    ASSERT_EQ( i, pre_order.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    i = 0;
    ASSERT_EQ( it--, document->end() );

    while( i < reverse.size() ) {
        ASSERT_EQ( ( it-- ).node(), reverse[i++] );
    }

    ASSERT_EQ( i, reverse.size() );
    ASSERT_EQ( it--, document->begin() ); //consistency check
}

TEST( parser_dom_NodeIterator_Tests, iteration_2 ) { //filtered forward iteration
    std::unique_ptr<node::Node> document;
    auto pre_order      = blogator::tests::createTestDOM_1( document );
    auto pre_order_text = std::vector<node::Node *>();

    for( auto * ptr : pre_order ) {
        if( ptr && ptr->nodeType() == NodeType::TEXT_NODE ) {
            pre_order_text.emplace_back( ptr );
        }
    }

    //increment
    size_t i  = 0;
    auto   it = document->begin( NodeFilter( NodeFilter::SHOW_TEXT ) );

    while( it != document->end() ) {
        ASSERT_EQ( it.node(), pre_order_text[i++] );
        ++it;
    }

    ASSERT_EQ( i, pre_order_text.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    auto reverse_text = std::vector<node::Node *>( pre_order_text.rbegin(), pre_order_text.rend() );

    i = 0;

    while( i < reverse_text.size() ) {
        --it;
        ASSERT_EQ( it.node(), reverse_text[i++] );
    }

    ASSERT_EQ( i, reverse_text.size() );
}

TEST( parser_dom_NodeIterator_Tests, reverse_iteration_0 ) { //simple pre-order backward prefix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = blogator::tests::createTestDOM_1( document );
    auto reverse   = std::vector<node::Node *>( pre_order.rbegin(), pre_order.rend() );

    size_t i  = 0;
    auto   it = document->rbegin();

    while( it != document->rend() ) {
        ASSERT_EQ( it.node(), reverse[ i++ ] );
        ++it;
    }

    ASSERT_EQ( i, reverse.size() );
    ASSERT_TRUE( it.done() );
}

TEST( parser_dom_NodeIterator_Tests, reverse_iteration_1 ) { //simple pre-order backward postfix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = blogator::tests::createTestDOM_1( document );
    auto reverse   = std::vector<node::Node *>( pre_order.rbegin(), pre_order.rend() );

    //increment
    size_t i  = 0;
    auto   it = document->rbegin();

    while( it != document->rend() ) {
        ASSERT_EQ( ( it++ ).node(), reverse[ i++ ] );
    }

    ASSERT_EQ( i, reverse.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    i = 0;
    ASSERT_EQ( it--, document->rend() );

    while( i < pre_order.size() ) {
        ASSERT_EQ( ( it-- ).node(), pre_order[i++] );
    }

    ASSERT_EQ( i, pre_order.size() );
    ASSERT_EQ( it--, document->rbegin() ); //consistency check
}

TEST( parser_dom_NodeIterator_Tests, reverse_iteration_2 ) { //filtered backward iteration
    std::unique_ptr<node::Node> document;
    auto pre_order      = blogator::tests::createTestDOM_1( document );
    auto pre_order_text = std::vector<node::Node *>();

    for( auto * ptr : pre_order ) {
        if( ptr && ptr->nodeType() == NodeType::TEXT_NODE ) {
            pre_order_text.emplace_back( ptr );
        }
    }

    auto reverse_text = std::vector<node::Node *>( pre_order_text.rbegin(), pre_order_text.rend() );

    //increment
    size_t i  = 0;
    auto   it = document->rbegin( NodeFilter( NodeFilter::SHOW_TEXT ) );

    while( it != document->rend() ) {
        ASSERT_EQ( it.node(), reverse_text[i++] );
        ++it;
    }

    ASSERT_EQ( i, reverse_text.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    i = 0;

    while( i < pre_order_text.size() ) {
        --it;
        ASSERT_EQ( it.node(), pre_order_text[i++] );
    }

    ASSERT_EQ( i, pre_order_text.size() );
}

TEST( parser_dom_NodeIterator_Tests, equality_operator_0 ) { //begin/end duplicates
    auto   doc     = node::Document();
    auto   beg_it  = doc.begin();
    auto   end_it  = doc.end();

    ASSERT_EQ( beg_it, doc.begin() );
    ASSERT_EQ( end_it, doc.end() );
    ASSERT_NE( beg_it, end_it );
}

TEST( parser_dom_NodeIterator_Tests, equality_operator_1 ) { //rbegin/rend duplicates
    auto   doc     = node::Document();
    auto   beg_it  = doc.rbegin();
    auto   end_it  = doc.rend();

    ASSERT_EQ( beg_it, doc.rbegin() );
    ASSERT_EQ( end_it, doc.rend() );
    ASSERT_NE( beg_it, end_it );
}

TEST( parser_dom_NodeIterator_Tests, equality_operator_2 ) { //overlapping fwd iterators
    auto   doc     = node::Document();
    auto * comment = doc.appendChild( std::make_unique<node::Comment>( U"comment" ) );
    auto   beg_it  = doc.begin();
    auto   end_it  = doc.end();

    ASSERT_EQ( ++beg_it, --end_it );
    ASSERT_EQ( ++beg_it, doc.end() );
    ASSERT_EQ( --end_it, doc.begin() );
}

TEST( parser_dom_NodeIterator_Tests, equality_operator_3 ) { //overlapping rev iterators
    auto   doc     = node::Document();
    auto * comment = doc.appendChild( std::make_unique<node::Comment>( U"comment" ) );
    auto   beg_it  = doc.rbegin();
    auto   end_it  = doc.rend();

    ASSERT_EQ( ++beg_it, --end_it );
    ASSERT_EQ( ++beg_it, doc.rend() );
    ASSERT_EQ( --end_it, doc.rbegin() );
}

TEST( parser_dom_NodeIterator_Tests, equality_operator_4 ) { //overlapping fwd & rev iterators
    auto   doc        = node::Document();
    auto * comment    = doc.appendChild( std::make_unique<node::Comment>( U"comment" ) );
    auto   fwd_it1    = doc.begin();
    auto   fwd_it2    = doc.begin();
    auto   rev_it1    = doc.rbegin();
    auto   rev_it2    = doc.rbegin();
    auto   end_fwd_it = doc.end();
    auto   end_rev_it = doc.rend();

    ASSERT_NE( fwd_it1, rev_it1 ); //[Document], [Comment]
    ASSERT_NE( end_fwd_it, end_rev_it ); //end(), rend()

    ASSERT_EQ( ++fwd_it1, rev_it1 ); //[Comment], [Comment]
    ASSERT_NE( ++fwd_it1, rev_it1 ); //end(), [Comment]
    ASSERT_NE( fwd_it1, ++rev_it1 ); //end(), [Document]
    ASSERT_EQ( ++fwd_it1, end_fwd_it ); //end(), end()
    ASSERT_NE( fwd_it1, rev_it1 ); //end(), rend()

    ASSERT_EQ( fwd_it2, ++rev_it2 ); //[Document], [Document]
    ASSERT_NE( fwd_it2, ++rev_it2 ); //[Document], rend()
    ASSERT_EQ( end_rev_it, ++rev_it2 ); //rend(), rend()

}

TEST( parser_dom_NodeIterator_Tests, dereference_operator ) {
    auto doc    = node::Document();
    auto begin  = doc.begin();
    auto end    = doc.end();
    auto rbegin = doc.rbegin();
    auto rend   = doc.rend();

    ASSERT_NO_THROW( *begin );
    ASSERT_NO_THROW( *rbegin );
    ASSERT_EQ( *begin, doc );
    ASSERT_EQ( *rbegin, doc );
    ASSERT_THROW( *end, std::runtime_error );
    ASSERT_THROW( *rend, std::runtime_error );
}

TEST( parser_dom_NodeIterator_Tests, access_operator ) {
    auto doc    = node::Document();
    auto begin  = doc.begin();
    auto end    = doc.end();
    auto rbegin = doc.rbegin();
    auto rend   = doc.rend();

    ASSERT_NO_THROW( std::ignore = begin->nodeType() );
    ASSERT_NO_THROW( std::ignore = rbegin->nodeType() );
    ASSERT_THROW( std::ignore = end->nodeType(), std::runtime_error );
    ASSERT_THROW( std::ignore = rend->nodeType(), std::runtime_error );
}

TEST( parser_dom_NodeIterator_Tests, whatToShow_0 ) { //default
    auto doc = node::Document();
    auto it  = doc.begin();

    ASSERT_EQ( it.whatToShow(), NodeFilter::SHOW_ALL );
}

TEST( parser_dom_NodeIterator_Tests, whatToShow_1 ) { //filtered
    auto doc = node::Document();
    auto it  = doc.begin( NodeFilter( NodeFilter::SHOW_TEXT & NodeFilter::SHOW_ATTRIBUTE ) );

    ASSERT_EQ( it.whatToShow(), NodeFilter::SHOW_TEXT & NodeFilter::SHOW_ATTRIBUTE );
}

TEST( parser_dom_NodeIterator_Tests, nodeFilter_0 ) { //none
    auto doc = node::Document();
    auto it  = doc.begin();

    ASSERT_FALSE( it.nodeFilter() );
}

TEST( parser_dom_NodeIterator_Tests, nodeFilter_1 ) { //specified
    auto doc = node::Document();
    auto it  = doc.begin( NodeFilter( NodeFilter::SHOW_TEXT ) );

    ASSERT_TRUE( it.nodeFilter() );
    ASSERT_EQ( it.nodeFilter()->whatToShow(), NodeFilter::SHOW_TEXT );
}

TEST( parser_dom_NodeIterator_Tests, nextNode ) {
    std::unique_ptr<node::Node> document;
    auto pre_order = blogator::tests::createTestDOM_1( document );

    size_t i  = 0;
    auto   it = document->begin();

    while( it != document->end() && i < pre_order.size() ) {
        ASSERT_EQ( it.nextNode(), pre_order[i++] );
    }

    ASSERT_EQ( it, document->end() );
    ASSERT_EQ( i, pre_order.size() );
    ASSERT_TRUE( it.done() );
}

TEST( parser_dom_NodeIterator_Tests, previousNode ) {
    std::unique_ptr<node::Node> document;
    auto pre_order = blogator::tests::createTestDOM_1( document );
    auto reverse   = std::vector<node::Node *>( pre_order.rbegin(), pre_order.rend() );

    size_t i  = 0;
    auto   it = document->end();

    while( i < reverse.size() ) {
        auto node = it.previousNode();
        ASSERT_EQ( node, reverse[i++] );
    }

    ASSERT_EQ( i, pre_order.size() );
    ASSERT_EQ( it.previousNode(), nullptr ); //trying to go back from 1st node
    ASSERT_EQ( it.previousNode(), nullptr ); //consistency check
}