#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_FOOTNOTEREFTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_FOOTNOTEREFTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class FootnoteRefTk : public MarkdownTk {
      public:
        FootnoteRefTk( std::u32string ref, TextPos position );

        const std::u32string & ref() const;

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_FOOTNOTEREFTK_H