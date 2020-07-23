#include "gtest/gtest.h"
#include "../../src/dom/html5/Html5Properties.h"

TEST( Html5Properties_Tests, strToTag_invalid_arg ) {
    using blogator::dom::html5::Html5Properties;
    ASSERT_THROW( Html5Properties::strToTag( U"invalid_tag" ), std::invalid_argument );
}

TEST( Html5Properties_Tests, strToTag_short ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( Tag::A, Html5Properties::strToTag( U"a" ) );
    ASSERT_EQ( Tag::A, Html5Properties::strToTag( U"A" ) );
}

TEST( Html5Properties_Tests, strToTag_long ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( Tag::ADDRESS, Html5Properties::strToTag( U"address" ) );
    ASSERT_EQ( Tag::ADDRESS, Html5Properties::strToTag( U"ADDRESS" ) );
    ASSERT_EQ( Tag::ADDRESS, Html5Properties::strToTag( U"AdDrEsS" ) );
}

TEST( Html5Properties_Tests, strToAttribute_datax ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( Attribute::DATA_X, Html5Properties::strToAttribute( U"data-" ) );
    ASSERT_EQ( Attribute::DATA_X, Html5Properties::strToAttribute( U"data-whatever" ) );
    ASSERT_THROW( Html5Properties::strToAttribute( U"datawhatever" ), std::invalid_argument );
}

TEST( Html5Properties_Tests, strToAttribute_ariax ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( Attribute::ARIA_X, Html5Properties::strToAttribute( U"aria-" ) );
    ASSERT_EQ( Attribute::ARIA_X, Html5Properties::strToAttribute( U"aria-whatever" ) );
    ASSERT_THROW( Html5Properties::strToAttribute( U"ariawhatever" ), std::invalid_argument );
}

TEST( Html5Properties_Tests, strToAttribute_xmlns ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( Attribute::XMLNS, Html5Properties::strToAttribute( U"xmlns" ) );
    ASSERT_EQ( Attribute::XMLNS, Html5Properties::strToAttribute( U"xmlns:whatever" ) );
    ASSERT_THROW( Html5Properties::strToAttribute( U"xmlnswhatever" ), std::invalid_argument );
}

TEST( Html5Properties_Tests, AttributeEnum_consistency_check ) {
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( static_cast<int>( Attribute::ENUM_END ), Html5Properties::attributeCount() );
}


TEST( Html5Properties_Tests, TagEnum_consistency_check ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( static_cast<int>( Tag::ENUM_END ), Html5Properties::tagCount() );
}

TEST( Html5Properties_Tests, ContentEnum_consistency_check ) {
    using blogator::dom::html5::ContentModel;
    using blogator::dom::html5::Html5Properties;

    ASSERT_EQ( static_cast<int>( ContentModel::ENUM_END ), Html5Properties::contentModelCount() );
}

TEST( Html5Properties_Tests, AttributeEnum_enum2str_map_test ) { //checks all enums have a corresponding entry in the Attribute->string map
    using blogator::dom::html5::Attribute;
    using blogator::dom::html5::Html5Properties;

    for( auto i = 0; i < static_cast<int>( Attribute::ENUM_END ); ++i )
        ASSERT_NO_THROW( Html5Properties::attributeToStr( static_cast<Attribute>( i ) ) );
}