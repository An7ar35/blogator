#ifndef BLOGATOR_EXCEPTION_FILE_ACCESS_FAILURE_H
#define BLOGATOR_EXCEPTION_FILE_ACCESS_FAILURE_H

#include <string>
#include <exception>

namespace blogator::exception {
    class file_access_failure : public std::exception {
      public:
        explicit file_access_failure( std::string msg ) : _msg(std::move( msg )) {}

        const char *what() const noexcept override {
            return _msg.c_str();
        }

      private:
        std::string _msg;
    };
}

#endif //BLOGATOR_EXCEPTION_FILE_ACCESS_FAILURE_H
