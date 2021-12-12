#ifndef BLOGATOR_LOGGER_ENGINE_OGWRITER_H
#define BLOGATOR_LOGGER_ENGINE_OGWRITER_H

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

#include "LogQueue.h"
#include "../formatter/LogFormatter.h"
#include "../output/LogOutput.h"
#include "../global/LogMsg.h"

namespace blogator::logger::engine {
    class LogWriter {
      public:
        const uint64_t MIN_SLEEP_TIMEOUT_MS  {  50 };
        const uint64_t DFLT_SLEEP_TIMEOUT_MS { 500 };

        explicit LogWriter( std::shared_ptr<LogQueue<LogMsg>> queue );
        ~LogWriter();

        [[maybe_unused]] bool setTimeout( uint64_t ms );
        void addOutput( LogLevel lvl, std::unique_ptr<formatter::LogFormatter> fmt, std::unique_ptr<output::LogOutput> out );

        bool start();
        bool stop( uint64_t ms = 1000 );
        void resume();

      private:
        struct Output {
            LogLevel                                 lvl_limit;
            std::unique_ptr<formatter::LogFormatter> formatter;
            std::unique_ptr<output::LogOutput>       output;
        };

        typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint_t;
        typedef std::chrono::duration<uint64_t, std::milli>        Duration_ms_t;

        std::shared_ptr<LogQueue<LogMsg>> _queue;
        std::vector<Output>               _outputs;

        std::thread                       _thread;
        std::mutex                        _sleep_mutex;
        std::atomic<bool>                 _is_sleeping;
        std::condition_variable           _wakeup_cond;
        std::atomic<bool>                 _interrupt;

        TimePoint_t                       _last_wakeup_ts;
        Duration_ms_t                     _suspend_timeout_ms;

        void runLoop();
        void checkTiredState( TimePoint_t now );
        void checkSuspendState();

        static bool isTimedOut( const TimePoint_t &start, const TimePoint_t &end, const Duration_ms_t &timeout_ms );
    };
}

#endif //BLOGATOR_LOGGER_ENGINE_OGWRITER_H