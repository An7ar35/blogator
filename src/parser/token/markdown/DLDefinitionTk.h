#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_DLDEFINITIONTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_DLDEFINITIONTK_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class DLDefinitionTk : public BlockBeginTk {
      public:
        explicit DLDefinitionTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_DLDEFINITIONTK_H