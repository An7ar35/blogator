#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATTINGTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATTINGTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class FormattingTk : public MarkdownTk {
      public:
        enum class Type {
            NONE = 0,
            BOLD,           //"**...**" or "__...__"
            ITALIC,         //"*...*" or "_..._"
            STRIKETHROUGH,  //"~~...~~"
            HIGHLIGHT,      //"==...=="
            SUBSCRIPT,      //"~...~"
            SUPERSCRIPT,    //"^...^"
            INLINE_CODE,    //"`...`"
        };

        FormattingTk( specs::markdown::TokenType type, TextPos position );
        FormattingTk( specs::markdown::TokenType type, std::u32string text, TextPos position );

        friend std::ostream & operator <<( std::ostream &os, FormattingTk::Type type );

        [[nodiscard]] Type formatType() const;

        void setFormatType( Type type );

        static Type resolveFormateType( const std::u32string & text );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        Type _format_type;
    };

    std::ostream & operator <<( std::ostream &os, FormattingTk::Type type );
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_FORMATTINGTK_H