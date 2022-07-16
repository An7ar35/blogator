#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKQUOTETK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKQUOTETK_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class BlockQuoteTk : public BlockBeginTk {
      public:
        BlockQuoteTk( size_t nesting, TextPos position );

        [[nodiscard]] size_t nesting() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        size_t _nesting;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKQUOTETK_H