#include "ListSpacing.h"

#include <unordered_map>
#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr ListSpacing enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, blogator::parser::specs::markdown::ListSpacing spacing ) {
    switch( spacing ) {
        case ListSpacing::DEFAULT_TIGHT: { os << "DEFAULT_TIGHT";         } break;
        case ListSpacing::MAYBE_LOOSE:   { os << "MAYBE_LOOSE";           } break;
        case ListSpacing::LOOSE:         { os << "LOOSE";                 } break;
        default:                         { os << "enum string not found"; } break;
    }

    return os;
}

/**
 * Converts a parser::specs::markdown::ListSpacing enum to a string representation
 * @param attr ListSpacing enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::markdown::ListSpacing spacing ) {
    std::stringstream ss;
    ss << spacing;
    return ss.str();
}