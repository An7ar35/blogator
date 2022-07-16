#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLE_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLE_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class TableTk : public BlockBeginTk {
      public:
        explicit TableTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLE_H