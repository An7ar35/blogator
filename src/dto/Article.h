#ifndef BLOGATOR_DTO_ARTICLE_H
#define BLOGATOR_DTO_ARTICLE_H

#include <string>
#include <filesystem>
#include <set>
#include <list>

#include "DateStamp.h"
#include "Template.h"

namespace blogator::dto {
    struct Article {
        struct SeekRange { //Hold the seek range for a section of a file's content
            SeekRange( const Template::InsertPosition &from, const Template::InsertPosition &to ) :
                _from( from ), _to( to )
            {};

            friend std::ostream &operator <<( std::ostream &s, const SeekRange &seek_range ) {
                s << "(" << seek_range._from << " -> " << seek_range._to << ")";
                return s;
            }

            Template::InsertPosition _from;
            Template::InsertPosition _to;
        };

        friend std::ostream &operator <<( std::ostream &s, const Article &article ) {
            s << "Article={ \n"
              << "\tNumber.............: " << article._number << "\n"
              << "\tTitle/Heading......: \"" << article._heading << "\"\n"
              << "\tAuthor(s) .........: ";
            for( const auto &a : article._authors )
                s << "\"" << a << "\" ";
            s << "\n\tDate-stamp.........: " << article._datestamp << "\n"
              << "\tSource file path...: " << article._paths.src_html.string() << "\n"
              << "\tSummary location(s): ";
            for( const auto &l : article._summary_pos )
                s << l << " ";
            s << "\n\tSummary cache......: " << ( article._summary_txt._lines.empty() ? "OFF" : "ON" ) << "\n"
              << "\tRelative html link.: " << article._paths.out_html.string() << "\n"
              << "\tIndex entry html...: " << article._paths.entry_html.string() << "\n"
              << "\tArticle stylesheet.: " << article._paths.css.string() << "\n"
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
        DateStamp             _datestamp;
        std::set<std::string> _authors;
        std::set<std::string> _tags;
        std::list<SeekRange>  _summary_pos;
        HTML                  _summary_txt;
    };
}

#endif //BLOGATOR_DTO_ARTICLE_H
