#include "gtest/gtest.h"
#include "../../../src/configuration/dto/ValueStore.h"

using namespace blogator::configuration;

TEST( configuration_ValueStore, equality_operator_0 ) {
    auto vs1 = ValueStore();
    auto vs2 = ValueStore();

    ASSERT_EQ( vs1, vs2 );
}

TEST( configuration_ValueStore, equality_operator_1 ) {
    auto vs1 = ValueStore();
    auto vs2 = ValueStore();

    vs1.emplace_back( std::make_unique<Value>( U"val" ) );
    ASSERT_NE( vs1, vs2 );
    vs2.emplace_back( std::make_unique<Value>( U"val" ) );
    ASSERT_EQ( vs1, vs2 );
}

TEST( configuration_ValueStore, inequality_operator_0 ) {
    auto vs1 = ValueStore();
    auto vs2 = ValueStore();

    vs1.emplace_back( std::make_unique<Value>( U"val1" ) );
    ASSERT_NE( vs1, vs2 );
    vs2.emplace_back( std::make_unique<Value>( U"val2" ) );
    ASSERT_NE( vs1, vs2 );
}

TEST( configuration_ValueStore, setArrayFlag_0 ) {
    auto vs = ValueStore();

    ASSERT_FALSE( vs.isArray() );
    vs.setArrayFlag();
    ASSERT_TRUE( vs.isArray() );
}

TEST( configuration_ValueStore, clear_0 ) {
    auto vs = ValueStore();

    ASSERT_TRUE( vs.empty() );
    vs.emplace_back( std::make_unique<Value>( U"val1" ) );
    vs.emplace_back( std::make_unique<Value>( U"val2" ) );
    vs.emplace_back( std::make_unique<Value>( U"val3" ) );
    ASSERT_FALSE( vs.empty() );
    ASSERT_EQ( vs.size(), 3 );

    vs.clear();
    ASSERT_TRUE( vs.empty() );
    ASSERT_EQ( vs.size(), 0 );
}

TEST( configuration_ValueStore, empty_0 ) {
    auto vs = ValueStore();

    ASSERT_TRUE( vs.empty() );
}

TEST( configuration_ValueStore, empty_1 ) {
    auto vs = ValueStore();
    vs.emplace_back( std::make_unique<Value>( false ) );

    ASSERT_FALSE( vs.empty() );
}

TEST( configuration_ValueStore, output_stream_operator_0 ) { //empty
    auto vs = ValueStore();

    std::stringstream ss;
    ss << vs;
    ASSERT_EQ( ss.str(), "" );
}

TEST( configuration_ValueStore, output_stream_operator_1 ) { //value
    auto vs = ValueStore();
    vs.emplace_back( std::make_unique<Value>( U"value" ) );

    std::stringstream ss;
    ss << vs;
    ASSERT_EQ( ss.str(), "\"value\"" );
}

TEST( configuration_ValueStore, output_stream_operator_2 ) { //array of 1 value
    auto vs = ValueStore();
    vs.emplace_back( std::make_unique<Value>( U"value" ) );
    vs.setArrayFlag();

    std::stringstream ss;
    ss << vs;
    ASSERT_EQ( ss.str(), "[ \"value\" ]" );
}

TEST( configuration_ValueStore, output_stream_operator_3 ) { //array of n values
    auto vs = ValueStore();
    vs.emplace_back( std::make_unique<Value>( U"text" ) );
    vs.emplace_back( std::make_unique<Value>( true ) );
    vs.emplace_back( std::make_unique<Value>( int64_t( 1 ) ) );
    vs.emplace_back( std::make_unique<Value>( double( 1.23 ) ) );
    vs.emplace_back( std::make_unique<Value>( U"ns:name", true ) );

    std::stringstream ss;
    ss << vs;
    ASSERT_EQ( ss.str(), "[ \"text\", true, 1, 1.230000, ns:name ]" );
}