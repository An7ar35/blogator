#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Text.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;

/**
 * Corruptible `Text` node
 * Its internal pointers can be set post append to create a tree corruption state
 * since `appendChild(..)` auto-corrects
 */
class CorruptibleTextNode : public node::Text {
  public:
    explicit CorruptibleTextNode( DOMString_t str ) :
        node::Text( std::move( str ), nullptr, nullptr )
    {}

    void setPointers( node::Node * parent, node::Node * prev = nullptr, node::Node * next = nullptr ) {
        this->setParent( parent );

        if( prev )
            this->setPrevSibling( prev );

        if( next )
            this->setNextSibling( next );
    }
};

TEST( parser_dom_node_Text_Tests, splitText_0 ) { //empty/no offset - should return pointer to original node
    DOMString_t  txt    = U"";
    auto         parent = node::Node();
    node::Node * child  = parent.appendChild( std::make_unique<node::Text>( txt, &parent, nullptr ) );

    auto split = dynamic_cast<node::Text *>( child )->splitText( 0 );

    ASSERT_EQ( split, child );
    ASSERT_EQ( split->data(), txt );
    ASSERT_EQ( parent.childNodes().size(), 1 );
}

TEST( parser_dom_node_Text_Tests, splitText_1 ) { //no offset - should return pointer to original node
    DOMString_t  txt    = U"123";
    auto         parent = node::Node();
    node::Node * child  = parent.appendChild( std::make_unique<node::Text>( txt, &parent, nullptr ) );

    auto split = dynamic_cast<node::Text *>( child )->splitText( 0 );

    ASSERT_EQ( split, child );
    ASSERT_EQ( split->data(), txt );
    ASSERT_EQ( parent.childNodes().size(), 1 );
}

TEST( parser_dom_node_Text_Tests, splitText_2 ) { //offset == length
    DOMString_t  txt    = U"12345";
    auto         parent = node::Node();
    node::Node * child  = parent.appendChild( std::make_unique<node::Text>( txt, &parent, nullptr ) );

    auto split = dynamic_cast<node::Text *>( child )->splitText( txt.size() );

    ASSERT_EQ( split, child );
    ASSERT_EQ( split->data(), U"12345" );
    ASSERT_EQ( parent.childNodes().size(), 1 );
}

TEST( parser_dom_node_Text_Tests, splitText_3 ) { //lower bound
    DOMString_t  txt    = U"12345";
    auto         parent = node::Node();
    node::Node * child  = parent.appendChild( std::make_unique<node::Text>( txt, &parent, nullptr ) );

    auto split = dynamic_cast<node::Text *>( child )->splitText( 1 );

    ASSERT_NE( split, child );
    ASSERT_EQ( dynamic_cast<node::Text *>( child )->data(), U"1" );
    ASSERT_EQ( split->data(), U"2345" );
    ASSERT_EQ( parent.childNodes().size(), 2 );
}

TEST( parser_dom_node_Text_Tests, splitText_4 ) { //upper bound
    DOMString_t  txt    = U"12345";
    auto         parent = node::Node();
    node::Node * child  = parent.appendChild( std::make_unique<node::Text>( txt, &parent, nullptr ) );

    auto split = dynamic_cast<node::Text *>( child )->splitText( txt.size() - 1 );

    ASSERT_NE( split, child );
    ASSERT_EQ( dynamic_cast<node::Text *>( child )->data(), U"1234" );
    ASSERT_EQ( split->data(), U"5" );
    ASSERT_EQ( parent.childNodes().size(), 2 );
}

TEST( parser_dom_node_Text_Tests, splitText_5 ) { //middle offset
    DOMString_t  txt    = U"12345";
    auto         parent = node::Node();
    node::Node * child  = parent.appendChild( std::make_unique<node::Text>( txt, &parent, nullptr ) );

    auto split = dynamic_cast<node::Text *>( child )->splitText( 3 );

    ASSERT_NE( split, child );
    ASSERT_EQ( dynamic_cast<node::Text *>( child )->data(), U"123" );
    ASSERT_EQ( split->data(), U"45" );
    ASSERT_EQ( parent.childNodes().size(), 2 );
}


