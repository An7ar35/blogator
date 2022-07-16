#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_FOOTNOTEREFTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_FOOTNOTEREFTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class FootnoteRefTk : public MarkdownTk {
      public:
        FootnoteRefTk( std::u32string text, TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_FOOTNOTEREFTK_H