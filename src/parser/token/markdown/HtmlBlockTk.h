#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_HTMLBLOCK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_HTMLBLOCK_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class HtmlBlockTk : public BlockBeginTk {
      public:
        explicit HtmlBlockTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_HTMLBLOCK_H