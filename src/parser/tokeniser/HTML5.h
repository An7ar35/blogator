#ifndef BLOGATOR_PARSER_TOKENISER_HTML5_H
#define BLOGATOR_PARSER_TOKENISER_HTML5_H

#include <deque>
#include <memory>
#include <filesystem>

#include "../dom/TreeBuilder.h"
#include "../specs/infra/specifications.h"
#include "../token/html5/HTML5Tk.h"

namespace blogator::parser::tokeniser {
    /**
     * HTML5 tokenizer
     * ---------------
     * As per the living standard (from the 27 September 2021)
     * Snapshot: https://html.spec.whatwg.org/commit-snapshots/bd12a97f421d2841866dec94589d41dc0ecaa948/#tokenization
     * Latest  : https://html.spec.whatwg.org/multipage/parsing.html#tokenization)
     */
    class HTML5 {
      public:
        explicit HTML5( dom::TreeBuilder & tree_builder );
        explicit HTML5( dom::TreeBuilder & tree_builder, specs::infra::TokeniserState init_state, std::u32string  last_start_tag = U"" );

        specs::Context parse( U32Text &source, specs::Context starting_ctx = specs::Context::HTML5 );
        void reset();

        [[nodiscard]] size_t errors() const;

      private:
        typedef specs::infra::TokeniserState State_e;
        typedef specs::infra::TokenType      Type_e;

        static const char32_t REPLACEMENT_CHAR      = 0xFFFD;
        static const char32_t UNDEFINED_CHAR_10FFFF = 0x10FFFF;
        static const auto     THIS_CONTEXT          = specs::Context::HTML5;

        bool                  _eof;
        std::filesystem::path _src_path;
        size_t                _error_count;
        dom::TreeBuilder &    _tree_builder;
        State_e               _current_state;
        State_e               _return_state;
        std::u32string        _last_start_tag;
        std::vector<char32_t> _temp_buffer;
        bool                  _acknowledgement_flag;

        struct Cache {
            Type_e                                 type          { Type_e::UNKNOWN };
            TextPos                                attr_position { 0, 0 };  //used for Tag.attribute duplication errors
            std::vector<char32_t>                  token_name_buffer;
            std::vector<char32_t>                  field_buffer_a;          //used for Tag.attribute.name or Doctype.pid
            std::vector<char32_t>                  field_buffer_b;          //used for Tag.attribute.name or Doctype.sid
            bool                                   not_missing_a { false }; //used for Doctype.pid
            bool                                   not_missing_b { false }; //used for Doctype.sid
            bool                                   quirks        { false }; //used for Doctype.forceQuirks
            char32_t                               char_ref_code { 0 };     //used for parsing character reference codes
            bool                                   bad_ref_code  { false }; //used as an overflow flag
            std::unique_ptr<token::html5::HTML5Tk> token;
        } _pending;

        void logError( TextPos position, int err_code );

        void setState( State_e state ) noexcept;
        void setReturnState( State_e state ) noexcept;
        [[nodiscard]] State_e currentState() const noexcept;
        [[nodiscard]] State_e returnState() const noexcept;

        void clearTempBuffer();
        void appendToTempBuffer( char32_t c );
        template<typename InputIt> void appendToTempBuffer( InputIt begin, InputIt end );
        [[nodiscard]] bool isEqualToTempBuffer( const std::u32string & str ) const;
        [[nodiscard]] const std::vector<char32_t> & tempBuffer() const;

        void clearPendingToken();
        void emitCharacterToken( TextPos position, char32_t c );
        void emitEndOfFileToken( TextPos position );

        void createCommentToken( TextPos position );
        void createDoctypeToken( TextPos position, bool force_quirks = false );
        void createStartTagToken( TextPos position );
        void createEndTagToken( TextPos position );

        bool appropriateEndTagToken();

        void emitPendingToken( TextPos position );
        void appendToPendingTokenText( char32_t c );
        void appendToPendingTokenText( const std::u32string & txt );
        void appendToPendingTokenAttrNameBuffer( char32_t c );
        void appendToPendingTokenAttrValueBuffer( char32_t c );
        void setPendingTokenAttrPosition( TextPos position );
        void appendToPendingTokenPIDBuffer( char32_t c );
        void appendToPendingTokenSIDBuffer( char32_t c );
        void setPendingDoctypeTokenQuirksFlag( bool force_quirks );
        void setPendingDoctypeTokenPIDFlag( bool not_missing );
        void setPendingDoctypeTokenSIDFlag( bool not_missing );
        bool addPendingTokenAttribute();
        void clearPendingTokenAttribute();
        void setPendingTokenSelfCloseFlag();

        void resetCharRefCode( char32_t val = 0 );
        void addToCharRefCode( char32_t base, char32_t num );
        [[nodiscard]] char32_t charRefCode() const;
        [[nodiscard]] bool validCharRefCode() const;

        void setAcknowledgementFlag();
        void resetAcknowledgementFlag();

        void flushCodePoints( TextPos position );
    };
}

#endif //BLOGATOR_PARSER_TOKENISER_HTML5_H
