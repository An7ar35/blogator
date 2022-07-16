#ifndef BLOGATOR_PARSER_TOKENISER_MARKDOWN_H
#define BLOGATOR_PARSER_TOKENISER_MARKDOWN_H

#include <deque>
#include <stack>

#include "markdown/Block.h"
#include "../specs/markdown/specifications.h"
#include "../token/markdown/FormatBeginTk.h"
#include "../builder/MarkdownToHtml.h"
#include "../logging/ErrorObject.h"

namespace blogator::parser::token::markdown {
    class MarkdownTk;
    class ListItemTk;
    class TableHeadingTk;
}

namespace blogator::parser::tokeniser {
    class Markdown {
      public:
        explicit Markdown( parser::builder::MarkdownToHtml & html_builder );

        specs::Context parse( U32Text & text, specs::Context starting_ctx = specs::Context::MARKDOWN );

        void reset();

        [[nodiscard]] size_t errors() const;

      private:
        typedef specs::markdown::TokeniserState              State_e;
        typedef specs::markdown::TokenType                   Type_e;
        typedef specs::markdown::BlockFenceType              Fence_e;
        typedef specs::markdown::ListType                    ListType_e;
        typedef std::unique_ptr<token::markdown::MarkdownTk> TokenPtr_t;

        static constexpr auto     THIS_CONTEXT     = specs::Context::MARKDOWN;
        static constexpr auto     FOUR_SPACE_CHARS = UR"(    )";

        bool                      _eof;
        std::filesystem::path     _src_path;
        size_t                    _error_count;
        builder::MarkdownToHtml & _html_builder;
        State_e                   _current_state;
        std::stack<State_e>       _return_states; //keeps track of the return states in a call chain when required

        /**
         * Character processing bit flags for encoding '>', '<', '&' chars
         * and/or processing markdown special characters as format token
         */
        enum class CharacterProcessing {
            NONE                = 0x00,
            FORMAT_ONLY         = 0x01,
            ENCODING_ONLY       = 0x10,
            FORMAT_AND_ENCODING = 0x11,
        };

        /**
         * Section Marker
         * @param tk_i Index in token queue
         * @param fmt_i Index in formatting markers
         * @param err_i Index in error queue
         */
        struct SectionMarker {
            size_t tk_i;
            size_t fmt_i;
            size_t err_i;
        };

        /**
         * Block cache
         * @param setext_flag edge-case signal flag to signal the possibility of the current block being an 'setext' styled heading
         * @param position start position in source text of the buffer
         * @param buffer character buffer
         * @param block_lines Current block line count
         * @param block_fence Current block's fence type (for code-blocks)
         */
        struct BlockCache {
            bool                  setext_flag { true };
            TextPos               position    { 0, 0 };
            std::vector<char32_t> buffer;
            size_t                block_lines { 0 };
            Fence_e               block_fence { Fence_e::NONE };
        };

        /**
         * Table cache
         * @param is_fake_table Flag to treat line beginning with '|' as normal text (indicates table is not actually a table or is mis-formatted)
         * @param starting_block_i Index in open blocks where the initial TABLE token is opened
         * @param heading_tokens Pointers to heading tokens (helps keep track of n° of columns)
         * @param col_i Current column
         */
        struct TableCache {
            bool                                           is_fake_table    { false };
            size_t                                         starting_block_i { 0 };
            size_t                                         col_i            { 0 };
            std::vector<token::markdown::TableHeadingTk *> heading_tokens;
        };

        BlockCache                                  _pending;               //cached variables for current block
        TableCache                                  _table_cache;           //cached variable for current table block
        std::deque<TokenPtr_t>                      _queued_tokens;         //token queue for the current root block
        std::deque<logging::ErrorObject>            _queued_errors;         //errors queued for the current root block
        std::deque<token::markdown::FormattingTk *> _formatting_markers;    //formatting characters for the block (added as single CharacterTk and converted as appropriate on dispatch)
        std::deque<SectionMarker>                   _section_markers;       //markers pointing to the beginning of sections that are not yet formally identified
        std::stack<markdown::Block>                 _open_blocks;           //keeps track of current blocks opened
        std::deque<markdown::Block>                 _open_container;        //keeps track of container blocks currently opened

