#include "failed_expectation.h"

using namespace blogator::exception;

/**
 * Constructor
 * @param src Source file (i.e.: pass the "__FILE__" macro)
 * @param line Line in source file (i.e.: pass the "__LINE__" macro)
 * @param description Description
 */
failed_expectation::failed_expectation( const char * src, int line, std::string description ) :
    LoggableException( src, line, std::move( description ) )
{}