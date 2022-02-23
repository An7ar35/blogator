#ifndef BLOGATOR_PARSER_DOM_NODE_CDATASECTION_H
#define BLOGATOR_PARSER_DOM_NODE_CDATASECTION_H

#include "Text.h"

namespace blogator::parser::dom::node {
    /**
     * 'CDATASection' DOM node
     * ref: https://dom.spec.whatwg.org/#interface-cdatasection
     */
    class CDATASection : public Text {
      public:
        explicit CDATASection( DOMString_t str );
        CDATASection( Document * document, DOMString_t str );

        void swap( Node & rhs ) override;
        void swap( CDATASection & rhs );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;
    };

    void swap( CDATASection & lhs, CDATASection & rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_CDATASECTION_HD