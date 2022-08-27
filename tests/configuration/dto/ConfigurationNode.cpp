#include "gtest/gtest.h"
#include "../../../src/configuration/dto/ConfigurationNode.h"
#include "../../../src/configuration/dto/Value.h"

using namespace blogator::configuration;

TEST( configuration_ConfigurationNode, isLeaf_0 ) {
    auto root = ConfigurationNode();

    ASSERT_TRUE( root.isLeaf() );
}

TEST( configuration_ConfigurationNode, isLeaf_1 ) {
    auto root = ConfigurationNode();
    root.children.emplace( U"key", ConfigurationNode() );

    ASSERT_FALSE( root.isLeaf() );
}

TEST( configuration_ConfigurationNode, size_0 ) {
    auto root = ConfigurationNode();

    ASSERT_EQ( root.size(), 0 );
}

TEST( configuration_ConfigurationNode, size_1 ) {
    auto root = ConfigurationNode();
    root.children.emplace( U"ns", ConfigurationNode() );

    ASSERT_EQ( root.size(), 0 );
}

TEST( configuration_ConfigurationNode, size_2 ) {
    auto root = ConfigurationNode();
    auto [it, flag] = root.children.emplace( U"ns", ConfigurationNode() );
    it->second.addValue( std::make_unique<Value>( true ) );

    ASSERT_EQ( root.size(), 1 );
}

TEST( configuration_ConfigurationNode, size_3 ) {
    auto root = ConfigurationNode();
    root.addValue( std::make_unique<Value>( U"text" ) );
    auto [it, flag] = root.children.emplace( U"ns", ConfigurationNode() );
    it->second.addValue( std::make_unique<Value>( true ) );

    ASSERT_EQ( root.size(), 2 );
}

TEST( configuration_ConfigurationNode, copy_0 ) { //root node
    auto root = ConfigurationNode();
    root.addValue( std::make_unique<Value>( U"text" ) );

    auto copy = root.copy();
    ASSERT_EQ( copy.values.values().front()->type(), ValueType::STRING );
    ASSERT_EQ( copy.size(), 1 );
}

TEST( configuration_ConfigurationNode, copy_1 ) { //subtree
    auto root = ConfigurationNode();
    root.addValue( std::make_unique<Value>( U"text" ) );
    auto [it, flag] = root.children.emplace( U"ns", ConfigurationNode() );
    it->second.addValue( std::make_unique<Value>( true ) );

    auto copy = root.copy();
    ASSERT_EQ( copy.values.values().front()->type(), ValueType::STRING );
    ASSERT_EQ( copy.children.at( U"ns" ).values.values().front()->type(), ValueType::BOOLEAN );
    ASSERT_EQ( copy.size(), 2 );
}

TEST( configuration_ConfigurationNode, equality_operator_0 ) { //empty
    auto root1 = ConfigurationNode();
    auto root2 = ConfigurationNode();
    ASSERT_EQ( root1, root2 );
}

TEST( configuration_ConfigurationNode, equality_operator_1 ) {
    auto root1 = ConfigurationNode();
    {
        root1.addValue( std::make_unique<Value>( U"text" ) );
        auto [it, flag] = root1.children.emplace( U"ns", ConfigurationNode() );
        it->second.addValue( std::make_unique<Value>( true ) );
    }

    auto root2 = ConfigurationNode();
    {
        root2.addValue( std::make_unique<Value>( U"text" ) );
        auto [it, flag] = root2.children.emplace( U"ns", ConfigurationNode() );
        it->second.addValue( std::make_unique<Value>( true ) );
    }

    ASSERT_EQ( root1, root2 );
}

TEST( configuration_ConfigurationNode, inequality_operator_0 ) {
    auto root1 = ConfigurationNode();
    {
        root1.addValue( std::make_unique<Value>( U"text" ) );
        auto [it, flag] = root1.children.emplace( U"ns", ConfigurationNode() );
        it->second.addValue( std::make_unique<Value>( true ) );
    }

    auto root2 = ConfigurationNode();
    {
        root2.addValue( std::make_unique<Value>( U"text" ) );
        auto [it, flag] = root2.children.emplace( U"ns", ConfigurationNode() );
        it->second.addValue( std::make_unique<Value>( false ) );
    }

    ASSERT_NE( root1, root2 );
}