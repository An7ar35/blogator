#ifndef BLOGATOR_REPORTER_PARSEREPORTER_H
#define BLOGATOR_REPORTER_PARSEREPORTER_H

#include <functional>
#include <unordered_map>
#include <mutex>

#include "dto/ReporterObject.h"

namespace blogator::reporter {
    /**
     * The ParseReporter is responsible for collecting and dispatching any parsing-specific errors
     * raised whilst a source file is processed. Once parsing on a file is completed the `flush(..)`
     * function should be called _if_buffering_is_set_ so that all errors raised in the scope of
     * that file can be dispatched out of the buffer as a block.
     */
    class ParseReporter {
      public:
        typedef std::function<void( const ReporterObject & )>        OutputCallback_f;
        typedef std::vector<ReporterObject>                          ErrorPool_t;
        typedef std::unordered_map<std::string, ErrorPool_t>      ErrorPoolMap_t;
        typedef std::unordered_map<std::string, OutputCallback_f> OutputMap_t;

        ~ParseReporter();

        static void setBuffering( bool flag );
        static void attachOutputCallback( OutputCallback_f cb );
        static void detachOutputCallback();
        static bool appendOutputCallback( const std::string &name, OutputCallback_f cb );
        static bool detachOutputCallback( const std::string &name );

        static void log( ReporterObject && err_obj );
        static void log( std::filesystem::path src, reporter::Context ctx, int err_code, TextPos position );
        static void log( std::filesystem::path src, reporter::Context ctx, int err_code, std::string txt, TextPos position );
        static void flush();
        static void flush( const std::filesystem::path &path );

      private:
        static ParseReporter _instance;

        bool             _buffering { false };
        std::mutex       _mutex;
        ErrorPoolMap_t   _pools     {};
        OutputCallback_f _output_cb;
        OutputMap_t      _outputs   {};

        void attachPrimaryOutputCb( OutputCallback_f cb );
        void detachPrimaryOutputCb();
        bool appendOutputCb( const std::string &name, OutputCallback_f cb );
        bool removeOutputCb( const std::string &name );
        void logErrorObject( ReporterObject &&err );
        void flushPool( const std::filesystem::path &path );
        void flushPools();
        void dispatch( const ReporterObject &err );
    };
}

#endif //BLOGATOR_REPORTER_PARSEREPORTER_H