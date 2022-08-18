#include "TokeniserState.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param type TokeniserState enum
 * @return Output stream
 */
std::ostream & blogator::configuration::parser::specs::operator <<( std::ostream & os, blogator::configuration::parser::specs::TokeniserState state ) {
    switch( state ) {
        case TokeniserState::INITIAL:              { os << "INITIAL";               } break;
        case TokeniserState::KEY:                  { os << "KEY";                   } break;
        case TokeniserState::NAMESPACE:            { os << "NAMESPACE";             } break;
        case TokeniserState::AFTER_KEY:            { os << "AFTER_KEY";             } break;
        case TokeniserState::BEFORE_VALUE:         { os << "BEFORE_VALUE";          } break;
        case TokeniserState::VALUE_NAME:           { os << "VALUE_NAME";            } break;
        case TokeniserState::VALUE_STRING:         { os << "VALUE_STRING";          } break;
        case TokeniserState::VALUE_INTEGER_FLOAT:  { os << "VALUE_INTEGER_FLOAT";   } break;
        case TokeniserState::VALUE_INTEGER_BASE10: { os << "VALUE_INTEGER_BASE10";  } break;
        case TokeniserState::VALUE_INTEGER_BASE16: { os << "VALUE_INTEGER_BASE16";  } break;
        case TokeniserState::VALUE_RAW:            { os << "VALUE_RAW";             } break;
        case TokeniserState::AFTER_VALUE:          { os << "AFTER_VALUE";           } break;
        case TokeniserState::ESCAPED_CHARACTER:    { os << "ESCAPED_CHARACTER";     } break;
        case TokeniserState::COMMENT_SOLIDUS:      { os << "COMMENT_SOLIDUS";       } break;
        case TokeniserState::COMMENT_INLINED:      { os << "COMMENT_INLINED";       } break;
        case TokeniserState::COMMENT_BLOCK:        { os << "COMMENT_BLOCK";         } break;
        case TokeniserState::COMMENT_ASTERISK:     { os << "COMMENT_ASTERISK";      } break;
        case TokeniserState::END_OF_FILE:          { os << "END_OF_FILE";           } break;
        default:                                   { os << "enum string not found"; } break;
    }

    return os;
}

/**
 * Converts a parser::dom::NodeType enum to a string representation
 * @param type NodeType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::configuration::parser::specs::TokeniserState state ) {
    std::stringstream ss;
    ss << state;
    return ss.str();
}