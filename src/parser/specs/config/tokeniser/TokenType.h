#ifndef BLOGATOR_PARSER_SPECS_CONFIG_TOKENISER_TOKENTYPE_H
#define BLOGATOR_PARSER_SPECS_CONFIG_TOKENISER_TOKENTYPE_H

namespace blogator::parser::specs::config {
    enum class TokenType {
        UNKNOWN = 0,
        KEY,
        VALUE,
        COMMENT,
        END_OF_FILE,
    };
}

#endif //BLOGATOR_PARSER_SPECS_CONFIG_TOKENISER_TOKENTYPE_H
