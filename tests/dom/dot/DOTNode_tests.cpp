#include "gtest/gtest.h"
#include "../../../src/dom/dot/DOTNode.h"

TEST( DOTNode_Tests, dfs_print ) {
    using blogator::dom::DOTNode;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;

    auto root = std::make_unique<DOTNode>( Tag::HTML );
    auto n1  = root->addChild( std::make_unique<DOTNode>( Tag::BODY ) );
    auto n2  = n1->addChild( std::make_unique<DOTNode>( Tag::H1, U"Test heading" ) );
    auto n3  = n1->addChild( std::make_unique<DOTNode>( Tag::UL ) );
    n3->addAttribute( U"class", Attribute { U"my-ul", AttrBoundaryChar::QUOTATION_MARK } );
    auto n4  = n3->addChild( std::make_unique<DOTNode>( Tag::LI ) );
    n3->addChild( std::make_unique<DOTNode>( Tag::LI, U"list item 2" ));
    n3->addChild( std::make_unique<DOTNode>( Tag::LI, U"list item 3" ));
    n4->addAttribute( U"id", Attribute { U"link1", AttrBoundaryChar::QUOTATION_MARK } );
    n4->addAttribute( U"class", Attribute { U"my-li", AttrBoundaryChar::QUOTATION_MARK } );
    auto n5  = n4->addChild( std::make_unique<DOTNode>( Tag::A, U"Click here!" ) );
    n5->addAttribute( U"href", Attribute { U"http://start.duckduckgo.com", AttrBoundaryChar::QUOTATION_MARK } );
    n5->addAttribute( U"class", Attribute { U"my-link", AttrBoundaryChar::QUOTATION_MARK } );
    n5->addAttribute( U"class", Attribute { U"nested-link", AttrBoundaryChar::QUOTATION_MARK } );

    std::string expected = "<html><body><h1>Test heading</h1><ul class=\"my-ul\"><li class=\"my-li\" id=\"link1\"><a class=\"my-link nested-link\" href=\"http://start.duckduckgo.com\">Click here!</a></li><li>list item 2</li><li>list item 3</li></ul></body></html>";
    std::stringstream ss;
    ss << *root;
    ASSERT_EQ( expected, ss.str() );
}

TEST( DOTNode_Tests, equality_operator ) {
    using blogator::dom::DOTNode;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;

    auto  a0 = std::make_unique<DOTNode>( Tag::DIV );
    auto a1 = a0->addChild( std::make_unique<DOTNode>( Tag::P, U"div content" ) );
    auto a2 = a0->addChild( std::make_unique<DOTNode>( Tag::I, U"italic content" ) );
    a2->addAttribute( U"class", Attribute { U"some-class", AttrBoundaryChar::QUOTATION_MARK } );
    auto a3 = a1->addChild( std::make_unique<DOTNode>( Tag::SPAN, U"span content" ) );

    auto  b0 = std::make_unique<DOTNode>( Tag::DIV );
    auto b1 = b0->addChild( std::make_unique<DOTNode>( Tag::P, U"div content" ) );
    auto b2 = b0->addChild( std::make_unique<DOTNode>( Tag::I, U"italic content" ) );
    b2->addAttribute( U"class", Attribute { U"some-class", AttrBoundaryChar::QUOTATION_MARK } );
    auto b3 = b1->addChild( std::make_unique<DOTNode>( Tag::SPAN, U"span content" ) );

    ASSERT_EQ( *a0, *b0 );
}

