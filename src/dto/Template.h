#ifndef BLOGATOR_DTO_TEMPLATE_H
#define BLOGATOR_DTO_TEMPLATE_H

#include <map>
#include <memory>
#include <functional>

#include "HTML.h"

namespace blogator::dto {
    struct Template {
        struct InsertPosition {
            InsertPosition( HTML::LineIndex_t l, HTML::CharIndex_t c ) :
                line( l ), col( c )
            {};

            HTML::LineIndex_t line;
            HTML::CharIndex_t col;

            bool operator==( const InsertPosition &rhs ) const {
                return line == rhs.line  && col == rhs.col;
            }

            bool operator<( const InsertPosition &rhs ) const {
                return line == rhs.line ? col < rhs.col : line < rhs.line;
            }
        };

        typedef std::map<std::string, std::vector<InsertPosition>> DivWritePositions_t;
        typedef std::map<InsertPosition, std::string>              ConsecutiveWritePositions_t;

        /**
         * Post/Article page template
         */
        struct PostPage {
            std::unique_ptr<HTML> html;
            DivWritePositions_t div_write_pos = {
                { "breadcrumb",       std::vector<InsertPosition>() },
                { "page-nav",         std::vector<InsertPosition>() },
                { "post-content",     std::vector<InsertPosition>() },
                { "index-pane-dates", std::vector<InsertPosition>() },
                { "index-pane-tags",  std::vector<InsertPosition>() }
            };
        } _post;

        /**
         * Index page template
         */
        struct IndexPage {
            std::unique_ptr<HTML> html;
            DivWritePositions_t   div_write_pos = {
                { "breadcrumb",    std::vector<InsertPosition>() },
                { "page-nav",      std::vector<InsertPosition>() },
                { "index-entries", std::vector<InsertPosition>() }
            };
        } _index;

        /**
         * Tag list (tags.html) page template
         */
        struct TagListIndexPage {
            const std::filesystem::path file_name = "tags.html";
            std::unique_ptr<HTML>       html;
            DivWritePositions_t         div_write_pos = {
                { "breadcrumb",         std::vector<InsertPosition>() },
                { "tag-list",           std::vector<InsertPosition>() },
                { "tag-list-hierarchy", std::vector<InsertPosition>() }
            };
        } _tag_list;

        /**
         * Index entry template
         */
        struct IndexEntry {
            std::unique_ptr<HTML> html;
            DivWritePositions_t   div_write_pos = {
                { "post-number", std::vector<InsertPosition>() },
                { "heading",     std::vector<InsertPosition>() },
                { "tags",        std::vector<InsertPosition>() },
                { "date-stamp",  std::vector<InsertPosition>() }
            };
        } _index_entry;

        /**
         * Blog landing page (start) template
         */
        struct LandingPage {
            const std::filesystem::path file_name = "index.html";
            std::unique_ptr<HTML>       html;
            DivWritePositions_t         div_write_pos = {
                { "breadcrumb",     std::vector<InsertPosition>() },
                { "newest-posts",   std::vector<InsertPosition>() },
                { "top-tags",       std::vector<InsertPosition>() },
                { "featured-posts", std::vector<InsertPosition>() }
            };
        } _start;

        /**
         * Landing page newest/featured posts entry template
         */
        struct LandingPageEntry {
            std::unique_ptr<HTML> html;
            DivWritePositions_t   div_write_pos = {
                { "post-number", std::vector<InsertPosition>() },
                { "heading",     std::vector<InsertPosition>() },
                { "tags",        std::vector<InsertPosition>() },
                { "date-stamp",  std::vector<InsertPosition>() }
            };
        } _start_entry;

        /**
         * Gets a map of write positions ordered consecutively
         * @param positions Tag->insert location map
         * @return Consecutive write positions
         */
        static std::map<InsertPosition, std::string> getConsecutiveWritePositions( const DivWritePositions_t &positions ) {
            auto cons_writes = std::map<InsertPosition, std::string>();

            for( const auto &t : positions ) {
                for( auto p : t.second )
                    cons_writes.emplace( p, t.first );
            }

            return cons_writes;
        };
    };
}

#endif //BLOGATOR_DTO_TEMPLATE_H
