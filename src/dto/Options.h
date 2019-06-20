#ifndef BLOGATOR_DTO_OPTIONS_H
#define BLOGATOR_DTO_OPTIONS_H

#include <filesystem>

namespace blogator::dto {
    struct Options {
        struct Paths {
            std::filesystem::path root_dir;   //blog/news root directory
            std::filesystem::path source_dir; //input directory for generating posts from
            std::filesystem::path posts_dir;  //output directory for generated posts
            std::filesystem::path css_dir;    //CSS directory where all required stylesheets exist
            std::filesystem::path index_dir;  //output directory for the blog/news index
            std::filesystem::path month_file; //{K,V} source file for month strings to use

        } _paths; //absolute paths to all directories

        struct Index {
            size_t items_per_page { 10 }; //number of posts-per-page in the indices

        } _index;

        struct Navigation {
            enum class IndexNavPos {
                NONE, BEFORE, AFTER
            } index_div_location { IndexNavPos::NONE }; //insertion point of the index div pane

            enum class PageNavPos {
                TOP, BOTTOM, TOP_AND_BOTTOM
            } page_nav_pos { PageNavPos::TOP }; //position of page navigation div

        } _navigation;
    };
}

#endif //BLOGATOR_DTO_OPTIONS_H
