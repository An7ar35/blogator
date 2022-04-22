#ifndef BLOGATOR_PARSER_DOM_VALIDATION_XML_H
#define BLOGATOR_PARSER_DOM_VALIDATION_XML_H

#include "../defs.h"

namespace blogator::parser::dom::validation {
    class XML {
      public:
        static std::vector<DOMString_t> validateNS( const DOMString_t &ns, const DOMString_t &qualified_name );

        static bool checkName( const DOMString_t & name );
        static bool checkQName( const DOMString_t & name );
        static bool checkQName( const DOMString_t & prefix, const DOMString_t & name );
        static bool checkNCName( const DOMString_t & name );
        static bool checkAttrName( const DOMString_t & name );
    };
}

#endif //BLOGATOR_PARSER_DOM_VALIDATION_XML_H