TEST( parser_dom_node_Text_Tests, splitText_fail_0 ) { //bad offset - throw `blogator::parser::dom::exception::RangeError`
    using blogator::parser::dom::exception::RangeError;

    DOMString_t  txt    = U"123";
    auto         parent = node::Node();
    node::Node * child  = parent.appendChild( std::make_unique<node::Text>( txt, &parent, nullptr ) );

    EXPECT_THROW( dynamic_cast<node::Text *>( child )->splitText( 4 ), RangeError );
}

TEST( parser_dom_node_Text_Tests, splitText_fail_1 ) { //no parent - throw `blogator::exception::failed_expectation`
    using blogator::exception::failed_expectation;

    DOMString_t  txt    = U"123";
    auto child  = std::make_unique<node::Text>( txt, nullptr, nullptr ); //no parent/orphaned

    EXPECT_THROW( child->splitText( 2 ), failed_expectation );
}

TEST( parser_dom_node_Text_Tests, splitText_fail_2 ) { //wrong parent - throw `blogator::exception::failed_expectation`
    using blogator::exception::failed_expectation;

    DOMString_t  txt         = U"123";
    auto         real_parent = node::Node();
    auto         fake_parent = node::Node();
    node::Node * child       = real_parent.appendChild( std::make_unique<CorruptibleTextNode>( txt ) );
    node::Node * sibling     = real_parent.appendChild( std::make_unique<node::Text>( txt, &real_parent, child ) );

    dynamic_cast<CorruptibleTextNode *>( child )->setPointers( &fake_parent ); //corrupt parent

    EXPECT_THROW( dynamic_cast<node::Text *>( child )->splitText( 2 ), failed_expectation );
}

TEST( parser_dom_node_Text_Tests, splitText_fail_3 ) { //wrong sibling parent - throw `blogator::exception::failed_expectation`
    using blogator::exception::failed_expectation;

    //inverse order of creation (on stack) to avoid segfault on Node destruction due to our little 'forced' corruption
    auto         parent_b     = node::Node();
    node::Node * fake_sibling = parent_b.appendChild( std::make_unique<CorruptibleTextNode>( U"sibling" ) );
    auto         parent_a     = node::Node();
    node::Node * child        = parent_a.appendChild( std::make_unique<CorruptibleTextNode>( U"child" ) );

    dynamic_cast<CorruptibleTextNode *>( child )->setPointers( &parent_a, nullptr, fake_sibling ); //corrupt sibling pointer

    //   [A]         [B]
    //    |           |
    // [child]->[fake_sibling]

    EXPECT_THROW( dynamic_cast<node::Text *>( child )->splitText( 2 ), failed_expectation );
}

TEST( parser_dom_node_Text_Tests, wholeText_0 ) { //no text
    auto text = DOMString_t( U"" );
    auto node = node::Text( text, nullptr, nullptr, nullptr );

    ASSERT_EQ( node.wholeText(), text );
}

TEST( parser_dom_node_Text_Tests, wholeText_1 ) { //self
    auto text = DOMString_t( U"testing..." );
    auto node = node::Text( text, nullptr, nullptr, nullptr );

    ASSERT_EQ( node.wholeText(), text );
}

TEST( parser_dom_node_Text_Tests, wholeText_2 ) { //consecutive Text nodes
    auto strings = std::vector<DOMString_t>{ U"A", U"B", U"C", U"E" };
    auto parent  = node::Node();
    auto child0 = parent.appendChild( std::make_unique<node::Text>( strings[0], &parent, nullptr ) );
    auto child1 = parent.appendChild( std::make_unique<node::Text>( strings[1], &parent, child0 ) );
    auto child2 = parent.appendChild( std::make_unique<node::Text>( strings[2], &parent, child1 ) );
    auto child3 = parent.appendChild( std::make_unique<node::Node>( NodeType::UNDEFINED, &parent, child2 ) );
    parent.appendChild( std::make_unique<node::Text>( strings[3], &parent, child3 ) );

    auto expected = strings[0] + strings[1] + strings[2]; //"ABC"
    auto returned = dynamic_cast<node::Text *>( child0 )->wholeText();
    ASSERT_EQ( returned, expected );
}

TEST( parser_dom_node_Text_Tests, nodeName ) {
    auto node = node::Text( U"", nullptr, nullptr );
    ASSERT_EQ( node.nodeName(), U"#text" );
}