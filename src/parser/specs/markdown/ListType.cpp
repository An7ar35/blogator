#include "ListType.h"

#include <unordered_map>
#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr ListType enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, blogator::parser::specs::markdown::ListType type ) {
    static const auto map = std::unordered_map<ListType, std::string>( {
        { ListType::NONE,           "NONE" },
        { ListType::UL_HYPHEN,      "UL_HYPHEN" },
        { ListType::UL_ASTERISK,    "UL_ASTERISK" },
        { ListType::UL_PLUS_SIGN,   "UL_PLUS_SIGN" },
        { ListType::UL_TASK,        "UL_TASK" },
        { ListType::OL_ALPHA_LOWER, "OL_ALPHA_LOWER" },
        { ListType::OL_ALPHA_UPPER, "OL_ALPHA_UPPER" },
        { ListType::OL_NUMERIC,     "OL_NUMERIC" },
        { ListType::FOOTNOTE_DEFS,  "FOOTNOTE_DEFS" },
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
 * @param attr ListType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::markdown::ListType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}