#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_DLTITLETK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_DLTITLETK_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class DLTitleTk : public BlockBeginTk {
      public:
        explicit DLTitleTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_DLTITLETK_H