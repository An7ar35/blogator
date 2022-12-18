#include "LogWriter.h"

#include <syslog.h>
#include <sstream>
#include <utility>

using namespace blogator::logger::engine;
using           blogator::logger::formatter::LogFormatter;
using           blogator::logger::output::LogOutput;

/**
 * Constructor
 * @param queue LogQueue to use as source
 */
LogWriter::LogWriter( std::shared_ptr<LogQueue<LogMsg>> queue ) :
    _queue( std::move( queue ) ),
    _is_sleeping( false ),
    _interrupt( false ),
    _last_wakeup_ts( std::chrono::steady_clock::now() ),
    _suspend_timeout_ms( DFLT_SLEEP_TIMEOUT_MS )
{}

/**
 * Destructor
 */
LogWriter::~LogWriter() {
    stop();
}

/**
 * Sets the duration of the sleep timeout value (i.e.: timer-to-sleep since last event)
 * @param ms Timeout value in milliseconds
 * @return Success
 */
[[maybe_unused]] bool LogWriter::setTimeout( uint64_t ms ) {
    if( ms >= MIN_SLEEP_TIMEOUT_MS ) {
        _suspend_timeout_ms = Duration_ms_t( ms );
        return true;
    }

    return false;
}

/**
 * Adds an formatter and output pair to the list of Outputs
 * @param lvl Level limit (i.e.: least significant level to be logged)
 * @param fmt Formatter
 * @param out Output
 * @throws exception::logger_exception when `open()` call fails on the LogOutput (output will be removed if this is the case)
 */
void LogWriter::addOutput( LogLevel lvl, std::unique_ptr<LogFormatter> fmt, std::unique_ptr<LogOutput> out ) {
    _outputs.emplace_back( Output{ lvl, std::move( fmt), std::move( out ) } );
    try {
        _outputs.back().output->open();

    } catch( const exception::logger_exception &e ) {
        _outputs.pop_back();
        throw e;
    }
}

/**
 * Removes a formatter and output pair from the list of Outputs
 * @param description Description used for the LogOutput class used
 * @return Success
 */
bool LogWriter::removeOutput( const std::string &description ) {
    std::lock_guard<std::mutex> guard( _output_mutex );
    auto output_it = std::find_if( _outputs.begin(),
                                                _outputs.end(),
                                                [&description]( const Output &o ) { return o.output->name() == description; } );

    if( output_it != _outputs.end() ) {
        _outputs.erase( output_it );
        return true;
    }

    return false;
}

/**
 * Starts the log writer (not thread safe!)
 * @return Success
 */
bool LogWriter::start() {
    if( _interrupt ) {
        syslog( LOG_ERR,
                "[logger::engine::LogWriter::start()] Log writer loop already active."
        );

        return false;
    }

    if( _outputs.empty() ) {
        syslog( LOG_ERR,
                "[logger::engine::LogWriter::start()] No outputs defined."
        );

        return false;
    }

    try {
        _thread = std::thread( &LogWriter::runLoop, this );

    } catch( const std::system_error &e ) {
        std::stringstream ss;

        ss << "[logger::engine::LogWriter::start()] "
           << "Failed to create/launch worker thread: "
           << e.code();

        syslog( LOG_ERR, "%s", ss.str().c_str() );

        return false;
    }

    return true;
}

/**
 * Stops the log writer
 * @param ms Timeout value in milliseconds before giving up
 * @return Success
 */
bool LogWriter::stop( uint64_t ms ) {
    resume(); //get the loop going so that any remaining object in queue can be processed

    _interrupt = true;

    const TimePoint_t start   = std::chrono::steady_clock::now();
    const auto        timeout = Duration_ms_t( ms );

    //Actively wait for the end or the timeout
    while( !_interrupt && !LogWriter::isTimedOut( start, std::chrono::steady_clock::now(), timeout ) );

    _thread.join();

    return !( _interrupt );
}

/**
 * Wake up the log writer
 */
void LogWriter::resume() {
    {
        std::lock_guard<std::mutex> guard( _sleep_mutex );
        _wakeup_cond.notify_one();
    }

    //reset suspend timeout timer
    _is_sleeping = false;
    _last_wakeup_ts = std::chrono::steady_clock::now();
}

/**
 * [PRIVATE] Main worker loop
 * @throws exception::logger_exception when `close()` on an output fails
 */
void LogWriter::runLoop() {
    while( !_interrupt ) {
        checkSuspendState();

        while( true ) {
            auto log_msg = _queue->dequeue();

            if( !log_msg ) {
                break;
            }

            {
                std::lock_guard<std::mutex> guard( _output_mutex );

                for( const auto &out: _outputs ) {
                    if( log_msg->level() <= out.lvl_limit ) {
                        out.output->write( log_msg->level(),
                                           out.formatter->format( *log_msg )
                        );
                    }
                }
            }
        }

        checkTiredState( std::chrono::steady_clock::now() );
    }

    for( const auto &out : _outputs ) {
        out.output->close();
    }
}

/**
 * [PRIVATE] Checks if it's time for the thread to go to sleep
 * @param now Current time point
 */
void LogWriter::checkTiredState( LogWriter::TimePoint_t now ) {
    if( !_interrupt && LogWriter::isTimedOut( _last_wakeup_ts, now, _suspend_timeout_ms ) ) {
        _is_sleeping = true;
    }
}

/**
 * [PRIVATE] Checks if the thread is sleeping and waits for wakeup call if it is
 */
void LogWriter::checkSuspendState() {
    std::unique_lock<std::mutex> guard( _sleep_mutex );

    while( _is_sleeping ) {
        _wakeup_cond.wait( guard );
    }
}

/**
 * [PRIVATE] Checks if a timer has timed out
 * @param start Starting time point
 * @param end   Current time point
 * @param timeout_ms Timeout value in ms
 * @return Timeout state
 */
bool LogWriter::isTimedOut( const TimePoint_t &start, const TimePoint_t &end, const Duration_ms_t &timeout_ms ) {
    const long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
    return ( elapsed > timeout_ms.count() );
}