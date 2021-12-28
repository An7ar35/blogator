#include "InsertionMode.h"

#include <map>
#include <ostream>
#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param mode InsertionMode enum
 * @return Output stream
 */
std::ostream &blogator::parser::specs::native::operator <<( std::ostream &os, blogator::parser::specs::native::InsertionMode mode ) {
    static std::map<InsertionMode, std::string> map;

    if( map.empty() ) {
        map.emplace( InsertionMode::INITIAL, "INITIAL" );
        map.emplace( InsertionMode::PRE_NAMESPACE, "PRE_NAMESPACE" );
        map.emplace( InsertionMode::IN_NAMESPACE, "IN_NAMESPACE" );
        map.emplace( InsertionMode::POST_NAMESPACE, "POST_NAMESPACE" );
        //TODO
    }

    if( map.contains( mode ) ) {
        os << map.at( mode );
    } else {
        os << "not found";
    }

    return os;
}

/**
 * Converts a parser::specs::native::InsertionMode enum to a string representation
 * @param mode InsertionMode enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::native::InsertionMode mode ) {
    std::stringstream ss;
    ss << mode;
    return ss.str();
}
