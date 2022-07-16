#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class HeadingTk : public BlockBeginTk {
      public:
        HeadingTk( std::u32string text, TextPos position );
        HeadingTk( uint8_t level, std::u32string text, TextPos position );

        [[nodiscard]] uint8_t level() const;

        void setLevel( uint8_t val );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        uint8_t _level;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_HEADING_H