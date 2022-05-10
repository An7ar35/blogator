#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_CODEBLOCKTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_CODEBLOCKTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class CodeBlockTk : public MarkdownTk {
      public:
        explicit CodeBlockTk( TextPos position );
        CodeBlockTk( std::u32string lang, TextPos position );

        [[nodiscard]] const std::u32string & lang() const;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_CODEBLOCKTK_H