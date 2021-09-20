#ifndef BLOGATOR_PARSER_TOKEN_HTML5_H
#define BLOGATOR_PARSER_TOKEN_HTML5_H

#include "Token.h"
#include "../specs/html5/Namespace.h"
#include "../specs/html5/Element.h"

namespace blogator::parser::token {
    class HTML5 : public Token {
      public:
        HTML5( specs::Context ns, specs::html5::Element element, size_t line_number, size_t col_number );
        HTML5( specs::Context ns, specs::html5::Element element, std::string text, size_t line_number, size_t col_number );

        [[nodiscard]] specs::html5::Element getElement() const;

      private:
        specs::html5::Element _element;
    };
}


#endif //BLOGATOR_PARSER_TOKEN_HTML5_H
