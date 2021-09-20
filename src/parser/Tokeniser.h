#ifndef BLOGATOR_PARSER_HTML5_TOKENISER_H
#define BLOGATOR_PARSER_HTML5_TOKENISER_H

#include <vector>

#include "token/Token.h"

namespace blogator::parser {
    typedef std::string::const_iterator TextIterator_t;
    
    class Tokeniser {
      public:
        static std::vector<token::Token> parse( const std::string &u8text, specs::Context ctx = specs::Context::UNKNOWN );

      private:
        static std::vector<token::Token> parseBlogator( const std::string &u8text );
        static std::vector<token::Token> parseHTML( const std::string &u8text );
        static std::vector<token::Token> parseMarkdown( const std::string &u8text );
        
        static TextIterator_t parseBlogator( const std::string &u8text, std::vector<token::Token> &tokens, TextIterator_t iterator );
        static TextIterator_t parseHTML( const std::string &u8text, std::vector<token::Token> &tokens, TextIterator_t iterator );
        static TextIterator_t parseMarkdown( const std::string &u8text, std::vector<token::Token> &tokens, TextIterator_t iterator );
    };
}

#endif //BLOGATOR_PARSER_HTML5_TOKENISER_H
