#ifndef BLOGATOR_DTO_OPTIONS_H
#define BLOGATOR_DTO_OPTIONS_H

#include <vector>
#include <filesystem>

namespace blogator::dto {
    struct Options {
        struct Paths {
            std::filesystem::path root_dir;   //site's root directory (used to create absolute paths)
            std::filesystem::path source_dir; //input directory for generating posts from
            std::filesystem::path posts_dir;  //output directory for generated posts
            std::filesystem::path css_dir;    //CSS directory where all required stylesheets exist
            std::filesystem::path index_dir;  //output directory for the blog/news index
            std::filesystem::path month_file; //{K,V} source file for month strings to use

            struct IndexSubDirs {
                const std::filesystem::path by_date = "by_date";
                const std::filesystem::path by_tag  = "by_tag";
            } index_sub_dirs;

        } _paths; //absolute paths to all directories

        struct Index {
            size_t items_per_page { 10 }; //number of posts-per-page in the indices

        } _index;

        struct PageNav {
            std::string separator = " / ";
            std::string forward   = ">>";
            std::string backwards = "<<";
            std::string first     = "First";
            std::string last      = "Last";

        } _page_nav;

        struct Breadcrumb {
            std::string start   = "Start";
            std::string by_tag  = "Tag list";
            std::string by_date = "Index";
            std::string page    = "Page #";
            std::string post    = "Post #";

        } _breadcrumb;

        struct RSS {
            std::string root;
        } _rss;
    };
}

#endif //BLOGATOR_DTO_OPTIONS_H
