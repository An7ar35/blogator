#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_RANGEERROR_H
#define BLOGATOR_PARSER_DOM_EXCEPTION_RANGEERROR_H

#include "DOMException.h"

namespace blogator::parser::dom::exception {
    class RangeError : public DOMException {
      public:
        explicit RangeError( const std::string &message );
    };
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_RANGEERROR_H