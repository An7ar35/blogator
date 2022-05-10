#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKENDTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKENDTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class BlockEndTk : public MarkdownTk {
      public:
        BlockEndTk( specs::markdown::TokenType type, TextPos position );

        [[nodiscard]] specs::markdown::TokenType blockType() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        specs::markdown::TokenType _block_type;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKENDTK_H