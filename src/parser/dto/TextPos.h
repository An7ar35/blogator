#ifndef BLOGATOR_PARSER_DTO_TEXTPOS_H
#define BLOGATOR_PARSER_DTO_TEXTPOS_H

#include <string>

namespace blogator::parser {
    struct TextPos {
        size_t col;
        size_t line;
    };
}

#endif //BLOGATOR_PARSER_DTO_TEXTPOS_H
