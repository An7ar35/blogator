#include "ContentModel.h"

#include <map>
#include <ostream>
#include <sstream>

using namespace blogator::parser::specs::infra;

/**
 * Output stream operator
 * @param os Output stream
 * @param model ContentModel enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, blogator::parser::specs::infra::ContentModel model ) {
    static std::map<ContentModel, std::string> map;

    if( map.empty() ) {
        map.emplace( ContentModel::NOTHING, "nothing" );
        map.emplace( ContentModel::METADATA, "metadata" );
        map.emplace( ContentModel::FLOW, "flow" );
        map.emplace( ContentModel::SECTIONING, "sectioning" );
        map.emplace( ContentModel::HEADING, "heading" );
        map.emplace( ContentModel::PHRASING, "phrasing" );
        map.emplace( ContentModel::EMBEDDED, "embedded" );
        map.emplace( ContentModel::INTERACTIVE, "interactive" );
        map.emplace( ContentModel::PALPABLE, "palpable" );
        map.emplace( ContentModel::SCRIPT_SUPPORT, "script support" );
    }

    if( map.contains( model ) ) {
        os << map.at( model );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::infra::ContentModel enum to a string representation
 * @param model ContentModel enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::infra::ContentModel model ) {
    std::stringstream ss;
    ss << model;
    return ss.str();
}
