#ifndef BLOGATOR_PARSER_DOM_NODE_COMMENT_H
#define BLOGATOR_PARSER_DOM_NODE_COMMENT_H

#include "CharacterData.h"

namespace blogator::parser::dom::node {
    /**
     * 'Comment' DOM node
     * ref: https://dom.spec.whatwg.org/#interface-comment
     */
    class Comment : public CharacterData {
      public:
        explicit Comment( DOMString_t str );
        Comment( DOMString_t str, node::Node * parent, node::Node * prev_sibling );
        Comment( DOMString_t str, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_COMMENT_H