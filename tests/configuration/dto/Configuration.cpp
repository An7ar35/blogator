#include "gtest/gtest.h"
#include "../../../src/configuration/dto/Configuration.h"

using namespace blogator::configuration;

TEST( configuration_Configuration, add_0 ) { //with no value
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"key" } );

    auto & val = cfg.add( nsk );
    auto val_ptr = cfg.find( nsk );

    ASSERT_EQ( val.type(), Value::Type_e::VOID );
    ASSERT_EQ( cfg.size(), 1 );
    ASSERT_NE( val_ptr, nullptr );
    ASSERT_EQ( *val_ptr, val );
}

TEST( configuration_Configuration, add_1 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"key" } );

    auto & val = cfg.add( nsk, U"value" );
    auto val_ptr = cfg.find( nsk );

    ASSERT_TRUE( val_ptr );
    ASSERT_EQ( cfg.size(), 1 );
    ASSERT_NE( val_ptr, nullptr );
    ASSERT_EQ( *val_ptr, val );
    ASSERT_EQ( val_ptr->getString(), U"value" );
}

TEST( configuration_Configuration, add_2 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"ns0", U"ns1", U"key" } );

    auto & val = cfg.add( nsk, true );
    auto val_ptr = cfg.find( nsk );

    ASSERT_TRUE( val_ptr );
    ASSERT_EQ( cfg.size(), 1 );
    ASSERT_NE( val_ptr, nullptr );
    ASSERT_EQ( *val_ptr, val );
    ASSERT_EQ( val_ptr->getBool(), true );
}

TEST( configuration_Configuration, add_3 ) { //2x keys in same namespace
    auto       cfg  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns1", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"ns1", U"key2" } );

    auto & val1 = cfg.add( nsk1, int64_t( 123456 ) );
    auto & val2 = cfg.add( nsk2, double( .654321 ) );
    ASSERT_EQ( cfg.size(), 2 );

    auto val1_ptr = cfg.find( nsk1 );
    ASSERT_NE( val1_ptr, nullptr );
    ASSERT_EQ( *val1_ptr, val1 );
    ASSERT_EQ( val1_ptr->getInt(), 123456 );

    auto val2_ptr = cfg.find( nsk2 );
    ASSERT_NE( val2_ptr, nullptr );
    ASSERT_EQ( *val2_ptr, val2 );
    ASSERT_EQ( val2_ptr->getDouble(), .654321 );
}

TEST( configuration_Configuration, add_4 ) { //2x keys in different namespace
    auto       cfg  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns01", U"key" } );
    const auto nsk2 = Configuration::Key_t( { U"ns1", U"ns11", U"key" } );

    auto & val1 = cfg.add( nsk1, U"value1" );
    auto & val2 = cfg.add( nsk2, U"value2", Configuration::NAMED_VALUE_FLAG );
    ASSERT_EQ( cfg.size(), 2 );

    auto val1_ptr = cfg.find( nsk1 );
    ASSERT_NE( val1_ptr, nullptr );
    ASSERT_EQ( *val1_ptr, val1 );
    ASSERT_EQ( val1_ptr->type(), ValueType::STRING );
    ASSERT_EQ( val1_ptr->getString(), U"value1" );

    auto val2_ptr = cfg.find( nsk2 );
    ASSERT_NE( val2_ptr, nullptr );
    ASSERT_EQ( *val2_ptr, val2 );
    ASSERT_EQ( val2_ptr->type(), ValueType::NAME );
    ASSERT_EQ( val2_ptr->getString(), U"value2" );
}

TEST( configuration_Configuration, add_5 ) { //single array value
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"key" } );

    auto & val = cfg.add( nsk, U"value", Configuration::NAMED_VALUE_FLAG | Configuration::ARRAY_VALUE_FLAG );
    auto val_ptr = cfg.find( nsk );

    ASSERT_TRUE( val_ptr );
    ASSERT_EQ( cfg.size(), 1 );
    ASSERT_NE( val_ptr, nullptr );
    ASSERT_EQ( *val_ptr, val );
    ASSERT_EQ( val_ptr->type(), ValueType::ARRAY );
    ASSERT_EQ( val_ptr->getArray().size(), 1 );
    ASSERT_EQ( val_ptr->getArray().front().getString(), U"value" );
}

TEST( configuration_Configuration, remove_0 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( {} );

    ASSERT_TRUE( cfg.empty() );
    ASSERT_EQ( cfg.remove( nsk ), 0 );
}

