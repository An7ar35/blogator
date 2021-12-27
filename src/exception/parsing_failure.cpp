#include "parsing_failure.h"

using namespace blogator::exception;

/**
 * Constructor
 * @param what Description
 */
parsing_failure::parsing_failure( const char * what ) :
    logger::LoggableException( what )
{}

/**
 * Constructor
 * @param what Description
 */
parsing_failure::parsing_failure( const std::string & what ) :
    logger::LoggableException( what )
{}
