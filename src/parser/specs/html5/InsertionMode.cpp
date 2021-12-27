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
        map.emplace( InsertionMode::INITIAL, "INITIAL" );
        map.emplace( InsertionMode::INITIAL, "INITIAL" );
        map.emplace( InsertionMode::BEFORE_HTML, "BEFORE_HTML" );
        map.emplace( InsertionMode::BEFORE_HEAD, "BEFORE_HEAD" );
        map.emplace( InsertionMode::IN_HEAD, "IN_HEAD" );
        map.emplace( InsertionMode::IN_HEAD_NOSCRIPT, "IN_HEAD_NOSCRIPT" );
        map.emplace( InsertionMode::AFTER_HEAD, "AFTER_HEAD" );
        map.emplace( InsertionMode::IN_BODY, "IN_BODY" );
        map.emplace( InsertionMode::TEXT, "TEXT" );
        map.emplace( InsertionMode::IN_TABLE, "IN_TABLE" );
        map.emplace( InsertionMode::IN_TABLE_TEXT, "IN_TABLE_TEXT" );
        map.emplace( InsertionMode::IN_CAPTION, "IN_CAPTION" );
        map.emplace( InsertionMode::IN_COLUMN_GROUP, "IN_COLUMN_GROUP" );
        map.emplace( InsertionMode::IN_TABLE_BODY, "IN_TABLE_BODY" );
        map.emplace( InsertionMode::IN_ROW, "IN_ROW" );
        map.emplace( InsertionMode::IN_CELL, "IN_CELL" );
        map.emplace( InsertionMode::IN_SELECT, "IN_SELECT" );
        map.emplace( InsertionMode::IN_SELECT_IN_TABLE, "IN_SELECT_IN_TABLE" );
        map.emplace( InsertionMode::IN_TEMPLATE, "IN_TEMPLATE" );
        map.emplace( InsertionMode::AFTER_BODY, "AFTER_BODY" );
        map.emplace( InsertionMode::IN_FRAMESET, "IN_FRAMESET" );
        map.emplace( InsertionMode::AFTER_FRAMESET, "AFTER_FRAMESET" );
        map.emplace( InsertionMode::AFTER_AFTER_BODY, "AFTER_AFTER_BODY" );
        map.emplace( InsertionMode::AFTER_AFTER_FRAMESET, "AFTER_AFTER_FRAMESET" );
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
