#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_IMAGETK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_IMAGETK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class ImageTk : public MarkdownTk {
      public:
        explicit ImageTk( TextPos position );
        ImageTk( std::u32string alt_text, TextPos position );
        ImageTk( std::u32string alt_text, std::u32string src, TextPos position );

        void setSource( const std::u32string & src );
        [[nodiscard]] const std::u32string & altTxt() const;
        [[nodiscard]] const std::u32string & src() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        std::u32string _src;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_IMAGETK_H