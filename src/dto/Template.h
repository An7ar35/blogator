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
                { "page_nav",         std::vector<InsertPosition>() },
                { "post_content",     std::vector<InsertPosition>() },
                { "index_pane_dates", std::vector<InsertPosition>() },
                { "index_pane_tags",  std::vector<InsertPosition>() }
            };
        } _post;

        /**
         * Index page template
         */
        struct IndexPage {
            std::unique_ptr<HTML> html;
            DivWritePositions_t   div_write_pos = {
                { "index_entries", std::vector<InsertPosition>() },
                { "page_nav", std::vector<InsertPosition>() }
            };
        } _index;

        /**
         * Blog landing page (start) template
         */
        struct LandingPage {
            std::unique_ptr<HTML> html;
            DivWritePositions_t   div_write_pos = {
                { "top_posts",      std::vector<InsertPosition>() },
                { "newest_posts",   std::vector<InsertPosition>() },
                { "featured_posts", std::vector<InsertPosition>() }
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
