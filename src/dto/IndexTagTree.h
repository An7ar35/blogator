#ifndef BLOGATOR_DTO_INDEXTAGTREE_H
#define BLOGATOR_DTO_INDEXTAGTREE_H

#include <unordered_map>
#include <vector>
#include <string>
#include "HTML.h"

namespace blogator::dto {
    struct IndexTagTree {
        /*
         * Maps the Articles to relevant HTML lines in the pane
         * -----------------------------------------------------------------------------------------
         * · Used to set the article's links in the tree as 'active' (i.e. change formatting to
         *   indicate that it is the current article viewed) without having to search all the html
         *   lines.
         *   K = Article position in the master index
         *   V = Collection of HTML lines i where the article is referenced to by a <a></a> link
         */
        typedef std::unordered_map<size_t, std::vector<HTML::LineIndex_t>> ArticlesLineMap_t;

        /*
         * Maps the Tags to their html <input type="checkbox" ../> parents
         * -----------------------------------------------------------------------------------------
         * · Used to set the parent tag(s) of an article link in the tag tree to
         *   'checked' (expanded) without having to search all the html lines
         * K = Tag string
         * V = Tag parent the html tag index
         */
        typedef std::unordered_map<std::string, HTML::LineIndex_t> ParentTagLineMap_t;


        HTML html;
        ArticlesLineMap_t  articles_line_map;
        ParentTagLineMap_t tag_line_map;
    };
}

#endif //BLOGATOR_DTO_INDEXTAGTREE_H
