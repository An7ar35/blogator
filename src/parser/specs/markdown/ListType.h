#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_LISTTYPE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_LISTTYPE_H

#include <ostream>
#include <string>

namespace blogator::parser::specs::markdown {
    enum class ListType {
        NONE = 0,
        UL_HYPHEN,
        UL_ASTERISK,
        UL_PLUS_SIGN,
        UL_TASK,
        OL_ALPHA_UPPER,
        OL_ALPHA_LOWER,
        OL_NUMERIC,
        FOOTNOTE_DEFS,
    };

    std::ostream & operator <<( std::ostream &os, ListType type );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::ListType type );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_LISTTYPE_H