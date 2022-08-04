#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENTYPE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENTYPE_H

#include <ostream>
#include <string>

namespace blogator::parser::specs::markdown {
    enum class TokenType {
        ENUM_BEGIN = 0, //used for iteration purposes

        NONE = ENUM_BEGIN,
        UNKNOWN,

        /* Blocks */
        HEADING,             //"# ..." or ".... \n---" or "....\n==="
        PARAGRAPH,           //text content
        HORIZONTAL_RULE,     //"---" or more on 1 line
        CODE_BLOCK,          //"```fenced code block```" or "~~~fenced code block~~~"
        BLOCKQUOTE,          //'> ...', '>>' nested
        LIST,
        LIST_ITEM,           //"a.", "A.", "1.", '*','-','+', "- [x]", "- []", "[^..]:"
        TABLE,               //equivalent to <table>
        TABLE_ROW,           //equivalent to <tr>
        TABLE_HEADING,       //equivalent to <th>
        TABLE_CELL,          //equivalent to <td>
        DEFINITION_LIST_DT,  //equivalent to <dt>
        DEFINITION_LIST_DD,  //equivalent to <dd>
        HTML,                //HTML markup block

        BLOCK_END,

        /* Inline */
        CHARACTER,
        LINE_BREAK,          //"  \n"
        FORMAT_BEGIN,        //'*','-','=','`','_'
        FORMAT_END,          //auto close on block end
        IMAGE,               //![alt text](path)
        HYPERLINK,           //[title](path)
        FOOTNOTE_REF,        //"[^1]"

        END_OF_FILE,

        ENUM_END = END_OF_FILE //used for iteration purposes
    };

    std::ostream & operator <<( std::ostream &os, TokenType type );

    bool isBlockToken( TokenType type );
    bool isContainerBlock( TokenType type );
    bool isLeafBlock( TokenType type );
    bool isTableBlock( TokenType type );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::TokenType type );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_TOKENTYPE_H