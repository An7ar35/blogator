#ifndef BLOGATOR_PARSER_TOKEN_HTML5_STARTTAGTK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_STARTTAGTK_H

#include "GenericTagTk.h"

namespace blogator::parser::token::html5 {
    /**
     * Start tag HTML5 token
     */
    class StartTagTk : public GenericTagTk {
      public:
        explicit StartTagTk( TextPos position );
        StartTagTk( std::u32string text, TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_HTML5_STARTTAGTK_H
