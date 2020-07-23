#include "gtest/gtest.h"
#include "../../../../src/dom/html5/validator/validator.h"
#include "../../../../src/exception/DOMException.h"

TEST( html5_validator_Tests, validateAttributes_fail1 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"\"";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Invalid character in named attribute \"\"\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail2 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"%/\"^&%";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Invalid character in named attribute \"%/\"^&%\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail3 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"name=  ";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Expected value for named attribute \"name\" missing.", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail4 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"name='value ";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Missing closing character (') from value for named attribute \"name\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail5 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"name=\"value ";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Missing closing character (\") from value for named attribute \"name\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail6 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"na\005me=value"; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Invalid character in named attribute \"na\x5me=value\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail7 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"name=va\005lue"; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Invalid character in value for named attribute \"name\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail8 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"name='va\005000lue'"; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Invalid character in value for named attribute \"name\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_fail9 ) {
    using blogator::dom::html5::validator::validateAttributes;
    using blogator::dom::dto::Attribute;
    auto str = U"name=\"va\005lue\""; //invalid char in val
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();

    EXPECT_THROW( {
        try {
            validateAttributes( str, v );
        } catch( const std::invalid_argument &e ) {
            EXPECT_STREQ( "Invalid character in value for named attribute \"name\".", e.what() );
            throw;
        }
    }, std::invalid_argument );
}

TEST( html5_validator_Tests, validateAttributes_pass0 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 0, v.size() );
}

TEST( html5_validator_Tests, validateAttributes_pass1 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"disabled";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"disabled", v.at( 0 ).first );
    ASSERT_EQ( U"", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::NONE, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass2 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"value=yes";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"value", v.at( 0 ).first );
    ASSERT_EQ( U"yes", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::NONE, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass3 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U" value =  yes ";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"value", v.at( 0 ).first );
    ASSERT_EQ( U"yes", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::NONE, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass4 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U" value1 =  yes value2  = no ";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 2, v.size() );
    ASSERT_EQ( U"value1", v.at( 0 ).first );
    ASSERT_EQ( U"yes", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::NONE, v.at( 0 ).second.boundary );
    ASSERT_EQ( U"value2", v.at( 1 ).first );
    ASSERT_EQ( U"no", v.at( 1 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::NONE, v.at( 1 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass5 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"href=../path/to/file.html";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"href", v.at( 0 ).first );
    ASSERT_EQ( U"../path/to/file.html", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::NONE, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass6 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"href='../path/to/file.html'";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"href", v.at( 0 ).first );
    ASSERT_EQ( U"../path/to/file.html", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::APOSTROPHE, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass7 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"href=\"../path/to/file.html\"";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"href", v.at( 0 ).first );
    ASSERT_EQ( U"../path/to/file.html", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass30 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"type='checkbox'";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"type", v.at( 0 ).first );
    ASSERT_EQ( U"checkbox", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::APOSTROPHE, v.at( 0 ).second.boundary );
}

TEST( html5_validator_Tests, validateAttributes_pass40 ) {
    using blogator::dom::html5::AttrBoundaryChar;
    using blogator::dom::dto::Attribute;
    using blogator::dom::html5::validator::validateAttributes;
    auto str = U"name=\"be evil\"";
    auto v   = std::vector<std::pair<std::u32string, Attribute>>();
    ASSERT_NO_THROW( validateAttributes( str, v ) );
    ASSERT_EQ( 1, v.size() );
    ASSERT_EQ( U"name", v.at( 0 ).first );
    ASSERT_EQ( U"be evil", v.at( 0 ).second.value );
    ASSERT_EQ( AttrBoundaryChar::QUOTATION_MARK, v.at( 0 ).second.boundary );
}