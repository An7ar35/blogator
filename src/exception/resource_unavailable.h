#ifndef BLOGATOR_EXCEPTION_RESOURCE_UNAVAILABLE_H
#define BLOGATOR_EXCEPTION_RESOURCE_UNAVAILABLE_H

#include "../logger/LoggableException.h"

namespace blogator::exception {
    class resource_unavailable : public logger::LoggableException {
      public:
        explicit resource_unavailable( const std::string& what );
        explicit resource_unavailable( const char * what );
    };
}

#endif //BLOGATOR_EXCEPTION_RESOURCE_UNAVAILABLE_H