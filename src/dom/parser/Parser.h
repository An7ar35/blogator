#ifndef BLOGATOR_DOM_PARSER_PARSER_H
#define BLOGATOR_DOM_PARSER_PARSER_H

#include <deque>

#include "Tokeniser.h"
#include "../dot/DOTNode.h"
#include "../dto/GlobalMaps.h"
#include "../html5/enums/Tag.h"
#include "../dot/enums/ComplianceLevel.h"

namespace blogator::dom::parser {
    class Parser {
      public:
        Parser();
        Parser( std::initializer_list<html5::Tag> list ); //helper for unit-testing

        DOTNode * parseToken( const parser::Token &token, DOTNode *parent, dto::GlobalMaps &global_attr_map );

        void setAttributeValidation( dom::ComplianceLevel level );
        [[nodiscard]] dom::ComplianceLevel attributeValidation() const;

        [[nodiscard]] size_t treeDepth() const;

      private:
        dom::ComplianceLevel   _attributes_validation;
        std::deque<html5::Tag> _stack;
        html5::NameSpace       _curr_ns;

        DOTNode *  parseOpeningTag( const std::u32string &str, DOTNode *parent, dto::GlobalMaps &global_attr_map );
        DOTNode *  parseClosingTag( const std::u32string &str, DOTNode *curr );
        DOTNode *  parseText( const parser::Token &token, DOTNode *parent );
        bool       autoClosePrevTag( html5::Tag prev, html5::Tag next );
        bool       autoClosePrevTag( const DOTNode *prev, html5::Tag next_tag );
        html5::Tag autoOpenTag( html5::Tag prev, html5::Tag next );

        static void populateGlobalAttrMap( std::unique_ptr<DOTNode> &node, dto::GlobalMaps &global_attr_map );

        [[nodiscard]] static bool validateAttributeName( ComplianceLevel level, html5::Tag tag_type, const std::u32string &attr_name );
    };
}

#endif //BLOGATOR_DOM_PARSER_PARSER_H
