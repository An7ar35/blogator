#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_TYPEERROR_H
#define BLOGATOR_PARSER_DOM_EXCEPTION_TYPEERROR_H

#include "DOMException.h"

namespace blogator::parser::dom::exception {
    class TypeError : public DOMException {
      public:
        explicit TypeError( const std::string &message );
    };
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_TYPEERROR_H