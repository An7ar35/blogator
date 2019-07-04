#ifndef BLOGATOR_DTO_ARTICLE_H
#define BLOGATOR_DTO_ARTICLE_H

#include <string>
#include <filesystem>
#include <set>

#include "DateStamp.h"

namespace blogator::dto {
    struct Article {
        friend std::ostream &operator <<( std::ostream &s, const Article &article ) {
            s << "Article={ \n"
              << "\tNumber.............: " << article._number << "\n"
              << "\tTitle/Heading......: \"" << article._heading << "\"\n"
              << "\tAuthor(s) .........: \"" << article._author << "\"\n"
              << "\tDate-stamp.........: '" << article._datestamp << "'\n"
              << "\tSource file path...: " << article._paths.src_html.string() << "\n"
              << "\tRelative html link.: " << article._paths.out_html.string() << "\n"
              << "\tIndex entry html...: " << article._paths.entry_html.string() << "\n"
              << "\tArticle stylesheet.: " << ( article._paths.css.empty() ? "none" : article._paths.css.string() ) << "\n"
              << "\tTags...............: ";
            for( const auto &t : article._tags )
                s << "\"" << t << "\" ";
            s << "\n};";

            return s;
        }

        struct Paths {
            std::filesystem::path src_html;
            std::filesystem::path entry_html;
            std::filesystem::path out_html;
            std::filesystem::path css;
        } _paths;

        size_t                _number;
        std::string           _heading;
        std::string           _author;
        DateStamp             _datestamp;
        std::set<std::string> _tags;
    };
}

#endif //BLOGATOR_DTO_ARTICLE_H
