#ifndef BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H
#define BLOGATOR_PARSER_SPECS_HTML5_NAMESPACE_H

#include <string>
#include <ostream>
#include "Element.h"


namespace blogator::parser::specs::infra {
    enum class Namespace {
        UNKNOWN = 0,
        NONE,
        DEPR,  //HTML4 and depreciated HTML5 tags
        HTML5, //Current HTML5 tags
        HTML,  //catch-all for HTML (depreciated or not)
        MATHML,
        SVG,
        XLINK,
        XML,
        XMLNS,
        OTHER,

        //used for iterating the enums
        ENUM_BEGIN     = UNKNOWN, //start of full range of enums
        ENUM_END       = OTHER,   //end of full range of enums
        URI_DEFS_BEGIN = HTML,    //start of unique URI defined enums
        URI_DEFS_END   = XMLNS,   //end of unique URI defined enums
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