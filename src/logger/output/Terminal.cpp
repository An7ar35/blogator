#include "Terminal.h"

#include <iostream>

using namespace blogator::logger::output;

/**
 * Constructor
 * @param name Output description/name
 */
Terminal::Terminal( std::string name ) :
    LogOutput( LogOutputType::TERMINAL, std::move( name ) )
{}

/**
 * Open stream (unused)
 */
void Terminal::open() {}

/**
 * Flush standard out stream
 */
void Terminal::close() {
    std::cout << std::flush;
}

/**
 * Writes line to output
 * @param lvl Logging level (unused)
 * @param msg Line
 */
void Terminal::write( [[maybe_unused]] LogLevel lvl, const std::string &msg ) {
    std::cout << msg << "\n";
}
