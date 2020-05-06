#ifndef BLOGATOR_DTO_TABLEOFCONTENTS_H
#define BLOGATOR_DTO_TABLEOFCONTENTS_H

#include <list>
#include <optional>
#include <deque>
#include "InsertPosition.h"

namespace blogator::dto {
    /**
     * Describes a Table of Contents from a single article based on the headings found (if any)
     */
    class TableOfContents {
      public:
        enum class HeadingLevel {
            H1 = 1, H2 = 2, H3 = 3, H4 = 4, H5 = 5, H6 = 6
        };

        struct Heading {
            Heading( HeadingLevel hTag, std::string heading_txt );
            Heading( HeadingLevel hTag, std::string heading_txt, InsertPosition heading_pos );

            [[nodiscard]] std::string printID() const;
            [[nodiscard]] std::string printNumbering() const;
            [[nodiscard]] std::string print() const;
            [[nodiscard]] int depth() const;

            bool operator==( const Heading &rhs ) const;
            bool operator!=( const Heading &rhs ) const;

            friend std::ostream& operator <<( std::ostream& os, const Heading& h ) {
                os << h.printNumbering() << " " << h.str;
                return os;
            }

            HeadingLevel     level;     //Heading HTML tag level
            std::string      str;       //Heading string
            InsertPosition   str_pos;   //Heading string position
            std::vector<int> numbering; //Numbering for the heading
        };

        TableOfContents();
        TableOfContents( int max_levels, int offset );

        void findHeading( const size_t &line_number, const std::string &line );
        void findTocBlock( const size_t &line_number, const std::string &line, const std::string &css_class );
        void finaliseToC();

        [[nodiscard]] HeadingLevel minHeadingLevel() const;
        [[nodiscard]] HeadingLevel maxHeadingLevel() const;
        [[nodiscard]] HeadingLevel rootHeadingLevel() const;
        [[nodiscard]] HeadingLevel leafHeadingLevel() const;
        [[nodiscard]] bool tocBlockExists() const;
        [[nodiscard]] const std::map<dto::InsertPosition, Heading> & headings() const;
        [[nodiscard]] const std::list<dto::InsertPosition> & tocPositions() const;

      private:
        static const int MAX_HEADING_LEVEL { 6 };

        int _min_level;
        int _max_level;
        int _root_level;
        int _leaf_level;

        std::list<dto::InsertPosition>    _toc_insert_pos;
        std::map<InsertPosition, Heading> _heading_insert_pos;

        void generateNumbering( std::map<InsertPosition, Heading> &headings ) const;
    };
}

#endif //BLOGATOR_DTO_TOC_H
