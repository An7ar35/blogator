#ifndef BLOGATOR_PARSER_SPECS_HTML5_NAMEDCHARREF_H
#define BLOGATOR_PARSER_SPECS_HTML5_NAMEDCHARREF_H

#include <cctype>
#include <string>
#include <unordered_map>
#include <memory>

#include "../../../datastructure/Trie.h"

namespace blogator::parser::specs::infra {
    /**
     * DTO for named character references
     * https://html.spec.whatwg.org/multipage/parsing.html#named-character-reference-state
     * https://html.spec.whatwg.org/entities.json
     */
    struct NamedCharRef {
        NamedCharRef();
        NamedCharRef( std::u32string name, char32_t codepoint1 );
        NamedCharRef( std::u32string name, char32_t codepoint1, char32_t codepoint2 );

        bool operator ==( const NamedCharRef &rhs ) const;
        bool operator !=( const NamedCharRef &rhs ) const;
        bool operator <( const NamedCharRef &rhs ) const;
        bool operator >( const NamedCharRef &rhs ) const;
        bool operator <=( const NamedCharRef &rhs ) const;
        bool operator >=( const NamedCharRef &rhs ) const;

        static std::pair<bool, const NamedCharRef &> fetch( const std::u32string &name );
        static bool match( TrieTracker<char32_t> &tracker, char32_t next_element );

        std::u32string name;
        char32_t       codepoint1;
        char32_t       codepoint2;

      private:
        typedef std::unordered_map<std::u32string, NamedCharRef> NamedCharRefMap_t;

        static NamedCharRef      _not_found;
        static Trie<char32_t>    _ncr_trie;
        static NamedCharRefMap_t _ncr_map;

        static NamedCharRef loadNotFound() noexcept;
        static NamedCharRefMap_t loadMap() noexcept;
        static Trie<char32_t> loadTrie() noexcept;
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_NAMEDCHARREF_H