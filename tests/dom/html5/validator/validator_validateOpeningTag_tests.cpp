#include "gtest/gtest.h"
#include "../../../../src/dom/html5/validator/validator.h"
#include "../../../../src/dom/exception/DOMException.h"

TEST( html5_validator_Tests, validateOpeningTag_pass0 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateOpeningTag;

    auto str = U"<div  >";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    ASSERT_EQ( Tag::DIV, validateOpeningTag( str, v ) );
    ASSERT_EQ( 0, v.size() );
}

TEST( html5_validator_Tests, validateOpeningTag_pass1 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateOpeningTag;

    auto str = U"<div  class=\"menu topbar\"  >";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    ASSERT_EQ( Tag::DIV, validateOpeningTag( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"class", v.at( 0 ).first );
    ASSERT_EQ( U"menu topbar", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateOpeningTag_pass2 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateOpeningTag;

    auto str = U"<!--";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    ASSERT_EQ( Tag::COMMENT, validateOpeningTag( str, v ) );
    ASSERT_EQ( 0, v.size() );
}

TEST( html5_validator_Tests, validateOpeningTag_pass3 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateOpeningTag;

    auto str = U"<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"../favicon.ico\" />";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    ASSERT_EQ( Tag::LINK, validateOpeningTag( str, v ) );
    ASSERT_EQ( 3, v.size() );
    ASSERT_EQ( U"rel", v.at( 0 ).first );
    ASSERT_EQ( U"shortcut icon", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 0 ).second.boundary );
    ASSERT_EQ( U"type", v.at( 1 ).first );
    ASSERT_EQ( U"image/x-icon", v.at( 1 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 1 ).second.boundary );
    ASSERT_EQ( U"href", v.at( 2 ).first );
    ASSERT_EQ( U"../favicon.ico", v.at( 2 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 2 ).second.boundary );
}

TEST( html5_validator_Tests, validateOpeningTag_pass4 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateOpeningTag;

    auto str = U"<input type=\"checkbox\" checked  id=\"checkbox_2020\"/>";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    ASSERT_EQ( Tag::INPUT, validateOpeningTag( str, v ) );
    ASSERT_EQ( 3, v.size() );
    ASSERT_EQ( U"type", v.at( 0 ).first );
    ASSERT_EQ( U"checkbox", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 0 ).second.boundary );
    ASSERT_EQ( U"checked", v.at( 1 ).first );
    ASSERT_EQ( U"", v.at( 1 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::NONE, v.at( 1 ).second.boundary );
    ASSERT_EQ( U"id", v.at( 2 ).first );
    ASSERT_EQ( U"checkbox_2020", v.at( 2 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 2 ).second.boundary );
}

TEST( html5_validator_Tests, validateOpeningTag_pass5 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateOpeningTag;

    auto str = U"<a class=\"my-link nested-link\" href=\"http://start.duckduckgo.com\">";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    ASSERT_EQ( Tag::A, validateOpeningTag( str, v ) );
    ASSERT_EQ( 2, v.size() );
    ASSERT_EQ( U"class", v.at( 0 ).first );
    ASSERT_EQ( U"my-link nested-link", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 0 ).second.boundary );
    ASSERT_EQ( U"href", v.at( 1 ).first );
    ASSERT_EQ( U"http://start.duckduckgo.com", v.at( 1 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 1 ).second.boundary );
}

TEST( html5_validator_Tests, validateOpeningTag_fail0 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateOpeningTag( str, v );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Opening tag string is empty.", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST( html5_validator_Tests, validateOpeningTag_fail1 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"|div>";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateOpeningTag( str, v );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 1st character of an opening tag must be U+003C ('<').", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST( html5_validator_Tests, validateOpeningTag_fail2 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<>";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateOpeningTag( str, v );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Valid tag names start with either ASCII alphanumeric character or a '!' only.", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST( html5_validator_Tests, validateOpeningTag_fail3 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"</>";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateOpeningTag( str, v );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Valid tag names start with either ASCII alphanumeric character or a '!' only.", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST( html5_validator_Tests, validateOpeningTag_fail4 ) {
    using blogator::dom::html5::validator::validateOpeningTag;
    using blogator::dom::dto::Attribute;
    auto str = U"<div >x";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateOpeningTag( str, v );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: String has character(s) after the U+003E character ('>').", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST( html5_validator_Tests, validateOpeningTag_fail5 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<div /";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateOpeningTag( str, v );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Invalid attribute(s) format: Invalid character in named attribute \"/\".", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}

TEST( html5_validator_Tests, validateOpeningTag_fail6 ) {
    using blogator::exception::DOMException;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateOpeningTag;
    auto str = U"<img /text>";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateOpeningTag( str, v );
        } catch( const blogator::exception::DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Invalid attribute(s) format: Invalid character in named attribute \"/text\".", e.what() );
            throw;
        }
    }, blogator::exception::DOMException );
}