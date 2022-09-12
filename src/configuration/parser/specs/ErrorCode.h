#ifndef BLOGATOR_CONFIGURATION_PARSER_SPECS_ERRORCODE_H
#define BLOGATOR_CONFIGURATION_PARSER_SPECS_ERRORCODE_H

#include <string>

namespace blogator::configuration::parser::specs {
    /**
     * Configuration parsing/loading error codes and descriptions
     */
    struct ErrorCode {
        enum {
            NONE    = 0,
            UNKNOWN = 1,

            /* Tokeniser */
            SYNTAX_ERROR,
            INVALID_NAMESPACE_SEPARATOR,
            INVALID_CHARACTER_IN_KEY,
            INVALID_VALUE_NAME_FORMAT,
            EOF_IN_COMMENT_BLOCK,
            EOF_IN_STRING_LITERAL,
            EOF_IN_VALUE,
            ASYMMETRIC_VALUE_NESTING,

            /* Parser */
            UNSPECIFIED_KEY,
            ROOT_IN_SCOPED_KEY,
            INVALID_CONFIG_FORMAT,
            INVALID_NAMESPACE_FORMAT,
            INVALID_KEY_FORMAT,
            INVALID_VALUE_FORMAT,
            UNRESOLVED_NAMED_VALUE,
            UNCLOSED_BRACKET,
            UNOPENED_BRACKET,
            MISSING_TERMINATOR,
            MISSING_LIST_DECLARATION,
            MISSING_LIST_SEPARATOR,
            VALUE_OVERRIDE,
            ABRUPT_EOF,

            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_SPECS_ERRORCODE_H