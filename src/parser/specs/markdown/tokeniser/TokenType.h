#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENISER_TOKENTYPE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENISER_TOKENTYPE_H

#include <ostream>
#include <string>

namespace blogator::parser::specs::markdown {
    enum class TokenType {
        UNKNOWN = 0,
        CHARACTER,


        //blocks
        HEADING, //"# ..." or ".... \n---" or "....\n==="
        BLOCK_END,

        //inline
        LINE_BREAK,   //"  \n"
        FORMAT_BEGIN, //'*','-','=','`','_'
        FORMAT_END,   //auto close on block end
        IMAGE,        //![alt text](path)
        BLOCK_ID,     //"{#...}"
        CODE_BLOCK,   //```fenced code block```


//        BLOCKQUOTE, //'> ...', '>>' nested
//        LIST_ITEM, //ordered ('a')/unordered ('*','-','+')/task ("- [x]", "- []")
//        HORIZONTAL_RULE, //"---" or more on 1 line
//        LINK,  //[title](path)
//
//        TABLE_HEADING,
//        TABLE_COL, //with alignment
//        TABLE_CELL,
//        FOOTNOTE,
//        DEFINITION,
        END_OF_FILE,

        ENUM_END = END_OF_FILE //used for iteration purposes
    };

    std::ostream & operator <<( std::ostream &os, TokenType type );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::TokenType type );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENISER_TOKENTYPE_H