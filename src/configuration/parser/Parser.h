#ifndef BLOGATOR_CONFIGURATION_PARSER_PARSER_H
#define BLOGATOR_CONFIGURATION_PARSER_PARSER_H

#include <memory>

namespace blogator::encoding {
    class Source;
}

namespace blogator::configuration {
    class Configuration;
}

namespace blogator::configuration::parser {
    class Parser {
      public:
        std::unique_ptr<Configuration> parse( encoding::Source &source );

        [[nodiscard]] size_t errors() const;

      private:

    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_PARSER_H