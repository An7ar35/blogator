#ifndef BLOGATOR_PARSER_SPECS_HTML5_CONTENTMODELS_H
#define BLOGATOR_PARSER_SPECS_HTML5_CONTENTMODELS_H

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

        ENUM_END //Not HTML - used for iterating enums
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_CONTENTMODELS_H
