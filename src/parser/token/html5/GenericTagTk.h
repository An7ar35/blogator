#ifndef BLOGATOR_PARSER_TOKEN_HTML5_GENERICTAGTK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_GENERICTAGTK_H

#include "HTML5Tk.h"

namespace blogator::parser::token::html5 {
    /**
     * DTO for attributes
     * @param name Name
     * @param value Value
     */
    struct Attribute {
        std::u32string name;
        std::u32string value;
    };

    typedef std::vector<Attribute> Attributes_t;

    /**
     * Generic HTML5 Tag base class
     */
    class GenericTagTk : public HTML5Tk {
      public:
        GenericTagTk( specs::html5::TokenType type, TextPos position );
        GenericTagTk( specs::html5::TokenType type, std::u32string text, TextPos position );

        [[nodiscard]] std::u32string name() const;
        [[nodiscard]] bool selfclosing() const;
        [[nodiscard]] const Attributes_t & attributes() const;

        void addAttribute( std::u32string name, std::u32string value );
        void setSelfClosing( bool flag = true );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        bool         _self_closing;
        Attributes_t _attributes;
    };
}


#endif //BLOGATOR_PARSER_TOKEN_HTML5_GENERICTAGTK_H
