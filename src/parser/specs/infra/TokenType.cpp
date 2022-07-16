#include "TokenType.h"

#include <unordered_map>
#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr TokenType enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, blogator::parser::specs::infra::TokenType type ) {
    static const auto map = std::unordered_map<TokenType, std::string>( {
        { TokenType::UNKNOWN,     "UNKNOWN" },
        { TokenType::DOCTYPE,     "DOCTYPE" },
        { TokenType::START_TAG,   "START_TAG" },
        { TokenType::END_TAG,     "END_TAG" },
        { TokenType::CHARACTER,   "CHARACTER" },
        { TokenType::COMMENT,     "COMMENT" },
        { TokenType::END_OF_FILE, "EOF" },
    } );

    if( map.contains( type ) ) {
        os << map.at( type );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::infra::TokenType enum to a string representation
 * @param attr TokenType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::infra::TokenType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}