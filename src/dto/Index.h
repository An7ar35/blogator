#ifndef BLOGATOR_DTO_INDEX_H
#define BLOGATOR_DTO_INDEX_H

#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include "Article.h"

namespace blogator::dto {
    struct Index {
        typedef std::vector<dto::Article>                    Articles_t;
        typedef std::map<std::string, std::vector<size_t>>   TagToArticleIndexMap_t;
        typedef std::map<std::string, std::filesystem::path> TagIndexPaths_t;
        typedef std::vector<std::filesystem::path>           PagePaths_t;

        struct Paths {
            struct Templates {
                std::filesystem::path post;
                std::filesystem::path index;
                std::filesystem::path start;
            } templates;

            struct CSS {
                std::filesystem::path index;
                std::filesystem::path blog;
            } css;

        } _paths;

        struct Indices {
            struct ByTag {
                TagIndexPaths_t        tag_directories;
                TagToArticleIndexMap_t tag_to_article_map; //(K=tag string, V=vector of indices to the master Article index)
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
            for( const auto &t : index._indices.byTag.tag_directories )
                s << "\"" << t.first << "\" ";
            s << std::endl << std::endl;

            s << "Tag to Article index mapping:\n";
            for( auto &e : index._indices.byTag.tag_to_article_map ) {
                s << e.first << ": { ";
                for( auto i : e.second )
                    s << i << " ";
                s << "}\n";
            }

            s << "\n=====ARTICLES=====\n";
            for( const auto &a : index._articles )
                s << a << std::endl;
            return s;
        }
    };
}

#endif //BLOGATOR_DTO_INDEX_H