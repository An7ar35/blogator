#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_HRULETK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_HRULETK_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class HorizontalRuleTk : public BlockBeginTk {
      public:
        HorizontalRuleTk( std::u32string text, TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_HRULETK_H