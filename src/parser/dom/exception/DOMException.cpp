#include "DOMException.h"

#include <utility>

using namespace blogator::parser::dom::exception;

/**
 * Constructor
 * @param what description 
 */
DOMException::DOMException( std::string what ) :
    _message( std::move( what ) ),
    _type( DOMExceptionType::UnknownExceptionType )
{}

/**
 * Constructor
 * @param what description 
 */
DOMException::DOMException( const char * what ) :
    _message( what ),
    _type( DOMExceptionType::UnknownExceptionType )
{}

/**
 * Constructor
 * @param type DOMException type 
 */
DOMException::DOMException( DOMExceptionType type ) {
    _message = blogator::to_string( type );
    _type    = type;
}

/**
 * Constructor
 * @param type DOMException type
 * @param what Description string
 */
DOMException::DOMException( DOMExceptionType type, const std::string &what ) {
    _message = blogator::to_string( type ) + ": " + what;
    _type    = type;
}

/**
 * Constructor
 * @param type DOMException type
 * @param what Description string
 */
DOMException::DOMException( DOMExceptionType type, const char * what ) {
    _message = blogator::to_string( type ) + ": " + std::string( what );
    _type    = type;
}

/**
 * Copy-constructor
 * @param other DOMException to copy
 */
DOMException::DOMException( const DOMException &other ) {
    _message = std::string( other._message );
    _type    = other._type;
}

/**
 * Copy-assignment operator
 * @param other DOMException to copy
 * @return Copy
 */
DOMException &  DOMException::operator =( const DOMException &other ) {
    _message = other._message;
    _type    = other._type;
    return *this;
}

/**
 * Gets the description string
 * @return Description
 */
const char * DOMException::what() const noexcept {
    return _message.c_str();
}

/**
 * Gets the DOMExceptionType
 * @return DOMExceptionType
 */
DOMExceptionType DOMException::type() const noexcept {
    return _type;
}
