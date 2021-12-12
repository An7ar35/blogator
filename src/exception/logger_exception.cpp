#include "logger_exception.h"

using namespace blogator::exception;

/**
 * Constructor
 * @param what Description
 */
logger_exception::logger_exception( std::string what ) :
    std::runtime_error( what )
{}
