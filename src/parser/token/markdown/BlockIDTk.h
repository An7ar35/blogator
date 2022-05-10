#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKIDTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKIDTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class BlockIDTk : public MarkdownTk {
      public:
        explicit BlockIDTk( TextPos position );
        BlockIDTk( std::u32string text, TextPos position );

        static bool validateName( const std::u32string & name );
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_BLOCKIDTK_H