TEST( configuration_Configuration, remove_1 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"ns0", U"ns1", U"key" } );

    ASSERT_TRUE( cfg.empty() );
    ASSERT_EQ( cfg.remove( nsk ), 0 );
}

TEST( configuration_Configuration, remove_2 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"ns0", U"ns1", U"key" } );

    cfg.add( nsk, U"text" );

    ASSERT_FALSE( cfg.empty() );
    ASSERT_NE( cfg.find( nsk ), nullptr );
    ASSERT_EQ( cfg.remove( nsk ), 1 );
    ASSERT_EQ( cfg.find( nsk ), nullptr );
}

TEST( configuration_Configuration, remove_3 ) {
    auto       cfg  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns1", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"ns1", U"key2" } );

    cfg.add( nsk1, U"value1" );
    cfg.add( nsk2, U"value2" );

    ASSERT_FALSE( cfg.empty() );
    ASSERT_NE( cfg.find( nsk1 ), nullptr );
    ASSERT_NE( cfg.find( nsk2 ), nullptr );
    ASSERT_EQ( cfg.remove( nsk1 ), 1 );
    ASSERT_EQ( cfg.find( nsk1 ), nullptr );
    ASSERT_EQ( cfg.size(), 1 );
}

TEST( configuration_Configuration, remove_4 ) {
    auto       cfg  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns1", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"ns1", U"key2" } );
    const auto rm   = Configuration::Key_t( { U"ns0", U"ns1", U"*"    } );

    cfg.add( nsk1, U"value1" );
    cfg.add( nsk2, U"value2" );

    ASSERT_FALSE( cfg.empty() );
    ASSERT_EQ( cfg.remove( rm ), 2 );
    ASSERT_TRUE( cfg.empty() );
}

TEST( configuration_Configuration, remove_5 ) {
    auto       cfg  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns1", U"key" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"ns2", U"key" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns3", U"key" } );
    const auto nsk4 = Configuration::Key_t( { U"ns1", U"ns1", U"key" } );
    const auto rm   = Configuration::Key_t( { U"ns0", U"*",   U"key" } );

    cfg.add( nsk1, U"value1" );
    cfg.add( nsk2, U"value2" );
    cfg.add( nsk3, U"value3" );
    cfg.add( nsk4, U"value4" );

    ASSERT_EQ( cfg.size(), 4 );
    ASSERT_EQ( cfg.remove( rm ), 3 );
    ASSERT_EQ( cfg.size(), 1 );
    ASSERT_NE( cfg.find( nsk4 ), nullptr );
}

TEST( configuration_Configuration, findAll_0 ) { //""
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( {} );

    ASSERT_TRUE( cfg.empty() );
    ASSERT_EQ( cfg.findAll( nsk ), Configuration() );
}

TEST( configuration_Configuration, findAll_1 ) { //"key"
    auto       src  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"key2" } );

    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );

    auto ref = Configuration();
    ref.add( nsk1, U"key1" );

    ASSERT_EQ( src.size(), 2 );
    ASSERT_EQ( ref.size(), 1 );
    ASSERT_EQ( src.findAll( nsk1 ), ref );
}

TEST( configuration_Configuration, findAll_2 ) { //"ns:key"
    auto       src  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns1", U"key1" } );

    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key1" );

    auto ref = Configuration();
    ref.add( nsk1, U"key1" );

    ASSERT_EQ( src.size(), 3 );
    ASSERT_EQ( ref.size(), 1 );
    ASSERT_EQ( src.findAll( nsk1 ), ref );
}

TEST( configuration_Configuration, findAll_3 ) { //"*" everything
    auto       src  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns1", U"key3" } );
    const auto find = Configuration::Key_t( { U"*" } );

    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key3" );

    auto ref = Configuration();
    ref.add( nsk1, U"key1" );
    ref.add( nsk2, U"key2" );
    ref.add( nsk3, U"key3" );

    ASSERT_EQ( src.size(), 3 );
    ASSERT_EQ( ref.size(), 3 );
    ASSERT_EQ( src.findAll( find ), ref );
}

