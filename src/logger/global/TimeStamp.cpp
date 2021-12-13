#include "TimeStamp.h"

#include <sstream>

using namespace blogator::logger;

/**
 * Constructor
 */
TimeStamp::TimeStamp() :
    _time_point( std::chrono::high_resolution_clock::now() )
{}

/**
 * Constructor (for testing)
 * @param tp standard lib. high resolution clock time-point
 */
TimeStamp::TimeStamp( std::chrono::time_point<std::chrono::high_resolution_clock> tp ) :
    _time_point( tp )
{}

/**
 * Equal operator
 * @param rhs TimeStamp to compare against
 * @return Equality
 */
bool TimeStamp::operator ==( const TimeStamp &rhs ) const {
    return ( _time_point == rhs._time_point );
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
 * Gets a local time string
 * @param fractional_precision Number of digits to show of the fractions of seconds (0-9)
 * @return Time represented as HH:MM:SS
 */
std::string TimeStamp::getTime( int fractional_precision ) const {
    return std::move( getTimeStamp( "%H:%M:%S", fractional_precision ) );
}

/**
 * Gets a local date string
 * @return Date formatted as dd/mm/yy
 */
std::string TimeStamp::getDate() const {
    return std::move( getTimeStamp( "%d/%m/%Y" ) );
}

/**
 * Gets a local timestamp
 * @param formatter Formatter string
 * @param fractional_precision Number of digits to show of the fractions of seconds (0-9)
 * @return Formatted timestamp
 */
std::string TimeStamp::getTimeStamp( const std::string &formatter, int fractional_precision ) const {
    const auto ts = std::chrono::high_resolution_clock::to_time_t( _time_point );

    std::stringstream ss;
    ss << std::put_time( std::localtime( &ts ), formatter.c_str() );

    if( fractional_precision > 0 ) {
        getFractional( ss, fractional_precision );
    }

    return ss.str();
}

/**
 * Gets a UTC formatted timestamp with fractional precision tagged onto the end
 * @param fractional_precision Number of digits to show of the fractions of seconds (0-9)
 * @return UTC timestamp string
 */
std::string TimeStamp::getUTC( int fractional_precision ) const {
    return std::move( getUTC( "%FT%TZ", fractional_precision ) );
}

/**
 * Gets a UTC custom formatted timestamp
 * @param formatter Formatter string
 * @param fractional_precision Number of digits to show of the fractions of seconds (0-9)
 * @return Formatted UTC timestamp
 */
std::string TimeStamp::getUTC( const std::string &formatter, int fractional_precision ) const {
    const auto ts = std::chrono::high_resolution_clock::to_time_t( _time_point );

    std::stringstream ss;
    ss << std::put_time( std::gmtime( &ts ), formatter.c_str() );

    if( fractional_precision > 0 ) {
        ss << '.';
        getFractional( ss, fractional_precision );
    }

    return ss.str();
}

/**
 * [PRIVATE] Add fractional representation to a stream
 * @param os Output stream
 * @param precision Number of digits to show
 */
void TimeStamp::getFractional( std::ostream &os, int precision ) const {
    if( precision > 9 ) {
        precision = 9;
    }

    const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>( _time_point );
    const auto fraction = _time_point - seconds;

    std::stringstream ss;
    ss << std::setfill( '0' ) << std::setw( 9 ) << fraction.count();

    const auto fraction_str = ss.str();
    os << fraction_str.substr( 0, precision );
}
