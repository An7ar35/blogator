#ifndef BLOGATOR_PARSER_TOKEN_HTML5_ENDOFFILETK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_ENDOFFILETK_H

#include "HTML5Tk.h"

namespace blogator::parser::token::html5 {
    /**
     * HTML5 end-of-file token
     */
    class EndOfFileTk : public HTML5Tk {
      public:
        explicit EndOfFileTk( TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_HTML5_ENDOFFILETK_H
