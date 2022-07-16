#ifndef BLOGATOR_PARSER_TOKENISER_MARKDOWN_BLOCK_H
#define BLOGATOR_PARSER_TOKENISER_MARKDOWN_BLOCK_H

#include "../../dto/TextPos.h"
#include "../../specs/markdown/TokenType.h"
#include "../../specs/markdown/ListType.h"

namespace blogator::parser::token::markdown {
    class MarkdownTk;
}

namespace blogator::parser::tokeniser::markdown {
    /**
     * Block description
     * @param type Token type of the block
     * @param position Position in source text
     * @param token Pointer to queued token (can be nullptr)
     */
    struct Block {
        Block( specs::markdown::TokenType t, TextPos pos );
        explicit Block( token::markdown::MarkdownTk * tk );
        bool operator ==( const Block & rhs ) const;
        [[nodiscard]] size_t column() const;
        [[nodiscard]] specs::markdown::ListType listType() const;

        specs::markdown::TokenType    type;
        TextPos                       position;
        token::markdown::MarkdownTk * token;
    };
}

#endif //BLOGATOR_PARSER_TOKENISER_MARKDOWN_BLOCK_H