#ifndef BLOGATOR_PARSER_SPECS_HTML5_TOKENISER_TOKENTYPE_H
#define BLOGATOR_PARSER_SPECS_HTML5_TOKENISER_TOKENTYPE_H

#include <ostream>
#include <string>

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

    std::ostream & operator <<( std::ostream &os, TokenType type );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::infra::TokenType type );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_TOKENISER_TOKENTYPE_H