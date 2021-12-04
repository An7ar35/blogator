#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_REFERENCEERROR_H
#define BLOGATO_PARSER_DOM_EXCEPTIONR_REFERENCEERROR_H

#include "DOMException.h"

namespace blogator::parser::dom::exception {
    class ReferenceError : public DOMException {
      public:
        explicit ReferenceError( const std::string &message );
    };
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_REFERENCEERROR_H