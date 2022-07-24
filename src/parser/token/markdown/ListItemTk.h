#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_LISTITEMTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_LISTITEMTK_H

#include <ostream>

#include "BlockBeginTk.h"

namespace blogator::parser::token::markdown {
    class ListItemTk : public BlockBeginTk {
      public:
        enum class Modality {
            NONE,
            CHECKED,
            UNCHECKED,
        };

        explicit ListItemTk( TextPos position );
        ListItemTk( bool ticked, TextPos position );
        ListItemTk( std::u32string ref_id, TextPos position );

        [[nodiscard]] Modality modality() const;
        [[nodiscard]] const std::u32string & refID() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        Modality _modality;
    };

    std::ostream & operator <<( std::ostream & os, ListItemTk::Modality modality );
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_LISTITEMTK_H