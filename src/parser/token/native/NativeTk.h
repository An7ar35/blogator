#ifndef BLOGATOR_PARSER_TOKEN_NATIVE_NATIVETK_H
#define BLOGATOR_PARSER_TOKEN_NATIVE_NATIVETK_H

#include "../Token.h"
#include "../../specs/native/specifications.h"

namespace blogator::parser::token::native {
    class NativeTk : public token::Token {
      public:
        NativeTk( reporter::Context ns, specs::native::Element element, TextPos position );
        NativeTk( reporter::Context ns, specs::native::Element element, std::u32string text, TextPos position );

        [[nodiscard]] specs::native::Element element() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        specs::native::Element _element;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_NATIVE_NATIVETK_H
