#include "Logger.h"

#include "formatter/DefaultFile.h"
#include "formatter/DefaultPrintMono.h"
#include "formatter/DefaultPrintColour.h"
#include "output/Terminal.h"
#include "output/SysLog.h"
#include "output/Stream.h"
#include "output/File.h"

using namespace blogator::logger;

//static var initialisation
std::mutex                                Logger::_mutex;
std::atomic<bool>                         Logger::_running     = false;
std::atomic<bool>                         Logger::_initialised = false;
uint64_t                                  Logger::_counter     = 0;
std::shared_ptr<engine::LogQueue<LogMsg>> Logger::_queue;
std::unique_ptr<engine::LogWriter>        Logger::_writer;

/**
 * Checks if the Logger is running
 * @return Running state
 */
bool Logger::running() {
    return Logger::_running;
}

/**
 * Adds a sink to the configuration
 * @param lvl Cut-off log level
 * @param fmt Formatter type
 * @param output Output type
 * @param desc Output description/name/path
 * @return Success
 * @throws exception::logger_exception when `open()` call fails on the LogOutput
 */
bool Logger::addOutput( LogLevel lvl, FormatterType_e fmt, OutputType_e output, const std::string &desc ) {
    Logger::init();

    try {
        if( Logger::_writer ) {
            Logger::_writer->addOutput( lvl,
                                        Logger::create( fmt ),
                                        Logger::create( output, desc ) );

            return true;
        }

    } catch( const exception::logger_exception &e ) {
        throw e;
    } catch( const std::invalid_argument &e ) {
        //discarded
    }

    return false;
}

/**
 * Adds a sink to the configuration
 * @param lvl Cut-off log level
 * @param fmt Formatter type
 * @param output LogOutput instance
 * @return Success
 * @throws exception::logger_exception when `open()` call fails on the LogOutput
 */
bool Logger::addOutput( LogLevel lvl, FormatterType_e fmt, std::unique_ptr<Output_t> output ) {
    Logger::init();

    try {
        if( Logger::_writer ) {
            Logger::_writer->addOutput( lvl,
                                        Logger::create( fmt ),
                                        std::move( output ) );

            return true;
        }

    } catch( const exception::logger_exception &e ) {
        throw e;
    } catch( const std::invalid_argument &e ) {
        //discarded
    }

    return false;
}

/**
 * Adds a sink to the configuration
 * @param lvl Cut-off log level
 * @param fmt LogFormatter instance
 * @param output Output type
 * @param desc Output description/name/path
 * @return Success
 * @throws exception::logger_exception when `open()` call fails on the LogOutput
 */
bool Logger::addOutput( LogLevel lvl, std::unique_ptr<Formatter_t> fmt, OutputType_e output, const std::string &desc ) {
    Logger::init();

    try {
        if( Logger::_writer ) {
            Logger::_writer->addOutput( lvl,
                                        std::move( fmt ),
                                        Logger::create( output, desc ) );

            return true;
        }

    } catch( const exception::logger_exception &e ) {
        throw e;
    } catch( const std::invalid_argument &e ) {
        //discarded
    }

    return false;
}

/**
 * Adds a sink to the configuration
 * @param lvl Cut-off log level
 * @param fmt LogFormatter instance
 * @param output LogOutput instance
 * @return Success
 * @throws exception::logger_exception when `open()` call fails on the LogOutput
 */
bool Logger::addOutput( LogLevel lvl, std::unique_ptr<Formatter_t> fmt, std::unique_ptr<Output_t> output ) {
    Logger::init();

    try {
        if( Logger::_writer ) {
            Logger::_writer->addOutput( lvl,
                                        std::move( fmt ),
                                        std::move( output ) );

            return true;
        }

    } catch( const exception::logger_exception &e ) {
        throw e;
    } catch( const std::invalid_argument &e ) {
        //discarded
    }

    return false;
}

/**
 * Adds a sink to the configuration
 * @param lvl Cut-off log level
 * @param fmt LogFormatter instance
 * @param out_stream Custom stream output
 * @param desc Output description/name/path
 * @return Success
 * @throws exception::logger_exception when `open()` call fails on the LogOutput
 */
bool Logger::addOutput( LogLevel lvl, std::unique_ptr<Formatter_t> fmt, std::ostream &out_stream, const std::string &desc ) {
    Logger::init();

    try {
        if( Logger::_writer ) {
            Logger::_writer->addOutput( lvl,
                                        std::move( fmt ),
                                        std::make_unique<output::Stream>( desc, out_stream ) );

            return true;
        }

    } catch( const exception::logger_exception &e ) {
        throw e;
    } catch( const std::invalid_argument &e ) {
        //discarded
    }

    return false;
}

/**
 * Checks if ready (initialised & has output(s) configured)
 * @return Readiness
 */
bool Logger::ready() {
    return Logger::_initialised;
}

/**
 * Starts the logger
 * @return Success
 */
bool Logger::start() {
    if( Logger::ready() ) {
        return ( Logger::_running = Logger::_writer->start() );

    } else {
        return false;
    }
}

/**
 * [PRIVATE] Initialises the logger
 */
void Logger::init() {
    if( !Logger::_initialised ) {
        if( !Logger::_queue ) {
            Logger::_queue = std::make_shared<engine::LogQueue<LogMsg>>();
        }

        if( !Logger::_writer ) {
            Logger::_writer = std::make_unique<engine::LogWriter>( Logger::_queue );
        }

        Logger::_queue->setReadyCallback( [LogWriterObj = &(*Logger::_writer)] { LogWriterObj->resume(); } );

        Logger::_initialised = true;
    }
}

/**
 * [PRIVATE] LogFormatter factory
 * @param fmt_type Formatter type
 * @return LogFormatter
 * @throws std::invalid_argument when type is not recognised/implemented in the factory
 */
std::unique_ptr<formatter::LogFormatter> Logger::create( FormatterType_e fmt_type ) {
    switch( fmt_type ) {
        case formatter::LogFormatterType::DEFAULT_FILE:
            return std::make_unique<formatter::DefaultFile>();

        case formatter::LogFormatterType::DEFAULT_PRINT_MONO:
            return std::make_unique<formatter::DefaultPrintMono>();

        case formatter::LogFormatterType::DEFAULT_PRINT_COLOUR:
            return std::make_unique<formatter::DefaultPrintColour>();

        default:
            throw std::invalid_argument( "[logger::Logger::create( .. )] Unrecognised/unimplemented formatter type." );
    }
}

/**
 * [PRIVATE] LogOutput factory
 * @param out_type Output type
 * @param description Output description
 * @return LogOutput
 * @throws std::invalid_argument when type is not recognised/implemented in the factory
 */
std::unique_ptr<output::LogOutput> Logger::create( OutputType_e out_type, const std::string &description ) {
    switch( out_type ) {
        case output::LogOutputType::SYSLOG:
            return std::make_unique<output::SysLog>( description );

        case output::LogOutputType::TERMINAL:
            return std::make_unique<output::Terminal>( description );

        case output::LogOutputType::FILE:
            return std::make_unique<output::File>( description );

        case output::LogOutputType::STREAM:
            throw std::invalid_argument( "[logger::Logger::create( .. )] STREAM output requires use of appropriate 'create(..)' method." );

        default:
            throw std::invalid_argument( "[logger::Logger::create( .. )] Unrecognised/unimplemented output type." );
    }
}