#include "gtest/gtest.h"
#include "../../../src/configuration/dto/Value.h"
#include "../../../src/configuration/exception/value_type_mismatch.h"

using namespace blogator::configuration;

TEST( configuration_Value, getter_void ) {
    const auto val = Value();
    ASSERT_EQ( val.type(), Value::Type_e::VOID );
}

TEST( configuration_Value, getter_bool ) {
    const auto val = Value( true );

    ASSERT_EQ( val.type(), Value::Type_e::BOOLEAN );
    ASSERT_EQ( val.getBool(), true );
}

TEST( configuration_Value, getter_int ) {
    const auto val = Value( int64_t( 123456789 ) );

    ASSERT_EQ( val.type(), Value::Type_e::INTEGER );
    ASSERT_EQ( val.getInt(), 123456789 );
}

TEST( configuration_Value, getter_float ) {
    const auto val = Value( double( .123456 ) );

    ASSERT_EQ( val.type(), Value::Type_e::FLOAT );
    ASSERT_EQ( val.getDouble(), .123456 );
}

TEST( configuration_Value, getter_string ) {
    const auto val = Value( U"text" );

    ASSERT_EQ( val.type(), Value::Type_e::STRING );
    ASSERT_EQ( val.getString(), U"text" );
}

TEST( configuration_Value, getter_name ) {
    const auto val = Value( U"name", Value::IS_NAMED_VALUE );

    ASSERT_EQ( val.type(), Value::Type_e::NAME );
    ASSERT_EQ( val.getString(), U"name" );
}

TEST( configuration_Value, getter_array_0 ) { //void
    const auto val = Value( Value::IS_ARRAY_VALUE );

    ASSERT_EQ( val.type(), Value::Type_e::ARRAY );
    ASSERT_TRUE( val.getArray().empty() );
}

TEST( configuration_Value, getter_array_1 ) {
    const auto val = Value( U"arr1", Value::IS_ARRAY_VALUE );

    ASSERT_EQ( val.type(), Value::Type_e::ARRAY );
    ASSERT_EQ( val.getArray().size(), 1 );
    ASSERT_EQ( val.getArray().front().getString(), U"arr1" );
}

TEST( configuration_Value, getter_fail_bool ) {
    const auto val = Value( true );

    ASSERT_THROW( ( std::ignore = val.getInt() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getDouble() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getString() ), blogator::configuration::exception::value_type_mismatch );
}

TEST( configuration_Value, getter_fail_int ) {
    const auto val = Value( int64_t( 123456789 ) );

    ASSERT_THROW( ( std::ignore = val.getBool() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getDouble() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getString() ), blogator::configuration::exception::value_type_mismatch );
}

TEST( configuration_Value, getter_fail_double ) {
    const auto val = Value( double( .123456789 ) );

    ASSERT_THROW( ( std::ignore = val.getBool() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getInt() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getString() ), blogator::configuration::exception::value_type_mismatch );
}

TEST( configuration_Value, getter_fail_string ) {
    const auto val = Value( U"text" );

    ASSERT_THROW( ( std::ignore = val.getBool() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getInt() ), blogator::configuration::exception::value_type_mismatch );
    ASSERT_THROW( ( std::ignore = val.getDouble() ), blogator::configuration::exception::value_type_mismatch );
}