#ifndef BLOGATOR_HTML_WRITER_H
#define BLOGATOR_HTML_WRITER_H

#include <unordered_map>

#include "../../dto/Index.h"
#include "../../dto/IndexDateTree.h"
#include "../../dto/IndexTagTree.h"
#include "../../dto/DateStamp.h"
#include "../../dto/Template.h"

namespace blogator::html::writer {
    void openTree( dto::IndexDateTree &date_tree );
    void closeTree( dto::IndexDateTree &date_tree );
    void openYearNode( const dto::DateStamp &datestamp, dto::IndexDateTree &date_tree );
    void closeYearNode( dto::IndexDateTree &date_tree );
    void openMonthNode( const dto::DateStamp &datestamp, const std::unordered_map<unsigned, std::string> &month_map, dto::IndexDateTree &date_tree );
    void closeMonthNode( dto::IndexDateTree &date_tree);
    void addArticleLeaf( const dto::Article &article, const size_t &article_pos, dto::IndexDateTree &date_tree );

    void openTree( dto::IndexTagTree &tag_tree );
    void closeTree( dto::IndexTagTree &tag_tree );
    void openTagNode( const std::string &tag, dto::IndexTagTree &tag_tree );
    void closeTagNode( dto::IndexTagTree &tag_tree );
    void addArticleLeaf( const dto::Article &article, const size_t &article_pos, dto::IndexTagTree &tag_tree );

    std::string setInputCheckboxState( const std::string &line, const std::string &state );
    std::string setHyperlinkClass( const std::string &line, const std::string &css_class );
}

#endif //BLOGATOR_HTML_WRITER_H