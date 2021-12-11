#include "LogMsg.h"

using namespace blogator::logger;

/**
 * Constructor
 * @param id Log entry id
 * @param lvl Severity level
 * @param src_file Name of source file
 * @param line_num Line number in source file
 * @param message Log message
 */
LogMsg::LogMsg( uint64_t id, LogLevel lvl, std::string src_file, int line_num, std::string message ) :
    _id( id ),
    _level( lvl ),
    _ts( TimeStamp() ),
    _src_file( std::move( src_file ) ),
    _src_line( line_num ),
    _message( std::move( message ) )
{}

/**
 * Gets the log entry id
 * @return ID
 */
uint64_t LogMsg::id() const {
    return _id;
}

/**
 * Gets the log level
 * @return Log level enum
 */
LogLevel LogMsg::level() const {
    return _level;
}

/**
 * Gets the source file name
 * @return Source filename
 */
const std::string & LogMsg::source() const {
    return _src_file;
}

/**
 * Gets the source line number
 * @return Line number
 */
int LogMsg::line() const {
    return _src_line;
}

/**
 * Gets the message's creation timestamp
 * @return Timestamp
 */
const TimeStamp & LogMsg::timestamp() const {
    return _ts;
}

/**
 * Gets the log message
 * @return Log message
 */
const std::string & LogMsg::message() const {
    return _message;
}
