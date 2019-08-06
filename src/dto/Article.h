#ifndef BLOGATOR_DTO_ARTICLE_H
#define BLOGATOR_DTO_ARTICLE_H

#include <string>
#include <filesystem>
#include <set>
#include <list>
#include <cstring>

#include "DateStamp.h"
#include "Template.h"
#include "SeekRange.h"

namespace blogator::dto {
    struct Article {
        struct CaseInsensitiveCompare {
            bool operator()( const std::string &lhs, const std::string &rhs ) const {
                std::string::size_type bound = std::min( lhs.size(), rhs.size() );
                std::string::size_type i     = 0;

                for( auto it1 = lhs.begin(), it2 = rhs.begin(); i < bound; ++i, ++it1, ++it2 ) {
                    if( std::tolower( *it1 ) < std::tolower( *it2 ) )
                        return true;
                    if( std::tolower( *it2 ) < std::tolower( *it1 ) )
                        return false;
                }

                return false;
            }
        };

        friend std::ostream &operator <<( std::ostream &s, const Article &article ) {
            s << "Article={ \n"
              << "\tNumber.............: " << article._number << "\n"
              << "\tTitle/Heading......: \"" << article._heading << "\"\n"
              << "\tFeatured...........: " << ( article._is_featured ? "yes" : "no" ) << "\n"
              << "\tAuthor(s) .........: ";
            for( const auto &a : article._authors )
                s << "\"" << a << "\" ";
            s << "\n\tDate-stamp.........: " << article._datestamp << "\n"
              << "\tSource file path...: " << article._paths.src_html.string() << "\n"
              << "\tRelative html link.: " << article._paths.out_html.string() << "\n"
              << "\tIndex entry html...: " << article._paths.entry_html.string() << "\n"
              << "\tArticle stylesheet.: " << article._paths.css.string() << "\n"
              << "\tTags...............: ";
            for( const auto &t : article._tags )
                s << "\"" << t << "\" ";
            s << "\n\tSummary rel path(s): ";
            for( const auto &l : article._summary.path_write_pos )
                s << l.first << " ";
            s << "\n};";

            return s;
        }

        struct Paths {
            std::filesystem::path src_html;
            std::filesystem::path entry_html;
            std::filesystem::path out_html;
            std::filesystem::path css;

        } _paths;

        struct Summary {
            HTML                       html;
            ConsecutivePathPositions_t path_write_pos;

        } _summary;

        size_t      _number;
        std::string _heading;
        DateStamp   _datestamp;
        bool        _is_featured;

        std::set<std::string, CaseInsensitiveCompare> _authors;
        std::set<std::string, CaseInsensitiveCompare> _tags;

        std::shared_ptr<dto::Template> _cust_index_entry; //cache
    };
}

#endif //BLOGATOR_DTO_ARTICLE_H
