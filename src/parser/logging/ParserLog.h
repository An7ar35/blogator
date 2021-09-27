#ifndef BLOGATOR_PARSER_LOGGING_PARSERLOG_H
#define BLOGATOR_PARSER_LOGGING_PARSERLOG_H

#include "ErrorObject.h"

namespace blogator::parser::logging {
    class ParserLog {
      public:
        static ParserLog & getInstance();
        ParserLog();
        virtual ~ParserLog();

        static void log( std::filesystem::path src, specs::Context ctx, int err_code, TextPos position );
      private:
        static ParserLog * _instance;
    };
}

#endif //BLOGATOR_PARSER_LOGGING_PARSERLOG_H
