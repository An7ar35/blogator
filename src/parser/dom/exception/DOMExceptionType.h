#ifndef BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTIONTYPE_H
#define BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTIONTYPE_H

namespace blogator::parser::dom::exception {
    enum class DOMExceptionType {
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
}

#endif //BLOGATOR_PARSER_DOM_EXCEPTION_DOMEXCEPTIONTYPE_H
