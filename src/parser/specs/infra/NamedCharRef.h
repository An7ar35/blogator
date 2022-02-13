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
        NamedCharRef( std::u32string name, uint32_t codepoint1 );
        NamedCharRef( std::u32string name, uint32_t codepoint1, uint32_t codepoint2 );

        bool operator ==( const NamedCharRef &rhs ) const;
        bool operator !=( const NamedCharRef &rhs ) const;
        bool operator <( const NamedCharRef &rhs ) const;
        bool operator >( const NamedCharRef &rhs ) const;
        bool operator <=( const NamedCharRef &rhs ) const;
        bool operator >=( const NamedCharRef &rhs ) const;

        static std::pair<bool, const NamedCharRef &> fetch( const std::u32string &name );
        static bool match( TrieTracker<uint32_t> &tracker, uint32_t next_element );

        std::u32string name;
        uint32_t       codepoint1;
        uint32_t       codepoint2;

      private:
        typedef std::unordered_map<std::u32string, NamedCharRef> NamedCharRefMap_t;

        static void loadNCR( NamedCharRef::NamedCharRefMap_t & map, Trie<uint32_t> &trie, std::unique_ptr<NamedCharRef> &ncr );

        static Trie<uint32_t>                _ncr_trie;
        static std::unique_ptr<NamedCharRef> _not_found;
        static NamedCharRefMap_t             _ncr_map;
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_NAMEDCHARREF_H