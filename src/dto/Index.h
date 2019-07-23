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

            std::string         prefix_id;       //item's prefix id
            std::vector<size_t> article_indices; //indices to the master Article index
            PagePaths_t         file_names;      //page file name(s) in the list's sub-directory
        };

        typedef std::map<std::string, ListIndexContent> ListIndexPaths_t;

        struct Category {
            size_t                   page_count { 0 };
            ListIndexPaths_t         cats;
            std::vector<std::string> top; //size set in configuration file
        };

        //======================================VARIABLES===========================================

        struct Paths {
            struct Templates {
                std::filesystem::path landing;
                std::filesystem::path landing_entry;
                std::filesystem::path post;
                std::filesystem::path index;
                std::filesystem::path index_list;
                std::filesystem::path year_list;
                std::filesystem::path year_index;
                std::filesystem::path tag_list;
                std::filesystem::path tag_index;
                std::filesystem::path author_list;
                std::filesystem::path author_index;
                std::filesystem::path index_entry;
            } templates;

            struct CSS {
                std::filesystem::path index;
                std::filesystem::path blog;
            } css;

        } _paths;

        struct Indices {
            Category byTag;
            Category byAuthor;
            Category byYear;

            struct ChronoIndex {
                size_t      page_count;
                PagePaths_t file_names;

            } chronological;

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
            s << "(Template) Landing page ......: " << index._paths.templates.landing.string() << "\n";
            s << "(Template) Landing entry .....: " << index._paths.templates.landing_entry.string() << "\n";
            s << "(Template) Post page .........: " << index._paths.templates.post.string() << "\n";
            s << "(Template) Index page  .......: " << index._paths.templates.index.string() << "\n";
            s << "(Template) Index entry .......: " << index._paths.templates.index_entry.string() << "\n";
            s << "(Template) Generic list page .: " << index._paths.templates.index_list.string() << "\n";
            s << "(Template) Year-list page ....: " << index._paths.templates.year_list.string() << "\n";
            s << "(Template) Year-index page ...: " << index._paths.templates.year_index.string() << "\n";
            s << "(Template) Tag-list page .....: " << index._paths.templates.tag_list.string() << "\n";
            s << "(Template) Tag-index page ....: " << index._paths.templates.year_index.string() << "\n";
            s << "(Template) Author-list page ..: " << index._paths.templates.author_list.string() << "\n";
            s << "(Template) Author-index page .: " << index._paths.templates.author_index.string() << "\n";
            s << "(CSS) Blog stylesheet ........: " << index._paths.css.blog.string() << "\n";
            s << "(CSS) Index stylesheet .......: " << index._paths.css.index.string() << "\n";
            s << "Article (posts) count.........: " << index._articles.size() << " found.\n";
            s << "Categories (tags) count ......: " << index._indices.byTag.cats.size() << " found." << std::endl;
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