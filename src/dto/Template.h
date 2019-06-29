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
        typedef std::map<InsertPosition, std::string>              ConsecutiveDivWritePositions_t;
        typedef std::unordered_map<unsigned, std::string>          MonthStringLookup_t;

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
            std::unique_ptr<HTML> html;
            DivWritePositions_t   div_write_pos = {
                { "breadcrumb",         std::vector<InsertPosition>() },
                { "tag-list",           std::vector<InsertPosition>() },
                { "tag-list-hierarchy", std::vector<InsertPosition>() },
                { "tag-list-block_0",   std::vector<InsertPosition>() },
                { "tag-list-block_a",   std::vector<InsertPosition>() },
                { "tag-list-block_b",   std::vector<InsertPosition>() },
                { "tag-list-block_c",   std::vector<InsertPosition>() },
                { "tag-list-block_d",   std::vector<InsertPosition>() },
                { "tag-list-block_e",   std::vector<InsertPosition>() },
                { "tag-list-block_f",   std::vector<InsertPosition>() },
                { "tag-list-block_g",   std::vector<InsertPosition>() },
                { "tag-list-block_h",   std::vector<InsertPosition>() },
                { "tag-list-block_i",   std::vector<InsertPosition>() },
                { "tag-list-block_j",   std::vector<InsertPosition>() },
                { "tag-list-block_k",   std::vector<InsertPosition>() },
                { "tag-list-block_l",   std::vector<InsertPosition>() },
                { "tag-list-block_m",   std::vector<InsertPosition>() },
                { "tag-list-block_n",   std::vector<InsertPosition>() },
                { "tag-list-block_o",   std::vector<InsertPosition>() },
                { "tag-list-block_p",   std::vector<InsertPosition>() },
                { "tag-list-block_q",   std::vector<InsertPosition>() },
                { "tag-list-block_r",   std::vector<InsertPosition>() },
                { "tag-list-block_s",   std::vector<InsertPosition>() },
                { "tag-list-block_t",   std::vector<InsertPosition>() },
                { "tag-list-block_u",   std::vector<InsertPosition>() },
                { "tag-list-block_v",   std::vector<InsertPosition>() },
                { "tag-list-block_w",   std::vector<InsertPosition>() },
                { "tag-list-block_x",   std::vector<InsertPosition>() },
                { "tag-list-block_y",   std::vector<InsertPosition>() },
                { "tag-list-block_z",   std::vector<InsertPosition>() }
            };
        } _tag_list;

        /**
         * Blog landing page (start) template
         */
        struct LandingPage {
            std::unique_ptr<HTML> html;
            DivWritePositions_t   div_write_pos = {
                { "breadcrumb",     std::vector<InsertPosition>() },
                { "newest-posts",   std::vector<InsertPosition>() },
                { "top-tags",       std::vector<InsertPosition>() },
                { "featured-posts", std::vector<InsertPosition>() }
            };
        } _start;

        /**
         * Month string lookup
         */
        MonthStringLookup_t _months = std::unordered_map<unsigned, std::string> ( {
            { 1, "January" }, { 2, "February" }, { 3, "March" }, { 4, "April" }, { 5, "May" },
            { 6, "June" }, { 7, "July" }, { 8, "August" }, { 9, "September" }, { 10, "October" },
            { 11, "November" }, { 12, "December" }
        } );

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
