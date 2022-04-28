#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATBEGINTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATBEGINTK_H

#include "FormattingTk.h"

namespace blogator::parser::token::markdown {
    class FormatBeginTk : public FormattingTk {
      public:
        explicit FormatBeginTk( TextPos position );
        FormatBeginTk( std::u32string text, TextPos position );
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATBEGINTK_H