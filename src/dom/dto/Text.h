#ifndef BLOGATOR_DOM_DTO_TEXT_H
#define BLOGATOR_DOM_DTO_TEXT_H

#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

#include "../../encoding/encoding.h"

namespace blogator::dom::dto {
    struct Text {
        Text() = default;
        template<typename ...LINE> Text( LINE ...lines );

        typedef std::u32string    Line_t;
        typedef size_t            LineIndex_t;
        typedef Line_t::size_type CharIndex_t;

        friend std::ostream &operator <<( std::ostream &s, const Text &html ) {
            for( const auto &l : html._lines )
                s << encoding::encodeToUTF8( l ) << "\n";
            return s;
        }

        template<typename ...LINE> void addLines( LINE ...lines );

        static bool isEOL( const dto::Text::Line_t::const_iterator &it, const dto::Text::Line_t &line );

        bool operator ==( const Text &rhs ) const;
        bool operator !=( const Text &rhs ) const;

        std::vector<Line_t> _lines;

      private:
        template<typename H, typename ...T> void addLines_( H head, T ...tail );
        void                                     addLines_() {}; //recurse stopper
    };
}

/**
 * Constructor
 * @tparam LINE Deducted var type passed (allowed: const char *, const char32_t, std::string, std::u32string )
 * @param lines List of lines (non-char32_t based args will be encoded from UTF8 to UTF32)
 */
template<typename... LINE> blogator::dom::dto::Text::Text( LINE... lines ) {
    addLines_( lines... );
}

/**
 * Adds a list of lines to the Text
 * @tparam LINE Deducted var type passed (allowed: const char *, const char32_t, std::string, std::u32string )
 * @param lines List of lines (non-char32_t based args will be encoded from UTF8 to UTF32)
 */
template<typename... LINE> void blogator::dom::dto::Text::addLines( LINE... lines ) {
    addLines_( lines... );
}

/**
 * Adds the first line in the packed argument list to the Text
 * @tparam H   Head var type
 * @tparam T   Tail var types
 * @param head First line in argument list
 * @param tail Remaining lines in argument list
 */
template<typename H, typename... T> void blogator::dom::dto::Text::addLines_( H head, T... tail ) {
    static_assert(
        ( std::is_same_v<H, const char32_t *> || std::is_same_v<H, std::basic_string<char32_t>> ||
          std::is_same_v<H, const char *>     || std::is_same_v<H, std::basic_string<char>> ),
        "Template arguments can only be of type `const char *`, `const char32_t *`, `std::string` or `std::u32string`."
    );

    if constexpr( std::is_same_v<H, const char *> || std::is_same_v<H, std::basic_string<char>> )
        _lines.emplace_back( blogator::encoding::encodeToUTF32( head ) );
    else
        _lines.emplace_back( std::move( head ) );

    addLines_( tail... );
}

#endif //BLOGATOR_DOM_DTO_TEXT_H
