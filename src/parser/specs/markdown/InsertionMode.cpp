#include "InsertionMode.h"

#include <unordered_map>
#include <ostream>
#include <sstream>

using namespace blogator::parser::specs::markdown;

/**
 * Output stream operator
 * @param os Output stream
 * @param mode InsertionMode enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, blogator::parser::specs::markdown::InsertionMode mode ) {
    static const auto map = std::unordered_map<InsertionMode, std::string>( {
        { InsertionMode::INITIAL,         "INITIAL" },
        { InsertionMode::CONTENT,         "CONTENT" },
        { InsertionMode::IMAGE,           "IMAGE" },
        { InsertionMode::DEFINITION_LIST, "DEFINITION_LIST" },
        { InsertionMode::END_OF_FILE,     "END_OF_FILE" },
    } );

    if( map.contains( mode ) ) {
        os << map.at( mode );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::markdown::InsertionMode enum to a string representation
 * @param mode InsertionMode enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::markdown::InsertionMode mode ) {
    std::stringstream ss;
    ss << mode;
    return ss.str();
}