#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_LINEBREAKTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_LINEBREAKTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class LineBreakTk : public MarkdownTk {
      public:
        explicit LineBreakTk( TextPos position );
        LineBreakTk( std::u32string text, TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_LINEBREAKTK_H