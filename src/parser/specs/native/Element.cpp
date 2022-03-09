#include "Element.h"

#include <map>
#include <ostream>
#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param el Element enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::native::operator <<( std::ostream &os, blogator::parser::specs::native::Element el ) {
    static const auto map = std::map<Element, std::string>( {
        { Element::NONE, "NONE" },
        { Element::UNKNOWN, "UNKNOWN" },
        { Element::NAMESPACE_OPEN, "NAMESPACE_OPEN" },
        { Element::NAMESPACE_CLOSE, "NAMESPACE_CLOSE" },
        { Element::SECTION_OPEN, "SECTION_OPEN" },
        { Element::SECTION_CLOSE, "SECTION_CLOSE" },
        { Element::VAR_IDENTIFIER, "VAR_IDENTIFIER" },
        { Element::STRING_LITERAL, "STRING_LITERAL" },
        { Element::ESCAPING_STRING, "ESCAPING_STRING" },
        { Element::INTEGER_LITERAL, "INTEGER_LITERAL" },
        { Element::OPERATOR, "OPERATOR" },
        //TODO fill
    } );

    if( map.contains( el ) ) {
        os << map.at( el );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::native::Element enum to a string representation
 * @param el Element enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::native::Element el ) {
    std::stringstream ss;
    ss << el;
    return ss.str();
}
