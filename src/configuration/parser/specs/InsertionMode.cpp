#include "InsertionMode.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param type InsertionMode enum
 * @return Output stream
 */
std::ostream & blogator::configuration::parser::specs::operator <<( std::ostream &os, blogator::configuration::parser::specs::InsertionMode mode ) {
    switch( mode ) {
        case InsertionMode::BEFORE_KEYS:       { os << "BEFORE_KEYS";           } break;
        case InsertionMode::BEFORE_KEY:        { os << "BEFORE_KEY";            } break;
        case InsertionMode::IN_KEY:            { os << "IN_KEY";                } break;
        case InsertionMode::AFTER_KEY:         { os << "AFTER_KEY";             } break;
        case InsertionMode::BEFORE_VALUES:     { os << "BEFORE_VALUES";         } break;
        case InsertionMode::BEFORE_VALUE:      { os << "BEFORE_VALUE";          } break;
        case InsertionMode::IN_NAMED_VALUE:    { os << "IN_VALUE";              } break;
        case InsertionMode::AFTER_VALUE:       { os << "AFTER_VALUE";           } break;
        case InsertionMode::AFTER_SCOPE:       { os << "AFTER_SCOPE";           } break;
        case InsertionMode::END_OF_FILE:       { os << "END_OF_FILE";           } break;
        default:                               { os << "enum string not found"; } break;
    }

    return os;
}

/**
 * Converts a configuration::parser::specs::InsertionMode enum to a string representation
 * @param type InsertionMode enum
 * @return String representation
 */
std::string blogator::to_string( blogator::configuration::parser::specs::InsertionMode mode ) {
    std::stringstream ss;
    ss << mode;
    return ss.str();
}
