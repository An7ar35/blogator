#ifndef BLOGATOR_HTML_H
#define BLOGATOR_HTML_H

#include <vector>
#include <string>

#include "../dto/Article.h"
#include "../dto/HTML.h"

namespace blogator::html {
    std::string readHeading( const std::string &line );
    std::string readDate( const std::string &line );
    std::vector<std::string> readTags( const std::string &line );
    size_t findLineOfTag( const std::string &tag, const dto::HTML &html );
}

#endif //BLOGATOR_HTML_H
