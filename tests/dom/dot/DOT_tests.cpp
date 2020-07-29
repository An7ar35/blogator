#include "gtest/gtest.h"
#include "../../../src/dom/dot/DOT.h"

//class DOT_Tests : public testing::Test {
//  protected:
//    void SetUp() override { dot = std::make_unique<blogator::dom::DOT>(); }
//    void TearDown() override {}
//  public:
//    std::unique_ptr<blogator::dom::DOT> dot;
//};

//TEST_F( DOT_Tests, getSpecs ) {
//    using blogator::dom::html5::Tag;
//    using blogator::dom::html5::TagStructure;
//
//    auto p1 = dot->getSpecs( blogator::html::dom::HtmlTag::Type::A );
//    ASSERT_EQ( Structure::PAIRED, p1->structure );
//    ASSERT_EQ( "<a" ,p1->opening_begin );
//    ASSERT_EQ( ">" ,p1->opening_end );
//    ASSERT_EQ( "</a>" ,p1->closing );
//    auto p2 = dot->getSpecs( blogator::html::dom::HtmlTag::Type::A );
//    ASSERT_EQ( p1, p2 );
//}

TEST( DOT_Tests, constructor_injector ) {
    using blogator::dom::DOT;
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
    n5->appendAttribute( U"class", U"nested-link" );

    auto dot = DOT( std::move( root ) );

    std::string expected = R"(<html><body><h1>Test heading</h1><ul class="my-ul"><li class="my-li" id="link1"><a class="my-link nested-link" href="http://start.duckduckgo.com">Click here!</a></li><li>list item 2</li><li>list item 3</li></ul></body></html>)";
    std::stringstream ss;
    ss << dot;
    ASSERT_EQ( expected, ss.str() );

    auto v_el = dot.getElementByClass( U"nested-link" );
    auto &el  = dot.getElementById( U"link1" );

    std::cout << el << std::endl;
    std::cout << *v_el.at( 0 ) << std::endl;
    ASSERT_EQ( el, *( v_el.at( 0 )->parent() ) );
}

TEST( DOT_Tests, constructor_path ) {
    using blogator::dom::DOT;
    using blogator::dom::DOTNode;
    using blogator::dom::DOTSetting;
    using blogator::dom::html5::Tag;

    auto dot = DOT( { }, std::filesystem::path( "../tests/dom/parser/text1.html" ) );
    std::cout << dot << std::endl;
    FAIL();
}

TEST( DOT_Tests, constructor_text ) {
    using blogator::dom::DOT;
    using blogator::dom::DOTConfig;
    using blogator::dom::DOTSetting;

    auto text = blogator::dom::dto::Text(
        U"<weird paragraph> text </weird> everything is going to",
        " break<p> oh fml...</p>"
    );

    auto dot = DOT( DOTConfig( DOTSetting::HTML5_PARTIAL ), text );
    std::cout << dot << std::endl;
    FAIL();
}