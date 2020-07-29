#include "gtest/gtest.h"
#include "../../src/dom/html5/kb/KnowledgeBase.h"

TEST( KnowledgeBase_Tests, strToTag_invalid_arg ) {
    using blogator::dom::html5::kb::KnowledgeBase;
    ASSERT_THROW( KnowledgeBase::strToTag( U"invalid_tag" ), std::invalid_argument );
}

TEST( KnowledgeBase_Tests, strToTag_short ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_EQ( Tag::A, KnowledgeBase::strToTag( U"a" ) );
    ASSERT_EQ( Tag::A, KnowledgeBase::strToTag( U"A" ) );
}

TEST( KnowledgeBase_Tests, strToTag_long ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_EQ( Tag::ADDRESS, KnowledgeBase::strToTag( U"address" ) );
    ASSERT_EQ( Tag::ADDRESS, KnowledgeBase::strToTag( U"ADDRESS" ) );
    ASSERT_EQ( Tag::ADDRESS, KnowledgeBase::strToTag( U"AdDrEsS" ) );
}

TEST( KnowledgeBase_Tests, strToAttribute_datax ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_EQ( Attribute::DATA_X, KnowledgeBase::strToAttribute( U"data-" ) );
    ASSERT_EQ( Attribute::DATA_X, KnowledgeBase::strToAttribute( U"data-whatever" ) );
    ASSERT_THROW( KnowledgeBase::strToAttribute( U"datawhatever" ), std::invalid_argument );
}

TEST( KnowledgeBase_Tests, strToAttribute_ariax ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_EQ( Attribute::ARIA_X, KnowledgeBase::strToAttribute( U"aria-" ) );
    ASSERT_EQ( Attribute::ARIA_X, KnowledgeBase::strToAttribute( U"aria-whatever" ) );
    ASSERT_THROW( KnowledgeBase::strToAttribute( U"ariawhatever" ), std::invalid_argument );
}

TEST( KnowledgeBase_Tests, strToAttribute_xmlns ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_EQ( Attribute::XMLNS, KnowledgeBase::strToAttribute( U"xmlns" ) );
    ASSERT_EQ( Attribute::XMLNS, KnowledgeBase::strToAttribute( U"xmlns:whatever" ) );
    ASSERT_THROW( KnowledgeBase::strToAttribute( U"xmlnswhatever" ), std::invalid_argument );
}

TEST( KnowledgeBase_Tests, AttributeEnum_consistency_check ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_EQ( static_cast<int>( Attribute::ENUM_END ), KnowledgeBase::attributeCount() );
}


TEST( KnowledgeBase_Tests, TagEnum_consistency_check ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_EQ( static_cast<int>( Tag::ENUM_END ), KnowledgeBase::tagCount() )
                        << "Not all Tags have matching specifications written in the \"dom/html5/properties/tag.cpp\" src file.";
}

TEST( KnowledgeBase_Tests, AttributeEnum_enum2str_map_test ) { //checks all enums have a corresponding entry in the Attribute->string map
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::kb::KnowledgeBase;

    for( auto i = 0; i < static_cast<int>( Attribute::ENUM_END ); ++i )
        ASSERT_NO_THROW( KnowledgeBase::attributeToStr( static_cast<Attribute>( i ) ) );
}

TEST( KnowledgeBase_Tests, belongsToNamespace ) {
    using blogator::dom::html5::NameSpace;
    using blogator::dom::html5::kb::KnowledgeBase;

    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::NONE, NameSpace::AGNOSTIC ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::NONE, NameSpace::BLOGATOR ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::NONE, NameSpace::HTML5 ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::NONE, NameSpace::MathML ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::NONE, NameSpace::SVG ) );
    ASSERT_FALSE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::A, NameSpace::BLOGATOR ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::A, NameSpace::HTML5 ) );
    ASSERT_FALSE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::A, NameSpace::MathML ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::A, NameSpace::SVG ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::SVG, NameSpace::SVG ) );
    ASSERT_TRUE( KnowledgeBase::belongsToNamespace( blogator::dom::html5::Tag::MATH, NameSpace::MathML ) );
}

TEST( KnowledgeBase_Tests, isNamespaceAgnostic ) {
    ASSERT_TRUE( blogator::dom::html5::kb::KnowledgeBase::isNamespaceAgnostic( blogator::dom::html5::Tag::NONE ) );
    ASSERT_FALSE( blogator::dom::html5::kb::KnowledgeBase::isNamespaceAgnostic( blogator::dom::html5::Tag::MATH ) );
}