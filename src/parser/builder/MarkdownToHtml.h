#ifndef BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H
#define BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H

#include <memory>
#include <filesystem>
#include <vector>

#include "../specs/markdown/InsertionMode.h"

namespace blogator::parser {
    class U32Text;
}

namespace blogator::parser::token::markdown {
    class MarkdownTk;
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

        size_t                                 _error_count;
        std::filesystem::path                  _src_path;
        std::unique_ptr<std::vector<char32_t>> _output;

        InsertionMode_e                        _original_insert_mode;
        InsertionMode_e                        _current_insert_mode;

        template<InsertionMode_e mode> void processContent( std::unique_ptr<token::markdown::MarkdownTk> token );
    };
}

#endif //BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H