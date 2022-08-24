#ifndef BLOGATOR_CONFIGURATION_EXCEPTION_VALUE_TYPE_MISMATCH_H
#define BLOGATOR_CONFIGURATION_EXCEPTION_VALUE_TYPE_MISMATCH_H

#include "../../logger/LoggableException.h"

namespace blogator::configuration::exception {
    class value_type_mismatch : public logger::LoggableException {
      public:
        explicit value_type_mismatch( const std::string & what );
        explicit value_type_mismatch( const char * what );
    };
}

#endif //BLOGATOR_CONFIGURATION_EXCEPTION_VALUE_TYPE_MISMATCH_H