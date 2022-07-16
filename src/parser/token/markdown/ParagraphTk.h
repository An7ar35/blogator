#ifndef BLOGATOR_PARSER_TOKEN_MAKRDOWN_PARAGRAPHTK_H
#define BLOGATOR_PARSER_TOKEN_MAKRDOWN_PARAGRAPHTK_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class ParagraphTk : public BlockBeginTk {
      public:
        explicit ParagraphTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MAKRDOWN_PARAGRAPHTK_H