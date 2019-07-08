#ifndef BLOGATOR_DTO_HTML_H
#define BLOGATOR_DTO_HTML_H

#include <vector>
#include <string>

namespace blogator::dto {
    struct HTML {
        typedef size_t                 LineIndex_t;
        typedef std::string::size_type CharIndex_t;

        std::vector<std::string> _lines;

        friend std::ostream &operator <<( std::ostream &s, const HTML &html ) {
            for( const auto &l : html._lines )
                s << l << "\n";
            return s;
        }

        /**
         * Equality operator
         * @param rhs HTML DTO to compare to
         * @return result
         */
        bool operator ==( const HTML &rhs ) const {
            if( _lines.size() != rhs._lines.size() )
                return false;

            auto it1 = _lines.cbegin();
            auto it2 = rhs._lines.cbegin();

            while( it1 != _lines.cend() && it2 !=_lines.cend() ){
                if( *it1 != *it2 )
                    return false;
                ++it1; ++it2;
            }

            return true;
        };

        /**
         * Inequality operator
         * @param rhs HTML DTO to compare to
         * @return result
         */
        bool operator !=( const HTML &rhs ) const {
            return !( *this == rhs );
        };
    };
}

#endif //BLOGATOR_DTO_HTML_H
