#include <memory>

#ifndef BLOGATOR_DTO_OPTIONS_H
#define BLOGATOR_DTO_OPTIONS_H

#include <vector>
#include <map>
#include <functional>
#include <filesystem>
#include <chrono>
#include <ctime>

#include "DateStamp.h"

namespace blogator::dto {
    struct Options {
        static const char* BLOGATOR_NAME;
        static const char* BLOGATOR_VERSION;
        static const char* BLOGATOR_URL;
        const std::string  BLOGATOR_SIGNATURE;

        typedef std::unordered_map<unsigned, std::string> MonthStringLookup_t;

        Options();

        void setupAbsolutePaths( const std::filesystem::path & root_path );
        void setTempPath( const std::filesystem::path &temp_path );
        std::string getRunTimeStamp() const;
        const DateStamp & getRuntimeDateStamp() const;

        struct AbsPaths { //Absolute directory paths
            std::filesystem::path temp_dir;         //system's temporary directory path + /blogator
            std::filesystem::path root_dir;         //site's root directory (used to create absolute paths)
            std::filesystem::path source_dir;       //input directory for generating posts from
            std::filesystem::path template_dir;     //input directory for generating posts from
            std::filesystem::path posts_dir;        //output directory for generated posts
            std::filesystem::path css_dir;          //CSS directory where all required stylesheets exist
            std::filesystem::path index_dir;        //output directory for the blog/news index
            std::filesystem::path index_date_dir;   //output directory for the blog/news index by dates
            std::filesystem::path index_year_dir;   //output directory for the blog/news index by years
            std::filesystem::path index_tag_dir;    //output directory for the blog/news index by tags
            std::filesystem::path index_author_dir; //output directory for the blog/news index by authors

        } _paths;

        struct RelPaths { //Relative directory i/o paths structure
            struct Source { //IN for all posts and templates
                const std::filesystem::path root = "source";
            } source;

            struct Templates { //IN for templates
                const std::filesystem::path root = "templates";
            } templates;

            struct Index { //OUT of all indices (date/tag/tag list/authors)
                const std::filesystem::path root      = "index";
                const std::filesystem::path by_date   = "index/by_date";
                const std::filesystem::path by_year   = "index/by_year";
                const std::filesystem::path by_tag    = "index/by_tag";
                const std::filesystem::path by_author = "index/by_author";
            } index;

            struct Posts { //OUT of processed posts
                const std::filesystem::path root = "posts";
            } posts;

            struct CSS { //IN for stylesheets
                const std::filesystem::path root = "css";
            } css;

        } _folders;

        struct FileNames {
            const std::filesystem::path landing     = "index.html";
            const std::filesystem::path year_list   = "years.html";
            const std::filesystem::path tag_list    = "tags.html";
            const std::filesystem::path author_list = "authors.html";

        } _filenames;

        struct Templates {
            bool adapt_rel_paths = false;

        } _templates;

        struct Posts {
            bool build_future = false;
            bool safe_purge   = true;

        } _posts;

        struct Index {
            bool   show_post_numbers { false }; //show post number flag
            size_t items_per_page    { 10 };    //number of posts-per-page in the indices
            bool   index_by_year     { false }; //flag to create a year based index
            bool   index_by_tag      { true  }; //flag to create a tag based index
            bool   index_by_author   { false }; //flag to create a author based index

        } _index;

        struct LandingPage {
            size_t                             most_recent = 5;
            size_t                             top_tags    = 3;
            size_t                             top_authors = 3;
            std::map<std::string, size_t>      featured; //{K=path str, V=order position}

        } _landing_page;

        struct PageNav {
            std::string separator = " / ";
            std::string forward   = ">>";
            std::string backward = "<<";
            std::string first     = "First";
            std::string last      = "Last";

        } _page_nav;

        struct Breadcrumb {
            std::string start     = "Start";
            std::string by_date   = "Index";
            std::string by_year   = "Year list";
            std::string by_tag    = "Tag list";
            std::string by_author = "Author list";
            std::string page      = "Page #";

        } _breadcrumb;

        struct RSS {
            bool        generate   { false };
            size_t      item_count { 10 };
            std::string file_name  { "feed.rss" };
            std::string root_url;
            std::string title;
            std::string description;
            std::string copyright;
            std::string img_url;
            std::string img_link;
            std::string img_alt;
            std::string img_width;
            std::string img_height;

        } _rss;

        /**
         * Month string lookup
         */
        MonthStringLookup_t _months = std::unordered_map<unsigned, std::string> ( {
            { 1, "January" }, { 2, "February" }, { 3, "March" }, { 4, "April" }, { 5, "May" },
            { 6, "June" }, { 7, "July" }, { 8, "August" }, { 9, "September" }, { 10, "October" },
            { 11, "November" }, { 12, "December" }
        } );

      private:
        const std::chrono::system_clock::time_point RUN_TIMESTAMP;
    };
}

#endif //BLOGATOR_DTO_OPTIONS_H
