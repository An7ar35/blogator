#ifndef BLOGATOR_DTO_INDEX_H
#define BLOGATOR_DTO_INDEX_H

#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include "Article.h"

namespace blogator::dto {
    struct Index {
        typedef std::map<std::string, std::vector<size_t>> TagIndexMap_t;

        struct Paths {
            std::filesystem::path index_css;
            std::filesystem::path blog_css;
            std::filesystem::path post_template;
            std::filesystem::path index_template;
            std::filesystem::path start_template;
        } _paths;

        std::vector<Article>                         _articles;
        std::map<std::string, std::filesystem::path> _global_tags; //where in the "index" folder each tag index folder lives in

        /**
         * Output stream operator
         * @param s     output stream
         * @param index Index DTO
         * @return output stream
         */
        friend std::ostream &operator <<( std::ostream &s, const Index &index ) {
            s << "Post page template..: " << index._paths.post_template.string() << std::endl;
            s << "Index page template.: " << index._paths.index_template.string() << std::endl;
            s << "Start page template.: " << index._paths.start_template.string() << std::endl;
            s << "Blog stylesheet ....: " << index._paths.blog_css.string() << std::endl;
            s << "Index stylesheet ...: " << index._paths.index_css.string() << std::endl;
            s << "Article count.......: " << index._articles.size() << " found." << std::endl;
            s << "Tags found..........: ";
            for( const auto &t : index._global_tags )
                s << "\"" << t.first << "\" ";
            s << std::endl << std::endl;
            for( const auto &a : index._articles )
                s << a << std::endl;
            return s;
        }

        /**
         * Generate a tag index map
         * @param master_index Master Index
         * @return TagIndexMap_t (K=tag string, V=vector of indices to the master Article index)
         */
        static std::unique_ptr<TagIndexMap_t> createTagIndex( const blogator::dto::Index &master_index ) {
            auto tag_index = std::make_unique<TagIndexMap_t>();

            size_t i = 0;
            for( const auto &article : master_index._articles ) {
                for( const auto &tag : article._tags ) {
                    if( tag_index->find( tag ) == tag_index->end() ) {
                        tag_index->insert( std::make_pair( tag, std::vector<size_t>( { i } ) ) );
                    } else {
                        tag_index->at( tag ).emplace_back( i );
                    }
                    ++i;
                }
            }

            return std::move( tag_index );
        }
    };
}

#endif //BLOGATOR_DTO_INDEX_H