#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_ENDOFFILETK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_ENDOFFILETK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    /**
     * Markdown end-of-file token
     */
    class EndOfFileTk : public MarkdownTk {
      public:
        explicit EndOfFileTk( TextPos position );
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_ENDOFFILETK_H