TEST( configuration_Configuration, findAll_4 ) { //"ns:*" everything in a namespace
    auto       src  = Configuration();
    const auto nsk0 = Configuration::Key_t( { U"ns0" } );
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns01", U"key3" } );
    const auto nsk4 = Configuration::Key_t( { U"ns1", U"key4" } );
    const auto nsk5 = Configuration::Key_t( { U"ns1", U"ns11", U"key4" } );

    const auto find = Configuration::Key_t( { U"ns0", U"*"    } );

    src.add( nsk0, U"ns0" );
    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key3" );
    src.add( nsk4, U"key3" );
    src.add( nsk5, U"key3" );

    auto ref = Configuration();
    ref.add( nsk0, U"ns0" );
    ref.add( nsk1, U"key1" );
    ref.add( nsk2, U"key2" );
    ref.add( nsk3, U"key3" );

    ASSERT_EQ( src.size(), 6 );
    ASSERT_EQ( ref.size(), 4 );
    ASSERT_EQ( src.findAll( find ), ref );
}

TEST( configuration_Configuration, findAll_5 ) { //"ns:" namespace
    auto       src  = Configuration();
    const auto nsk0 = Configuration::Key_t( { U"ns0" } );
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns01", U"key3" } );
    const auto nsk4 = Configuration::Key_t( { U"ns1", U"key4" } );
    const auto nsk5 = Configuration::Key_t( { U"ns1", U"ns11", U"key4" } );

    const auto find = Configuration::Key_t( { U"ns0" } );

    src.add( nsk0, U"ns0" );
    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key3" );
    src.add( nsk4, U"key3" );
    src.add( nsk5, U"key3" );

    auto ref = Configuration();
    ref.add( nsk0, U"ns0" );
    ref.add( nsk1, U"key1" );
    ref.add( nsk2, U"key2" );
    ref.add( nsk3, U"key3" );

    ASSERT_EQ( src.size(), 6 );
    ASSERT_EQ( ref.size(), 4 );
    ASSERT_EQ( src.findAll( find ), ref );
}

TEST( configuration_Configuration, findAll_6 ) { //"*:key" namespace
    auto       src  = Configuration();
    const auto nsk0 = Configuration::Key_t( { U"ns0" } );
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns01", U"key1" } );
    const auto nsk4 = Configuration::Key_t( { U"ns1", U"key1" } );
    const auto nsk5 = Configuration::Key_t( { U"ns1", U"key2" } );
    const auto nsk6 = Configuration::Key_t( { U"ns1", U"ns11", U"key1" } );

    const auto find = Configuration::Key_t( { U"*", U"key1" } );

    src.add( nsk0, U"ns0" );
    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key1" );
    src.add( nsk4, U"key1" );
    src.add( nsk5, U"key2" );
    src.add( nsk6, U"key1" );

    auto ref = Configuration();
    ref.add( nsk1, U"key1" );
    ref.add( nsk4, U"key1" );

    ASSERT_EQ( src.size(), 7 );
    ASSERT_EQ( ref.size(), 2 );
    ASSERT_EQ( src.findAll( find ), ref );
}

TEST( configuration_Configuration, findAll_7 ) { //"ns:*:key"
    auto       src  = Configuration();
    const auto nsk0 = Configuration::Key_t( { U"ns0" } );
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns00", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"ns00", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns00", U"ns01", U"key1" } );
    const auto nsk4 = Configuration::Key_t( { U"ns0", U"ns01", U"key1" } );
    const auto nsk5 = Configuration::Key_t( { U"ns0", U"ns00" } );
    const auto nsk6 = Configuration::Key_t( { U"ns1", U"ns00", U"key2" } );
    const auto nsk7 = Configuration::Key_t( { U"ns1", U"ns00", U"ns11", U"key1" } );

    const auto find = Configuration::Key_t( { U"ns0", U"*", U"key1" } );

    src.add( nsk0, U"ns0" );
    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key1" );
    src.add( nsk4, U"key1" );
    src.add( nsk5, U"ns00" );
    src.add( nsk6, U"key2" );
    src.add( nsk7, U"key1" );

    auto ref = Configuration();
    ref.add( nsk1, U"key1" );
    ref.add( nsk4, U"key1" );

    ASSERT_EQ( src.size(), 8 );
    ASSERT_EQ( ref.size(), 2 );
    ASSERT_EQ( src.findAll( find ), ref );
}

