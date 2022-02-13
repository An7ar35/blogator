#ifndef BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H
#define BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H

#include <string>
#include <ostream>
#include "Element.h"


namespace blogator::parser::specs::infra {
    enum class Namespace {
        UNKNOWN = 0,
        HTML5,
        MATHML,
        SVG,
        XLINK,
        XML,
        XMLNS,
        OTHER,

        ENUM_END = XMLNS //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, Namespace ns );
    std::u32string to_namespaceURI( Namespace ns );
    std::u32string to_prefix( Namespace ns );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::infra::Namespace ns );
    std::u32string to_u32string( blogator::parser::specs::infra::Namespace ns );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H