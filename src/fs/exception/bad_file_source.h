#include <utility>

#ifndef BLOGATOR_BAD_FILE_SOURCE_H
#define BLOGATOR_BAD_FILE_SOURCE_H

#include <string>
#include <exception>

namespace blogator::fs::exception {
    class bad_file_source : public std::exception {
      public:
        explicit bad_file_source( std::string msg ) : _msg(std::move( msg )) {}

        const char *what() const noexcept override {
            return _msg.c_str();
        }

      private:
        std::string _msg;
    };
}

#endif //BLOGATOR_BAD_FILE_SOURCE_H
