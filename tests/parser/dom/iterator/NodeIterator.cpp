#include "gtest/gtest.h"
#include "../../../src/parser/dom/iterator/NodeIterator.h"

#include <iostream>

#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/CDATASection.h"
#include "../../../src/parser/dom/node/Comment.h"
#include "../../../src/parser/dom/node/Document.h"
#include "../../../src/parser/dom/node/DocumentFragment.h"
#include "../../../src/parser/dom/node/DocumentType.h"
#include "../../../src/parser/dom/node/Element.h"
#include "../../../src/parser/dom/node/Text.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeIterator;

/**
 * Creates a test document
 * @param root Unique pointer to set the Document node at
 * @return Vector of node pointers pre-order (to compare iteration with)
 */
std::vector<node::Node *> createTestDocument( std::unique_ptr<node::Node> &root ) {
    auto pre_order = std::vector<node::Node *>();

    root = std::make_unique<node::Document>();
    auto * cdata    = root->appendChild( std::make_unique<node::CDATASection>( U"cdata" ) );
    auto * comment1 = root->appendChild( std::make_unique<node::Comment>( U"comment 1" ) );
    auto * html     = root->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::html5::Element::HTML5_HTML ) );
    auto * head     = html->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::html5::Element::HTML5_HEAD ) );
    auto * div1     = head->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto * h1       = div1->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_H1 ) );
    auto * h1_txt   = h1->appendChild( std::make_unique<node::Text>( U"Heading 1" ) );
    auto * div2     = head->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto * img      = div2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_IMG ) );
    auto * body     = html->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::html5::Element::HTML5_BODY ) );
    auto * comment2 = body->appendChild( std::make_unique<node::Comment>( U"comment 2" ) );
    auto * div3     = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto * h2       = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_H2 ) );
    auto * h2_txt   = h2->appendChild( std::make_unique<node::Text>( U"Heading 2" ) );
    auto * h3       = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_H3 ) );
    auto * h3_txt   = h3->appendChild( std::make_unique<node::Text>( U"Heading 3" ) );
    auto * div4     = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV ) );
    auto * p1       = div4->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_P ) );
    auto * p1_txt1  = p1->appendChild( std::make_unique<node::Text>( U"Paragraph 1.1\n" ) );
    auto * p1_txt2  = p1->appendChild( std::make_unique<node::Text>( U"Paragraph 1.2" ) );
    auto * p2       = div4->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_P ) );
    auto * p2_txt2  = p2->appendChild( std::make_unique<node::Text>( U"Paragraph 2: " ) );
    auto * p2_bold  = p2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_B ) );
    auto * p2_txt3  = p2_bold->appendChild( std::make_unique<node::Text>( U"bold text" ) );


    pre_order.insert(
        pre_order.end(), { root.get(),
          cdata,                            // <![CDATA[cdata]]>
          comment1,                         // <!--comment 1-->
          html,                             // <html>
            head,                           //   <head>
              div1,                         //      <div>
                h1, h1_txt,                 //        <h1>Heading 1</h1>
                                            //      </div>
              div2,                         //      <div>
                img,                        //        <img />
                                            //      </div>
                                            //   </head>
              body,                         //   <body>
              comment2,                     //      <!--comment 2-->
              div3,                         //      <div>
                h2, h2_txt,                 //        <h2>Heading 2</h2>
                h3, h3_txt,                 //        <h3>Heading 3</h3>
                                            //      </div>
              div4,                         //      <div>
              p1, p1_txt1,                  //        <p>Paragraph 1.1
                  p1_txt2,                  // Paragraph 1.2</p>
              p2, p2_txt2, p2_bold, p2_txt3 //        <p>Paragraph 2: <b>bold text</b></p>
                                            //      </div>
                                            //   </body>
        }
    );

    return std::move( pre_order );
}

TEST( parser_dom_NodeIterator_Tests, iteration_0 ) { //simple pre-order forward prefix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = createTestDocument( document );

    size_t node_i = 0;
    auto   it     = document->begin();

    while( it != document->end() && node_i < pre_order.size() ) {
        ASSERT_EQ( it.node(), pre_order[node_i] );
        ++it;
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order.size() );
    ASSERT_TRUE( it.done() );
}

