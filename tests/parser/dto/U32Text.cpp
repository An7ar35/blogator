#include "gtest/gtest.h"
#include "../../../src/parser/dto/U32Text.h"
#include "../../../src/exception/failed_expectation.h"

using blogator::parser::U32Text;
using blogator::parser::TextPos;

TEST( parser_dto_U32Text, constructor_1 ) { //string
    const auto str  = std::u32string( U"B" );
    const auto path = std::filesystem::path( "path/to/source" );
    auto       text = U32Text( path, str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );
    ASSERT_EQ( text.path(), path );
    ASSERT_EQ( text.id(), std::hash<std::string>()( text.path() ) );
}

TEST( parser_dto_U32Text, constructor_2 ) { //vector
    const auto v    = std::vector<char32_t>( { U'B' } );
    const auto path = std::filesystem::path( "path/to/source" );
    auto       text = U32Text( "path/to/source", v );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );
    ASSERT_EQ( text.path(), path );
    ASSERT_EQ( text.id(), std::hash<std::string>()( text.path() ) );
}

TEST( parser_dto_U32Text, advanceCaret_1 ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );

    ASSERT_EQ( text.advanceCaret(), 1 );
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) );
    ASSERT_EQ( text.character(), U'1' );

    ASSERT_EQ( text.advanceCaret( 3 ), 3 );
    ASSERT_EQ( text.position(), TextPos( 1, 5 ) );
    ASSERT_EQ( text.character(), U'4' );

    ASSERT_EQ( text.advanceCaret( 6 ), 6 );
    ASSERT_EQ( text.position(), TextPos( 1, 11 ) );
    ASSERT_FALSE( text.reachedEnd() );
    ASSERT_EQ( text.character(), U'E' );

    ASSERT_EQ( text.advanceCaret( 4 ), 1 );
    ASSERT_EQ( text.position(), TextPos( 1, 12 ) );
    ASSERT_TRUE( text.reachedEnd() );
    ASSERT_EQ( text.character(), 0x00 );
}

TEST( parser_dto_U32Text, advanceCaret_2 ) {
    const std::u32string str = U"B\n123\n4E";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );

    ASSERT_EQ( text.advanceCaret(), 1 );
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) );
    ASSERT_EQ( text.character(), U'\n' );

    ASSERT_EQ( text.advanceCaret(), 1 );
    ASSERT_EQ( text.position(), TextPos( 2, 1 ) );
    ASSERT_EQ( text.character(), U'1' );

    ASSERT_EQ( text.advanceCaret( 5 ), 5 );
    ASSERT_EQ( text.position(), TextPos( 3, 2 ) );
    ASSERT_EQ( text.character(), 'E' );

    ASSERT_EQ( text.advanceCaret( 2 ), 1 );
    ASSERT_EQ( text.position(), TextPos( 3, 3 ) );
    ASSERT_TRUE( text.reachedEnd() );
    ASSERT_EQ( text.character(), 0x00 );
}

TEST( parser_dto_U32Text, character_1 ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.character(), U'B' );
}

TEST( parser_dto_U32Text, character_2 ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( "path/to/source", str );

    auto [cp, end] = text.character( 0 );
    ASSERT_EQ( cp, U'B' );
    ASSERT_FALSE( end );
}

TEST( parser_dto_U32Text, character_3 ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( "path/to/source", str );

    auto [cp, end] = text.character( 5 );
    ASSERT_EQ( cp, U'5' );
    ASSERT_FALSE( end );
}

TEST( parser_dto_U32Text, character_4 ) {
    const std::u32string str = U"";
    auto text = U32Text( "path/to/source", str );

    auto [cp, end] = text.character( 1 );
    ASSERT_EQ( cp, 0x00 );
    ASSERT_TRUE( end );
}

TEST( parser_dto_U32Text, character_fwd ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );

    for( auto i = 1; i <= 9; ++i ) {
        auto [val, end] = text.character( i );
        ASSERT_FALSE( end );
        ASSERT_EQ( val, ( 0x30 + i ) );
    }

    {
        auto[val, end] = text.character( 10 );
        ASSERT_FALSE( end );
        ASSERT_EQ( val, U'E' );
    }

    {
        auto[val, end] = text.character( 11 );
        ASSERT_TRUE( end );
        ASSERT_EQ( val, 0x00 );
    }
}

