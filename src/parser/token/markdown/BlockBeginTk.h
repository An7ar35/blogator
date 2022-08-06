#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKBEGINTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKBEGINTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class BlockBeginTk : public MarkdownTk {
      public:
        BlockBeginTk( specs::markdown::TokenType type, TextPos position );
        BlockBeginTk( specs::markdown::TokenType type, std::u32string text, TextPos position );

        [[nodiscard]] unsigned indent() const;
        [[nodiscard]] bool hasID() const;
        [[nodiscard]] const std::u32string & id() const;

        void setID( const std::u32string & id );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        std::u32string _id;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKBEGINTK_H