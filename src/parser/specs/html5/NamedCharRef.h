#ifndef BLOGATOR_PARSER_SPECS_HTML5_CHARACTERREFERENCE_H
#define BLOGATOR_PARSER_SPECS_HTML5_CHARACTERREFERENCE_H

#include <cctype>
#include <string>

namespace blogator::parser::specs::html5 {
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

        std::u32string name;
        uint32_t       codepoint1;
        uint32_t       codepoint2;
    };

    std::pair<bool, const NamedCharRef &> getNamedCharRef( const std::u32string &name );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_CHARACTERREFERENCE_H