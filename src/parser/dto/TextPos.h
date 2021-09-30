#ifndef BLOGATOR_PARSER_DTO_TEXTPOS_H
#define BLOGATOR_PARSER_DTO_TEXTPOS_H

#include <string>

namespace blogator::parser {
    struct TextPos {
        TextPos();
        TextPos( size_t line, size_t col );

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