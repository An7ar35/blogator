#ifndef BLOGATOR_PARSER_DOM_ENUM_QUIRKSMODE_H
#define BLOGATOR_PARSER_DOM_ENUM_QUIRKSMODE_H

#include <ostream>
#include <string>

namespace blogator::parser::dom {
    enum class QuirksMode {
        NO_QUIRKS = 0,
        LIMITED_QUIRKS,
        QUIRKS,
    };

    std::ostream & operator <<( std::ostream &os, QuirksMode mode );
}

namespace blogator {
    std::string to_string( blogator::parser::dom::QuirksMode mode );
    std::u32string to_u32string( blogator::parser::dom::QuirksMode mode );
}

#endif //BLOGATOR_PARSER_DOM_ENUM_QUIRKSMODE_H