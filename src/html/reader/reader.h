#ifndef BLOGATOR_HTML_READER_H
#define BLOGATOR_HTML_READER_H

#include <vector>
#include <unordered_map>
#include <deque>
#include <string>

#include "../../dto/Article.h"
#include "../../dto/HTML.h"
#include "../../dto/Templates.h"

namespace blogator::html::reader {
    std::string getContentBetween( const std::string &from, const std::string &to, const std::string &str );
    dto::HTML   getContentBetween( const std::string &from, const std::string &to, const dto::HTML &html );
    std::vector<std::string> getContentsBetween( const std::string &from, const std::string &to, const std::string &str );
    void getSummaryPositions( const size_t &line_number, const std::string &line, std::deque<dto::Template::InsertPosition> &positions );
    std::vector<std::string> getTags( const std::string &str );
    size_t findLineOfTag( const std::string &tag, const dto::HTML &html );
    dto::Template::ConsecutiveWritePositions_t getConsecutiveWritePositions( const dto::HTML &html, dto::Template::BlockInsertClasses_t &blocks );
    std::string getIndent( const std::string &line );
}

#endif //BLOGATOR_HTML_READER_H