TEST( parser_dto_U32Text, reconsume_1 ) {
    const std::u32string str = U"012";
    auto text = U32Text( "path/to/source", str );

    text.reconsume();
    ASSERT_EQ( text.nextChar(), str.at( 0 ) );
    ASSERT_EQ( text.nextChar(), str.at( 1 ) );
    text.reconsume();
    ASSERT_EQ( text.nextChar(), str.at( 1 ) );
    ASSERT_EQ( text.nextChar(), str.at( 2 ) );
}

TEST( parser_dto_U32Text, reconsume_2 ) { //text position tracker
    const std::u32string str = U"012";
    auto text = U32Text( "path/to/source", str );

    text.reconsume();
    text.nextChar();
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    text.nextChar();
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) );
    text.reconsume();
    text.nextChar();
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) );
    text.nextChar();
    ASSERT_EQ( text.position(), TextPos( 1, 3 ) );
}

TEST( parser_dto_U32Text, nextChar_1 ) {
    const std::u32string str = U"0123456789";
    auto text = U32Text( "path/to/source", str );

    for( auto i = 1; i < 10; ++i ) {
        ASSERT_EQ( text.nextChar(), str.at( i ) );
    }

    ASSERT_EQ( text.nextChar(), 0x00 );
}

TEST( parser_dto_U32Text, nextChar_2 ) { //test position tracker
    const std::u32string str = U"0123456789";
    auto text = U32Text( "path/to/source", str );

    for( auto i = 1; i < 10; ++i ) {
        text.nextChar();
        ASSERT_EQ( text.position(), TextPos( 1, i + 1 ) );
    }

    text.nextChar();
    ASSERT_EQ( text.position(), TextPos( 1, 11 ) );
    ASSERT_TRUE( text.reachedEnd() );
}

TEST( parser_dto_U32Text, nextChar_3 ) {
    const std::u32string str = U"\n1";
    auto text = U32Text( "path/to/source", str );

    text.nextChar();
    ASSERT_EQ( text.position(), TextPos( 2, 1 ) );
    ASSERT_EQ( text.character(), U'1' );
}

TEST( parser_dto_U32Text, nextChar_4 ) {
    const std::u32string str = U"1";
    auto text = U32Text( "path/to/source", str );

    text.nextChar();
    ASSERT_TRUE( text.reachedEnd() ) << "A";

    text.nextChar();
    ASSERT_TRUE( text.reachedEnd() ) << "B";
}

TEST( parser_dto_U32Text, characters ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( "path/to/source", str );

    auto range1 = text.characters( 5 );
    ASSERT_EQ( range1, U"B1234" );
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) ); //positional caret should not have moved
    ASSERT_EQ( text.character(), U'B' ); //iterator should not have moved

    text.advanceCaret( 5 );
    ASSERT_EQ( text.position(), TextPos( 1, 6 ) );
    auto range2 = text.characters( 10 );

    ASSERT_EQ( range2, U"56789E" );
    ASSERT_EQ( text.character(), U'5' );
    ASSERT_FALSE( text.reachedEnd() );

    text.advanceCaret( 6 );
    ASSERT_EQ( text.position(), TextPos( 1, 12 ) );
    auto range3 = text.characters( 5 );
    ASSERT_EQ( range3, U"" );
    ASSERT_TRUE( text.reachedEnd() );
}

TEST( parser_dto_U32Text, reachedEnd ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );
    ASSERT_FALSE( text.reachedEnd() );

    text.advanceCaret( 10 ); //'B'->'E'
    ASSERT_EQ( text.position(), TextPos( 1, 11 ) );
    ASSERT_EQ( text.character(), U'E' );
    ASSERT_FALSE( text.reachedEnd() );

    text.advanceCaret();
    ASSERT_TRUE( text.reachedEnd() );
}

TEST( parser_dto_U32Text, advanceCarret_1 ) {
    const std::u32string str = U"12345";
    auto text = U32Text( "path/to/source", str );

    for( auto i = 0; i < 5; ++i ) {
        ASSERT_EQ( text.position(), TextPos( 1, i + 1 ) );
        ASSERT_EQ( text.character(), str.at( i ) );
        text.advanceCaret();
    }
}

