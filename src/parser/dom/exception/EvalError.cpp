#include "EvalError.h"

using namespace blogator::parser::dom::exception;

/**
 * Constructor
 * @param message description
 */
EvalError::EvalError( const std::string &message ) :
    DOMException( "EvalError: " + message )
{}