        size_t                                      _empty_lines;           //consecutive counter to help with closing container blocks
        size_t                                      _curr_open_container_i; //helper for keeping track of current container block prefixes to consume on a line
        size_t                                      _actual_blockquote_lvl; //number of '>' prefixing the last opened Blockquote block
        size_t                                      _curr_blockquote_lvl;   //number of '>' currently found on the line
        bool                                        _loose_list_spacing;    //flag signalling a potential 'loosened' spacing for the current list (i.e.: paragraphed)

        void logError( TextPos position, int err_code, bool bypass_buffer = false );
        void logError( TextPos position, int err_code, char32_t c );
        void logError( TextPos position, int err_code, std::string str );

        void setState( State_e state ) noexcept;
        [[nodiscard]] State_e currentState() const noexcept;
        void modifyReturnState( State_e state );
        void pushReturnState( State_e state );
        State_e consumeReturnState();

        /* Block level cache methods */
        void setSETextFlag( bool state );
        void setPendingBufferStartPosition( TextPos position );
        void appendToPendingBuffer( char32_t c );
        void appendToPendingBuffer( const std::u32string & str );
        void popPendingBuffer();
        void resetPendingBuffer( TextPos position );
        size_t incrementBlockLineCount();
        void setCurrentBlockFence( Fence_e fence );
        void clearPendingCacheData();
        [[nodiscard]] bool isPossibleStateForSETextHeading() const;
        [[nodiscard]] bool isPendingBufferLastChar( char32_t c );
        [[nodiscard]] TextPos pendingBufferPosition() const;
        [[nodiscard]] std::u32string pendingBufferToStr( size_t offset = 0 ) const;

        size_t flushPendingBufferToQueue( CharacterProcessing auto_process, size_t offset = 0 );

        void pushToOpenBlockStack( markdown::Block block );
        [[nodiscard]] const markdown::Block & peekLastOpenBlock() const;
        [[nodiscard]] const markdown::Block & peekOpenContainerBlock( size_t i ) const;
        [[nodiscard]] const markdown::Block & peekLastOpenContainerBlock() const;
        [[nodiscard]] Type_e peekLastOpenBlockType() const;
        [[nodiscard]] Type_e peekLastOpenContainerBlockType() const;
        [[nodiscard]] ListType_e peekLastOpenListType() const;
        [[nodiscard]] bool hasOpenBlock() const;
        [[nodiscard]] bool hasOpenContainerBlock() const;
        [[nodiscard]] size_t openContainerBlockCount() const;
        void popOpenBlockStack();
        void loosenLastListBlockSpacing();

        void pushSectionMarker();
        void pushSectionMarker( size_t pos );
        [[nodiscard]] bool emptySectionMarkers() const;
        [[nodiscard]] size_t peekLastSectionMarker() const;
        void popSectionMarker();
        void clearPendingTokensFromLastSectionMarkerPosition();
        void clearSectionMarkers();

        void pushFormattingMarker( token::markdown::FormattingTk * tk );
        token::markdown::FormattingTk::Type lastFormattingMarker();
        token::markdown::FormattingTk * peekLastFormattingMarker();
        void closeFormattingMarkers( TextPos position );
        void clearFormattingMarkers();

        size_t queueToken( TokenPtr_t token );
        TokenPtr_t & getQueuedToken( size_t queue_position );
        size_t queueParagraphToken( TextPos position );
        size_t queueFormattingToken( TextPos position, char32_t fmt_char );
        size_t queueFormattingToken( TextPos position, char32_t fmt_char_1, char32_t fmt_char_2 );
        void queueListToken( ListType_e type, TextPos position );
        size_t queueTableToken( TextPos position );
        void queueTableRowToken( TextPos position );
        void queueTableHeadingToken( TextPos position );
        void queueTableCellToken( TextPos position );
        void queueEndOfFileToken( TextPos position );
        void closeLastOpenedBlock( TextPos position );
        void closeOpenContainerBlocks( size_t index, TextPos position );
        void closeOpenContainerBlocks( const TextPos & target, bool inclusive, TextPos position );
        token::markdown::MarkdownTk * peekLastQueuedToken();
        void popLastQueuedToken();
        [[nodiscard]] bool requiresSpaceSeparatorToken() const;
        [[nodiscard]] bool hasQueuedTokens() const;
        void dispatchQueuedTokens();

        static bool isEscapable( char32_t c );
        static bool isWhiteSpace( const std::u32string &str );
        static bool isFormattingChar( char32_t c ) noexcept;
    };
}

#endif //BLOGATOR_PARSER_TOKENISER_MARKDOWN_H