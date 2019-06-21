#ifndef BLOGATOR_HTML_WRITER_H
#define BLOGATOR_HTML_WRITER_H

#include <unordered_map>

#include "../../dto/Index.h"
#include "../../dto/IndexPane.h"
#include "../../dto/DateStamp.h"

namespace blogator::html::writer {
    void openTree( dto::IndexPane::DateTree &date_tree );
    void closeTree( dto::IndexPane::DateTree &date_tree );
    void openYearNode( const dto::DateStamp &datestamp, dto::IndexPane::DateTree &date_tree );
    void closeYearNode( dto::IndexPane::DateTree &date_tree );
    void openMonthNode( const dto::DateStamp &datestamp, const std::unordered_map<unsigned, std::string> &month_map, dto::IndexPane::DateTree &date_tree );
    void closeMonthNode( dto::IndexPane::DateTree &date_tree);
    void addArticleLeaf( const dto::Article &article, const size_t &article_pos, dto::IndexPane::DateTree &date_tree );

    void openTree( dto::IndexPane::TagTree &tag_tree );
    void closeTree( dto::IndexPane::TagTree &tag_tree );
}

#endif //BLOGATOR_HTML_WRITER_H