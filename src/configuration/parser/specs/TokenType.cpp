#include "TokenType.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param type TokenType enum
 * @return Output stream
 */
std::ostream & blogator::configuration::parser::specs::operator <<( std::ostream &os, blogator::configuration::parser::specs::TokenType type ) {
    switch( type ) {
        case TokenType::COMMENT:             { os << "COMMENT";               } break;
        case TokenType::NAMESPACE:           { os << "NAMESPACE";             } break;
        case TokenType::KEY:                 { os << "KEY";                   } break;
        case TokenType::OPERATOR:            { os << "OPERATOR";              } break;
        case TokenType::PUNCTUATOR:          { os << "PUNCTUATOR";            } break;
        case TokenType::BRACKET:             { os << "BRACKET";               } break;
        case TokenType::CHARACTER:           { os << "CHARACTER";             } break;
        case TokenType::VALUE_NAME:          { os << "VALUE_NAME";            } break;
        case TokenType::STRING_LITERAL:      { os << "STRING_LITERAL";        } break;
        case TokenType::INTEGER_LITERAL_10:  { os << "INTEGER_LITERAL_10";    } break;
        case TokenType::INTEGER_LITERAL_16:  { os << "INTEGER_LITERAL_16";    } break;
        case TokenType::INTEGER_LITERAL_FLT: { os << "INTEGER_LITERAL_FLT";   } break;
        case TokenType::BOOLEAN_LITERAL:     { os << "BOOLEAN_LITERAL";       } break;
        case TokenType::END_OF_FILE:         { os << "END_OF_FILE";           } break;
        default:                             { os << "enum string not found"; } break;
    }

    return os;
}

/**
 * Converts a parser::dom::NodeType enum to a string representation
 * @param type TokenType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::configuration::parser::specs::TokenType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}