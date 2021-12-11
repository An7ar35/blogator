#include "LogOutput.h"

using namespace blogator::logger::output;

/**
 * Constructor
 * @param type Output type
 * @param name Output description/name
 */
LogOutput::LogOutput( LogOutputType type, std::string name ) :
    _type( type ),
    _name( std::move( name ) )
{}

/**
 * Equality operator
 * @param rhs LogOutput to compare to
 * @return Equality state (same object or same type & name)
 */
bool LogOutput::operator ==( const LogOutput &rhs ) const {
    return ( this == &rhs ) || ( _type == rhs._type && _name == rhs._name );
}

/**
 * Negative-Equality operator
 * @param rhs LogOutput to compare to
 * @return No-equality state
 */
bool LogOutput::operator !=( const LogOutput &rhs ) const {
    return !( *this == rhs );
}

/**
 * Gets the output type
 * @return LogOutputType enum
 */
LogOutputType LogOutput::type() const {
    return _type;
}

/**
 * Gets the description string
 * @return Output description
 */
const std::string &LogOutput::name() const {
    return _name;
}
