#ifndef BLOGATOR_HTML_READER_H
#define BLOGATOR_HTML_READER_H

#include <vector>
#include <unordered_map>
#include <string>

#include "../../dto/Article.h"
#include "../../dto/HTML.h"

namespace blogator::html::reader {
    std::string getHeading( const std::string &line );
    std::string getDate( const std::string &line );
    std::vector<std::string> getTags( const std::string &line );
    size_t findLineOfTag( const std::string &tag, const dto::HTML &html );
}

#endif //BLOGATOR_HTML_READER_H
