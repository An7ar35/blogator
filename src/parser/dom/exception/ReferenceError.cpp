#include "ReferenceError.h"

using namespace blogator::parser::dom::exception;

/**
 * Constructor
 * @param message description
 */
ReferenceError::ReferenceError( const std::string &message ) :
    DOMException( "ReferenceError: " + message )
{}