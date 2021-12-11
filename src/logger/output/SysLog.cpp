#include "SysLog.h"

#include <syslog.h>

using namespace blogator::logger::output;

/**
 * Constructor
 * @param name Output description/name
 */
SysLog::SysLog( std::string name ) :
    LogOutput( LogOutputType::SYSLOG, std::move( name ) )
{}

/**
 * Open stream (unused)
 */
void SysLog::open() {}

/**
 * Close stream (unused)
 */
void SysLog::close() {}

/**
 *
 * @param lvl
 * @param msg
 */
void SysLog::write( LogLevel lvl, const std::string &msg ) {
    if( lvl == LogLevel::TRACE ) {
        lvl = LogLevel::DEBUG;
    }

    syslog( static_cast<int>( lvl ), "%s", msg.c_str() );
}
