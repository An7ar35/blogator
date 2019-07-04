#ifndef BLOGATOR_DTO_OPTIONS_H
#define BLOGATOR_DTO_OPTIONS_H

#include <vector>
#include <map>
#include <functional>
#include <filesystem>
#include <chrono>
#include <ctime>

namespace blogator::dto {
    struct Options {
        typedef std::unordered_map<unsigned, std::string> MonthStringLookup_t;

        const std::string BLOGATOR_NAME      = "Blogator";
        const std::string BLOGATOR_VERSION   = "1.0a";
        const std::string BLOGATOR_URL       = "https://an7ar35.bitbucket.io"; //TODO update when a proper 'Blogator' page is created
        const std::string BLOGATOR_SIGNATURE = "<!-- Generated with " + BLOGATOR_NAME + " " + BLOGATOR_VERSION + " (" + BLOGATOR_URL + ") -->";

        struct AbsPaths { //Absolute directory paths
            std::filesystem::path root_dir;       //site's root directory (used to create absolute paths)
            std::filesystem::path source_dir;     //input directory for generating posts from
            std::filesystem::path template_dir;   //input directory for generating posts from
            std::filesystem::path posts_dir;      //output directory for generated posts
            std::filesystem::path css_dir;        //CSS directory where all required stylesheets exist
            std::filesystem::path index_dir;      //output directory for the blog/news index
            std::filesystem::path index_date_dir; //output directory for the blog/news index
            std::filesystem::path index_tag_dir;  //output directory for the blog/news index

        } _paths;

        struct RelPaths { //[HARD-CODED] Relative directory i/o paths structure
            struct Source { //IN for all posts and templates
                const std::filesystem::path root    = "source";
            } source;

            struct Templates { //IN for templates
                const std::filesystem::path root    = "templates";
            } templates;

            struct Index { //OUT of all indices (date/tag/tag list)
                const std::filesystem::path root    = "index";
                const std::filesystem::path by_date = "index/by_date";
                const std::filesystem::path by_tag  = "index/by_tag";
            } index;

            struct Posts { //OUT of processed posts
                const std::filesystem::path root    = "posts";
            } posts;

            struct CSS { //IN for stylesheets
                const std::filesystem::path root    = "css";
            } css;

        } _folders;

        struct Index {
            bool   show_post_numbers { false }; //show post number flag
            size_t items_per_page    { 10 };    //number of posts-per-page in the indices

        } _index;

        struct LandingPage {
            size_t                             most_recent = 5;
            size_t                             top_tags    = 3;
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
         * Sets the root path and generates all absolute paths from it
         * @param root_path Root directory path
         */
        void setupAbsolutePaths( const std::filesystem::path & root_path ) {
            _paths.root_dir       = root_path;
            _paths.template_dir   = _paths.root_dir / _folders.templates.root;
            _paths.source_dir     = _paths.root_dir / _folders.source.root;
            _paths.posts_dir      = _paths.root_dir / _folders.posts.root;
            _paths.css_dir        = _paths.root_dir / _folders.css.root;
            _paths.index_dir      = _paths.root_dir / _folders.index.root;
            _paths.index_date_dir = _paths.root_dir / _folders.index.by_date;
            _paths.index_tag_dir  = _paths.root_dir / _folders.index.by_tag;
        }

        /**
         * Gets the program's run time-stamp
         * @return Timestamp as a RFC 822 compliant string
         */
        std::string getRunTimeStamp() const {
            auto now = std::chrono::system_clock::to_time_t( RUN_TIMESTAMP );
            std::stringstream ss;
            ss << std::put_time( std::localtime( &now ), "%a, %d %b %Y %T %z" ); //RFC 822
            return ss.str();
        }

      private:
        const std::chrono::system_clock::time_point RUN_TIMESTAMP = std::chrono::system_clock::now();
    };
}

#endif //BLOGATOR_DTO_OPTIONS_H
