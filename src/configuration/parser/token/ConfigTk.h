#ifndef BLOGATOR_CONFIGURATION_PARSER_TOKEN_CONFIGTK_H
#define BLOGATOR_CONFIGURATION_PARSER_TOKEN_CONFIGTK_H

#include <string>

#include "../specs/TokenType.h"
#include "../../../dto/TextPos.h"

namespace blogator::configuration::parser::token {
    class ConfigTk {
      public:
        ConfigTk( specs::TokenType type, TextPos position );
        ConfigTk( specs::TokenType type, char32_t c, TextPos position );
        ConfigTk( specs::TokenType type, std::u32string text, TextPos position );

        [[nodiscard]] specs::TokenType type() const;
        [[nodiscard]] const std::u32string & text() const;
        [[nodiscard]] TextPos position() const;

      private:
        specs::TokenType _type;
        std::u32string   _text;
        TextPos          _pos;
    };

    std::ostream & operator <<( std::ostream & os, const ConfigTk & tk );
}

#endif //BLOGATOR_CONFIGURATION_PARSER_TOKEN_CONFIGTK_H