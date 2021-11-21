#include "gtest/gtest.h"
#include "../../../src/parser/dto/U32Text.h"

using blogator::parser::U32Text;
using blogator::parser::TextPos;

TEST( parser_dto_U32Text, advanceColTracker ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );

    text.advanceColTracker();
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) );

    text.advanceColTracker( 3 );
    ASSERT_EQ( text.position(), TextPos( 1, 5 ) );

    text.advanceColTracker( 8 );
    ASSERT_EQ( text.position(), TextPos( 1, 13 ) );
    ASSERT_EQ( text.character(), U'B' );
}

TEST( parser_dto_U32Text, advanceLineTracker ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );

    text.advanceLineTracker();
    ASSERT_EQ( text.position(), TextPos( 2, 1 ) );
    ASSERT_EQ( text.character(), U'B' );
}

TEST( parser_dto_U32Text, advanceCol ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );

    ASSERT_EQ( text.advanceCol(), 1 );
    ASSERT_EQ( text.position(), TextPos( 1, 2 ) );
    ASSERT_EQ( text.character(), U'1' );

    ASSERT_EQ( text.advanceCol( 3 ), 3 );
    ASSERT_EQ( text.position(), TextPos( 1, 5 ) );
    ASSERT_EQ( text.character(), U'4' );

    ASSERT_EQ( text.advanceCol( 6 ), 6 );
    ASSERT_EQ( text.position(), TextPos( 1, 11 ) );
    ASSERT_FALSE( text.reachedEnd() );
    ASSERT_EQ( text.character(), U'E' );

    ASSERT_EQ( text.advanceCol( 4 ), 1 );
    ASSERT_EQ( text.position(), TextPos( 1, 12 ) );
    ASSERT_TRUE( text.reachedEnd() );
    ASSERT_EQ( text.character(), 0x00 );
}

TEST( parser_dto_U32Text, advanceLine ) {
    const std::u32string str = U"B\n1\n2\n3\n45\n6789\nE";
    auto text = U32Text( str );

    size_t line = 1;

    while( !text.reachedEnd() ) {
        auto c = text.character();

        if( c == U'\n' ) {
            text.advanceLine();
            ++line;
        } else {
            text.advanceCol();
        }
    }

    ASSERT_EQ( text.position(), TextPos( 7, 2 ) );
}

TEST( parser_dto_U32Text, character_fwd ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( str );

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

TEST( parser_dto_U32Text, characters ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( str );

    auto range1 = text.characters( 5 );
    ASSERT_EQ( range1, U"B1234" );
    ASSERT_EQ( text.position(), TextPos( 1, 1 ) ); //positional caret should not have moved
    ASSERT_EQ( text.character(), U'B' ); //iterator should not have moved

    text.advanceCol( 5 );
    ASSERT_EQ( text.position(), TextPos( 1, 6 ) );
    auto range2 = text.characters( 10 );

    ASSERT_EQ( range2, U"56789E" );
    ASSERT_EQ( text.character(), U'5' );
    ASSERT_FALSE( text.reachedEnd() );

    text.advanceCol( 6 );
    ASSERT_EQ( text.position(), TextPos( 1, 12 ) );
    auto range3 = text.characters( 5 );
    ASSERT_EQ( range3, U"" );
    ASSERT_TRUE( text.reachedEnd() );
}

TEST( parser_dto_U32Text, reachedEnd ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( str );

    ASSERT_EQ( text.position(), TextPos( 1, 1 ) );
    ASSERT_EQ( text.character(), U'B' );
    ASSERT_FALSE( text.reachedEnd() );

    text.advanceCol( 10 ); //'B'->'E'
    ASSERT_EQ( text.position(), TextPos( 1, 11 ) );
    ASSERT_EQ( text.character(), U'E' );
    ASSERT_FALSE( text.reachedEnd() );

    text.advanceCol();
    ASSERT_TRUE( text.reachedEnd() );
}

TEST( parser_dto_U32Text, reachedEnd_with_iterator ) {
    const std::u32string str = U"B123456789E";
    auto text = U32Text( str );
    auto it   = text.iteratorCurr();

    ASSERT_FALSE( text.reachedEnd( it ) );
    std::advance( it, 10 );
    ASSERT_FALSE( text.reachedEnd( it ) );
    std::advance( it, 1 );
    ASSERT_TRUE( text.reachedEnd( it ) );
    ASSERT_FALSE( text.reachedEnd() );
}