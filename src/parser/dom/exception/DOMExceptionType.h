#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTIONTYPE_H
#define BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTIONTYPE_H

#include <ostream>
#include <string>

namespace blogator::parser::dom::exception {
    enum class DOMExceptionType {
        UnknownExceptionType = 0, //for when type is not defined
        HierarchyRequestError,
        WrongDocumentError,
        InvalidCharacterError,
        NoModificationAllowedError,
        NotFoundError,
        NotSupportedError,
        InUseAttributeError,
        InvalidStateError,
        SyntaxError,
        InvalidModificationError,
        NamespaceError,
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

    std::ostream & operator <<( std::ostream &os, DOMExceptionType type );
}

namespace blogator {
    std::string to_string( blogator::parser::dom::exception::DOMExceptionType type );
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTIONTYPE_H
