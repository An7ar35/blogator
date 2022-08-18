#ifndef BLOGATOR_CONFIGURATION_PARSER_SPECS_TOKENISERSTATE_H
#define BLOGATOR_CONFIGURATION_PARSER_SPECS_TOKENISERSTATE_H

#include <ostream>
#include <string>

namespace blogator::configuration::parser::specs {
    enum class TokeniserState {
        ENUM_BEGIN = 0, //used for iteration purposes

        INITIAL = ENUM_BEGIN,
        KEY,
        NAMESPACE,
        AFTER_KEY,
        BEFORE_VALUE,
        VALUE_NAME,
        VALUE_STRING,
        VALUE_INTEGER_FLOAT,
        VALUE_INTEGER_BASE10,
        VALUE_INTEGER_BASE16,
        VALUE_RAW,
        AFTER_VALUE,
        ESCAPED_CHARACTER,
        COMMENT_SOLIDUS,
        COMMENT_INLINED,
        COMMENT_BLOCK,
        COMMENT_ASTERISK,

        END_OF_FILE,

        ENUM_END = END_OF_FILE //used for iteration purposes
    };

    std::ostream & operator <<( std::ostream & os, TokeniserState state );
}

namespace blogator {
    std::string to_string( configuration::parser::specs::TokeniserState state );
}

#endif //BLOGATOR_CONFIGURATION_PARSER_SPECS_TOKENISERSTATE_H