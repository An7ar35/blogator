#ifndef BLOGATOR_LOGGER_LOGGABLEEXCEPTION_H
#define BLOGATOR_LOGGER_LOGGABLEEXCEPTION_H

#include <stdexcept>
#include <string>

namespace blogator::logger {
    class LoggableException : std::exception {
      public:
        explicit LoggableException( std::string what );
        LoggableException( char * src, int line, std::string what );

        LoggableException & operator =( const LoggableException & other );

        [[nodiscard]] const char * what() const noexcept override;

      private:
        std::string _what;
    };
}

#endif //BLOGATOR_LOGGER_LOGGABLEEXCEPTION_H