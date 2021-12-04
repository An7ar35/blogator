#include "RangeError.h"

using namespace blogator::parser::dom::exception;

/**
 * Constructor
 * @param message description
 */
RangeError::RangeError( const std::string &message ) :
    DOMException( "RangeError: " + message )
{}