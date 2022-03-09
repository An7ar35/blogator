#include "LoggableException.h"

#include "Logger.h"

using namespace blogator::logger;

/**
 * Constructor
 * @param what Description string
 * @param quiet (optional) Flag to suppress logger call
 */
LoggableException::LoggableException( std::string what, bool quiet ) :
    _what( std::move( what ) )
{
    if( !quiet ) {
        LOG_ERROR( _what.c_str() );
    }
}


/**
 * Constructor
 * @param src Source file (i.e.: pass the "__FILE__" macro)
 * @param line Line in source file (i.e.: pass the "__LINE__" macro)
 * @param what Description
 * @param quiet (optional) Flag to suppress logger call
 */
LoggableException::LoggableException( const char * src, int line, std::string what, bool quiet ) :
    _what( std::move( what ) )
{
    if( !quiet ) {
        Logger::print<LogLevel::ERROR>( { line, basename( (char*) src ) }, _what );
    }
}

/**
 * Copy-assignment operator
 * @param other LoggableException to copy
 * @return Copy
 */
LoggableException & LoggableException::operator =( const LoggableException &other ) {
    _what = other._what;
    return *this;
}

/**
 * Gets the event description string
 * @return Description as a C string
 */
const char * LoggableException::what() const noexcept {
    return _what.c_str();
}