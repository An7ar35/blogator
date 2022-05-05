#ifndef BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H
#define BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H

#include "../dto/U32Text.h"
#include "../token/markdown/MarkdownTk.h"

namespace blogator::parser::builder {
    class MarkdownToHtml {
      public:
        explicit MarkdownToHtml( std::vector<uint32_t> & output );

        virtual void dispatch( std::unique_ptr<token::markdown::MarkdownTk> token );

        [[nodiscard]] size_t errors() const;

      private:
        size_t                  _error_count;
        std::vector<uint32_t> & _output;
    };
}

#endif //BLOGATOR_PARSER_BUILDER_MARKDOWNTOHTML_H