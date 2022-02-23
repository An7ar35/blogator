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
        Text( Document * document, DOMString_t str );
        Text( NodeType type, DOMString_t str );
        Text( Document * document, NodeType type, DOMString_t str );

        void swap( Node & rhs ) override;
        void swap( Text & rhs );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;

      public: /* 'Text' interface */
        virtual node::Text * splitText( size_t offset );
        [[nodiscard]] virtual DOMString_t wholeText() const;
    };

    void swap( Text & lhs, Text & rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_TEXT_H