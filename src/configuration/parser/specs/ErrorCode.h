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

            SYNTAX_ERROR,
            INVALID_NAMESPACE_SEPARATOR,
            INVALID_CHARACTER_IN_KEY,
            INVALID_VALUE_NAME_FORMAT,
            EOF_IN_COMMENT_BLOCK,
            EOF_IN_STRING_LITERAL,
            EOF_IN_VALUE,

            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_SPECS_ERRORCODE_H