#include "TokenType.h"

#include <unordered_map>
#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr TokenType enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, blogator::parser::specs::markdown::TokenType type ) {
    static const auto map = std::unordered_map<TokenType, std::string>( {
        { TokenType::UNKNOWN,      "UNKNOWN" },
        { TokenType::BLOCK_END,    "BLOCK_END" },
        { TokenType::CHARACTER,    "CHARACTER" },
        { TokenType::HEADING,      "HEADING" },
        { TokenType::LINE_BREAK,   "LINE_BREAK" },
        { TokenType::FORMAT_BEGIN, "FORMAT_BEGIN" },
        { TokenType::FORMAT_END,   "FORMAT_END" },
        { TokenType::BLOCK_ID,     "BLOCK_ID" },
        { TokenType::IMAGE,        "IMAGE" },
        { TokenType::CODE_BLOCK,   "CODE_BLOCK" },
        { TokenType::END_OF_FILE,  "EOF" },
    } );

    if( map.contains( type ) ) {
        os << map.at( type );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::markdown::TokenType enum to a string representation
 * @param attr TokenType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::markdown::TokenType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}