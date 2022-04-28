#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENISER_TOKENTYPE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENISER_TOKENTYPE_H

#include <ostream>
#include <string>

namespace blogator::parser::specs::markdown {
    enum class TokenType {
        UNKNOWN = 0,
        CHARACTER,

        //blocks
        HEADING, //"# ... {#custom-id}" or ".... \n---" or "....\n==="

        //inline
        FORMAT_BEGIN,
        FORMAT_END,   //auto close on block end
        BLOCKQUOTE, //'> ...', '>>' nested
        LIST_ITEM, //ordered ('a')/unordered ('*','-','+')/task ("- [x]", "- []")
        CODE_BLOCK, //```fenced code block```
        CODE_INLINE, //`code`
        HORIZONTAL_RULE, //"---" or more on 1 line
        LINK,  //[title](path)
        IMAGE, //![alt text](path)
        TABLE_HEADING,
        TABLE_COL, //with alignment
        TABLE_CELL,
        FOOTNOTE,
        DEFINITION,
        END_OF_FILE,
    };

    std::ostream &operator <<( std::ostream &os, TokenType type );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::TokenType type );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENISER_TOKENTYPE_H