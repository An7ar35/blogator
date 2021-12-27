#ifndef BLOGATOR_PARSER_SPECS_HTML5_CONTENTMODELS_H
#define BLOGATOR_PARSER_SPECS_HTML5_CONTENTMODELS_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::html5 {
    /**
     * HTML Content models
     */
    enum class ContentModel {
        NOTHING = 0,

        METADATA,
        FLOW,
        SECTIONING,
        HEADING,
        PHRASING,
        EMBEDDED,
        INTERACTIVE,
        PALPABLE,
        SCRIPT_SUPPORT,

        ENUM_END = SCRIPT_SUPPORT, //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, ContentModel model );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::html5::ContentModel model );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_CONTENTMODELS_H
