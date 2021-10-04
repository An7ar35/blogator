#ifndef BLOGATOR_PARSER_LOGGING_ERROROBJECT_H
#define BLOGATOR_PARSER_LOGGING_ERROROBJECT_H

#include <filesystem>

#include "../specs/Context.h"
//#include "../specs/blogator/ErrorCode.h"
#include "../specs/html5/ErrorCode.h"
//#include "../specs/markdown/ErrorCode.h"
#include "../dto/TextPos.h"

namespace blogator::parser::logging {
    class ErrorObject {
      public:
        ErrorObject( std::filesystem::path src, specs::Context ctx, int err_code, TextPos position );

        [[nodiscard]] std::string filepath() const;
        [[nodiscard]] std::string context() const;
        [[nodiscard]] std::string error() const;
        [[nodiscard]] std::string detailed() const;
        [[nodiscard]] std::string position() const;

        std::ostream & filepath( std::ostream & os ) const;
        std::ostream & context( std::ostream & os ) const;
        std::ostream & error( std::ostream & os ) const;
        std::ostream & detailed( std::ostream & os ) const;
        std::ostream & position( std::ostream & os ) const;

      private:
        std::filesystem::path _src_file;
        specs::Context        _context;
        int                   _code;
        TextPos               _position;
    };
}

#endif //BLOGATOR_PARSER_LOGGING_ERROROBJECT_H