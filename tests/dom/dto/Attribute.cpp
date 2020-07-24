#include "gtest/gtest.h"
#include "../../../src/dom/dto/Attribute.h"

TEST( dto_Attribute_Tests, checkNestedBoundaryChar_pass0 ) {
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;

    auto str = U"";
    ASSERT_EQ( AttrBoundaryChar::NONE, Attribute::checkNestedBoundaryChar( str ) );
}

TEST( dto_Attribute_Tests, checkNestedBoundaryChar_pass1 ) {
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;

    auto str = U"attribute-name";
    ASSERT_EQ( AttrBoundaryChar::NONE, Attribute::checkNestedBoundaryChar( str ) );
}

TEST( dto_Attribute_Tests, checkNestedBoundaryChar_pass2 ) {
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;

    auto str = U"attribute's name 'attrnm'";
    ASSERT_EQ( AttrBoundaryChar::APOSTROPHE, Attribute::checkNestedBoundaryChar( str ) );
}

TEST( dto_Attribute_Tests, checkNestedBoundaryChar_pass3 ) {
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;

    auto str = U"attribute \"name\"";
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, Attribute::checkNestedBoundaryChar( str ) );
}

TEST( dto_Attribute_Tests, checkNestedBoundaryChar_fail0 ) {
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;

    auto str = U"attribute's \"name\"";
    ASSERT_THROW( Attribute::checkNestedBoundaryChar( str ), std::invalid_argument );
}

TEST( dto_Attribute_Tests, checkNestedBoundaryChar_fail1 ) {
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;

    auto str = U"\"named\" attributes's";
    ASSERT_THROW( Attribute::checkNestedBoundaryChar( str ), std::invalid_argument );
}