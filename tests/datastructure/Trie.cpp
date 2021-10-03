#include "gtest/gtest.h"
#include "../../src/datastructure/Trie.h"
#include "../../src/unicode/utf8.h"

using namespace blogator;

TEST( Trie_Tests, add_element ) {
    auto str  = std::u32string( U"abcd" );
    auto trie = Trie<uint32_t>();

    for( auto c : str )
        trie.add( c );

    auto collection = trie.collect();

    ASSERT_EQ( collection.size(), str.length() );
    ASSERT_EQ( std::u32string( collection.at( 0 ).begin(), collection.at( 0 ).end() ), U"a" );
    ASSERT_EQ( std::u32string( collection.at( 1 ).begin(), collection.at( 1 ).end() ), U"b" );
    ASSERT_EQ( std::u32string( collection.at( 2 ).begin(), collection.at( 2 ).end() ), U"c" );
    ASSERT_EQ( std::u32string( collection.at( 3 ).begin(), collection.at( 3 ).end() ), U"d" );
}

TEST( Trie_Tests, add_elements ) {
    auto str1 = std::u32string( U"abcd" );
    auto str2 = std::u32string( U"abef" );
    auto str3 = std::u32string( U"abcde" );
    auto str4 = std::u32string( U"abcgh" );
    auto str5 = std::u32string( U"cde" );
    auto str6 = std::u32string( U"cdab" );
    auto trie = Trie<uint32_t>();

    trie.add( str1.begin(), str1.end() );
    trie.add( str2.begin(), str2.end() );
    trie.add( str3.begin(), str3.end() );
    trie.add( str4.begin(), str4.end() );
    trie.add( str5.begin(), str5.end() );
    trie.add( str6.begin(), str6.end() );

    auto collection = trie.collect();

    ASSERT_EQ( collection.size(), 6 );
    ASSERT_EQ( std::u32string( collection.at( 0 ).begin(), collection.at( 0 ).end() ), str1 );
    ASSERT_EQ( std::u32string( collection.at( 1 ).begin(), collection.at( 1 ).end() ), str3 );
    ASSERT_EQ( std::u32string( collection.at( 2 ).begin(), collection.at( 2 ).end() ), str4 );
    ASSERT_EQ( std::u32string( collection.at( 3 ).begin(), collection.at( 3 ).end() ), str2 );
    ASSERT_EQ( std::u32string( collection.at( 4 ).begin(), collection.at( 4 ).end() ), str6 );
    ASSERT_EQ( std::u32string( collection.at( 5 ).begin(), collection.at( 5 ).end() ), str5 );

//    for( const auto & variation : collection ) {
//        unicode::utf8::convert( std::cout, std::u32string( variation.begin(), variation.end() ) );
//        std::cout << "\n";
//    }
//
//    std::cout << std::flush;
}

TEST( Trie_Tests, match_pass ) {
    auto str1    = std::u32string( U"abcd" );
    auto trie    = Trie<uint32_t>();
    auto tracker = TrieTracker<uint32_t>();

    trie.add( str1.begin(), str1.end() );

    for( auto e : str1 ) {
        ASSERT_TRUE( trie.match( tracker, e ) );
    }
}

TEST( Trie_Tests, match_fail1 ) {
    auto str1    = std::u32string( U"abcd" );
    auto str2    = std::u32string( U"abdc" );
    auto trie    = Trie<uint32_t>();
    auto tracker = TrieTracker<uint32_t>();

    trie.add( str1.begin(), str1.end() );

    ASSERT_TRUE( trie.match( tracker, str2.at( 0 ) ) );
    ASSERT_TRUE( trie.match( tracker, str2.at( 1 ) ) );
    ASSERT_FALSE( trie.match( tracker, str2.at( 2 ) ) );
    ASSERT_FALSE( trie.match( tracker, str2.at( 3 ) ) );
}

TEST( Trie_Tests, match_fail2 ) {
    auto str1    = std::u32string( U"abcd" );
    auto str2    = std::u32string( U"dcba" );
    auto trie    = Trie<uint32_t>();
    auto tracker = TrieTracker<uint32_t>();

    trie.add( str1.begin(), str1.end() );

    ASSERT_FALSE( trie.match( tracker, str2.at( 0 ) ) );
    ASSERT_FALSE( trie.match( tracker, str2.at( 1 ) ) );
    ASSERT_FALSE( trie.match( tracker, str2.at( 2 ) ) );
    ASSERT_TRUE( trie.match( tracker, str2.at( 3 ) ) );
}

TEST( Trie_Tests, match_fail3 ) {
    auto str1    = std::u32string( U"abcd" );
    auto str2    = std::u32string( U"efgh" );
    auto trie    = Trie<uint32_t>();
    auto tracker = TrieTracker<uint32_t>();

    trie.add( str1.begin(), str1.end() );

    ASSERT_FALSE( trie.match( tracker, str2.at( 0 ) ) );
    ASSERT_FALSE( trie.match( tracker, str2.at( 1 ) ) );
    ASSERT_FALSE( trie.match( tracker, str2.at( 2 ) ) );
    ASSERT_FALSE( trie.match( tracker, str2.at( 3 ) ) );
}

TEST( Trie_Tests, has ) {
    auto str1 = std::u32string( U"abcd" );
    auto str2 = std::u32string( U"abce" );
    auto trie = Trie<uint32_t>();

    ASSERT_FALSE( trie.has( str1.begin(), str1.end() ) );
    trie.add( str1.begin(), str1.end() );
    ASSERT_TRUE( trie.has( str1.begin(), str1.end() ) );
    ASSERT_FALSE( trie.has( str2.begin(), str2.end() ) );
}

TEST( Trie_Tests, clear ) {
    auto str1 = std::u32string( U"abcd" );
    auto str2 = std::u32string( U"abef" );
    auto str3 = std::u32string( U"abcde" );
    auto trie = Trie<uint32_t>();

    { //pre-check
        auto collection = trie.collect();
        ASSERT_EQ( collection.size(), 0 );
    }

    { //add content
        trie.add( str1.begin(), str1.end() );
        trie.add( str2.begin(), str2.end() );
        trie.add( str3.begin(), str3.end() );
        auto collection = trie.collect();
        ASSERT_EQ( collection.size(), 3 );
    }

    { //clear and post-check
        trie.clear();
        auto collection = trie.collect();
        ASSERT_EQ( collection.size(), 0 );
    }
}