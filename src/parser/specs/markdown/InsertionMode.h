#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::markdown {
    enum class InsertionMode {
        ENUM_BEGIN = 0, //used for iterating enums

        INITIAL = ENUM_BEGIN,
        CONTENT,
        IMAGE,
        DEFINITION_LIST,
        END_OF_FILE,

        ENUM_END = END_OF_FILE, //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, InsertionMode mode );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::InsertionMode mode );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_INSERTIONMODE_H