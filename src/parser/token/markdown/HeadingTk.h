#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class HeadingTk : public MarkdownTk {
      public:
        explicit HeadingTk( TextPos position );
        HeadingTk( std::u32string text, TextPos position );

        [[nodiscard]] uint8_t indent() const;
        [[nodiscard]] const std::u32string & customID() const;

        void setIndent( uint8_t val );
        void setCustomID( const std::u32string & id );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        uint8_t        _indent;
        std::u32string _custom_id;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H