#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_MARKDOWNTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_MARKDOWNTK_H

#include "../Token.h"
#include "../../specs/markdown/TokenType.h"

namespace blogator::parser::token::markdown {
    /**
    * Generic Markdown token
    */
    class MarkdownTk : public token::Token {
      public:
        MarkdownTk( specs::markdown::TokenType type, TextPos position );
        MarkdownTk( specs::markdown::TokenType type, std::u32string text, TextPos position );

        [[nodiscard]] specs::markdown::TokenType type() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        specs::markdown::TokenType _type;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_MARKDOWNTK_H