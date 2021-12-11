#ifndef BLOGATOR_EXCEPTION_FAILED_EXPECTATION_H
#define BLOGATOR_EXCEPTION_FAILED_EXPECTATION_H

#include "../logger/LoggableException.h"

namespace blogator::exception {
    class failed_expectation : public logger::LoggableException {
      public:
        explicit failed_expectation( const std::string &msg );
        failed_expectation( char * src, int line, std::string description );
    };
}

#endif //BLOGATOR_EXCEPTION_FAILED_EXPECTATION_H
