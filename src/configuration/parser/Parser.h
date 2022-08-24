#ifndef BLOGATOR_CONFIGURATION_PARSER_PARSER_H
#define BLOGATOR_CONFIGURATION_PARSER_PARSER_H

#include <memory>

namespace blogator::configuration {
    class Configuration;
}

namespace blogator::configuration::parser::token {
    class ConfigTk;
}

namespace blogator::configuration::parser {
    class Parser {
      public:
        virtual void dispatch( std::unique_ptr<token::ConfigTk> token );

        std::unique_ptr<Configuration> reset();

        [[nodiscard]] size_t errors() const;

      private:

    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_PARSER_H