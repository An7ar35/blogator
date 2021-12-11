#include "failed_expectation.h"

#include <utility>

using namespace blogator::exception;

/**
 * Constructor
 * @param msg Description
 */
failed_expectation::failed_expectation( const std::string &msg ) :
    logger::LoggableException( msg )
{}

/**
 * Constructor
 * @param src Source file (i.e.: pass the "__FILE__" macro)
 * @param line Line in source file (i.e.: pass the "__LINE__" macro)
 * @param description Description
 */
failed_expectation::failed_expectation( char * src, int line, std::string description ) :
    LoggableException( src, line, std::move( description ) )
{}
