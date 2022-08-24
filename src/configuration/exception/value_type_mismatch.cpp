#include "value_type_mismatch.h"

/**
 * Constructor
 * @param what Description
 */
blogator::configuration::exception::value_type_mismatch::value_type_mismatch( const std::string &what ) :
    logger::LoggableException( what )
{}

/**
 * Constructor
 * @param what Description
 */
blogator::configuration::exception::value_type_mismatch::value_type_mismatch( const char *what ) :
    logger::LoggableException( what )
{}