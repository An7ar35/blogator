#ifndef BLOGATOR_EXCEPTION_LOGGER_EXCEPTION_H
#define BLOGATOR_EXCEPTION_LOGGER_EXCEPTION_H

#include <stdexcept>

namespace blogator::exception {
    class logger_exception  : std::runtime_error {
      public:
        explicit logger_exception( std::string what );
    };
}

#endif //BLOGATOR_EXCEPTION_LOGGER_EXCEPTION_H