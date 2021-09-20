#ifndef BLOGATOR_PARSER_SPECS_H
#define BLOGATOR_PARSER_SPECS_H

namespace blogator::parser::specs {
    enum class Context {
        UNKNOWN = 0,
        BLOGATOR,
        HTML5,
        MARKDOWN,
    };
}

#endif //BLOGATOR_PARSER_SPECS_H