TEST( parser_dom_NodeIterator_Tests, iteration_1 ) { //simple pre-order forward postfix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = createTestDocument( document );

    //increment
    size_t node_i = 0;
    auto   it     = document->begin();

    while( it != document->end() && node_i < pre_order.size() ) {
        ASSERT_EQ( ( it++ ).node(), pre_order[node_i] );
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    node_i = 0;
    ASSERT_EQ( it--, document->end() );

    while( node_i < pre_order.size() ) {
        auto j = pre_order.size() - 1 - node_i;
        ASSERT_EQ( ( it-- ).node(), pre_order[j] );
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order.size() );
    ASSERT_EQ( it--, document->begin() ); //consistency check
}

TEST( parser_dom_NodeIterator_Tests, iteration_2 ) { //filtered forward iteration
    std::unique_ptr<node::Node> document;
    auto pre_order      = createTestDocument( document );
    auto pre_order_text = std::vector<node::Node *>();

    for( auto * ptr : pre_order ) {
        if( ptr && ptr->nodeType() == NodeType::TEXT_NODE ) {
            pre_order_text.emplace_back( ptr );
        }
    }

    //increment
    size_t node_i = 0;
    auto   it     = document->begin( NodeFilter( NodeFilter::SHOW_TEXT ) );

    while( it != document->end() ) {
        ASSERT_EQ( it.node(), pre_order_text[node_i] );
        ++it;
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order_text.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    node_i = 0;

    while( node_i < pre_order_text.size() ) {
        auto j = pre_order_text.size() - 1 - node_i;
        --it;
        ASSERT_EQ( it.node(), pre_order_text[j] );
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order_text.size() );
}

TEST( parser_dom_NodeIterator_Tests, reverse_iteration_0 ) { //simple pre-order backward prefix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = createTestDocument( document );

    size_t node_count = 0;
    size_t node_i     = pre_order.size() - 1; //@ last pre-order node
    auto   it         = document->rbegin();

    while( it != document->rend() ) {
        ASSERT_EQ( it.node(), pre_order[ node_i ] );
        ++it;
        ++node_count;
        --node_i;
    }

    ASSERT_EQ( node_count, pre_order.size() );
    ASSERT_TRUE( it.done() );
}

TEST( parser_dom_NodeIterator_Tests, reverse_iteration_1 ) { //simple pre-order backward postfix iteration
    std::unique_ptr<node::Node> document;
    auto pre_order = createTestDocument( document );

    //increment
    size_t node_count = 0;
    size_t node_i     = pre_order.size() - 1; //@ last pre-order node
    auto   it         = document->rbegin();

    while( it != document->rend() ) {
        ASSERT_EQ( ( it++ ).node(), pre_order[ node_i ] );
        ++node_count;
        --node_i;
    }

    ASSERT_EQ( node_count, pre_order.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    node_i = 0;
    ASSERT_EQ( it--, document->rend() );

    while( node_i < pre_order.size() ) {
        ASSERT_EQ( ( it-- ).node(), pre_order[node_i] );
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order.size() );
    ASSERT_EQ( it--, document->rbegin() ); //consistency check
}

TEST( parser_dom_NodeIterator_Tests, reverse_iteration_2 ) { //filtered backward iteration
    std::unique_ptr<node::Node> document;
    auto pre_order      = createTestDocument( document );
    auto pre_order_text = std::vector<node::Node *>();

    for( auto * ptr : pre_order ) {
        if( ptr && ptr->nodeType() == NodeType::TEXT_NODE ) {
            pre_order_text.emplace_back( ptr );
        }
    }

    //increment
    size_t node_count = 0;
    size_t node_i     = pre_order_text.size() - 1; //@ last pre-order node matching the filter
    auto   it         = document->rbegin( NodeFilter( NodeFilter::SHOW_TEXT ) );

    while( it != document->rend() ) {
        ASSERT_EQ( it.node(), pre_order_text[node_i] );
        ++it;
        ++node_count;
        --node_i;
    }

    ASSERT_EQ( node_count, pre_order_text.size() );
    ASSERT_TRUE( it.done() );

    //decrement
    node_i = 0;

    while( node_i < pre_order_text.size() ) {
        --it;
        ASSERT_EQ( it.node(), pre_order_text[node_i] );
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order_text.size() );
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

    ASSERT_NO_THROW( begin->nodeType() );
    ASSERT_NO_THROW( rbegin->nodeType() );
    ASSERT_THROW( end->nodeType(), std::runtime_error );
    ASSERT_THROW( rend->nodeType(), std::runtime_error );
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
    auto pre_order = createTestDocument( document );

    size_t node_i = 0;
    auto   it     = document->begin();

    while( it != document->end() && node_i < pre_order.size() ) {
        ASSERT_EQ( it.nextNode(), pre_order[node_i] );
        ++node_i;
    }

    ASSERT_EQ( it, document->end() );
    ASSERT_EQ( node_i, pre_order.size() );
    ASSERT_TRUE( it.done() );
}

TEST( parser_dom_NodeIterator_Tests, previousNode ) {
    std::unique_ptr<node::Node> document;
    auto pre_order = createTestDocument( document );

    size_t node_i = 0;
    auto   it     = document->end();

    while( node_i < pre_order.size() ) {
        auto j    = pre_order.size() - 1 - node_i;
        auto node = it.previousNode();
        ASSERT_EQ( node, pre_order[j] );
        ++node_i;
    }

    ASSERT_EQ( node_i, pre_order.size() );
    ASSERT_EQ( it.previousNode(), nullptr ); //trying to go back from 1st node
    ASSERT_EQ( it.previousNode(), nullptr ); //consistency check
}