TEST( DOTNode_Tests, not_equal_operator ) {
    using blogator::dom::DOTNode;
    using blogator::dom::html5::Tag;

    auto  a0 = std::make_unique<DOTNode>( Tag::DIV );
    auto a1 = a0->addChild( std::make_unique<DOTNode>( Tag::P, U"div content" ) );
    auto a3 = a1->addChild( std::make_unique<DOTNode>( Tag::SPAN, U"span content" ) );

    auto  b0 = std::make_unique<DOTNode>( Tag::DIV );
    auto b1 = b0->addChild( std::make_unique<DOTNode>( Tag::P, U"div content" ) );
    auto b2 = b0->addChild( std::make_unique<DOTNode>( Tag::I, U"italic content" ) );
    auto b3 = b1->addChild( std::make_unique<DOTNode>( Tag::SPAN, U"span content" ) );

    ASSERT_NE( *a0, *b0 );
}

TEST( DOTNode_Tests, recursiveCopy ) {
    using blogator::dom::DOTNode;
    using blogator::dom::html5::Tag;

    auto  a0 = std::make_unique<DOTNode>( Tag::DIV );
    auto a1 = a0->addChild( std::make_unique<DOTNode>( Tag::P, U"div content" ) );
    auto a2 = a0->addChild( std::make_unique<DOTNode>( Tag::I, U"italic content" ) );
    auto a3 = a1->addChild( std::make_unique<DOTNode>( Tag::SPAN, U"span content" ) );

    auto copy = a0->recursiveCopy();

    ASSERT_EQ( *a0, *copy );
}

TEST( DOTNode_Tests, addChild ) {
    using blogator::dom::DOTNode;
    using blogator::dom::html5::Tag;

    auto root  = std::make_unique<DOTNode>( Tag::HTML );
    root->addChild( std::make_unique<DOTNode>( Tag::BODY ) );
    root->addChild( std::make_unique<DOTNode>( Tag::DIV ) );
    root->addChild( std::make_unique<DOTNode>( Tag::P ) );

    ASSERT_EQ( 3, root->childrenCount() );
}

TEST( DOTNode_Tests, attribute_fail ) {
    using blogator::dom::DOTNode;
    using blogator::dom::html5::Tag;
    using blogator::exception::DOMException;

    auto node = DOTNode( Tag::DIV );
    EXPECT_THROW( node.attribute( U"class" ), DOMException );
}

TEST( DOTNode_Tests, addAttribute1 ) {
    using blogator::dom::DOTNode;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;

    auto node = DOTNode( Tag::DIV );
    ASSERT_TRUE( node.addAttribute( U"id", Attribute { U"my-div", AttrBoundaryChar::QUOTATION_MARK } ) );
    ASSERT_EQ( U"my-div", node.attribute( U"id" ) );
}

TEST( DOTNode_Tests, addAttribute2 ) {
    using blogator::dom::DOTNode;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;

    auto node = DOTNode( Tag::DIV );
    ASSERT_TRUE( node.addAttribute( U"class", Attribute { U"block left", AttrBoundaryChar::NONE } ) );
    ASSERT_EQ( U"block left", node.attribute( U"class" ) );
    ASSERT_FALSE( node.addAttribute( U"class", Attribute { U"bordered", AttrBoundaryChar::NONE } ) );
    ASSERT_EQ( U"block left bordered", node.attribute( U"class" ) );
}

TEST( DOTNode_Tests, replaceAttribute_fail ) {
    using blogator::dom::DOTNode;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;
    using blogator::exception::DOMException;

    auto node = DOTNode( Tag::DIV );
    ASSERT_THROW( node.replaceAttribute( U"class", Attribute { U"centered", AttrBoundaryChar::QUOTATION_MARK } ), DOMException );
}

TEST( DOTNode_Tests, replaceAttribute ) {
    using blogator::dom::DOTNode;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;

    auto node = DOTNode( Tag::DIV );
    ASSERT_TRUE( node.addAttribute( U"class", Attribute { U"original", AttrBoundaryChar::QUOTATION_MARK } ) );
    ASSERT_EQ( U"original", node.attribute( U"class" ) );
    node.replaceAttribute( U"class", Attribute { U"modified", AttrBoundaryChar::QUOTATION_MARK } );
    ASSERT_EQ( U"modified", node.attribute( U"class" ) );
}