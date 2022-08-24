#include "gtest/gtest.h"
#include "../../../src/configuration/dto/Configuration.h"

using namespace blogator::configuration;


TEST( configuration_Configuration, add_0 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"key" } );

    cfg.add( nsk, U"value" );
    auto val = cfg.find( nsk );

    ASSERT_EQ( cfg.size(), 1 );
    ASSERT_NE( val, nullptr );
    ASSERT_EQ( val->getString(), U"value" );
}

TEST( configuration_Configuration, add_1 ) {
    auto       cfg = Configuration();
    const auto nsk = Configuration::Key_t( { U"ns0", U"ns1", U"key" } );

    cfg.add( nsk, true );
    auto val = cfg.find( nsk );

    ASSERT_EQ( cfg.size(), 1 );
    ASSERT_NE( val, nullptr );
    ASSERT_EQ( val->getBool(), true );
}

TEST( configuration_Configuration, add_2 ) { //2x keys in same namespace
    auto       cfg  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns1", U"key1" } );
    const auto nsk2 = Configuration::Key_t( { U"ns0", U"ns1", U"key2" } );

    cfg.add( nsk1, int64_t( 123456 ) );
    cfg.add( nsk2, double( .654321 ) );
    ASSERT_EQ( cfg.size(), 2 );

    auto val1 = cfg.find( nsk1 );
    ASSERT_NE( val1, nullptr );
    ASSERT_EQ( val1->getInt(), 123456 );

    auto val2 = cfg.find( nsk2 );
    ASSERT_NE( val2, nullptr );
    ASSERT_EQ( val2->getDouble(), .654321 );
}

TEST( configuration_Configuration, add_3 ) { //2x keys in different namespace
    auto       cfg  = Configuration();
    const auto nsk1 = Configuration::Key_t( { U"ns0", U"ns01", U"key" } );
    const auto nsk2 = Configuration::Key_t( { U"ns1", U"ns11", U"key" } );

    cfg.add( nsk1, U"value1" );
    cfg.add( nsk2, U"value2", true );
    ASSERT_EQ( cfg.size(), 2 );

    auto val1 = cfg.find( nsk1 );
    ASSERT_NE( val1, nullptr );
    ASSERT_EQ( val1->type(), ValueType::STRING );
    ASSERT_EQ( val1->getString(), U"value1" );

    auto val2 = cfg.find( nsk2 );
    ASSERT_NE( val2, nullptr );
    ASSERT_EQ( val2->type(), ValueType::NAME );
    ASSERT_EQ( val2->getString(), U"value2" );
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