#include "InsertionMode.h"

#include <map>
#include <ostream>
#include <sstream>

using namespace blogator::parser::specs::infra;

/**
 * Output stream operator
 * @param os Output stream
 * @param mode InsertionMode enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, blogator::parser::specs::infra::InsertionMode mode ) {
    static const auto map = std::map<InsertionMode, std::string>( {
        { InsertionMode::INITIAL, "initial" },
        { InsertionMode::BEFORE_HTML, "before html" },
        { InsertionMode::BEFORE_HEAD, "before head" },
        { InsertionMode::IN_HEAD, "in head" },
        { InsertionMode::IN_HEAD_NOSCRIPT, "in head noscript" },
        { InsertionMode::AFTER_HEAD, "after head" },
        { InsertionMode::IN_BODY, "in body" },
        { InsertionMode::TEXT, "text" },
        { InsertionMode::IN_TABLE, "in table" },
        { InsertionMode::IN_TABLE_TEXT, "in table text" },
        { InsertionMode::IN_CAPTION, "in caption" },
        { InsertionMode::IN_COLUMN_GROUP, "in column group" },
        { InsertionMode::IN_TABLE_BODY, "in table body" },
        { InsertionMode::IN_ROW, "in row" },
        { InsertionMode::IN_CELL, "in cell" },
        { InsertionMode::IN_SELECT, "in select" },
        { InsertionMode::IN_SELECT_IN_TABLE, "in select in table" },
        { InsertionMode::IN_TEMPLATE, "in template" },
        { InsertionMode::AFTER_BODY, "after body" },
        { InsertionMode::IN_FRAMESET, "in frameset" },
        { InsertionMode::AFTER_FRAMESET, "after frameset" },
        { InsertionMode::AFTER_AFTER_BODY, "after after body" },
        { InsertionMode::AFTER_AFTER_FRAMESET, "after after frameset" },
    } );

    if( map.contains( mode ) ) {
        os << map.at( mode );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::infra::InsertionMode enum to a string representation
 * @param mode InsertionMode enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::infra::InsertionMode mode ) {
    std::stringstream ss;
    ss << mode;
    return ss.str();
}
