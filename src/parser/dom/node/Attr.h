#ifndef BLOGATOR_PARSER_DOM_NODE_ATTRIBUTE_H
#define BLOGATOR_PARSER_DOM_NODE_ATTRIBUTE_H

#include "Node.h"

namespace blogator::parser::dom::node {
    /**
     * 'Attr' DOM node
     * ref: https://dom.spec.whatwg.org/#interface-attr
     */
    class Attr : public node::Node {
      public:
        explicit Attr( DOMString_t name );
        Attr( DOMString_t prefix, DOMString_t name );
        Attr( DOMString_t prefix, DOMString_t name, DOMString_t value );
        Attr( DOMString_t prefix, DOMString_t name, node::Node * parent, node::Node * prev_sibling );
        Attr( DOMString_t prefix, DOMString_t name, DOMString_t value, node::Node * parent, node::Node * prev_sibling );
        Attr( DOMString_t prefix, DOMString_t name, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );
        Attr( DOMString_t prefix, DOMString_t name, DOMString_t value, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );
        Attr( const node::Attr & node );
        Attr( node::Attr && node ) noexcept;

        node::Attr & operator =( const node::Attr & node );
        node::Attr & operator =( node::Attr && node ) noexcept;

      public: /* 'Attr' interface */
        //[[nodiscard]] DOMString_t namespaceURI() const;
        [[nodiscard]] const DOMString_t & prefix() const;
        [[nodiscard]] const DOMString_t & localName() const;
        [[nodiscard]] const DOMString_t & name() const;
        [[nodiscard]] const DOMString_t & value() const;
        [[nodiscard]] bool hasValue() const;

        [[nodiscard]] node::Element * ownerElement();

      public: /* 'Node' interface */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] DOMString_t * nodeValue() override;
        [[nodiscard]] DOMString_t textContent() const override;
        [[nodiscard]] size_t length() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] bool isEqualNode( const Node &other ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;

      private:
        DOMString_t _prefix;
        DOMString_t _name;
        DOMString_t _value;
        bool        _has_value;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_ATTRIBUTE_H