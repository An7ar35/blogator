#ifndef BLOGATOR_PARSER_TOKEN_MARKDOWN_LISTTK_H
#define BLOGATOR_PARSER_TOKEN_MARKDOWN_LISTTK_H

#include "BlockBeginTk.h"

namespace blogator::parser::specs::markdown {
    enum class ListType;
}

namespace blogator::parser::token::markdown {
    class ListTk : public BlockBeginTk {
      public:
        ListTk( specs::markdown::ListType list_type, TextPos position );

        void setTightFlag( bool flag );

        [[nodiscard]] specs::markdown::ListType listType() const;
        [[nodiscard]] bool tight() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        specs::markdown::ListType _list_type;
        bool                      _tight;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_MARKDOWN_LISTTK_H