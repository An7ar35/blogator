#include "Namespace.h"

#include <ostream>
#include <sstream>

using namespace blogator::parser::specs::html5;

/**
 * Output stream operator
 * @param os Output stream
 * @param ns Namespace enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::html5::operator <<( std::ostream &os, blogator::parser::specs::html5::Namespace ns ) {
    switch( ns ) {
        case Namespace::UNKNOWN: { os << "UNKNOWN"; } break;
        case Namespace::HTML5:   { os << "HTML5"; } break;
        case Namespace::MATHML:  { os << "MathML"; } break;
        case Namespace::SVG:     { os << "SVG"; } break;
        default:                 { os << "enum string not found"; }
    }

    return os;
}

/**
 * Converts a parser::specs::html5::Namespace enum to a string representation
 * @param ns Namespace enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::html5::Namespace ns ) {
    std::stringstream ss;
    ss << ns;
    return ss.str();
}
