#ifndef BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H
#define BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::html5 {
    enum class Namespace {
        UNKNOWN = 0,
        HTML5,
        MATHML,
        SVG,

        ENUM_END = SVG //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, Namespace ns );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::html5::Namespace ns );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H