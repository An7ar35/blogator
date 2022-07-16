#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATENDTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATENDTK_H

#include "FormattingTk.h"

namespace blogator::parser::token::markdown {
    class FormatEndTk : public FormattingTk {
      public:
        explicit FormatEndTk( TextPos position );
        FormatEndTk( std::u32string text, TextPos position );
        FormatEndTk( uint32_t c, TextPos position );
        FormatEndTk( uint32_t c1, uint32_t c2, TextPos position );
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATENDTK_H