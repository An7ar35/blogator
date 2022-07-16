#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::markdown {
    enum class InsertionMode {
        ENUM_BEGIN = 0, //used for iterating enums

        INITIAL = ENUM_BEGIN,
        HEADING,
        FORMATTING,
        BLOCKQUOTE,
        CODE_BLOCK,
        HORIZONTAL_RULE,
        HYPERLINK,
        IMAGE,
        TABLE,
        LIST,
        TASK_LIST,
        DEFINITION_LIST,
        FOOTNOTE,

        ENUM_END = FOOTNOTE, //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, InsertionMode mode );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::InsertionMode mode );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H