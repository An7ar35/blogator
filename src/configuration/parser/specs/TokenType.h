#ifndef BLOGATOR_CONFIGURATION_PARSER_SPECS_TOKENTYPE_H
#define BLOGATOR_CONFIGURATION_PARSER_SPECS_TOKENTYPE_H

#include <ostream>
#include <string>

namespace blogator::configuration::parser::specs {
    enum class TokenType {
        ENUM_BEGIN = 0, //used for iteration purposes

        COMMENT = ENUM_BEGIN, //inlined "//..." or block /* ... */
        NAMESPACE,            //[A-Za-z][A-Za-z\d-]*[A-Za-z\d]
        KEY,                  //[A-Za-z][A-Za-z\d-]*[A-Za-z\d]
        OPERATOR,             //'[', ']', ':', '='
        PUNCTUATOR,           //',', ';'
        BRACKET,              //'{', '}', '(', ')'
        CHARACTER,
        VALUE_NAME,           //words in the same format as keys
        STRING_LITERAL,       //"..."
        INTEGER_LITERAL_10,   //1, -1
        INTEGER_LITERAL_16,   //0x?
        INTEGER_LITERAL_FLT,  //-1.0, 1.0, -.1, .1, -1., 1.
        BOOLEAN_LITERAL,      //true, false
        END_OF_FILE,

        ENUM_END = END_OF_FILE //used for iteration purposes
    };

    std::ostream & operator <<( std::ostream & os, TokenType type );
}

namespace blogator {
    std::string to_string( configuration::parser::specs::TokenType type );
}

#endif //BLOGATOR_CONFIGURATION_PARSER_SPECS_TOKENTYPE_H