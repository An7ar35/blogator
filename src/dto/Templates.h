#ifndef BLOGATOR_DTO_TEMPLATES_H
#define BLOGATOR_DTO_TEMPLATES_H

#include <map>
#include <memory>
#include <functional>
#include <regex>
#include <filesystem>

#include "Template.h"

namespace blogator::dto {
    struct Templates {
        Templates();

        std::unique_ptr<Template> _post;          //Post/Article page template
        std::shared_ptr<Template> _index;         //Generic index page template
        std::shared_ptr<Template> _index_list;    //Generic category index list page templates
        std::shared_ptr<Template> _year_list;     //Year list (dates.html) page template
        std::shared_ptr<Template> _year_index;    //Year index page template
        std::shared_ptr<Template> _tag_list;      //Tag list (tags.html) page template
        std::shared_ptr<Template> _tag_index;     //Tag index page template
        std::shared_ptr<Template> _author_list;   //Author list (authors.html) page template
        std::shared_ptr<Template> _author_index;  //Author index page template
        std::shared_ptr<Template> _index_entry;   //Index entry template
        std::unique_ptr<Template> _landing;       //Blog landing page (start) template
        std::shared_ptr<Template> _landing_entry; //Landing page newest/featured posts entry template

        /**
         * Extracts any relative paths found in an HTML text
         * @param source Source HTML DTO to extract any relative paths from
         * @return Consecutive positions and extracted paths in the HTML
         */
        static Template::ConsecutivePathPositions_t extractRelativePaths( HTML &source ) {
            const auto path_rx = std::regex( R"((.*?<.*?(?:src|href)=")([^/]{1}[a-zA-Z\.\-_\d/]+\.[a-zA-Z\d_]{2,4})("))" );
            auto       pos     = Template::ConsecutivePathPositions_t();
            size_t     line_i  = 0;

            std::string::size_type last_size = 0;

            for( auto &line : source._lines ) {
                std::stringstream ss;
                auto it = std::sregex_iterator( line.begin(), line.end(), path_rx );
                auto substr_start = 0;
                auto stream_size  = 0;

                while( it != std::sregex_iterator() ) {
                    auto col = substr_start + it->str( 1 ).length() - last_size;

                    pos.insert( std::make_pair( Template::InsertPosition( line_i, col ), it->str( 2 ) ) );
                    ss << it->str( 1 ) << it->str( 3 );

                    stream_size  += it->str( 1 ).size() + it->str( 3 ).size();
                    substr_start += it->str( 0 ).length();
                    last_size     = it->str( 2 ).length();

                    ++it;
                }

                ss << line.substr( substr_start );
                line = ss.str();
                last_size = 0;
                ++line_i;
            }

            return pos;
        }
    };
}

#endif //BLOGATOR_DTO_TEMPLATES_H
