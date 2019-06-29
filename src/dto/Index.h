#ifndef BLOGATOR_DTO_INDEX_H
#define BLOGATOR_DTO_INDEX_H

#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include "Article.h"

namespace blogator::dto {
    struct Index {
        typedef std::vector<std::filesystem::path> PagePaths_t;
        typedef std::vector<dto::Article>          Articles_t;

        struct TagIndexContent {
            explicit TagIndexContent( std::string id ) : tag_id( std::move( id ) ) {};

            std::string           tag_id;          //tag's prefix id
            std::vector<size_t>   article_indices; //indices to the master Article index
            PagePaths_t           file_names;      //page file names in the tag's sub-directory
        };

        typedef std::map<std::string, TagIndexContent> TagIndexPaths_t;

        struct Paths {
            struct Templates {
                std::filesystem::path start;
                std::filesystem::path post;
                std::filesystem::path index;
                std::filesystem::path tag_list;
            } templates;

            struct CSS {
                std::filesystem::path index;
                std::filesystem::path blog;
            } css;

        } _paths;

        struct Indices {
            struct ByTag {
                TagIndexPaths_t tags;
            } byTag;

            struct ByDate {
                PagePaths_t page_file_names;
            } byDate;

        } _indices;

        Articles_t _articles;

        /**
         * Output stream operator
         * @param s     output stream
         * @param index Index DTO
         * @return output stream
         */
        friend std::ostream &operator <<( std::ostream &s, const Index &index ) {
            s << "Post page template..: " << index._paths.templates.post.string() << std::endl;
            s << "Index page template.: " << index._paths.templates.index.string() << std::endl;
            s << "Start page template.: " << index._paths.templates.start.string() << std::endl;
            s << "Blog stylesheet ....: " << index._paths.css.blog.string() << std::endl;
            s << "Index stylesheet ...: " << index._paths.css.index.string() << std::endl;
            s << "Article count.......: " << index._articles.size() << " found." << std::endl;
            s << "Tags found..........: ";
            for( const auto &t : index._indices.byTag.tags )
                s << "\"" << t.first << "\" ";
            s << std::endl << std::endl;

            s << "Tag to Article index mapping:\n";
            for( auto &e : index._indices.byTag.tags ) {
                s << e.first << ": { ";
                for( auto i : e.second.article_indices )
                    s << i << " ";
                s << "}\n";
            }

            s << "\n=================================ARTICLES START=================================\n";
            for( const auto &a : index._articles )
                s << a << std::endl;
            s << "==================================ARTICLES END==================================\n";
            return s;
        }
    };
}

#endif //BLOGATOR_DTO_INDEX_H