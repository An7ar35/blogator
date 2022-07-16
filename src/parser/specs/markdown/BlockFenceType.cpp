#include "BlockFenceType.h"

#include <sstream>
#include <unordered_map>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr BlockFenceType enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, blogator::parser::specs::markdown::BlockFenceType type ) {
    static const auto map = std::unordered_map<BlockFenceType, std::string>( {
        { BlockFenceType::NONE,                "NONE" },
        { BlockFenceType::TRIPLE_TILDE,        "TRIPLE_TILDE" },
        { BlockFenceType::TRIPLE_GRAVE_ACCENT, "TRIPLE_GRAVE_ACCENT" },
    } );

    if( map.contains( type ) ) {
        os << map.at( type );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::markdown::BlockFenceType enum to a string representation
 * @param attr BlockFenceType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::markdown::BlockFenceType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}
