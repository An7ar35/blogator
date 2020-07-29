#ifndef BLOGATOR_DOM_PARSER_TOKENISER_H
#define BLOGATOR_DOM_PARSER_TOKENISER_H

#include <string>
#include <vector>

#include "../dto/Text.h"

namespace blogator::dom::parser {
    enum class TokenType { START_TAG, END_TAG, TEXT };

    struct Token {
        Token( dto::Text::LineIndex_t line_number,
               dto::Text::CharIndex_t char_number,
               TokenType              token_type,
               std::u32string         text_content );

        friend std::ostream & operator <<( std::ostream &os, const Token &token ) {
            os << "(" << token.line_i << ", " << token.char_i << ") ";
            switch( token.type ) {
                case TokenType::START_TAG: os << "START_TAG"; break;
                case TokenType::END_TAG  : os << "END_TAG";   break;
                case TokenType::TEXT     : os << "TEXT";      break;
            }
            os << " {" << encoding::encodeToUTF8( token.content ) << "}";
            return os;
        }

        bool operator ==( const Token &rhs ) const;
        bool operator !=( const Token &rhs ) const;

        dto::Text::LineIndex_t line_i;
        dto::Text::CharIndex_t char_i;
        TokenType              type;
        std::u32string         content;
    };

    class Tokeniser {
      public:
        explicit Tokeniser( const dto::Text &utf32_src );
        explicit Tokeniser( dto::Text &&utf32_src ) = delete;

        friend std::ostream & operator <<( std::ostream &os, const Tokeniser &tokeniser ) {
            for( const auto &t : tokeniser._tokens )
                os << t << "\n";
            return os;
        }

        /* access to the list of tokens created */
        typedef typename std::vector<Token>::iterator iterator;
        typedef typename std::vector<Token>::const_iterator const_iterator;
        [[nodiscard]] iterator begin();
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator cbegin() const;
        [[nodiscard]] const_iterator cend() const;

        [[nodiscard]] Token & at( std::vector<Token>::size_type pos );
        [[nodiscard]] const Token & at( std::vector<Token>::size_type pos ) const;
        [[nodiscard]] std::vector<Token>::size_type tokenCount() const;

      private:
        struct LineInfo {
            const dto::Text::Line_t      &line;
            const dto::Text::LineIndex_t &index;
        };

        struct CharInfo {
            dto::Text::Line_t::const_iterator &it;
            dto::Text::CharIndex_t            &index;
        };

        const dto::Text &  _source;
        std::vector<Token> _tokens;
        bool               _inside_comment_flag;

        void processLine( const dto::Text::Line_t &line, const dto::Text::LineIndex_t &line_i );
        void processTag( LineInfo line_info, CharInfo char_info );
        void processText( LineInfo line_info, CharInfo char_info );

        bool processCommentOpen( LineInfo line_info, CharInfo char_info );
        void processCommentText( LineInfo line_info, CharInfo char_info );
        void processCommentClose( LineInfo line_info, CharInfo char_info );
        static bool isCommentClose( LineInfo line_info, CharInfo char_info );

        //TODO deal with data-x and svg
    };
}

#endif //BLOGATOR_DOM_PARSER_TOKENISER_H
