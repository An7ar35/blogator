#ifndef BLOGATOR_EXCEPTION_NOT_IMPLEMENTED_H
#define BLOGATOR_EXCEPTION_NOT_IMPLEMENTED_H

#include <string>
#include <exception>
#include <stdexcept>

namespace blogator {
    class not_implemented : public std::logic_error {
      public:
        explicit not_implemented( const std::string &msg ) :
            std::logic_error( msg )
        {};
    };
}

#endif //BLOGATOR_EXCEPTION_NOT_IMPLEMENTED_H
