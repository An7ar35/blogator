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
        Comment( Document * document, DOMString_t str );

        void swap( Node & rhs ) override;
        void swap( Comment & rhs );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;
        void toUTF8Stream( std::ostream &os ) const override;
    };

    void swap( Comment & lhs, Comment & rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_COMMENT_H