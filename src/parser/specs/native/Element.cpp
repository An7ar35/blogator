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
    static std::map<Element, std::string> map;

    if( map.empty() ) {
        map.emplace( Element::NONE, "NONE" );
        map.emplace( Element::UNKNOWN, "UNKNOWN" );
        map.emplace( Element::NAMESPACE_OPEN, "NAMESPACE_OPEN" );
        map.emplace( Element::NAMESPACE_CLOSE, "NAMESPACE_CLOSE" );
        map.emplace( Element::SECTION_OPEN, "SECTION_OPEN" );
        map.emplace( Element::SECTION_CLOSE, "SECTION_CLOSE" );
        map.emplace( Element::VAR_IDENTIFIER, "VAR_IDENTIFIER" );
        map.emplace( Element::STRING_LITERAL, "STRING_LITERAL" );
        map.emplace( Element::ESCAPING_STRING, "ESCAPING_STRING" );
        map.emplace( Element::INTEGER_LITERAL, "INTEGER_LITERAL" );
        map.emplace( Element::OPERATOR, "OPERATOR" );
        //TODO fill
    }

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
