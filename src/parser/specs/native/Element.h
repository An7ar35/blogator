#ifndef BLOGATOR_PARSER_SPECS_NATIVE_ELEMENTS_H
#define BLOGATOR_PARSER_SPECS_NATIVE_ELEMENTS_H

#include <iostream>

namespace blogator::parser::specs::native {
    enum class Element {
        NONE = 0,
        WHITESPACE = NONE,
        UNKNOWN,

        NAMESPACE_OPEN, //'{{'
        NAMESPACE_CLOSE, //'}}'
        SECTION_OPEN, //'{'
        SECTION_CLOSE, //'}'

        VAR_IDENTIFIER,  //'.?'
        INTEGER_LITERAL, // '1', '-1'
        DOUBLE_LITERAL,  // '.01', '1.01', '1.'
        STRING_LITERAL,  // '"..."'
        ESCAPING_STRING, // '\"'
        OPERATOR,        // '+', '-', '/', '*', '&&', '||'
        PARANTHESES,     // '('

        PAGE_REF,       //BL.PAGE_REF="../source/path/post.html";
        PAGE_REF_SELF,  //BL.PAGE_REF_SELF
        TITLE,          //BL.POST_TITLE
        AUTO_TOC,       //BL.AUTO_TOC
        TIMESTAMP,      //BL.TIMESTAMP
        DATE,           //BL.DATE


        ENUM_END //used for iterating enums
    };
}

#endif //BLOGATOR_PARSER_SPECS_NATIVE_ELEMENTS_H
