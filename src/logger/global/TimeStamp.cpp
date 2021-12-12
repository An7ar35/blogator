#include "TimeStamp.h"

#include <sstream>

using namespace blogator::logger;

/**
 * Constructor
 */
TimeStamp::TimeStamp() :
    _ts( std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() ) )
{}

/**
 * Constructor (for testing)
 * @param ts standard lib. timestamp object
 */
TimeStamp::TimeStamp( std::time_t ts ) :
    _ts( ts )
{}

/**
 * Equal operator
 * @param rhs TimeStamp to compare against
 * @return Equality
 */
bool TimeStamp::operator ==( const TimeStamp &rhs ) const {
    return ( _ts == rhs._ts );
}

/**
 * Not-Equal operator
 * @param rhs TimeStamp to compare against
 * @return Not-equal state
 */
bool TimeStamp::operator !=( const TimeStamp &rhs ) const {
    return !( *this == rhs );
}

/**
 * Get a local time string
 * @return Time represented as HH:MM:SS
 */
std::string TimeStamp::getTime() const {
    std::stringstream ss;
    ss << std::put_time( std::localtime(&_ts ), "%H:%M:%S");
    return ss.str();
}

/**
 * Gets a local date string
 * @return Date formatted as dd/mm/yy
 */
std::string TimeStamp::getDate() const {
    std::stringstream ss;
    ss << std::put_time( std::localtime( &_ts ), "%d/%m/%Y");
    return ss.str();
}

/**
 * Gets a local timestamp
 * @param formatter Formatter string
 * @return Formatted timestamp
 */
std::string TimeStamp::getTimeStamp( const std::string &formatter ) const {
    std::stringstream ss;
    ss << std::put_time( std::localtime( &_ts ), formatter.c_str() );
    return ss.str();
}

/**
 * Gets a UTC formatted timestamp
 * @return UTC timestamp string
 */
std::string TimeStamp::getUTC() const {
    std::stringstream ss;
    ss << std::put_time( std::gmtime( &_ts ), "%FT%TZ" );
    return ss.str();
}

/**
 * Gets a UTC formatted timestamp with
 * @param formatter Formatter string
 * @return Formatted UTC timestamp
 */
std::string TimeStamp::getUTC( const std::string &formatter ) const {
    std::stringstream ss;
    ss << std::put_time( std::gmtime( &_ts ), formatter.c_str() );
    return ss.str();
}