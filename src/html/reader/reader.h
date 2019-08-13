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
    dto::HTML   getContentBetween( const std::string &from, const std::string &to, const dto::HTML &html ); //TODO remove? not currently used
    std::vector<std::string> getContentsBetween( const std::string &from, const std::string &to, const std::string &str );
    void getSummaryPositions( const size_t &line_number, const std::string &line, std::deque<dto::InsertPosition> &positions );
    std::vector<std::string> getTags( const std::string &str );
    dto::ConsecutiveWritePositions_t getConsecutiveWritePositions( const dto::HTML &html, dto::Template::BlockInsertClasses_t &blocks );
    std::string getIndent( const std::string &line );
}

#endif //BLOGATOR_HTML_READER_H