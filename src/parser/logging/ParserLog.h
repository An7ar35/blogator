#ifndef BLOGATOR_PARSER_LOGGING_PARSERLOG_H
#define BLOGATOR_PARSER_LOGGING_PARSERLOG_H

#include <functional>

#include "ErrorObject.h"

namespace blogator::parser::logging {
    class ParserLog {
      public:
        typedef std::function<void( const ErrorObject & )> OutputCallback_f;

        static ParserLog & getInstance();
        ParserLog();
        virtual ~ParserLog();

        static void attachOutputCallback( OutputCallback_f cb );
        static void log( std::filesystem::path src, specs::Context ctx, int err_code, TextPos position );
      private:
        static OutputCallback_f _out_cb;
        static ParserLog *      _instance;
    };
}

#endif //BLOGATOR_PARSER_LOGGING_PARSERLOG_H
