#include "gtest/gtest.h"
#include "../../../../src/dom/html5/rules/Rules.h"
#include "../../../../src/encoding/encoding.h"

TEST( html5_Rules_Tests, strToTag_invalid_arg ) {
    using blogator::dom::html5::Rules;
    ASSERT_THROW( Rules::getInstance().strToTag( U"invalid_tag" ), std::invalid_argument );
}

TEST( html5_Rules_Tests, strToTag_short ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Rules;

    ASSERT_EQ( Tag::A, Rules::getInstance().strToTag( U"a" ) );
    ASSERT_EQ( Tag::A, Rules::getInstance().strToTag( U"A" ) );
}

TEST( html5_Rules_Tests, strToTag_long ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Rules;

    ASSERT_EQ( Tag::ADDRESS, Rules::getInstance().strToTag( U"address" ) );
    ASSERT_EQ( Tag::ADDRESS, Rules::getInstance().strToTag( U"ADDRESS" ) );
    ASSERT_EQ( Tag::ADDRESS, Rules::getInstance().strToTag( U"AdDrEsS" ) );
}

TEST( html5_Rules_Tests, TagEnum_consistency_check ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Rules;

    ASSERT_EQ( static_cast<int>( Tag::ENUM_END ), Rules::getInstance().tagCount() );
}

TEST( html5_Rules_Tests, AttributeEnum_consistency_check ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::Rules;

    ASSERT_EQ( static_cast<int>( Attribute::ENUM_END ), Rules::getInstance().attributeCount() );
}

TEST( html5_Rules_Tests, TagEnum_enum2str_map_test ) { //checks all enums have a corresponding entry in the Tag->string map
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Rules;

    for( auto i = 0; i < static_cast<int>( Tag::ENUM_END ); ++i )
        ASSERT_NO_THROW( Rules::getInstance().tagToStr( static_cast<Tag>( i ) ) );
}

TEST( html5_Rules_Tests, AttributeEnum_enum2str_map_test ) { //checks all enums have a corresponding entry in the Attribute->string map
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::Rules;

    for( auto i = 0; i < static_cast<int>( Attribute::ENUM_END ); ++i )
        ASSERT_NO_THROW( Rules::getInstance().attributeToStr( static_cast<Attribute>( i ) ) );
}