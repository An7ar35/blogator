#ifndef BLOGATOR_HTML_EDITOR_H
#define BLOGATOR_HTML_EDITOR_H

#include <string>

#include "../../dto/HTML.h"

namespace blogator::html::editor {
    size_t removeHyperlinks( dto::HTML &html);
    size_t removeHyperlinks( std::string &html_line );
}

#endif //BLOGATOR_HTML_EDITOR_H
