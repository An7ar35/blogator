#ifndef BLOGATOR_EXCEPTION_SRC_CONTENT_EXCEPTION_H
#define BLOGATOR_EXCEPTION_SRC_CONTENT_EXCEPTION_H

#include <string>
#include <exception>

namespace blogator::exception {
    class file_parsing_failure : public std::exception {
      public:
        explicit file_parsing_failure( std::string msg ) : _msg(std::move( msg )) {}

        const char *what() const noexcept override {
            return _msg.c_str();
        }

      private:
        std::string _msg;
    };
}

#endif //BLOGATOR_EXCEPTION_SRC_CONTENT_EXCEPTION_H