TEST( parser_dto_U32Text, advanceCarret_2 ) {
    const std::u32string str = U"1\n2";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), str.at( 0 ) );
    text.advanceCaret();
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) );
    ASSERT_EQ( text.character(), str.at( 1 ) );
    text.advanceCaret();
    ASSERT_EQ( text.position(), TextPos( 2, 1 ) );
    ASSERT_EQ( text.character(), str.at( 2 ) );
}

TEST( parser_dto_U32Text, advanceCarret_3 ) {
    const std::u32string str = U"";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.advanceCaret( 5 ), 0 );
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_TRUE( text.reachedEnd() );
}

TEST( parser_dto_U32Text, advanceCarret_4 ) {
    const std::u32string str = U"012345";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.advanceCaret( 5 ), 5 );
    ASSERT_EQ( text.position(), TextPos( 1, 6 ) );
    ASSERT_EQ( text.character(), str.at( 5 ) );
}

TEST( parser_dto_U32Text, advanceCarret_5 ) {
    const std::u32string str = U"123";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.advanceCaret( 4 ), 3 );
    ASSERT_EQ( text.position(), TextPos( 1, 4 ) );
    ASSERT_EQ( text.character(), 0x00 );
}

TEST( parser_dto_U32Text, advanceCarret_6 ) {
    const std::u32string str = U"123";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.advanceCaret( 5 ), 3 );
    ASSERT_EQ( text.position(), TextPos( 1, 4 ) );
    ASSERT_EQ( text.character(), 0x00 );
    ASSERT_TRUE( text.reachedEnd() );
}

TEST( parser_dto_U32Text, advanceCarret_7 ) {
    const std::u32string str = U"1\n3";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.advanceCaret( 2 ), 2 );
    ASSERT_EQ( text.position(), TextPos( 2, 1 ) );
    ASSERT_EQ( text.character(), str.at( 2 ) );
}

TEST( parser_dto_U32Text, reverseCaret_1 ) {
    const std::u32string str = U"";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.reverseCaret(), 0 );
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
}

TEST( parser_dto_U32Text, reverseCaret_2 ) {
    const std::u32string str = U"1";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.reverseCaret(), 0 );
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
}

TEST( parser_dto_U32Text, reverseCaret_3 ) {
    const std::u32string str = U"123";
    auto text = U32Text( "path/to/source", str );

    text.advanceCaret( 2 );
    ASSERT_EQ( text.character(), U'3' ) << "Failed advanceCaret() -> check relevant unit tests for this method.";

    ASSERT_EQ( text.reverseCaret(), 1 )           << "i=2";
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) ) << "i=2";
    ASSERT_EQ( text.character(), U'2' )           << "i=2";

    ASSERT_EQ( text.reverseCaret(), 1 )           << "i=1";
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) ) << "i=1";
    ASSERT_EQ( text.character(), U'1' )           << "i=1";

    ASSERT_EQ( text.reverseCaret(), 0 )           << "i=0";
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) ) << "i=0";
    ASSERT_EQ( text.character(), U'1' )           << "i=0";
}

TEST( parser_dto_U32Text, reverseCaret_4 ) {
    const std::u32string str = U"123";
    auto text = U32Text( "path/to/source", str );

    text.advanceCaret( 2 );
    ASSERT_EQ( text.character(), U'3' ) << "Failed advanceCaret() -> check relevant unit tests for this method.";

    ASSERT_EQ( text.reverseCaret( 3 ), 2 );
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'1' );
}

TEST( parser_dto_U32Text, reverseCaret_5 ) {
    const std::u32string str = U"123";
    auto text = U32Text( "path/to/source", str );

    text.advanceCaret( 2 );
    ASSERT_EQ( text.character(), U'3' ) << "Failed advanceCaret() -> check relevant unit tests for this method.";

    ASSERT_EQ( text.reverseCaret( 4 ), 2 );
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'1' );
}

TEST( parser_dto_U32Text, reverseCaret_6 ) {
    const std::u32string str = U"12\n456\n8";
    auto text = U32Text( "path/to/source", str );

    text.advanceCaret( 7 );
    ASSERT_EQ( text.character(), U'8' )           << "Failed advanceCaret() -> check relevant unit tests for this method.";
    ASSERT_EQ( text.position(), TextPos( 3, 1 ) ) << "Failed advanceCaret() -> check relevant unit tests for this method.";

    ASSERT_EQ( text.reverseCaret(), 1 )           << "2:4";
    ASSERT_EQ( text.position(), TextPos( 2, 4 ) ) << "2:4";
    ASSERT_EQ( text.character(), U'\n' )          << "2:4";

    ASSERT_EQ( text.reverseCaret( 5 ), 5 )        << "1:2";
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) ) << "1:2";
    ASSERT_EQ( text.character(), U'2' )           << "1:2";
}

