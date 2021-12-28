#include "InsertionMode.h"

#include <map>
#include <ostream>
#include <sstream>

using namespace blogator::parser::specs::html5;

/**
 * Output stream operator
 * @param os Output stream
 * @param mode InsertionMode enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::html5::operator <<( std::ostream &os, blogator::parser::specs::html5::InsertionMode mode ) {
    static std::map<InsertionMode, std::string> map;

    if( map.empty() ) {
        map.emplace( InsertionMode::INITIAL, "initial" );
        map.emplace( InsertionMode::BEFORE_HTML, "before html" );
        map.emplace( InsertionMode::BEFORE_HEAD, "before head" );
        map.emplace( InsertionMode::IN_HEAD, "in head" );
        map.emplace( InsertionMode::IN_HEAD_NOSCRIPT, "in head noscript" );
        map.emplace( InsertionMode::AFTER_HEAD, "after head" );
        map.emplace( InsertionMode::IN_BODY, "in body" );
        map.emplace( InsertionMode::TEXT, "text" );
        map.emplace( InsertionMode::IN_TABLE, "in table" );
        map.emplace( InsertionMode::IN_TABLE_TEXT, "in table text" );
        map.emplace( InsertionMode::IN_CAPTION, "in caption" );
        map.emplace( InsertionMode::IN_COLUMN_GROUP, "in column group" );
        map.emplace( InsertionMode::IN_TABLE_BODY, "in table body" );
        map.emplace( InsertionMode::IN_ROW, "in row" );
        map.emplace( InsertionMode::IN_CELL, "in cell" );
        map.emplace( InsertionMode::IN_SELECT, "in select" );
        map.emplace( InsertionMode::IN_SELECT_IN_TABLE, "in select in table" );
        map.emplace( InsertionMode::IN_TEMPLATE, "in template" );
        map.emplace( InsertionMode::AFTER_BODY, "after body" );
        map.emplace( InsertionMode::IN_FRAMESET, "in frameset" );
        map.emplace( InsertionMode::AFTER_FRAMESET, "after frameset" );
        map.emplace( InsertionMode::AFTER_AFTER_BODY, "after after body" );
        map.emplace( InsertionMode::AFTER_AFTER_FRAMESET, "after after frameset" );
    }

    if( map.contains( mode ) ) {
        os << map.at( mode );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::html5::InsertionMode enum to a string representation
 * @param mode InsertionMode enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::html5::InsertionMode mode ) {
    std::stringstream ss;
    ss << mode;
    return ss.str();
}
