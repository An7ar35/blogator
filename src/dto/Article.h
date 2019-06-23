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
              << "\tTitle/Heading......: \"" << article._heading << "\"\n"
              << "\tDate-stamp.........: '" << article._datestamp << "'\n"
              << "\tSource file path...: " << article._paths.src_html.string() << "\n"
              << "\tRelative html link.: " << article._paths.out_html.string() << "\n"
              << "\tArticle stylesheet.: " << ( article._paths.css.empty() ? "none" : article._paths.css.string() ) << "\n"
              << "\tTags...............: ";
            for( const auto &t : article._tags )
                s << "\"" << t << "\" ";
            s << "\n};";

            return s;
        }

        struct Paths {
            std::filesystem::path src_html;
            std::filesystem::path out_html;
            std::filesystem::path css;
        } _paths;

        std::string           _heading;
        DateStamp             _datestamp;
        std::set<std::string> _tags;
    };
}

#endif //BLOGATOR_DTO_ARTICLE_H