TEST( parser_dto_U32Text, reverseCaret_advanceCarret_1 ) { //forth and back over a newline
    const std::u32string str = U"1\n3";
    auto text = U32Text( "path/to/source", str );

    text.advanceCaret();
    ASSERT_EQ( text.character(), U'\n' )          << "a) advance by 1";
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) ) << "a) advance by 1";

    text.reverseCaret();
    ASSERT_EQ( text.character(), U'1' )           << "b) reverse by 1";
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) ) << "b) reverse by 1";

    text.advanceCaret();
    ASSERT_EQ( text.character(), U'\n' )          << "c) advance by 1";
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) ) << "c) advance by 1";
}

TEST( parser_dto_U32Text, reverseCaret_advanceCarret_2 ) { //back and forth over a newline
    const std::u32string str = U"1\n3";
    auto text = U32Text( "path/to/source", str );

    text.advanceCaret( 2 );
    ASSERT_EQ( text.character(), U'3' )           << "a) advance by 2";
    ASSERT_EQ( text.position(), TextPos( 2, 1 ) ) << "a) advance by 2";

    text.reverseCaret();
    ASSERT_EQ( text.character(), U'\n' )          << "b) reverse by 1";
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) ) << "b) reverse by 1";

    text.advanceCaret();
    ASSERT_EQ( text.character(), U'3' )           << "c) advance by 1";
    ASSERT_EQ( text.position(), TextPos( 2, 1 ) ) << "c) advance by 1";
}

TEST( parser_dto_U32Text, id_1 ) {
    const std::u32string str;
    const auto hash_val = std::hash<std::string>()( "path/to/source" );
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.id(), hash_val );
}

TEST( parser_dto_U32Text, id_2 ) {
    const std::u32string str;
    auto text1 = U32Text( "path/to/source/1", str );
    auto text2 = U32Text( "path/to/source/2", str );

    ASSERT_NE( text1.id(), text2.id() );
}

TEST( parser_dto_U32Text, path ) {
    const std::u32string str;
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.path(), "path/to/source" );
}

TEST( parser_dto_U32Text, reset ) {
    const std::u32string str = U"B1234\n56789E";
    auto text = U32Text( "path/to/source", str );

    //setup
    ASSERT_EQ( text.advanceCaret( 11 ), 11 );
    ASSERT_EQ( text.position(), TextPos( 2, 6 ) );
    ASSERT_EQ( text.character(), U'E' );

    text.reset();

    //test
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );
}

TEST( parser_dto_U32Text, markers_1 ) {
    const std::u32string str = U"B1234M56789E";
    auto text = U32Text( "path/to/source", str );

    ASSERT_EQ( text.advanceCaret( 5 ), 5 );
    ASSERT_EQ( text.position(), TextPos( 1, 6 ) );
    ASSERT_EQ( text.character(), U'M' );

    auto marker = text.createMarker();

    ASSERT_EQ( text.advanceCaret( 6 ), 6 );
    ASSERT_EQ( text.position(), TextPos( 1, 12 ) );
    ASSERT_EQ( text.character(), U'E' );

    text.resetToMarker( marker );

    ASSERT_EQ( text.position(), TextPos( 1, 6 ) );
    ASSERT_EQ( text.character(), U'M' );
}

TEST( parser_dto_U32Text, markers_2 ) {
    const std::u32string str = U"B1234M56789E";
    auto text1 = U32Text( "path/to/source/1", str );
    auto text2 = U32Text( "path/to/source/2", str );

    ASSERT_EQ( text1.advanceCaret( 5 ), 5 );
    ASSERT_EQ( text1.position(), TextPos( 1, 6 ) );
    ASSERT_EQ( text1.character(), U'M' );

    auto marker = text1.createMarker();

    ASSERT_THROW( text2.resetToMarker( marker ), blogator::exception::failed_expectation );

    ASSERT_EQ( text1.position(), TextPos( 1, 6 ) );
    ASSERT_EQ( text1.character(), U'M' );

    ASSERT_EQ( text2.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text2.character(), U'B' );
}