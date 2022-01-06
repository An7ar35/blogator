#ifndef BLOGATOR_PARSER_DOM_NODE_TEXT_H
#define BLOGATOR_PARSER_DOM_NODE_TEXT_H

#include "CharacterData.h"
#include "../../../exception/failed_expectation.h"

namespace blogator::parser::dom::node {
    /**
     * 'Text' DOM node
     * ref: https://dom.spec.whatwg.org/#interface-text
     */
    class Text : public node::CharacterData {
      public:
        explicit Text( DOMString_t str );
        Text( DOMString_t str, node::Node * parent, node::Node * prev_sibling );
        Text( DOMString_t str, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );
        Text( NodeType type, DOMString_t str );
        Text( NodeType type, DOMString_t str, node::Node * parent, node::Node * prev_sibling );
        Text( NodeType type, DOMString_t str, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;

      public: /* 'Text' interface */
        virtual node::Text * splitText( size_t offset );
        [[nodiscard]] virtual DOMString_t wholeText() const;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_TEXT_H