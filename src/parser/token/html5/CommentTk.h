#ifndef BLOGATOR_PARSER_TOKEN_HTML5_COMMENTTK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_COMMENTTK_H

#include "HTML5Tk.h"

namespace blogator::parser::token::html5 {
    /**
     * HTML5 comment token
     */
    class CommentTk : public HTML5Tk {
      public:
        explicit CommentTk( TextPos position );
        CommentTk( std::u32string text, TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_HTML5_COMMENTTK_H