#include "gtest/gtest.h"
#include "../../../../src/dom/html5/validator/validator.h"

TEST( html5_validator_Tests, isTagEnd_pass ) {
    ASSERT_TRUE( blogator::dom::html5::validator::isTagEnd( U'>' ) );
}

TEST( html5_validator_Tests, isTagEnd_fail ) {
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U' ' ) );
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U'<' ) );
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U'/' ) );
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U'a' ) );
}

TEST( html5_validator_Tests, isValidCharReference1 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_TRUE( isValidCharReference( U"&amp;" ) );
}

TEST( html5_validator_Tests, isValidCharReference2 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_TRUE( isValidCharReference( U"&#38;" ) );
}

TEST( html5_validator_Tests, isValidCharReference3 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_TRUE( isValidCharReference( U"&#xC39E;" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail0 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail1 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&amp" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail2 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"amp;" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail3 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&amp;d" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail4 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"x&amp;" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail5 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&;" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail6 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&#;" ) );
}

TEST( html5_validator_Tests, isValidCharReference_fail7 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&#x;" ) );
}

TEST( html5_validator_Tests, escapeReservedChar_reserved ) {
    using blogator::dom::html5::validator::escapeReservedChar;

    ASSERT_EQ( U"&amp;",  escapeReservedChar( U'&' ) );
    ASSERT_EQ( U"&lt;",   escapeReservedChar( U'<' ) );
    ASSERT_EQ( U"&gt;",   escapeReservedChar( U'>' ) );
    ASSERT_EQ( U"&quot;", escapeReservedChar( U'\"' ) );
    ASSERT_EQ( U"&apos;", escapeReservedChar( U'\'' ) );
}

TEST( html5_validator_Tests, escapeReservedChar_unreserved ) {
    using blogator::dom::html5::validator::escapeReservedChar;

    ASSERT_EQ( U"&#x5C;", escapeReservedChar( static_cast<char32_t>( 92 ) ) );
    ASSERT_EQ( U"&#x2265;", escapeReservedChar( static_cast<char32_t>( 8805 ) ) );
    ASSERT_EQ( U"&#x2995;", escapeReservedChar( static_cast<char32_t>( 10645 ) ) );
    ASSERT_EQ( U"&#x1D54A;", escapeReservedChar( static_cast<char32_t>( 120138 ) ) );
}