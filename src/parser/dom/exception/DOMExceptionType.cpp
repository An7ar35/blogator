#include "DOMExceptionType.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param type DOMExceptionType enum
 * @return Output stream
 */
std::ostream & blogator::parser::dom::exception::operator <<( std::ostream &os, blogator::parser::dom::exception::DOMExceptionType type ) {
    switch( type ) {
        case DOMExceptionType::HierarchyRequestError:      { os << "HierarchyRequestError";      } break;
        case DOMExceptionType::WrongDocumentError:         { os << "WrongDocumentError";         } break;
        case DOMExceptionType::InvalidCharacterError:      { os << "InvalidCharacterError";      } break;
        case DOMExceptionType::NoModificationAllowedError: { os << "NoModificationAllowedError"; } break;
        case DOMExceptionType::NotFoundError:              { os << "NotFoundError";              } break;
        case DOMExceptionType::NotSupportedError:          { os << "NotSupportedError";          } break;
        case DOMExceptionType::InUseAttributeError:        { os << "InUseAttributeError";        } break;
        case DOMExceptionType::InvalidStateError:          { os << "InvalidStateError";          } break;
        case DOMExceptionType::SyntaxError:                { os << "SyntaxError";                } break;
        case DOMExceptionType::InvalidModificationError:   { os << "InvalidModificationError";   } break;
        case DOMExceptionType::NamespaceError:             { os << "NamespaceError";             } break;
        case DOMExceptionType::ValidationError:            { os << "ValidationError";            } break;
        case DOMExceptionType::TypeMismatchError:          { os << "TypeMismatchError";          } break;
        case DOMExceptionType::SecurityError:              { os << "SecurityError";              } break;
        case DOMExceptionType::NetworkError:               { os << "NetworkError";               } break;
        case DOMExceptionType::AbortError:                 { os << "AbortError";                 } break;
        case DOMExceptionType::URLMismatchError:           { os << "URLMismatchError";           } break;
        case DOMExceptionType::QuotaExceededError:         { os << "QuotaExceededError";         } break;
        case DOMExceptionType::TimeoutError:               { os << "TimeoutError";               } break;
        case DOMExceptionType::InvalidNodeTypeError:       { os << "InvalidNodeTypeError";       } break;
        case DOMExceptionType::DataCloneError:             { os << "DataCloneError";             } break;
        case DOMExceptionType::EncodingError:              { os << "EncodingError";              } break;
        case DOMExceptionType::NotReadableError:           { os << "NotReadableError";           } break;
        case DOMExceptionType::UnknownError:               { os << "UnknownError";               } break;
        case DOMExceptionType::ConstraintError:            { os << "ConstraintError";            } break;
        case DOMExceptionType::DataError:                  { os << "DataError";                  } break;
        case DOMExceptionType::TransactionInactiveError:   { os << "TransactionInactiveError";   } break;
        case DOMExceptionType::ReadOnlyError:              { os << "ReadOnlyError";              } break;
        case DOMExceptionType::VersionError:               { os << "VersionError";               } break;
        case DOMExceptionType::OperationError:             { os << "OperationError";             } break;
        case DOMExceptionType::NotAllowedError:            { os << "NotAllowedError";            } break;
        default:                                           { os << "UnknownExceptionType";       } break;
    }

    return os;
}

/**
 * Converts a parser::dom::exception::DOMExceptionType enum to a string representation
 * @param type DOMExceptionType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::dom::exception::DOMExceptionType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}
