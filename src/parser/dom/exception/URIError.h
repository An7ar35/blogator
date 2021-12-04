#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_URIERROR_H
#define BLOGATOR_PARSER_DOM_EXCEPTION_URIERROR_H

#include "DOMException.h"

namespace blogator::parser::dom::exception {
    class URIError : public DOMException {
      public:
        explicit URIError( const std::string &message );
    };
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_URIERROR_H