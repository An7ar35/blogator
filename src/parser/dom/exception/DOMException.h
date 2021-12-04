#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTION_H
#define BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTION_H

#include <stdexcept>
#include <string>

#include "DOMExceptionType.h"

namespace blogator::parser::dom::exception {
    class DOMException : public std::exception {
      public:
        explicit DOMException( std::string what );
        explicit DOMException( const char * what );
        explicit DOMException( DOMExceptionType type );
        explicit DOMException( DOMExceptionType type, std::string what );
        explicit DOMException( DOMExceptionType type, const char * what );
        DOMException( const DOMException & other );

        DOMException & operator =( const DOMException & other );

        [[nodiscard]] const char * what() const noexcept override;

      private:
        static std::string errToStr( DOMExceptionType type );

        std::string _message;
    };
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTION_H