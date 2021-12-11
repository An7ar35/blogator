#include "LoggableException.h"

#include "Logger.h"

using namespace blogator::logger;

/**
 * Constructor
 * @param what Description string
 */
LoggableException::LoggableException( std::string what ) :
    _what( std::move( what ) )
{
    LOG_ERROR( _what.c_str() );
}

/**
 * Constructor
 * @param src Source file (i.e.: pass the "__FILE__" macro)
 * @param line Line in source file (i.e.: pass the "__LINE__" macro)
 * @param what Description
 */
LoggableException::LoggableException( char * src, int line, std::string what ) :
    _what( std::move( what ) )
{
    Logger::print<LogLevel::ERROR>( { line, src }, what );
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