#ifndef BLOGATOR_PARSER_TOKEN_TOKEN_H
#define BLOGATOR_PARSER_TOKEN_TOKEN_H

#include <string>
#include <variant>

#include "../specs/Context.h"

namespace blogator::parser::token {
    class Token {
      public:
        Token( specs::Context ns, size_t line_number, size_t col_number );
        Token( specs::Context ns, std::string text, size_t line_number, size_t col_number );

        virtual void setText( std::string text );

        [[nodiscard]] virtual specs::Context context() const;
        [[nodiscard]] virtual std::string text() const;
        [[nodiscard]] virtual size_t lineNum() const;
        [[nodiscard]] virtual size_t colPos() const;

      private:
        specs::Context _context;
        std::string    _text;
        size_t         _line_pos;
        size_t         _col_pos;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_TOKEN_H
