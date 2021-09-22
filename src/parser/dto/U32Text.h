#ifndef BLOGATOR_PARSER_DTO_U32TEXT_H
#define BLOGATOR_PARSER_DTO_U32TEXT_H

#include <string>

#include "TextPos.h"

namespace blogator::parser {
    typedef std::u32string::const_iterator TextIterator_t;

    struct U32Text {
        const std::u32string & src;
        TextPos                position { 0, 0 };
        TextIterator_t         iterator;
    };
}

#endif //BLOGATOR_PARSER_DTO_U32TEXT_H
