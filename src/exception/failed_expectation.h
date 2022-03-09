#ifndef BLOGATOR_EXCEPTION_FAILED_EXPECTATION_H
#define BLOGATOR_EXCEPTION_FAILED_EXPECTATION_H

#include "../logger/LoggableException.h"

#define FAILED_EXPECTATION_EXCEPTION( desc ) blogator::exception::failed_expectation( __FILE__, __LINE__, desc );

namespace blogator::exception {
    class failed_expectation : public logger::LoggableException {
      public:
        failed_expectation( const char * src, int line, std::string description );
    };
}

#endif //BLOGATOR_EXCEPTION_FAILED_EXPECTATION_H