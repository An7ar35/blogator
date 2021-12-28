#ifndef BLOGATOR_PARSER_SPECS_H
#define BLOGATOR_PARSER_SPECS_H

#include <string>
#include <ostream>

namespace blogator::parser::specs {
    enum class Context {
        UNKNOWN = 0,
        NATIVE,
        HTML5,
        MARKDOWN,

        ENUM_END = MARKDOWN //used for iterating enum
    };

    std::ostream & operator <<( std::ostream &os, Context ctx );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::Context ctx );
}

#endif //BLOGATOR_PARSER_SPECS_H