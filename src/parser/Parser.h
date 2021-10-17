#ifndef BLOGATOR_PARSER_HTML5_TOKENISER_H
#define BLOGATOR_PARSER_HTML5_TOKENISER_H

#include <vector>
#include <memory>
#include <functional>

#include "dom/TreeBuilder.h"
#include "token/Token.h"

namespace blogator::parser {
    class Tokeniser {
      public:
        static Tokens_t parse( std::u32string &u32text, specs::Context ctx = specs::Context::UNKNOWN );
        static std::u32string preprocess( std::u32string & u32str, const std::filesystem::path & path = "" );

      private:
        static Tokens_t parseBlogator( std::u32string &u32text );
        static Tokens_t parseHTML( std::u32string &u32text );
        static Tokens_t parseMarkdown( std::u32string &u32text );
    };
}

#endif //BLOGATOR_PARSER_HTML5_TOKENISER_H
