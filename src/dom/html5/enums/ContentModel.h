#ifndef BLOGATOR_DOM_HTML5_CONTENTMODEL_H
#define BLOGATOR_DOM_HTML5_CONTENTMODEL_H

namespace blogator::dom::html5 {
    /**
     * Content models
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

#endif //BLOGATOR_DOM_HTML5_CONTENTMODEL_H