TEST( configuration_Configuration, findAll_8 ) { //"*:ns00:*"
    auto src = Configuration();
    const auto nsk0 = Configuration::Key_t( { U"ns0" } );
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns00", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns1", U"ns00", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns00", U"ns01", U"key1" } );
    const auto nsk4 = Configuration::Key_t( { U"ns2", U"ns01", U"key1" } );
    const auto nsk5 = Configuration::Key_t( { U"ns0", U"ns00" } );
    const auto nsk6 = Configuration::Key_t( { U"ns3", U"ns00", U"key2" } );
    const auto nsk7 = Configuration::Key_t( { U"ns3", U"ns02", U"ns11", U"key1" } );

    const auto find = Configuration::Key_t( { U"*", U"ns00", U"*" } );

    src.add( nsk0, U"ns0" );
    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key1" );
    src.add( nsk4, U"key1" );
    src.add( nsk5, U"ns00" );
    src.add( nsk6, U"key2" );
    src.add( nsk7, U"key1" );

    auto ref = Configuration();
    ref.add( nsk1, U"key1" );
    ref.add( nsk2, U"key2" );
    ref.add( nsk3, U"key1" );
    ref.add( nsk5, U"ns00" );
    ref.add( nsk6, U"key2" );

    ASSERT_EQ( src.size(), 8 );
    ASSERT_EQ( ref.size(), 5 );
    ASSERT_EQ( src.findAll( find ), ref );
}

TEST( configuration_Configuration, copy ) {
    const auto nsk0 = Configuration::Key_t( { U"ns0" } );
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns01", U"key3" } );
    const auto nsk4 = Configuration::Key_t( { U"ns1", U"key4" } );
    const auto nsk5 = Configuration::Key_t( { U"ns1", U"ns11", U"key4" } );

    auto src = Configuration();
    src.add( nsk0, U"ns0" );
    src.add( nsk1, U"key1" );
    src.add( nsk2, U"key2" );
    src.add( nsk3, U"key3" );
    src.add( nsk4, U"key3" );
    src.add( nsk5, U"key3" );

    auto ref = Configuration();
    ref.add( {                  }, U"ns0" );
    ref.add( { U"key1"          }, U"key1" );
    ref.add( { U"key2"          }, U"key2" );
    ref.add( { U"ns01", U"key3" }, U"key3" );

    ASSERT_EQ( src.copy( { U"ns0" } ), ref );
}

TEST( configuration_Configuration, validate_str_fail_0 ) {
    const auto str = std::u32string( U"" );
    ASSERT_FALSE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_fail_1 ) {
    const auto str = std::u32string( U"_" );
    ASSERT_FALSE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_fail_2 ) {
    const auto str = std::u32string( U"1" );
    ASSERT_FALSE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_fail_3 ) {
    const auto str = std::u32string( U"_1" );
    ASSERT_FALSE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_fail_4 ) {
    const auto str = std::u32string( U"A" );
    ASSERT_FALSE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_fail_5 ) {
    const auto str = std::u32string( U"$" );
    ASSERT_FALSE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_0 ) {
    const auto str = std::u32string( U"ns" );
    ASSERT_TRUE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_1 ) {
    const auto str = std::u32string( U"_ns" );
    ASSERT_TRUE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_2 ) {
    const auto str = std::u32string( U"ns1" );
    ASSERT_TRUE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_3 ) {
    const auto str = std::u32string( U"_ns_1" );
    ASSERT_TRUE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_str_4 ) {
    const auto str = std::u32string( U"ns_" );
    ASSERT_TRUE( Configuration::validate( str ) );
}

TEST( configuration_Configuration, validate_nsk_fail_0 ) {
    const auto nsk = Configuration::Key_t( { U"___", U"key" } );
    ASSERT_FALSE( Configuration::validate( nsk ) );
}

TEST( configuration_Configuration, validate_nsk_fail_1 ) {
    const auto nsk = Configuration::Key_t( { U"ABC", U"key" } );
    ASSERT_FALSE( Configuration::validate( nsk ) );
}

TEST( configuration_Configuration, validate_nsk_fail_2 ) {
    const auto nsk = Configuration::Key_t( { U"123", U"key" } );
    ASSERT_FALSE( Configuration::validate( nsk ) );
}

TEST( configuration_Configuration, validate_nsk_0 ) {
    const auto nsk = Configuration::Key_t( { U"ns", U"_ns", U"ns_", U"ns1" } );
    ASSERT_TRUE( Configuration::validate( nsk ) );
}

TEST( configuration_Configuration, output_stream_operator_Key_t_0 ) {
    const auto nsk = Configuration::Key_t( {} );
    std::stringstream ss;
    ss << nsk;
    ASSERT_EQ( ss.str(), ":" );
}

