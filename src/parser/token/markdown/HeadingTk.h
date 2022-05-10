#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class HeadingTk : public MarkdownTk {
      public:
        explicit HeadingTk( TextPos position );
        HeadingTk( std::u32string text, TextPos position );

        [[nodiscard]] uint8_t indent() const;

        void setIndent( uint8_t val );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        uint8_t _indent;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H