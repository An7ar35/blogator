#ifndef BLOGATOR_PARSER_DTO_TEXTPOS_H
#define BLOGATOR_PARSER_DTO_TEXTPOS_H

#include <string>
#include <ostream>

namespace blogator::parser {
    struct TextPos {
        TextPos();
        TextPos( size_t line, size_t col );

        friend std::ostream & operator<<( std::ostream &os, const TextPos &pos ) {
            os << pos.line << ":" << pos.col;
            return os;
        }

        bool operator ==( const TextPos &rhs ) const;
        bool operator !=( const TextPos &rhs ) const;
        bool operator <( const TextPos &rhs ) const;
        bool operator >( const TextPos &rhs ) const;
        bool operator <=( const TextPos &rhs ) const;
        bool operator >=( const TextPos &rhs ) const;

        size_t line;
        size_t col;
    };
}

#endif //BLOGATOR_PARSER_DTO_TEXTPOS_H