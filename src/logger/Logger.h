#ifndef BLOGATOR_LOGGER_H
#define BLOGATOR_LOGGER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>
#include <memory>
#include <libgen.h> //basename(..)

#include "global/LogLevel.h"
#include "global/LogMsg.h"
#include "engine/LogQueue.h"
#include "engine/LogWriter.h"

#ifdef DEBUG
    #define LOG_EMERGENCY( args... ) blogator::logger::Logger::print< blogator::logger::LogLevel::EMERGENCY >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_ALERT( args... )     blogator::logger::Logger::print< blogator::logger::LogLevel::ALERT >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_CRITICAL( args... )  blogator::logger::Logger::print< blogator::logger::LogLevel::CRITICAL >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_ERROR( args... )     blogator::logger::Logger::print< blogator::logger::LogLevel::ERROR >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_WARNING( args... )   blogator::logger::Logger::print< blogator::logger::LogLevel::WARNING >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_NOTICE( args... )    blogator::logger::Logger::print< blogator::logger::LogLevel::NOTICE >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG( args... )           blogator::logger::Logger::print< blogator::logger::LogLevel::INFO >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_DEBUG( ... )
    #define LOG_TRACE( ... )
#else
    #define LOG_EMERGENCY( args... ) blogator::logger::Logger::print< blogator::logger::LogLevel::EMERGENCY >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_ALERT( args... )     blogator::logger::Logger::print< blogator::logger::LogLevel::ALERT >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_CRITICAL( args... )  blogator::logger::Logger::print< blogator::logger::LogLevel::CRITICAL >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_ERROR( args... )     blogator::logger::Logger::print< blogator::logger::LogLevel::ERROR >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_WARNING( args... )   blogator::logger::Logger::print< blogator::logger::LogLevel::WARNING >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_NOTICE( args... )    blogator::logger::Logger::print< blogator::logger::LogLevel::NOTICE >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG( args... )           blogator::logger::Logger::print< blogator::logger::LogLevel::INFO >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_DEBUG( args... )     blogator::logger::Logger::print< blogator::logger::LogLevel::DEBUG >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
    #define LOG_TRACE( args... )     blogator::logger::Logger::print< blogator::logger::LogLevel::TRACE >( { __LINE__, basename( (char*) __FILE__ ) }, ##args );
#endif

namespace blogator::logger {
    /**
     * DTO for source information
     */
    struct Source {
        const int    line_number;
        const char * filename;
    };

    /**
     * Logging interface and driver
     * ============================
     * Usage:
     *  Step.1: add formatter/output pair(s)
     *  Step.2: start logger
     */
    class Logger {
      public:
        typedef formatter::LogFormatterType FormatterType_e;
        typedef formatter::LogFormatter     Formatter_t;
        typedef output::LogOutputType       OutputType_e;
        typedef output::LogOutput           Output_t;

        static bool addOutput( LogLevel lvl, FormatterType_e fmt, OutputType_e output, const std::string &desc );
        static bool addOutput( LogLevel lvl, FormatterType_e fmt, std::unique_ptr<Output_t> output );
        static bool addOutput( LogLevel lvl, std::unique_ptr<Formatter_t> fmt, OutputType_e output, const std::string &desc );
        static bool addOutput( LogLevel lvl, std::unique_ptr<Formatter_t> fmt, std::unique_ptr<Output_t> output );
        static bool addOutput( LogLevel lvl, std::unique_ptr<Formatter_t> fmt, std::ostream &out_stream, const std::string &desc );

        static bool ready();
        static bool start();
        static bool running();

        template<logger::LogLevel event_type, typename...Args> static void print( Source src, Args...args );

      private:
        static std::atomic<bool>                         _initialised;
        static std::atomic<bool>                         _running;
        static std::mutex                                _mutex;
        static uint64_t                                  _counter;
        static std::shared_ptr<engine::LogQueue<LogMsg>> _queue;
        static std::unique_ptr<engine::LogWriter>        _writer;

        static void init();

        static std::unique_ptr<formatter::LogFormatter> create( FormatterType_e fmt_type );
        static std::unique_ptr<output::LogOutput> create( OutputType_e out_type, const std::string &description );

        template<LogLevel event_type> static void print_( const Source &src, std::stringstream &ss );
        template<LogLevel event_type, typename H, typename...T> static void print_( const Source &src, std::stringstream &ss, H head, T...tail );
    };

    /**
     * Prints log message
     * @tparam event_type Event type
     * @tparam Args       Argument types
     * @param args        Arguments in message
     */
    template<LogLevel event_type, typename...Args> void Logger::print( Source src, Args...args ) {
        if( Logger::running() ) {
            std::stringstream msg_ss;
            Logger::print_<event_type>( src, msg_ss, args... );
        }
    };

    /**
     * Print helper [end function for recursion]
     * @tparam event_type Event type
     */
    template<LogLevel event_type> void Logger::print_( const Source &src, std::stringstream &ss ) {
        std::lock_guard<std::mutex> guard( Logger::_mutex );

        Logger::_queue->enqueue(
            std::make_unique<logger::LogMsg>(
                LogMsg{ Logger::_counter++,
                        event_type,
                        src.filename,
                        src.line_number,
                        ss.str() }
            )
        );
    }

    /**
     * Print helper [recursive function]
     * @tparam event_type Event type
     * @tparam H          Head type
     * @tparam T          Tail type
     * @param head        Head argument
     * @param tail        Rest of the arguments
     */
    template<LogLevel event_type, typename H, typename...T> void Logger::print_( const Source &src, std::stringstream &ss, H head, T...tail ) {
        ss << head;
        print_<event_type>( src, ss, tail... );
    }
}

#endif //BLOGATOR_LOGGER_H
