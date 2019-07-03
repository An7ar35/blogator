#ifndef BLOGATOR_DTO_OPTIONS_H
#define BLOGATOR_DTO_OPTIONS_H

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <chrono>
#include <ctime>

namespace blogator::dto {
    struct Options {
        typedef std::unordered_map<unsigned, std::string> MonthStringLookup_t;

        const std::string PROG_NAME    = "Blogator";
        const std::string PROG_VERSION = "1.0a";

        struct Paths {
            std::filesystem::path root_dir;   //site's root directory (used to create absolute paths)
            std::filesystem::path source_dir; //input directory for generating posts from
            std::filesystem::path posts_dir;  //output directory for generated posts
            std::filesystem::path css_dir;    //CSS directory where all required stylesheets exist
            std::filesystem::path index_dir;  //output directory for the blog/news index

            struct IndexSubDirs {
                const std::filesystem::path by_date = "by_date";
                const std::filesystem::path by_tag  = "by_tag";

            } index_sub_dirs;

        } _paths; //absolute paths to all directories

        struct Index {
            bool   show_post_numbers { false }; //show post number flag
            size_t items_per_page    { 10 };    //number of posts-per-page in the indices

        } _index;

        struct LandingPage {
            size_t                             most_recent = 5;
            size_t                             top_tags    = 3;
            std::vector<std::filesystem::path> featured; //source filenames of posts to feature

        } _landing_page;

        struct PageNav {
            std::string separator = " / ";
            std::string forward   = ">>";
            std::string backward = "<<";
            std::string first     = "First";
            std::string last      = "Last";

        } _page_nav;

        struct Breadcrumb {
            std::string start   = "Start";
            std::string by_tag  = "Tag list";
            std::string by_date = "Index";
            std::string page    = "Page #";

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

        /**
         * Gets the program's run time-stamp
         * @return Timestamp as a RFC 822 compliant string
         */
        std::string getRunTimeStamp() const {
            auto now = std::chrono::system_clock::to_time_t( RUN_TIMESTAMP );
            std::stringstream ss;
            ss << std::put_time( std::localtime( &now ), "%a, %d %b %Y %T %z" ); //RFC 822
            return ss.str();
        };

      private:
        const std::chrono::system_clock::time_point RUN_TIMESTAMP = std::chrono::system_clock::now();
    };
}

#endif //BLOGATOR_DTO_OPTIONS_H
