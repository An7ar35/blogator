#ifndef BLOGATOR_DTO_HTML_H
#define BLOGATOR_DTO_HTML_H

#include <vector>
#include <string>

namespace blogator::dto {
    struct HTML {
        typedef size_t LineIndex_t;
        std::vector<std::string> _lines;
    };
}

#endif //BLOGATOR_DTO_HTML_H
