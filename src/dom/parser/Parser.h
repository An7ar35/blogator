#ifndef BLOGATOR_DOM_PARSER_PARSER_H
#define BLOGATOR_DOM_PARSER_PARSER_H

#include "Tokeniser.h"
#include "../dot/DOT.h"
#include "../html5/html5.h"

namespace blogator::dom::parser {
    struct Parser {
      public:
        static DOTNode & parseTag( const parser::Token &token, DOTNode &parent );
        static DOTNode & parseText( const parser::Token &token, DOTNode &parent );

      private:
        static dom::html5::Tag parseOpeningTag( const std::u32string &str, std::unordered_map<std::u32string, std::u32string> &attributes );
        static dom::html5::Tag parseClosingTag( const std::u32string &str );

        //TODO maybe public + static all these and use them as callable helpers in DOT parser call>?

        //Tokeniser -> Parser into a DOT tree -> Validator of DOT tree?
        //OR Tokeniser -> Validator using stack -> Parser into a DOT tree?

        //TODO in any case maybe have initializer methods in the blogator::dom:: namespace for a easier way to understand how it all fits together at a glance

    };
}

#endif //BLOGATOR_DOM_PARSER_PARSER_H
