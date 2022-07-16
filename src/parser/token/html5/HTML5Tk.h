#ifndef BLOGATOR_PARSER_TOKEN_HTML5_HTML5TK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_HTML5TK_H

#include "../Token.h"
#include "../../specs/infra/TokenType.h"

namespace blogator::parser::token::html5 {
    /**
     * Generic HTML5 token
     */
    class HTML5Tk : public token::Token {
      public:
        HTML5Tk( specs::infra::TokenType type, TextPos position );
        HTML5Tk( specs::infra::TokenType type, std::u32string text, TextPos position );

        [[nodiscard]] specs::infra::TokenType type() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        specs::infra::TokenType _type;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_HTML5_HTML5TK_H
