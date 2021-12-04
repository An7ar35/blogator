#include "TypeError.h"

using namespace blogator::parser::dom::exception;

/**
 * Constructor
 * @param message description
 */
TypeError::TypeError( const std::string &message ) :
    DOMException( "TypeError: " + message )
{}