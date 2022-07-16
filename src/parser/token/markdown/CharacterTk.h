#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_CHARACTERTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_CHARACTERTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    /**
     * Markdown character token
     */
    class CharacterTk : public  MarkdownTk {
      public:
        explicit CharacterTk( TextPos position );
        CharacterTk( char32_t c, TextPos position );
        CharacterTk( std::u32string text, TextPos position );
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_CHARACTERTK_H