#include "resource_unavailable.h"

using namespace blogator::exception;

/**
 * Constructor
 * @param what Description
 */
resource_unavailable::resource_unavailable( const char * what ) :
    logger::LoggableException( what )
{}

/**
 * Constructor
 * @param what Description
 */
resource_unavailable::resource_unavailable( const std::string & what ) :
    logger::LoggableException( what )
{}