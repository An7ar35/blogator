#ifndef BLOGATOR_PARSER_SPECS_HTML5_NUMERICCHARREF_H
#define BLOGATOR_PARSER_SPECS_HTML5_NUMERICCHARREF_H

#include <cctype>
#include <string>

namespace blogator::parser::specs::infra {
    /**
     * DTO for named character references
     * https://html.spec.whatwg.org/multipage/parsing.html#numeric-character-reference-end-state
     */
    struct NumericCharRef {
        NumericCharRef();
        NumericCharRef( uint32_t number, uint32_t codepoint );

        bool operator ==( const NumericCharRef &rhs ) const;
        bool operator !=( const NumericCharRef &rhs ) const;
        bool operator <( const NumericCharRef &rhs ) const;
        bool operator >( const NumericCharRef &rhs ) const;
        bool operator <=( const NumericCharRef &rhs ) const;
        bool operator >=( const NumericCharRef &rhs ) const;

        static std::pair<bool, uint32_t> fetch( uint32_t number );

        uint32_t       number;
        uint32_t       codepoint;
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_NUMERICCHARREF_H
