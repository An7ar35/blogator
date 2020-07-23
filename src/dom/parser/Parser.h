#ifndef BLOGATOR_DOM_PARSER_PARSER_H
#define BLOGATOR_DOM_PARSER_PARSER_H

#include <deque>

#include "Tokeniser.h"
#include "../dot/DOTNode.h"
#include "../dto/GlobalMaps.h"
#include "../html5/enums/Tag.h"

namespace blogator::dom::parser {
    class Parser {
      public:
        Parser() = default;
        Parser( std::initializer_list<html5::Tag> list ); //helper for unit-testing

        DOTNode * parseToken( const parser::Token &token, DOTNode *parent, dto::GlobalMaps &global_attr_map );

        [[nodiscard]] size_t treeDepth() const;

      private:
        std::deque<html5::Tag> _stack;

        DOTNode *  parseOpeningTag( const std::u32string &str, DOTNode *parent, dto::GlobalMaps &global_attr_map );
        DOTNode *  parseClosingTag( const std::u32string &str, DOTNode *curr );
        DOTNode *  parseText( const parser::Token &token, DOTNode *parent );
        bool       autoClosePrevTag( html5::Tag prev, html5::Tag next );
        html5::Tag autoOpenTag( html5::Tag prev, html5::Tag next );

        static void populateGlobalAttrMap( std::unique_ptr<DOTNode> &node, dto::GlobalMaps &global_attr_map );
    };
}

#endif //BLOGATOR_DOM_PARSER_PARSER_H
