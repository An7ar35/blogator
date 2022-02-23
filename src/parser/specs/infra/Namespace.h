#ifndef BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H
#define BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H

#include <string>
#include <ostream>
#include "Element.h"


namespace blogator::parser::specs::infra {
    enum class Namespace {
        UNKNOWN = 0,
        NONE,
        HTML5,
        MATHML,
        SVG,
        XLINK,
        XML,
        XMLNS,
        OTHER,

        //used for iterating the enums
        ENUM_BEGIN     = UNKNOWN, //start of full range of enums
        ENUM_END       = OTHER,   //end of full range of enums
        URI_DEFS_BEGIN = HTML5,   //start of URI defined enums
        URI_DEFS_END   = XMLNS,   //end of URI defined enums
    };

    std::ostream & operator <<( std::ostream &os, Namespace ns );
    std::u32string to_namespaceURI( Namespace ns );
    std::u32string to_prefix( Namespace ns );
    Namespace to_namespace( const std::u32string &uri );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::infra::Namespace ns );
    std::u32string to_u32string( blogator::parser::specs::infra::Namespace ns );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H