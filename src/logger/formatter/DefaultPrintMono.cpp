#include "DefaultPrintMono.h"

using namespace blogator::logger::formatter;

/**
 * Constructor
 */
DefaultPrintMono::DefaultPrintMono() :
    LogFormatter( LogFormatterType::DEFAULT_PRINT_MONO )
{}

/**
 * Constructor
 * @param type Formatter type
 */
DefaultPrintMono::DefaultPrintMono( LogFormatterType type ) :
    LogFormatter( type )
{}

/**
 * Format log message object
 * @param obj Log message object
 * @return Formatted string
 */
std::string DefaultPrintMono::format( const LogMsg &obj ) {
    std::stringstream ss;
    formatLogCounter( ss, obj );
    formatTimestamp( ss, obj );
    formatSource( ss, obj );
    formatLogLevel( ss, obj );
    formatMessageStr( ss, obj );
    return ss.str();
}

/**
 * Format log message number counter
 * @param os Output stream
 * @param obj Log message object
 */
void DefaultPrintMono::formatLogCounter( std::ostream &os, const LogMsg &obj ) {
    os << "[";
    os.fill( '0' );
    os.width( 7 );
    os << obj.id() << "] ";
}

/**
 * Format log level description
 * @param os Output stream
 * @param obj Log message object
 */
void DefaultPrintMono::formatLogLevel( std::ostream &os, const LogMsg &obj ) {
    switch(  obj.level()  ) {
        case LogLevel::EMERGENCY: { os << "EMERGENCY "; } break;
        case LogLevel::ALERT:     { os << "ALERT     "; } break;
        case LogLevel::CRITICAL:  { os << "CRITICAL  "; } break;
        case LogLevel::ERROR:     { os << "ERROR     "; } break;
        case LogLevel::WARNING:   { os << "WARNING   "; } break;
        case LogLevel::NOTICE:    { os << "NOTICE    "; } break;
        case LogLevel::INFO:      { os << "INFO      "; } break;
        case LogLevel::DEBUG:     { os << "DEBUG     "; } break;
        case LogLevel::TRACE:     { os << "TRACE     "; } break;
        default:                  { os << "UNKNOWN   "; } break;
    }
}

/**
 * Format timestamp
 * @param os Output stream
 * @param obj Log message object
 */
void DefaultPrintMono::formatTimestamp( std::ostream &os, const LogMsg &obj ) {
    os << obj.timestamp() << " ";
}

/**
 * Format source details
 * @param os Output stream
 * @param obj Log message object
 */
void DefaultPrintMono::formatSource( std::ostream &os, const LogMsg &obj ) {
    os << "[" << obj.source() << ":" << obj.line() << "] ";
}

/**
 * Format message
 * @param os Output stream
 * @param obj Log message object
 */
void DefaultPrintMono::formatMessageStr( std::ostream &os, const LogMsg &obj ) {
    os << obj.message();
}