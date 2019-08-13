#ifndef BLOGATOR_DTO_HTML_H
#define BLOGATOR_DTO_HTML_H

#include <vector>
#include <string>
#include <iostream>

namespace blogator::dto {
    struct HTML {
        typedef size_t                 LineIndex_t;
        typedef std::string::size_type CharIndex_t;

        friend std::ostream &operator <<( std::ostream &s, const HTML &html ) {
            for( const auto &l : html._lines )
                s << l << "\n";
            return s;
        }

        bool operator ==( const HTML &rhs ) const;
        bool operator !=( const HTML &rhs ) const;

        size_t findLineOfTag( const std::string &tag );

        std::vector<std::string> _lines;
    };
}

#endif //BLOGATOR_DTO_HTML_H