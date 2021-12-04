#include "URIError.h"

using namespace blogator::parser::dom::exception;

/**
 * Constructor
 * @param message description
 */
URIError::URIError( const std::string &message ) :
    DOMException( "URIError: " + message )
{}