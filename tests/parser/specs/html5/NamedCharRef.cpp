#include "gtest/gtest.h"
#include "../../src/parser/specs/html5/NamedCharRef.h"
#include "../../src/unicode/utf8.h"

using namespace blogator::parser::specs::html5;

TEST( parser_specs_html5_NamedCharRef_Tests, match_1 ) {
    auto tracker = blogator::TrieTracker<uint32_t>();
    auto str     = std::u32string( U"&curvearrowleft;" );
    auto char_n  = 0;

    for( uint32_t c : str ) {
        ASSERT_TRUE( NamedCharRef::match( tracker, c ) ) << "Failed to match character " << char_n << ": " << c;
        ++char_n;
    }

    ASSERT_TRUE( tracker.complete() );
    ASSERT_EQ( std::u32string( tracker.lastMatch().cbegin(), tracker.lastMatch().cend() ), str );
}

TEST( parser_specs_html5_NamedCharRef_Tests, match_2 ) {
    auto tracker = blogator::TrieTracker<uint32_t>();
    auto str     = std::u32string( U"&curvearrow;" );
    auto substr  = str.substr( 0, 11 ); //"&curvearrow"
    auto char_n  = 0;

    for( uint32_t c : substr ) {
        ASSERT_TRUE( NamedCharRef::match( tracker, c ) ) << "Failed to match character " << char_n << ": " << c;
        ++char_n;
    }

    ASSERT_FALSE( NamedCharRef::match( tracker, str.at( 11 ) ) ); //';'
    ASSERT_FALSE( tracker.complete() );
    ASSERT_EQ( std::u32string( tracker.lastMatch().cbegin(), tracker.lastMatch().cend() ), substr );
}

TEST( parser_specs_html5_NamedCharRef_Tests, match_3 ) {
    auto tracker = blogator::TrieTracker<uint32_t>();
    auto str     = std::u32string( U"&AElig" ); //extra ';' goes on to match "&AElig;"
    auto char_n  = 0;

    for( uint32_t c : str ) {
        ASSERT_TRUE( NamedCharRef::match( tracker, c ) ) << "Failed to match character " << char_n << ": " << c;
        ASSERT_TRUE( c == U'g' ? !tracker.partial() : tracker.partial() );
        ASSERT_TRUE( tracker.matched() );
        ASSERT_TRUE( tracker.matching() );
        ++char_n;
    }

    ASSERT_TRUE( tracker.complete() );
    ASSERT_TRUE( tracker.matched() );
    ASSERT_TRUE( tracker.matching() );
    ASSERT_EQ( std::u32string( tracker.lastMatch().cbegin(), tracker.lastMatch().cend() ), str );

    ASSERT_TRUE( NamedCharRef::match( tracker, U';' ) );
    ASSERT_TRUE( tracker.complete() );
    ASSERT_TRUE( tracker.matched() );
    ASSERT_TRUE( tracker.matching() );

    ASSERT_EQ( std::u32string( tracker.lastMatch().cbegin(), tracker.lastMatch().cend() ), U"&AElig;" );
}