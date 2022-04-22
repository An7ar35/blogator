#ifndef BLOGATOR_PARSER_LOGGING_PARSERLOG_H
#define BLOGATOR_PARSER_LOGGING_PARSERLOG_H

#include <functional>
#include <unordered_map>
#include <mutex>

#include "ErrorObject.h"

namespace blogator::parser::logging {
    /**
     * The ParserLog is responsible for collecting and dispatching any parsing-specific errors
     * raised whilst a source file is processed. Once parsing on a file is completed the `flush(..)`
     * function should be called _if_buffering_is_set_ so that all errors raised in the scope of
     * that file can be dispatched out of the buffer as a block.
     */
    class ParserLog {
      public:
        typedef std::function<void( const ErrorObject & )>        OutputCallback_f;
        typedef std::vector<ErrorObject>                          ErrorPool_t;
        typedef std::unordered_map<std::string, ErrorPool_t>      ErrorPoolMap_t;
        typedef std::unordered_map<std::string, OutputCallback_f> OutputMap_t;

        ~ParserLog();

        static void setBuffering( bool flag );
        static void attachOutputCallback( OutputCallback_f cb );
        static void detachOutputCallback();
        static bool appendOutputCallback( const std::string &name, OutputCallback_f cb );
        static bool detachOutputCallback( const std::string &name );

        static void log( std::filesystem::path src, specs::Context ctx, int err_code, TextPos position );
        static void log( std::filesystem::path src, specs::Context ctx, int err_code, std::string txt, TextPos position );
        static void flush();
        static void flush( const std::filesystem::path &path );

      private:
        static ParserLog _instance;

        bool             _buffering { false };
        std::mutex       _mutex;
        ErrorPoolMap_t   _pools     {};
        OutputCallback_f _output_cb;
        OutputMap_t      _outputs   {};

        void attachPrimaryOutputCb( OutputCallback_f cb );
        void detachPrimaryOutputCb();
        bool appendOutputCb( const std::string &name, OutputCallback_f cb );
        bool removeOutputCb( const std::string &name );
        void log( ErrorObject &&err );
        void flushPool( const std::filesystem::path &path );
        void flushPools();
        void dispatch( const ErrorObject &err );
    };
}

#endif //BLOGATOR_PARSER_LOGGING_PARSERLOG_H