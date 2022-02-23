#ifndef BLOGATOR_DOM_EXCEPTION_DOMEXCEPTION_H
#define BLOGATOR_DOM_EXCEPTION_DOMEXCEPTION_H

#include <string>
#include <exception>
#include <map>
#include <sstream>
#include <utility>

namespace blogator::exception {
    enum class DOMErrorType {
        IndexSizeError, //Deprecated. Use `RangeError` instead.
        DOMStringSizeError, //Deprecated. Use `RangeError` instead.
        HierarchyRequestError,
        WrongDocumentError,
        InvalidCharacterError,
        NoDataAllowedError, //Deprecated.
        NoModificationAllowedError,
        NotFoundError,
        NotSupportedError,
        InUseAttributeError,
        InvalidStateError,
        SyntaxError,
        InvalidModificationError,
        NamespaceError,
        InvalidAccessError, //Deprecated. Use TypeError for invalid arguments, "NotSupportedError" DOMException for unsupported operations, and "NotAllowedError" DOMException for denied requests instead.
        ValidationError,
        TypeMismatchError,
        SecurityError,
        NetworkError,
        AbortError,
        URLMismatchError,
        QuotaExceededError,
        TimeoutError,
        InvalidNodeTypeError,
        DataCloneError,
        EncodingError,
        NotReadableError,
        UnknownError,
        ConstraintError,
        DataError,
        TransactionInactiveError,
        ReadOnlyError,
        VersionError,
        OperationError,
        NotAllowedError
    };

    /**
     * DOM Exception
     */
    class DOMException : public std::exception {
      public:
        explicit DOMException( std::string message ) :
            _message( std::move( message ) )
        {};

        DOMException( const std::string &message, DOMErrorType error_type ) :
            _message( errorStr( error_type ) + ": " + message )
        {};

        DOMException( const std::string &message, const std::string &error ) :
            _message( error + ": " + message )
        {};

        DOMException( std::string loc_details, const std::string &message, DOMErrorType error_type ) :
            _provenance( std::move( loc_details ) ),
            _message( errorStr( error_type ) + ": " + message )
        {};

        DOMException( std::string loc_details, const std::string &message, const std::string &error ) :
            _provenance( std::move( loc_details ) ),
            _message( error + ": " + message )
        {};

        ~DOMException() override = default;

        [[nodiscard]] const char *where() const noexcept {
            return _provenance.c_str();
        };

        [[nodiscard]] const char *what() const noexcept override {
            return _message.c_str();
        };


      private:
        const std::string  _provenance;
        const std::string  _message;

        static std::string errorStr( DOMErrorType type ) {
            switch( type ) {
                case DOMErrorType::IndexSizeError:
                    return "IndexSizeError";
                case DOMErrorType::DOMStringSizeError:
                    return "DOMStringSizeError";
                case DOMErrorType::HierarchyRequestError:
                    return "HierarchyRequestError";
                case DOMErrorType::WrongDocumentError:
                    return "WrongDocumentError";
                case DOMErrorType::InvalidCharacterError:
                    return "InvalidCharacterError";
                case DOMErrorType::NoDataAllowedError:
                    return "NoDataAllowedError";
                case DOMErrorType::NoModificationAllowedError:
                    return "NoModificationAllowedError";
                case DOMErrorType::NotFoundError:
                    return "NotFoundError";
                case DOMErrorType::NotSupportedError:
                    return "NotSupportedError";
                case DOMErrorType::InUseAttributeError:
                    return "InUseAttributeError";
                case DOMErrorType::InvalidStateError:
                    return "InvalidStateError";
                case DOMErrorType::SyntaxError:
                    return "SyntaxError";
                case DOMErrorType::InvalidModificationError:
                    return "InvalidModificationError";
                case DOMErrorType::NamespaceError:
                    return "NamespaceError";
                case DOMErrorType::InvalidAccessError:
                    return "InvalidAccessError";
                case DOMErrorType::ValidationError:
                    return "ValidationError";
                case DOMErrorType::TypeMismatchError:
                    return "TypeMismatchError";
                case DOMErrorType::SecurityError:
                    return "SecurityError";
                case DOMErrorType::NetworkError:
                    return "NetworkError";
                case DOMErrorType::AbortError:
                    return "AbortError";
                case DOMErrorType::URLMismatchError:
                    return "URLMismatchError";
                case DOMErrorType::QuotaExceededError:
                    return "QuotaExceededError";
                case DOMErrorType::TimeoutError:
                    return "TimeoutError";
                case DOMErrorType::InvalidNodeTypeError:
                    return "InvalidNodeTypeError";
                case DOMErrorType::DataCloneError:
                    return "DataCloneError";
                case DOMErrorType::EncodingError:
                    return "EncodingError";
                case DOMErrorType::NotReadableError:
                    return "NotReadableError";
                case DOMErrorType::UnknownError:
                    return "UnknownError";
                case DOMErrorType::ConstraintError:
                    return "ConstraintError";
                case DOMErrorType::DataError:
                    return "DataError";
                case DOMErrorType::TransactionInactiveError:
                    return "TransactionInactiveError";
                case DOMErrorType::ReadOnlyError:
                    return "ReadOnlyError";
                case DOMErrorType::VersionError:
                    return "VersionError";
                case DOMErrorType::OperationError:
                    return "OperationError";
                case DOMErrorType::NotAllowedError:
                    return "NotAllowedError";
                default:
                    return "UnknownErrorType";
            }
        }
    };

    class EvalError : DOMException {
        explicit EvalError( const std::string &message ) : DOMException( message , "EvalError"  ) {}
        EvalError( const std::string &loc_details, const std::string &message ) : DOMException( loc_details, message , "EvalError"  ) {}
    };

    class RangeError : DOMException {
        explicit RangeError( const std::string &message ) : DOMException( message , "RangeError"  ) {}
        RangeError( const std::string &loc_details, const std::string &message ) : DOMException( loc_details, message , "RangeError"  ) {}
    };

    class ReferenceError : DOMException {
        explicit ReferenceError( const std::string &message ) : DOMException( message , "ReferenceError"  ) {}
        ReferenceError( const std::string &loc_details, const std::string &message ) : DOMException( loc_details, message , "ReferenceError"  ) {}
    };

    class TypeError : DOMException {
        explicit TypeError( const std::string &message ) : DOMException( message , "TypeError"  ) {}
        TypeError( const std::string &loc_details, const std::string &message ) : DOMException( loc_details, message , "TypeError"  ) {}
    };

    class URIError : DOMException {
        explicit URIError( const std::string &message ) : DOMException( message , "URIError"  ) {}
        URIError( const std::string &loc_details, const std::string &message ) : DOMException( loc_details, message , "URIError"  ) {}
    };
}

#endif //BLOGATOR_DOM_EXCEPTION_DOMEXCEPTION_H

