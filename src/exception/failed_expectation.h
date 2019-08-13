#ifndef BLOGATOR_FAILED_EXPECTATION_H
#define BLOGATOR_FAILED_EXPECTATION_H

#include <string>
#include <exception>
#include <stdexcept>

namespace blogator::exception {
    class failed_expectation : public std::runtime_error {
      public:
        explicit failed_expectation( const std::string &msg ) :
            runtime_error( msg )
        {};
    };
}

#endif //BLOGATOR_FAILED_EXPECTATION_H
