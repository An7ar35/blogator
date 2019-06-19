#ifndef BLOGATOR_DTO_OPTIONS_H
#define BLOGATOR_DTO_OPTIONS_H

#include <filesystem>

namespace blogator::dto {
    struct Options { //absolute paths to all directories
        std::filesystem::path _root_dir;    //blog/news root directory
        std::filesystem::path _source_dir;  //input directory for generating posts from
        std::filesystem::path _posts_dir;   //output directory for generated posts
        std::filesystem::path _css_dir;     //CSS directory where all required stylesheets exist
        std::filesystem::path _index_dir;   //output directory for the blog/news index

        //add option (enum?) for placing the index div pane (none, before, after)
        //add option (enum?) for placing the page navigation div (top, bottom, both)
    };
}

#endif //BLOGATOR_DTO_OPTIONS_H
