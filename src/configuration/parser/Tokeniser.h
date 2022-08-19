#ifndef BLOGATOR_CONFIGURATION_PARSER_TOKENISER_H
#define BLOGATOR_CONFIGURATION_PARSER_TOKENISER_H

#include <vector>
#include <stack>
#include <filesystem>

#include "specs/TokenType.h"
#include "specs/TokeniserState.h"
#include "../../dto/TextPos.h"

namespace blogator {
    class U32Text;
}

namespace blogator::configuration::parser {
    class Parser;
}

namespace blogator::configuration::parser::token {
    class ConfigTk;
}

namespace blogator::configuration::parser {
    class Tokeniser {
      public:
        explicit Tokeniser( Parser & parser );

        void parse( U32Text & text );

      private:
        typedef specs::TokenType                 Type_e;
        typedef specs::TokeniserState            State_e;
        typedef std::unique_ptr<token::ConfigTk> TokenPtr_t;

        /**
         * Cache
         * @param position Position of first character in source text
         * @param buffer   Character buffer
         */
        struct Cache {
            TextPos               position { 0, 0 };
            std::vector<char32_t> buffer;
        };


        bool                  _eof;
        std::filesystem::path _src_path;
        size_t                _error_count;
        Parser &              _parser;
        State_e               _current_state;
        std::stack<State_e>   _return_states;
        int                   _nesting;
        Cache                 _pending;


        void logError( TextPos position, int err_code );
        void logError( TextPos position, int err_code, char32_t c );
        void logError( TextPos position, int err_code, std::string str );

        void setState( State_e state ) noexcept;
        [[nodiscard]] State_e currentState() const noexcept;
        [[nodiscard]] State_e returnState() const;
        void modifyReturnState( State_e state );
        void pushReturnState( State_e state );
        State_e consumeReturnState();

        void appendToPendingBuffer( char32_t c );
        void resetPendingBuffer( TextPos position );
        void clearPendingBuffer();
        [[nodiscard]] bool matchPendingBuffer( const std::u32string & txt ) const;
        [[nodiscard]] TextPos pendingBufferPosition() const;
        [[nodiscard]] std::u32string pendingBufferToStr() const;

        void flushPendingBufferCharactersToDispatch();
        void dispatchPendingBufferAsToken( Type_e tk_type );
        void dispatchToken( TokenPtr_t token );
    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_TOKENISER_H