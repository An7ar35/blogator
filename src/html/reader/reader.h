#ifndef BLOGATOR_HTML_READER_H
#define BLOGATOR_HTML_READER_H

#include <vector>
#include <unordered_map>
#include <string>

#include "../../dto/Article.h"
#include "../../dto/HTML.h"
#include "../../dto/Template.h"

namespace blogator::html::reader {
    std::string getContentBetween( const std::string &from, const std::string &to, const std::string &line );
    dto::HTML getContentBetween( const std::string &from, const std::string &to, const dto::HTML &html );
    std::vector<std::string> getContentsBetween( const std::string &from, const std::string &to, const std::string &line );
    std::vector<std::string> getTags( const std::string &line );
    size_t findLineOfTag( const std::string &tag, const dto::HTML &html );
    dto::Template::DivWritePositions_t & findInsertPositions( const dto::HTML &html, dto::Template::DivWritePositions_t &divs );
    std::string getIndent( const std::string &line );

    std::vector<std::string> getClassContent( const dto::HTML &html, const std::string &css_class ); //TODO
    std::vector<std::string> getTagContent( const dto::HTML &html, const std::string &html_tag ); //TODO
}

#endif //BLOGATOR_HTML_READER_H