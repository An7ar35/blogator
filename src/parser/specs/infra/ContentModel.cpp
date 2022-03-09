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
    static const auto map = std::map<ContentModel, std::string>( {
        { ContentModel::NOTHING, "nothing" },
        { ContentModel::METADATA, "metadata" },
        { ContentModel::FLOW, "flow" },
        { ContentModel::SECTIONING, "sectioning" },
        { ContentModel::HEADING, "heading" },
        { ContentModel::PHRASING, "phrasing" },
        { ContentModel::EMBEDDED, "embedded" },
        { ContentModel::INTERACTIVE, "interactive" },
        { ContentModel::PALPABLE, "palpable" },
        { ContentModel::SCRIPT_SUPPORT, "script support" },
    } );

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
