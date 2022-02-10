#ifndef BLOGATOR_PARSER_DOM_VALIDATION_XML_H
#define BLOGATOR_PARSER_DOM_VALIDATION_XML_H

#include "../defs.h"

namespace blogator::parser::dom::validation {
    class XML {
      public:
        static bool isValidName( const DOMString_t &name );

      private:
        static bool isNameStartChar( char32_t c );
        static bool isNameChar( char32_t c );
    };
}

#endif //BLOGATOR_PARSER_DOM_VALIDATION_XML_H