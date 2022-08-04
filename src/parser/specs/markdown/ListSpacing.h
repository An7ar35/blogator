#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_LISTSPACING_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_LISTSPACING_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::markdown {
    enum class ListSpacing {
        DEFAULT_TIGHT = 0,
        MAYBE_LOOSE,
        LOOSE,
    };

    std::ostream & operator <<( std::ostream &os, ListSpacing spacing );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::ListSpacing spacing );
}

#endif //BLOGATOR_LISTSPACING_H