#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_CODEBLOCKTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_CODEBLOCKTK_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class CodeBlockTk : public BlockBeginTk {
      public:
        explicit CodeBlockTk( TextPos position );
        CodeBlockTk( std::u32string lang, TextPos position );

        [[nodiscard]] const std::u32string & lang() const;

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_CODEBLOCKTK_H