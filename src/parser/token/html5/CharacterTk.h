#ifndef BLOGATOR_PARSER_TOKEN_HTML5_CHARACTERTK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_CHARACTERTK_H

#include "HTML5Tk.h"

namespace blogator::parser::token::html5 {
    /**
     * HTML5 character token
     */
    class CharacterTk : public HTML5Tk {
      public:
        explicit CharacterTk( TextPos position );
        CharacterTk( std::u32string text, TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_HTML5_CHARACTERTK_H