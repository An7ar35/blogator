#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLEHEADING_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLEHEADING_H

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class TableHeadingTk : public BlockBeginTk {
      public:
        enum class Alignment {
            NONE = 0,
            LEFT,
            CENTER,
            RIGHT,
        };

        explicit TableHeadingTk( TextPos position );

        friend std::ostream & operator <<( std::ostream &os, TableHeadingTk::Alignment alignment );

        void setAlignment( Alignment alignment );
        [[nodiscard]] Alignment alignment() const;

        static Alignment resolveAlignment( const std::u32string & def );
        static Alignment resolveAlignment( const std::vector<char32_t> & def );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        static Alignment resolveAlignment( char32_t first, char32_t last );
        Alignment _alignment;
    };

    std::ostream & operator <<( std::ostream &os, TableHeadingTk::Alignment alignment );
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_TABLEHEADING_H