#ifndef BLOGATOR_CONFIGURATION_PARSER_TOKENISER_H
#define BLOGATOR_CONFIGURATION_PARSER_TOKENISER_H


namespace blogator::configuration::parser {
    class Parser;

    class Tokeniser {
      public:
        explicit Tokeniser( Parser & parser );

      private:
        Parser & _parser;

    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_TOKENISER_H