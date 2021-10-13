#ifndef BLOGATOR_PARSER_TOKENISER_HTML5_H
#define BLOGATOR_PARSER_TOKENISER_HTML5_H

#include <deque>
#include <memory>
#include <filesystem>

#include "../dom/TreeBuilder.h"
#include "../specs/html5/Specifications.h"
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
        explicit HTML5( dom::TreeBuilder & tree_builder, specs::html5::TokeniserState init_state, std::u32string  last_start_tag = U"" );

        specs::Context parse( U32Text &source, specs::Context starting_ctx = specs::Context::HTML5 );
        void reset();

        [[nodiscard]] size_t errors() const;

      private:
        typedef specs::html5::TokeniserState State_e;
        typedef specs::html5::TokenType      Type_e;

        static const uint32_t NULL_CHAR              = 0x0000;
        static const uint32_t CARRIAGE_RETURN        = 0x000D; //CR
        static const uint32_t EXCLAMATION_MARK       = 0x0021; //'!'
        static const uint32_t QUOTATION_MARK         = 0x0022; //'"'
        static const uint32_t NUMBER_SIGN            = 0x0023; //'#'
        static const uint32_t AMPERSAND              = 0x0026; //'&'
        static const uint32_t APOSTROPHE             = 0x0027; //'''
        static const uint32_t HYPHEN_MINUS           = 0x002D; //'-'
        static const uint32_t SOLIDUS                = 0x002F; //'/'
        static const uint32_t DIGIT_ZERO             = 0x0030; //'0'
        static const uint32_t SEMICOLON              = 0x003B; //';'
        static const uint32_t LESS_THAN_SIGN         = 0x003C; //'<'
        static const uint32_t EQUALS_SIGN            = 0x003D; //'='
        static const uint32_t GREATER_THAN_SIGN      = 0x003E; //'>'
        static const uint32_t QUESTION_MARK          = 0x003F; //'?'
        static const uint32_t LATIN_CAPITAL_LETTER_X = 0x0058; //'X'
        static const uint32_t RIGHT_SQUARE_BRACKET   = 0x005D; //']'
        static const uint32_t GRAVE_ACCENT           = 0x0060; //'`'
        static const uint32_t LATIN_SMALL_LETTER_X   = 0x0078; //'x'
        static const uint32_t REPLACEMENT_CHAR       = 0xFFFD;
        static const uint32_t UNDEFINED_CHAR_10FFFF  = 0x10FFFF;
        static const auto     THIS_CONTEXT           = specs::Context::HTML5;

        bool                       _eof;
        std::filesystem::path      _src_path;
        size_t                     _error_count;
        dom::TreeBuilder         & _tree_builder;
        State_e                    _current_state;
        State_e                    _return_state;
        std::u32string             _last_start_tag;
        std::vector<uint32_t>      _temp_buffer;

        struct Cache {
            Type_e                                 type          { Type_e::UNKNOWN };
            TextPos                                position      { 0, 0 }; //TODO dynamically change this after everything is cleared
            std::vector<uint32_t>                  token_name_buffer;
            std::vector<uint32_t>                  field_buffer_a;          //used for Tag.attribute.name or Doctype.pid
            std::vector<uint32_t>                  field_buffer_b;          //used for Tag.attribute.name or Doctype.sid
            bool                                   not_missing_a { false }; //used for Doctype.pid
            bool                                   not_missing_b { false }; //used for Doctype.sid
            bool                                   quirks        { false }; //used for Doctype.forceQuirks
            uint32_t                               char_ref_code { 0 };     //used for parsing character reference codes
            bool                                   bad_ref_code  { false }; //used as an overflow flag
            std::unique_ptr<token::html5::HTML5Tk> token;
        } _pending;

        void logError( TextPos position, int err_code );

        void setState( State_e state ) noexcept;
        void setReturnState( State_e state ) noexcept;
        [[nodiscard]] State_e currentState() const noexcept;
        [[nodiscard]] State_e returnState() const noexcept;

        void clearTempBuffer();
        void appendToTempBuffer( uint32_t c );
        template<typename InputIt> void appendToTempBuffer( InputIt begin, InputIt end );
        [[nodiscard]] bool isEqualToTempBuffer( const std::u32string & str ) const;
        const std::vector<uint32_t> & tempBuffer();

        void clearPendingToken();
        void emitCharacterToken( TextPos position, uint32_t c );
        void emitCharacterToken( TextPos position, const std::u32string& str );
        void emitEndOfFileToken( TextPos position );

        void createCommentToken( TextPos position );
        void createDoctypeToken( TextPos position, bool force_quirks = false );
        void createStartTagToken( TextPos position );
        void createEndTagToken( TextPos position );

        bool appropriateEndTagToken();

        void emitPendingToken( TextPos position );
        void appendToPendingTokenText( uint32_t c );
        void appendToPendingTokenText( const std::u32string & txt );
        void appendToPendingTokenAttrNameBuffer( uint32_t c );
        void appendToPendingTokenAttrValueBuffer( uint32_t c );
        void appendToPendingTokenPIDBuffer( uint32_t c );
        void appendToPendingTokenSIDBuffer( uint32_t c );
        void setPendingDoctypeTokenQuirksFlag( bool force_quirks );
        void setPendingDoctypeTokenPIDFlag( bool not_missing );
        void setPendingDoctypeTokenSIDFlag( bool not_missing );
        bool addPendingTokenAttribute();
        void clearPendingTokenAttribute();
        void setPendingTokenSelfCloseFlag();

        void resetCharRefCode( uint32_t val = 0 );
        void addToCharRefCode( uint32_t base, uint32_t num );
        [[nodiscard]] uint32_t charRefCode() const;
        [[nodiscard]] bool validCharRefCode() const;

        void flushCodePoints( TextPos position );
    };
}

#endif //BLOGATOR_PARSER_TOKENISER_HTML5_H
