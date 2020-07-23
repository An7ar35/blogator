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