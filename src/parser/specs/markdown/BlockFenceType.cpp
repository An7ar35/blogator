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
        { BlockFenceType::QUAD_TILDE,          "QUAD_TILDE" },
        { BlockFenceType::QUAD_GRAVE_ACCENT,   "QUAD_GRAVE_ACCENT" },
    } );

    if( map.contains( type ) ) {
        os << map.at( type );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Gets the character width of a fence
 * @param fence BlockFenceType enum
 * @return Number of characters
 */
size_t blogator::parser::specs::markdown::sizeOf( BlockFenceType fence ) {
    switch( fence ) {
        case BlockFenceType::NONE:                return 0;
        case BlockFenceType::TRIPLE_TILDE:        [[fallthrough]];
        case BlockFenceType::TRIPLE_GRAVE_ACCENT: return 3;
        case BlockFenceType::QUAD_TILDE:          [[fallthrough]];
        case BlockFenceType::QUAD_GRAVE_ACCENT:   return 4;
    }
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
