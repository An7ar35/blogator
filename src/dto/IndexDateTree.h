#ifndef BLOGATOR_DTO_INDEXDATETREE_H
#define BLOGATOR_DTO_INDEXDATETREE_H

#include <unordered_map>
#include <vector>
#include <string>
#include "HTML.h"

namespace blogator::dto {
    struct IndexDateTree {
        /*
         * Maps the Articles to relevant HTML lines in the pane
         * -----------------------------------------------------------------------------------------
         * · Used to change formatting on the html line in the date tree referencing to the
         *   current article being displayed on the main content pane.
         *   K = Article position in the master index
         *   V = Collection of HTML lines i where the article is referenced to by a <a></a> link
         */
        typedef std::unordered_map<size_t, HTML::LineIndex_t> ArticleLineMap_t;

        /*
         * Maps the Articles to their html <input type="checkbox" ../> parent(s)
         * -----------------------------------------------------------------------------------------
         * · Used to set the year and month parents of a article link in the tree to
         *   'checked' (expanded) without having to search all the html lines
         *   K = YYYY or YYYYMM of an article
         *   V = Year or Month parent in the html date index
         */
        typedef std::unordered_map<unsigned, HTML::LineIndex_t> ParentDateLineMap_t;


        HTML html;
        ArticleLineMap_t article_line_map;
        ParentDateLineMap_t date_line_map;
    };
}

#endif //BLOGATOR_DTO_INDEXDATETREE_H
