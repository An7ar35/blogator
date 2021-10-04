#include "gtest/gtest.h"
#include "../../src/datastructure/TrieNode.h"

using namespace blogator;

TEST( TrieNode_Tests, addChild ) {
    auto root = TrieNode<uint32_t>( U'a', false );

    auto [it1, success1] = root.addChild( U'b', false );
    ASSERT_TRUE( success1 );
    ASSERT_EQ( it1->first, U'b' );

    auto [it2, success2] = it1->second.addChild( 'c', false );
    ASSERT_TRUE( success2 );
    ASSERT_EQ( it2->first, U'c' );

    ASSERT_EQ( root.usage(), 1 );
    ASSERT_EQ( it1->second.usage(), 1 );
    ASSERT_EQ( it2->second.usage(), 0 );
}

TEST( TrieNode_Tests, addChild_same ) {
    auto root = TrieNode<uint32_t>( U'a', false );

    {
        auto[it, inserted] = root.addChild( U'b', false );
        ASSERT_TRUE( inserted );
        ASSERT_EQ( it->first, U'b' );
        ASSERT_FALSE( it->second.end() );
    }

    {
        auto[it, inserted] = root.addChild( 'b', true );
        ASSERT_FALSE( inserted );
        ASSERT_EQ( it->first, U'b' );
        ASSERT_TRUE( it->second.end() );
    }

    ASSERT_EQ( root.usage(), 2 );
}

TEST( TrieNode_Tests, setEnd ) {
    auto root = TrieNode<uint32_t>( U'a', false );
    ASSERT_FALSE( root.end() );
    root.setEnd( true );
    ASSERT_TRUE( root.end() );
}

TEST( TrieNode_Tests, hasChildren ) {
    auto root = TrieNode<uint32_t>( U'a', false );
    ASSERT_FALSE( root.hasChildren() );
    auto [it, success] = root.addChild( U'b' );
    ASSERT_TRUE( root.hasChildren() );
}

TEST( TrieNode_Tests, hasChild ) {
    auto root = TrieNode<uint32_t>( U'a', false );
    auto [it, success] = root.addChild( U'b' );
    ASSERT_TRUE( success );
    ASSERT_EQ( it->first, U'b' );
    ASSERT_TRUE( root.hasChild( U'b' ) );
}

TEST( TrieNode_Tests, next_failed ) {
    auto root = TrieNode<uint32_t>( U'a', false );
    ASSERT_FALSE( root.hasChild( U'b' ) );
}