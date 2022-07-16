#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLECELL_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLECELL_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class TableCellTk : public BlockBeginTk {
      public:
        explicit TableCellTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLECELL_H