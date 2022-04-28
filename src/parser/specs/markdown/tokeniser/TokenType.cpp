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
        { TokenType::CHARACTER,    "CHARACTER" },
        { TokenType::FORMAT_BEGIN, "UNKNOWN" },
        { TokenType::FORMAT_END,   "UNKNOWN" },
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