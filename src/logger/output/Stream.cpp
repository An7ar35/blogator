#include "Stream.h"

using namespace blogator::logger::output;

/**
 * Constructor
 * @param name Output description/name
 * @param stream Stream to use as output
 */
Stream::Stream( std::string name, std::ostream &stream ) :
    LogOutput( LogOutputType::STREAM, std::move( name ) ),
    _out( stream )
{}

/**
 * Opens stream (unused)
 */
void Stream::open() {}

/**
 * Flushes output stream
 */
void Stream::close() {
    _out.flush();
}

/**
 * Writes line to output (checked)
 * @param lvl Logging level (unused)
 * @param msg Line
 */
void Stream::write( LogLevel lvl, const std::string &msg ) {
    if( _out ) {
        _out << msg << "\n";
    }
}
