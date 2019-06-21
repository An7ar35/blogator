#ifndef BLOGATOR_INDEXPANE_H
#define BLOGATOR_INDEXPANE_H

#include <unordered_map>
#include "HTML.h"

namespace blogator::dto {
    struct IndexPane {
        /*
         * Maps the Articles to their html <input type="checkbox" ../> parent(s)
         * K = YYYY or YYYYMM of an article
         * V = Year or Month parent in the html date index
         */
        typedef std::unordered_map<unsigned, HTML::LineIndex_t>            ParentDateLineMap_t;
        /*
         * Maps the Articles to relevant HTML lines in the pane
         * K = Article position in the master index
         * V = HTML line i where the article is referenced to by a <a></a> link
         */
        typedef std::unordered_map<size_t, HTML::LineIndex_t>              ArticleLineMap_t;
        /*
         * Maps the Tags to their html <input type="checkbox" ../> parents
         * K = Tag string
         * V = Tag parent the html tag index
         */
        typedef std::unordered_map<std::string, HTML::LineIndex_t>         ParentTagLineMap_t;
        /*
         * Maps the Articles to relevant HTML lines in the pane
         * K = Article position in the master index
         * V = Collection of HTML lines i where the article is referenced to by a <a></a> link
         */
        typedef std::unordered_map<size_t, std::vector<HTML::LineIndex_t>> ArticlesLineMap_t;

        struct DateTree {
            HTML                html;
            ArticleLineMap_t    article_line_map;
            ParentDateLineMap_t date_line_map;
        } _date_tree;

        struct TagTree {
            HTML               html;
            ArticlesLineMap_t  articles_line_map;
            ParentTagLineMap_t tag_line_map;
        } _tag_tree;
    };
}

#endif //BLOGATOR_INDEXPANE_H