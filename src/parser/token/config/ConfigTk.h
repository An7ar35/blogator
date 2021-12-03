#ifndef BLOGATOR_PARSER_TOKEN_CONFIG_CONFIGTOKEN_H
#define BLOGATOR_PARSER_TOKEN_CONFIG_CONFIGTOKEN_H

#include "../Token.h"
#include "../../specs/config/tokeniser/TokenType.h"

namespace blogator::parser::token::config {
    /**
     * Generic configuration token
     */
    class ConfigTk : public token::Token {
      public:
        ConfigTk( specs::config::TokenType type, TextPos position );
        ConfigTk( specs::config::TokenType type, std::u32string text, TextPos position );

        [[nodiscard]] specs::config::TokenType type() const;

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        specs::config::TokenType _type;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_CONFIG_CONFIGTOKEN_H