#ifndef BLOGATOR_DTO_TEXTPOS_H
#define BLOGATOR_DTO_TEXTPOS_H

#include <string>
#include <ostream>

namespace blogator {
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

        TextPos operator +( const TextPos &rhs ) const;
        TextPos operator -( const TextPos &rhs ) const;
        TextPos & operator +=( const TextPos &rhs );
        TextPos & operator -=( const TextPos &rhs );

        TextPos & newline();
        TextPos & increment( bool new_line = false );
        [[nodiscard]] std::string str() const;

        size_t line;
        size_t col;
    };
}

#endif //BLOGATOR_DTO_TEXTPOS_H