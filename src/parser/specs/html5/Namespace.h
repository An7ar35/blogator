#ifndef BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H
#define BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H

namespace blogator::parser::specs::html5 {
    enum class Namespace {
        UNKNOWN     = 0,
        UNSUPPORTED = 1,

        HTML5,
        MATHML,
        SVG,

        //Unsupported
        XMLNS = UNSUPPORTED,

        ENUM_END //used for iterating enums
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H
