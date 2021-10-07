#ifndef BLOGATOR_PARSER_SPECS_H
#define BLOGATOR_PARSER_SPECS_H

#include <string>

namespace blogator::parser::specs {
    enum class Context {
        UNKNOWN = 0,
        BLOGATOR,
        HTML5,
        MARKDOWN,
    };

    std::string ctxToStr( Context ctx );
}

#endif //BLOGATOR_PARSER_SPECS_H