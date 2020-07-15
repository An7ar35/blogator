#include "gtest/gtest.h"
#include "../../../../src/dom/html5/validator/validator.h"
#include "../../../../src/exception/DOMException.h"

TEST( HTML5rules_Tests, isTagEnd_pass ) {
    ASSERT_TRUE( blogator::dom::html5::validator::isTagEnd( U'>' ) );
}

TEST( HTML5rules_Tests, isTagEnd_fail ) {
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U' ' ) );
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U'<' ) );
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U'/' ) );
    ASSERT_FALSE( blogator::dom::html5::validator::isTagEnd( U'a' ) );
}

TEST( HTML5rules_Tests, isValidCharReference1 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_TRUE( isValidCharReference( U"&amp;" ) );
}

TEST( HTML5rules_Tests, isValidCharReference2 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_TRUE( isValidCharReference( U"&#38;" ) );
}

TEST( HTML5rules_Tests, isValidCharReference3 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_TRUE( isValidCharReference( U"&#xC39E;" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail0 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail1 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&amp" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail2 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"amp;" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail3 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&amp;d" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail4 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"x&amp;" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail5 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&;" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail6 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&#;" ) );
}

TEST( HTML5rules_Tests, isValidCharReference_fail7 ) {
    using blogator::dom::html5::validator::isValidCharReference;
    ASSERT_FALSE( isValidCharReference( U"&#x;" ) );
}

TEST( HTML5rules_Tests, validateOpeningTag ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<>";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_EQ( Tag::NONE, validateOpeningTag( str, v ) );
}

TEST( HTML5rules_Tests, validateOpeningTag_fail0 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateOpeningTag( str, v ), DOMException );
}

TEST( HTML5rules_Tests, validateOpeningTag_fail1 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<>";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateOpeningTag( str, v ), DOMException );
}

TEST( HTML5rules_Tests, validateOpeningTag_fail2 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"</>";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateOpeningTag( str, v ), DOMException );
}

TEST( HTML5rules_Tests, validateOpeningTag_fail3 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<div >x";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateOpeningTag( str, v ), DOMException );
}

TEST( HTML5rules_Tests, validateOpeningTag_fail4 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<div /";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateOpeningTag( str, v ), DOMException );
}

TEST( HTML5rules_Tests, validateOpeningTag_fail5 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<img /text>";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateOpeningTag( str, v ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail0 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail1 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</>";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail2 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"<tagname/>";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail3 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</tagname>";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail4 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"<a/>";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail5 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"< /a>";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail6 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</ a>";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_fail7 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</\"*>";
    ASSERT_THROW( validateClosingTag( str ), DOMException );
}

TEST( HTML5rules_Tests, validateClosingingTag_pass0 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</a>";
    ASSERT_EQ( Tag::A, validateClosingTag( str ) );
}

TEST( HTML5rules_Tests, validateClosingingTag_pass1 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</div>";
    ASSERT_EQ( Tag::DIV, validateClosingTag( str ) );
}

TEST( HTML5rules_Tests, validateClosingingTag_pass2 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</div   >";
    ASSERT_EQ( Tag::DIV, validateClosingTag( str ) );
}

TEST( HTML5rules_Tests, validateAttributes_fail1 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"\"";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail2 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"%/\"^&%";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail3 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name=  ";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail4 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name='value ";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail5 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name=\"value ";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail6 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"na\005me=value"; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail7 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name=va\005lue"; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail8 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name='va\005000lue'"; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_fail9 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name=\"va\005lue\""; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_THROW( validateAttributes( str, v ), std::invalid_argument );
}

TEST( HTML5rules_Tests, validateAttributes_pass0 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 0, v.size() );
}

TEST( HTML5rules_Tests, validateAttributes_pass1 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"disabled";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"disabled", v.at( 0 ).first );
    ASSERT_EQ( U"", v.at( 0 ).second );
}

TEST( HTML5rules_Tests, validateAttributes_pass2 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"value=yes";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"value", v.at( 0 ).first );
    ASSERT_EQ( U"yes", v.at( 0 ).second );
}

TEST( HTML5rules_Tests, validateAttributes_pass3 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U" value =  yes ";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"value", v.at( 0 ).first );
    ASSERT_EQ( U"yes", v.at( 0 ).second );
}

TEST( HTML5rules_Tests, validateAttributes_pass4 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U" value1 =  yes value2  = no ";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 2, v.size() );
    ASSERT_EQ( U"value1", v.at( 0 ).first );
    ASSERT_EQ( U"yes", v.at( 0 ).second );
    ASSERT_EQ( U"value2", v.at( 1 ).first );
    ASSERT_EQ( U"no", v.at( 1 ).second );
}

TEST( HTML5rules_Tests, validateAttributes_pass30 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"type='checkbox'";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"type", v.at( 0 ).first );
    ASSERT_EQ( U"checkbox", v.at( 0 ).second );
}

TEST( HTML5rules_Tests, validateAttributes_pass40 ) {
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name=\"be evil\"";
    auto v   = std::vector<std::pair<std::u32string, std::u32string>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"name", v.at( 0 ).first );
    ASSERT_EQ( U"be evil", v.at( 0 ).second );
}