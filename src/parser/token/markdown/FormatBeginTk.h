#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATBEGINTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATBEGINTK_H

#include "FormattingTk.h"

namespace blogator::parser::token::markdown {
    class FormatBeginTk : public FormattingTk {
      public:
        explicit FormatBeginTk( TextPos position );
        FormatBeginTk( std::u32string text, TextPos position );
        FormatBeginTk( uint32_t c, TextPos position );
        FormatBeginTk( uint32_t c1, uint32_t c2, TextPos position );
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATBEGINTK_H