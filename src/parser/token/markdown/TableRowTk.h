#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLEROW_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLEROW_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class TableRowTk : public BlockBeginTk {
      public:
        explicit TableRowTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLEROW_H