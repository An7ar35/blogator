#include "QuirksMode.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param type QuirksMode enum
 * @return Output stream
 */
std::ostream & blogator::parser::dom::operator <<( std::ostream &os, blogator::parser::dom::QuirksMode mode ) {
    switch( mode ) {
        case QuirksMode::NO_QUIRKS:      { os << "CSS1Compat"; } break;
        case QuirksMode::LIMITED_QUIRKS: [[fallthrough]];
        case QuirksMode::QUIRKS:         { os << "BackCompat"; } break;
    }

    return os;
}

/**
 * Converts a parser::dom::QuirksMode enum to a string representation
 * @param type QuirksMode enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::dom::QuirksMode mode ) {
    std::stringstream ss;
    ss << mode;
    return ss.str();
}

/**
 * Converts a parser::dom::QuirksMode enum to a string representation
 * @param content_type ContentType enum
 * @return String representation
 */
std::u32string blogator::to_u32string( blogator::parser::dom::QuirksMode mode ) {
    auto u8str = to_string( mode );
    return { u8str.cbegin(), u8str.cend() };
}