#ifndef BLOGATOR_INDEXPANE_H
#define BLOGATOR_INDEXPANE_H

#include <unordered_map>
#include "HTML.h"

namespace blogator::dto {
    struct IndexPane {
        typedef std::unordered_map<unsigned, HTML::LineIndex_t>            ParentDateLineMap_t;
        typedef std::unordered_map<size_t, HTML::LineIndex_t>              ArticleLineMap_t;
        typedef std::unordered_map<std::string, HTML::LineIndex_t>         ParentTagLineMap_t;
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


//        //html content
//        HTML _by_date_html;
//        HTML _by_tags_html;
//
//        /*
//         * Maps the Articles to relevant HTML lines in the pane
//         * K = Article position in the master index
//         * V = HTML line(s) i where the article is referenced to by a <a></a> link
//         */
//        std::unordered_map<size_t, HtmlLineIndex_t>              _html_date_index_map;
//        std::unordered_map<size_t, std::vector<HtmlLineIndex_t>> _html_tags_index_map;
//
//        /*
//         * Maps the Articles to their html <input type="checkbox" ../> parent(s)
//         * K = YYYY or YYYYMM of an article
//         * V = Year or Month parent in the html date index
//         */
//        std::unordered_map<unsigned, HtmlLineIndex_t> _parent_date_checkbox_map;
//        /*
//         * Maps the Tags to their html <input type="checkbox" ../> parents
//         * K = Tag string
//         * V = Tag parent the html tag index
//         */
//        std::unordered_map<std::string, HtmlLineIndex_t> _parent_tag_checkbox_map;
    };
}

#endif //BLOGATOR_INDEXPANE_H