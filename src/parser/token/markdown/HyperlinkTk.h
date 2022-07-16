#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_HYPERLINKTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_HYPERLINKTK_H

#include "MarkdownTk.h"

namespace blogator::parser::token::markdown {
    class HyperlinkTk : public MarkdownTk {
      public:
        explicit HyperlinkTk( TextPos position );

        void setURL( const std::u32string & url );
        [[nodiscard]] const std::u32string & url() const;

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_HYPERLINKTK_H