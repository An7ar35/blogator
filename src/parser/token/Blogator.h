#ifndef BLOGATOR_PARSER_TOKEN_BLOGATOR_H
#define BLOGATOR_PARSER_TOKEN_BLOGATOR_H

#include "Token.h"
#include "../specs/blogator/Element.h"

namespace blogator::parser::token {
    class Blogator : public Token {
      public:
        Blogator( specs::Context ns, specs::blogator::Element element, size_t line_number, size_t col_number );
        Blogator( specs::Context ns, specs::blogator::Element element, std::string text, size_t line_number, size_t col_number );

        [[nodiscard]] specs::blogator::Element getElement() const;

      private:
        specs::blogator::Element _element;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_BLOGATOR_H
