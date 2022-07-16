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
        { InsertionMode::INITIAL,         "initial" },
        { InsertionMode::HEADING,         "heading" },
        { InsertionMode::FORMATTING,      "formatting" },
        { InsertionMode::BLOCKQUOTE,      "blockquote" },
        { InsertionMode::CODE_BLOCK,      "code block" },
        { InsertionMode::HORIZONTAL_RULE, "horizontal rule" },
        { InsertionMode::HYPERLINK,       "hyperlink" },
        { InsertionMode::IMAGE,           "image" },
        { InsertionMode::TABLE,           "table" },
        { InsertionMode::LIST,            "list" },
        { InsertionMode::TASK_LIST,       "task list" },
        { InsertionMode::DEFINITION_LIST, "definition list" },
        { InsertionMode::FOOTNOTE,        "footnote" },
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