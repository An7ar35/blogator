#ifndef BLOGATOR_DTO_TEXT_H
#define BLOGATOR_DTO_TEXT_H

#include <vector>
#include <string>
#include <iostream>

#include "../encoding/encoding.h"

namespace blogator::dto {
    struct Text {
        typedef std::u32string    Line_t;
        typedef size_t            LineIndex_t;
        typedef Line_t::size_type CharIndex_t;

        friend std::ostream &operator <<( std::ostream &s, const Text &html ) {
            for( const auto &l : html._lines )
                s << encoding::encodeToUTF8( l ) << "\n";
            return s;
        }

        bool operator ==( const Text &rhs ) const;
        bool operator !=( const Text &rhs ) const;

        std::vector<Line_t> _lines;
    };
}

#endif //BLOGATOR_DTO_TEXT_H
