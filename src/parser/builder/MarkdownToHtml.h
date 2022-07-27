#ifndef BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H
#define BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H

#include <memory>
#include <filesystem>
#include <vector>
#include <stack>
#include <deque>
#include <unordered_map>

#include "../specs/markdown/InsertionMode.h"
#include "../specs/markdown/TokenType.h"
#include "../specs/infra/Element.h"
#include "../tokeniser/HTML5.h"

namespace blogator::parser {
    class U32Text;
}

namespace blogator::parser::tokeniser {
    class HTML5;
}

namespace blogator::parser::token::markdown {
    class MarkdownTk;
    class BlockEndTk;
}

namespace blogator::parser::builder {
    class MarkdownToHtml {
      public:
        MarkdownToHtml();

        void init( std::filesystem::path path );
        virtual void dispatch( std::unique_ptr<token::markdown::MarkdownTk> token );
        std::unique_ptr<U32Text> reset();

        [[nodiscard]] size_t errors() const;

      private:
        typedef specs::markdown::InsertionMode InsertionMode_e;
        typedef specs::infra::Element          HtmlElement_e;
        typedef specs::markdown::TokenType     TkType_e;

        /**
         * Footnote reference
         * @param position Reference position in markdown text
         * @param ref_id   Reference ID for HTML output
         * @param has_def  Flag to track whether or not a reference has a definition of the same id
         */
        struct FootnoteRef {
            FootnoteRef( TextPos position, std::u32string ref_id );

            TextPos        position;
            std::u32string id;
            bool           has_def;
        };

        struct ImageCache {
            std::unique_ptr<token::markdown::MarkdownTk> token;
            std::vector<char32_t>                        buffer;
        };

        size_t                                          _error_count;
        std::filesystem::path                           _src_path;
        std::unique_ptr<std::vector<char32_t>>          _output;

        ImageCache                                      _image_cache;
        std::stack<InsertionMode_e>                     _insertion_mode;
        std::deque<HtmlElement_e>                       _open_elements;
        std::stack<bool>                                _open_list_tightness;
        std::unordered_map<std::u32string, FootnoteRef> _footnotes;


        void logError( TextPos position, int err_code );

        void processContent( std::unique_ptr<token::markdown::MarkdownTk> token );
        template<InsertionMode_e mode> void processContent( std::unique_ptr<token::markdown::MarkdownTk> token );

        [[nodiscard]] InsertionMode_e currentInsertionMode() const;
        void pushInsertionMode( InsertionMode_e mode );
        void popInsertionMode();

        [[nodiscard]] HtmlElement_e peekOpenElements() const;

        void popOpenListTightnessStack();
        [[nodiscard]] bool peekOpenListTightness() const;

        void openElement( HtmlElement_e element );
        void openElement( HtmlElement_e element, const std::u32string & property );

        void closeElement( HtmlElement_e element );
        HtmlElement_e closeLastElement();
        void closeBlock( const token::markdown::BlockEndTk * token );

        void sendToOutput( const std::u32string & str );

        void initImageCache( std::unique_ptr<token::markdown::MarkdownTk> token );
        void clearImageCache();
        void appendToImageCacheBuffer( const std::u32string & str );

        static bool isListElement( HtmlElement_e el );
    };
}

#endif //BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H