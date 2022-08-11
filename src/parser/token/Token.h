#ifndef BLOGATOR_PARSER_TOKEN_TOKEN_H
#define BLOGATOR_PARSER_TOKEN_TOKEN_H

#include <string>
#include <vector>
#include <memory>

#include "../../reporter/enum/Context.h"
#include "../../dto/TextPos.h"

namespace blogator::parser::token {
    class Token {
      public:
        Token( reporter::Context ns, TextPos position );
        Token( reporter::Context ns, std::u32string text, TextPos position );

        friend std::ostream & operator<<( std::ostream &os, const Token &token ) {
            token.toStr( os );
            return os;
        }

        virtual void setText( std::u32string text ) final;

        [[nodiscard]] virtual reporter::Context context() const final;
        [[nodiscard]] virtual std::u32string & text() final;
        [[nodiscard]] virtual const std::u32string & text() const final;
        [[nodiscard]] virtual size_t lineNum() const final;
        [[nodiscard]] virtual size_t colPos() const final;
        [[nodiscard]] virtual TextPos position() const final;

      protected:
        virtual void toStr( std::ostream & os ) const;

      private:
        reporter::Context _context;
        std::u32string    _text;
        TextPos           _position;
    };
}

namespace blogator::parser {
    typedef std::vector<std::unique_ptr<token::Token>> Tokens_t;
}

#endif //BLOGATOR_PARSER_TOKEN_TOKEN_H
