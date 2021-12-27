#ifndef BLOGATOR_EXCEPTION_PARSING_FAILURE_H
#define BLOGATOR_EXCEPTION_PARSING_FAILURE_H

#include "../logger/LoggableException.h"

namespace blogator::exception {
    class parsing_failure : public logger::LoggableException {
      public:
        explicit parsing_failure( const std::string& what );
        explicit parsing_failure( const char * what );
    };
}

#endif //BLOGATOR_PARSER_EXCEPTION_PARSING_FAILURE_H
