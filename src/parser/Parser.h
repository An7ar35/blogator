#ifndef BLOGATOR_PARSER_HTML5_TOKENISER_H
#define BLOGATOR_PARSER_HTML5_TOKENISER_H

#include <vector>
#include <memory>
#include <functional>

#include "token/Token.h"
#include "../encoding/Transcode.h"
#include "../encoding/dto/Source.h"

namespace blogator::parser {
    namespace dom::node {
        class Document;
    }

    class Parser {
      public:
        static std::unique_ptr<dom::node::Document> parse( encoding::Source &source, reporter::Context ctx = reporter::Context::UNKNOWN );

      private:
//        static bool parse( U32Text &u32text, dom::TreeBuilder &tree_builder, reporter::Context ctx = reporter::Context::UNKNOWN );
//
//        static bool parseNative( U32Text &u32text, dom::TreeBuilder &tree_builder );
//        static bool parseHTML( U32Text &u32text, dom::TreeBuilder &tree_builder );
//        static bool parseMarkdown( U32Text &u32text, dom::TreeBuilder &tree_builder );
    };
}

#endif //BLOGATOR_PARSER_HTML5_TOKENISER_H