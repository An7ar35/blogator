#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_HTMLBLOCK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_HTMLBLOCK_H

#include "BlockBeginTk.h"
#include "../../specs/markdown/HtmlBlockType.h"

namespace blogator::parser::token::markdown {
    class HtmlBlockTk : public BlockBeginTk {
      public:
        explicit HtmlBlockTk( TextPos position );

        void setHtmlBlockType( specs::markdown::HtmlBlockType type );
        [[nodiscard]] specs::markdown::HtmlBlockType htmlBlockType() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        specs::markdown::HtmlBlockType _html_block_type;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_HTMLBLOCK_H