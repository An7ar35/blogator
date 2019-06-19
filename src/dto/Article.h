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
              << "\theading: \"" << article._heading << "\",\n"
              << "\tdate   : '" << article._datestamp << "',\n"
              << "\tfile   : " << article._html_filepath.string() << ",\n"
              << "\tcss    : " << ( article._css_filepath.empty() ? "none" : article._css_filepath.string() ) << ",\n"
              << "\ttags   : ";
            for( const auto &t : article._tags )
                s << "\"" << t << "\" ";
            s << "\n};";

            return s;
        }

        std::string           _heading;
        DateStamp             _datestamp;
        std::filesystem::path _html_filepath;
        std::filesystem::path _css_filepath;
        std::set<std::string> _tags;
    };
}

#endif //BLOGATOR_DTO_ARTICLE_H
