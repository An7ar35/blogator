#ifndef BLOGATOR_DTO_INSERTPOSITION_H
#define BLOGATOR_DTO_INSERTPOSITION_H

#include <iostream>
#include <map>
#include <list>
#include <filesystem>

#include "HTML.h"

namespace blogator::dto {
    struct InsertPosition {
        InsertPosition( HTML::LineIndex_t l, HTML::CharIndex_t c ) :
            line( l ), col( c )
        {};

        HTML::LineIndex_t line;
        HTML::CharIndex_t col;

        friend std::ostream &operator <<( std::ostream &s, const InsertPosition &insert_pos ) {
            s << insert_pos.line << ":" << insert_pos.col;
            return s;
        }

        bool operator==( const InsertPosition &rhs ) const {
            return line == rhs.line  && col == rhs.col;
        }

        bool operator<( const InsertPosition &rhs ) const {
            return line == rhs.line ? col < rhs.col : line < rhs.line;
        }

        bool operator>( const InsertPosition &rhs ) const {
            return line == rhs.line ? col > rhs.col : line > rhs.line;
        }
    };

    typedef std::map<InsertPosition, std::string>           ConsecutiveWritePositions_t;
    typedef std::map<InsertPosition, std::filesystem::path> ConsecutivePathPositions_t;
}

#endif //BLOGATOR_DTO_INSERTPOSITION_H