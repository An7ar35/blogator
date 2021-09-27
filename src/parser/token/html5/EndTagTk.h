#ifndef BLOGATOR_PARSER_TOKEN_HTML5_ENDTAGTK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_ENDTAGTK_H

#include "GenericTagTk.h"

namespace blogator::parser::token::html5 {
    /**
     * End tag HTML5 token
     */
    class EndTagTk : public GenericTagTk {
      public:
        explicit EndTagTk( TextPos position );
        EndTagTk( std::u32string text, TextPos position );

      protected:
        void toStr( std::ostream &os ) const override;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_HTML5_ENDTAGTK_H
