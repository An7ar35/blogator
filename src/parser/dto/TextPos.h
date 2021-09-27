#ifndef BLOGATOR_PARSER_DTO_TEXTPOS_H
#define BLOGATOR_PARSER_DTO_TEXTPOS_H

#include <string>

namespace blogator::parser {
    struct TextPos {
        size_t col  { 1 };
        size_t line { 1 };
    };
}

#endif //BLOGATOR_PARSER_DTO_TEXTPOS_H
