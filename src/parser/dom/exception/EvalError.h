#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_EVALERROR_H
#define BLOGATOR_PARSER_DOM_EXCEPTION_EVALERROR_H

#include "DOMException.h"

namespace blogator::parser::dom::exception {
    class EvalError : public DOMException {
      public:
        explicit EvalError( const std::string &message );
    };
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_EVALERROR_H