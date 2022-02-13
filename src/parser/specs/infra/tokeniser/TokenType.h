#ifndef BLOGATOR_PARSER_SPECS_HTML5_TOKENISER_TOKENTYPE_H
#define BLOGATOR_PARSER_SPECS_HTML5_TOKENISER_TOKENTYPE_H

namespace blogator::parser::specs::infra {
    enum class TokenType {
        UNKNOWN = 0,
        DOCTYPE,
        START_TAG,
        END_TAG,
        CHARACTER,
        COMMENT,
        END_OF_FILE,
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_TOKENISER_TOKENTYPE_H