#include "gtest/gtest.h"
#include "../../../../src/configuration/parser/specs/TokenType.h"

using namespace blogator::configuration::parser::specs;

TEST( configuration_parser_specs_TokenType, output_stream_operator ) {
    for( auto i = static_cast<int>( TokenType::ENUM_BEGIN ); i <= static_cast<int>( TokenType::ENUM_END ); ++i ) {
        std::stringstream ss;
        ss << static_cast<TokenType>( i );
        ASSERT_NE( "enum string not found", ss.str() )
            << "TokenType '" << i << "' has no string representation ("
            << "prev: \"" << static_cast<TokenType>( i - 1 ) << "\", "
            << "next: \"" << static_cast<TokenType>( i + 1 ) << "\""
            << ").";
    }
}