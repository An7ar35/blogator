#include "DOMException.h"

#include <utility>

using namespace blogator::parser::dom::exception;

/**
 * Constructor
 * @param what description 
 */
DOMException::DOMException( std::string what ) :
    _message( std::move( what ) )
{}

/**
 * Constructor
 * @param what description 
 */
DOMException::DOMException( const char *what ) :
    _message( what )
{}

/**
 * Constructor
 * @param type DOMException type 
 */
DOMException::DOMException( DOMExceptionType type ) {
    _message = errToStr( type );
}

/**
 * Constructor
 * @param type DOMException type
 * @param what Description string
 */
DOMException::DOMException( DOMExceptionType type, std::string what ) {
    _message = errToStr( type ) + ": " + what;
}

/**
 * Constructor
 * @param type DOMException type
 * @param what Description string
 */
DOMException::DOMException( DOMExceptionType type, const char * what ) {
    _message = errToStr( type ) + ": " + std::string( what );
}

/**
 * Copy-constructor
 * @param other DOMException to copy
 */
DOMException::DOMException( const DOMException &other ) {
    _message = std::string( other._message );
}

/**
 * Copy-assignment operator
 * @param other DOMException to copy
 * @return Copy
 */
DOMException &  DOMException::operator =( const DOMException &other ) {
    _message = other._message;
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
 * Converts a DOMExceptionType enum to its string representation
 * @param type DOMExceptionType enum
 * @return String representation
 */
std::string DOMException::errToStr( DOMExceptionType type ) {
    switch( type ) {
        case DOMExceptionType::HierarchyRequestError:      { return "HierarchyRequestError"; };
        case DOMExceptionType::WrongDocumentError:         { return "WrongDocumentError"; };
        case DOMExceptionType::InvalidCharacterError:      { return "InvalidCharacterError"; };
        case DOMExceptionType::NoModificationAllowedError: { return "NoModificationAllowedError"; };
        case DOMExceptionType::NotFoundError:              { return "NotFoundError"; };
        case DOMExceptionType::NotSupportedError:          { return "NotSupportedError"; };
        case DOMExceptionType::InUseAttributeError:        { return "InUseAttributeError"; };
        case DOMExceptionType::InvalidStateError:          { return "InvalidStateError"; };
        case DOMExceptionType::SyntaxError:                { return "SyntaxError"; };
        case DOMExceptionType::InvalidModificationError:   { return "InvalidModificationError"; };
        case DOMExceptionType::NamespaceError:             { return "NamespaceError"; };
        case DOMExceptionType::ValidationError:            { return "ValidationError"; };
        case DOMExceptionType::TypeMismatchError:          { return "TypeMismatchError"; };
        case DOMExceptionType::SecurityError:              { return "SecurityError"; };
        case DOMExceptionType::NetworkError:               { return "NetworkError"; };
        case DOMExceptionType::AbortError:                 { return "AbortError"; };
        case DOMExceptionType::URLMismatchError:           { return "URLMismatchError"; };
        case DOMExceptionType::QuotaExceededError:         { return "QuotaExceededError"; };
        case DOMExceptionType::TimeoutError:               { return "TimeoutError"; };
        case DOMExceptionType::InvalidNodeTypeError:       { return "InvalidNodeTypeError"; };
        case DOMExceptionType::DataCloneError:             { return "DataCloneError"; };
        case DOMExceptionType::EncodingError:              { return "EncodingError"; };
        case DOMExceptionType::NotReadableError:           { return "NotReadableError"; };
        case DOMExceptionType::UnknownError:               { return "UnknownError"; };
        case DOMExceptionType::ConstraintError:            { return "ConstraintError"; };
        case DOMExceptionType::DataError:                  { return "DataError"; };
        case DOMExceptionType::TransactionInactiveError:   { return "TransactionInactiveError"; };
        case DOMExceptionType::ReadOnlyError:              { return "ReadOnlyError"; };
        case DOMExceptionType::VersionError:               { return "VersionError"; };
        case DOMExceptionType::OperationError:             { return "OperationError"; };
        case DOMExceptionType::NotAllowedError:            { return "NotAllowedError"; };
        default:                                           { return "UnknownExceptionType"; };
    }
}
