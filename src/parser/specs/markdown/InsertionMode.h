#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::markdown {
    enum class InsertionMode {
        ENUM_BEGIN = 0, //used for iterating enums

        INITIAL = ENUM_BEGIN,
        CONTENT,
        BLOCKQUOTE,
        CODE_BLOCK,
        HYPERLINK,
        IMAGE,
        TABLE,
        LIST,
        DEFINITION_LIST,
        FOOTNOTE,
        BLOCK_END,
        END_OF_FILE,

        ENUM_END = END_OF_FILE, //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, InsertionMode mode );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::InsertionMode mode );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H