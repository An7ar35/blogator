#ifndef BLOGATOR_PARSER_HTML5_TOKENISER_H
#define BLOGATOR_PARSER_HTML5_TOKENISER_H

#include <vector>
#include <memory>
#include <functional>

#include "dom/TreeBuilder.h"
#include "token/Token.h"
#include "encoding/Transcode.h"
#include "dto/Source.h"

namespace blogator::parser {
    class Parser {
      public:
        static std::unique_ptr<dom::DOM> parse( Source &source, specs::Context ctx = specs::Context::UNKNOWN );

      private:
        static bool parse( U32Text &u32text, dom::TreeBuilder &tree_builder, specs::Context ctx = specs::Context::UNKNOWN );

        static bool parseBlogator( U32Text &u32text, dom::TreeBuilder &tree_builder );
        static bool parseHTML( U32Text &u32text, dom::TreeBuilder &tree_builder );
        static bool parseMarkdown( U32Text &u32text, dom::TreeBuilder &tree_builder );
    };
}

#endif //BLOGATOR_PARSER_HTML5_TOKENISER_H