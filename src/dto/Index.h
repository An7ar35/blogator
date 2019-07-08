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

        struct ListIndexContent {
            explicit ListIndexContent( std::string id ) : prefix_id( std::move( id ) ) {};

            std::string           prefix_id;       //item's prefix id
            std::vector<size_t>   article_indices; //indices to the master Article index
            PagePaths_t           file_names;      //page file name(s) in the list's sub-directory
        };

        typedef std::map<std::string, ListIndexContent> ListIndexPaths_t;

        struct Paths {
            struct Templates {
                std::filesystem::path start;
                std::filesystem::path start_entry;
                std::filesystem::path post;
                std::filesystem::path index;
                std::filesystem::path tag_list;
                std::filesystem::path author_list;
                std::filesystem::path index_entry;
            } templates;

            struct CSS {
                std::filesystem::path index;
                std::filesystem::path blog;
            } css;

        } _paths;

        struct Indices {
            struct ByDate {
                size_t      page_count { 0 };
                PagePaths_t page_file_names;
            } byDate;

            struct ByTag {
                size_t                   page_count { 0 };
                ListIndexPaths_t         tags;
                std::vector<std::string> top_tags; //size='landing-top-tags' in blogator.cfg
            } byTag;

            struct ByAuthor {
                size_t                   page_count { 0 };
                ListIndexPaths_t         authors;
                std::vector<std::string> top_authors; //TODO implement in the future
            } byAuthor;

        } _indices;

        Articles_t _articles;
        Articles_t _featured;

        /**
         * Output stream operator
         * @param s     output stream
         * @param index Index DTO
         * @return output stream
         */
        friend std::ostream &operator <<( std::ostream &s, const Index &index ) {
            s << "(Template) Landing page .: " << index._paths.templates.start.string() << "\n";
            s << "(Template) Landing entry : " << index._paths.templates.start_entry.string() << "\n";
            s << "(Template) Post page ....: " << index._paths.templates.post.string() << "\n";
            s << "(Template) Index page  ..: " << index._paths.templates.index.string() << "\n";
            s << "(Template) Index entry ..: " << index._paths.templates.index_entry.string() << "\n";
            s << "(Template) Tag-list page : " << index._paths.templates.tag_list.string() << "\n";
            s << "(CSS) Blog stylesheet ...: " << index._paths.css.blog.string() << "\n";
            s << "(CSS) Index stylesheet ..: " << index._paths.css.index.string() << "\n";
            s << "Article (posts) count....: " << index._articles.size() << " found.\n";
            s << "Categories (tags) count .: " << index._indices.byTag.tags.size() << " found." << std::endl;
//            for( const auto &t : index._indices.byTag.tags )
//                s << "\"" << t.first << "\" ";
//            s << std::endl << std::endl;
//
//            s << "Tag to Article index mapping:\n";
//            for( auto &e : index._indices.byTag.tags ) {
//                s << e.first << ": { ";
//                for( auto i : e.second.article_indices )
//                    s << i << " ";
//                s << "}\n";
//            }
//
//            s << "\n=================================ARTICLES START=================================\n";
//            for( const auto &a : index._articles )
//                s << a << std::endl;
//            s << "==================================ARTICLES END==================================\n";
            return s;
        }
    };
}

#endif //BLOGATOR_DTO_INDEX_H