TEST( configuration_Configuration, output_stream_operator_Key_t_1 ) {
    const auto nsk = Configuration::Key_t( { U"ns", U"_ns", U"ns_", U"ns1", U"key" } );
    std::stringstream ss;
    ss << nsk;
    ASSERT_EQ( ss.str(), ":ns:_ns:ns_:ns1:key" );
}

TEST( configuration_Configuration, convert_nks2str_0 ) {
    const auto nsk = Configuration::Key_t( {} );
    ASSERT_EQ( Configuration::convert( nsk ), U":" );
}

TEST( configuration_Configuration, convert_nks2str_1 ) {
    const auto nsk = Configuration::Key_t( { U"ns", U"key" } );
    ASSERT_EQ( Configuration::convert( nsk ), U":ns:key" );
}

TEST( configuration_Configuration, convert_str2nks_0 ) {
    const auto str = U":";
    ASSERT_EQ( Configuration::convert( str ), Configuration::Key_t( {} ) );
}

TEST( configuration_Configuration, convert_str2nks_1 ) {
    const auto str = U"ns:key";
    ASSERT_EQ( Configuration::convert( str ), Configuration::Key_t( { U"ns", U"key" } ) );
}

TEST( configuration_Configuration, convert_str2nks_2 ) {
    const auto str = U":ns:key";
    ASSERT_EQ( Configuration::convert( str ), Configuration::Key_t( { U"ns", U"key" } ) );
}

TEST( configuration_Configuration, convert_str2nks_fail_0 ) {
    const auto str = U":_ns£:  ";
    ASSERT_THROW( Configuration::convert( str ), std::invalid_argument );
}

TEST( configuration_Configuration, printToCollection ) {
    auto       src  = Configuration();
    const auto nsk0 = Configuration::Key_t( { U"ns0" } );
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns00", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"ns00", U"key2" } );
    const auto nsk3 = Configuration::Key_t( { U"ns0", U"ns00", U"ns01", U"key1" } );
    const auto nsk4 = Configuration::Key_t( { U"ns1", U"ns01", U"key1" } );
    const auto nsk5 = Configuration::Key_t( { U"ns2", U"ns00" } );

    src.add( nsk0, true );
    src.add( nsk1, int64_t( 123 ) );
    src.add( nsk2, U"value string" );
    src.add( nsk3, U"value_name", Configuration::NAMED_VALUE_FLAG );
    src.add( nsk4, double( .123 ) );
    src.add( nsk5, U"arr1", Value::IS_ARRAY_VALUE );
    src.add( nsk5, U"arr2", Configuration::NAMED_VALUE_FLAG );
    src.add( nsk5, int64_t( 9 ) );
    src.add( nsk5, false );

    auto expected = std::vector<std::pair<std::string, std::string>>( {
        { R"(:ns0)", R"(true)" },
        { R"(:ns0:ns00:key1)", R"(123)" },
        { R"(:ns0:ns00:key2)", R"("value string")" },
        { R"(:ns0:ns00:ns01:key1)", R"(value_name)" },
        { R"(:ns1:ns01:key1)", R"(0.123000)" },
        { R"(:ns2:ns00)", R"([ "arr1", arr2, 9, false ])" },
    } );

    ASSERT_EQ( src.printToCollection(), expected );
}

TEST( configuration_Configuration, exists_0 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"key" } );

    ASSERT_FALSE( cfg.exists( nsk ) );
}

TEST( configuration_Configuration, exists_1 ) {
    auto       cfg = Configuration();
    const auto ns  = Configuration::Key_t( { U"ns" } );
    const auto nsk = Configuration::Key_t( { U"ns", U"key" } );

    cfg.add( nsk, "value" );
    ASSERT_FALSE( cfg.exists( ns ) );
    ASSERT_TRUE( cfg.exists( nsk ) );
}

TEST( configuration_Configuration, exists_2 ) { //empty value
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"ns", U"key" } );

    cfg.add( nsk );
    ASSERT_TRUE( cfg.exists( nsk ) );
}

TEST( configuration_Configuration, exists_3 ) { //empty value added after the fact
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"ns", U"key" } );

    auto & ref = cfg.add( nsk );
    ASSERT_TRUE( cfg.exists( nsk ) );

    ref = Value( "value" );

    ASSERT_TRUE( cfg.exists( nsk ) );

    auto * ptr = cfg.find( nsk );
    ASSERT_NE( ptr, nullptr );
    ASSERT_EQ( *ptr, Value( "value" ) );

}