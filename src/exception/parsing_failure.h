#ifndef BLOGATOR_EXCEPTION_PARSINGFAILURE_H
#define BLOGATOR_EXCEPTION_PARSINGFAILURE_H

#include <stdexcept>

namespace blogator::exception {
    class parsing_failure : public std::logic_error {
      public:
        explicit parsing_failure( const std::string& what );
        explicit parsing_failure( const char * what );
    };
}

#endif //BLOGATOR_PARSER_EXCEPTION_PARSINGFAILURE_H
