#include "gtest/gtest.h"
#include "../../../../src/dom/html5/validator/validator.h"
#include "../../../../src/dom/exception/DOMException.h"

TEST( html5_validator_Tests, validateClosingTag_fail0 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Closing tag is empty.", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail1 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"<";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 2nd character of an end tag must be U+002F ('/').", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail2 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: No name found in closing tag \"</\".", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail3 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</>";

    EXPECT_THROW( {
    try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: No name found in closing tag \"</>\".", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail4 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"<tagname/>";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 2nd character of an end tag must be U+002F ('/').", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail5 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</tagname>";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Tag name is not valid.", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail6 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"<a/>";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 2nd character of an end tag must be U+002F ('/').", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail7 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"< /a>";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: 2nd character of an end tag must be U+002F ('/').", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail8 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</ a>";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: No name detected after \"</\" in closing tag.", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_fail9 ) {
    using blogator::exception::DOMException;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</\"*>";

    EXPECT_THROW( {
        try {
            validateClosingTag( str );
        } catch( const DOMException &e ) {
            EXPECT_STREQ( "SyntaxError: Tag name must be made of ASCII alphanumeric characters only.", e.what() );
            throw;
        }
    }, DOMException );
}

TEST( html5_validator_Tests, validateClosingTag_pass0 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</a>";
    ASSERT_EQ( Tag::A, validateClosingTag( str ) );
}

TEST( html5_validator_Tests, validateClosingTag_pass1 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</div>";
    ASSERT_EQ( Tag::DIV, validateClosingTag( str ) );
}

TEST( html5_validator_Tests, validateClosingTag_pass2 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"</div   >";
    ASSERT_EQ( Tag::DIV, validateClosingTag( str ) );
}

TEST( html5_validator_Tests, validateClosingTag_pass3 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::validator::validateClosingTag;
    auto str = U"-->";
    ASSERT_EQ( Tag::COMMENT, validateClosingTag( str ) );
}