#ifndef BLOGATOR_PARSER_DOM_NODE_ATTRIBUTE_H
#define BLOGATOR_PARSER_DOM_NODE_ATTRIBUTE_H

#include "Node.h"
#include "../datastructure/NamespaceMap.h"

namespace blogator::parser::dom {
    class NamedNodeMap;
}

namespace blogator::parser::dom::node {
    /**
     * 'Attr' DOM node
     * ref: https://dom.spec.whatwg.org/#interface-attr
     */
    class Attr : public node::Node {
        friend Element;
        friend Document;
        friend dom::NamedNodeMap;

      public:
        explicit Attr( DOMString_t local_name );
        Attr( DOMString_t local_name, DOMString_t value );
        Attr( NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t local_name );
        Attr( const DOMString_t & ns, DOMString_t prefix, DOMString_t local_name );
        Attr( NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t name, DOMString_t value );
        Attr( const DOMString_t & ns, DOMString_t prefix, DOMString_t local_name, DOMString_t value );
        Attr( Document * document, DOMString_t local_name );
        Attr( Document * document, DOMString_t local_name, DOMString_t value );
        Attr( Document * document, const DOMString_t & ns, DOMString_t prefix, DOMString_t local_name );
        Attr( Document * document, const DOMString_t & ns, DOMString_t prefix, DOMString_t local_name, DOMString_t value );

        Attr( const node::Attr & node );
        Attr( node::Attr && node ) noexcept;

        node::Attr & operator =( const node::Attr & node );
        node::Attr & operator =( node::Attr && node ) noexcept;

        void swap( Node &rhs ) override;
        void swap( Attr &rhs );

        [[nodiscard]] bool equivalent( const Attr &rhs ) const;
        [[nodiscard]] NamespaceMap::id_t namespaceID() const;

      public: /* 'Attr' interface */
        [[nodiscard]] DOMString_t namespaceURI() const;
        [[nodiscard]] const DOMString_t & prefix() const;
        [[nodiscard]] const DOMString_t & localName() const;
        [[nodiscard]] const DOMString_t & name() const;
        [[nodiscard]] DOMString_t * value();
        [[nodiscard]] const DOMString_t * value() const;
        [[nodiscard]] bool hasValue() const;

        [[nodiscard]] node::Element * ownerElement();

      public: /* 'Node' interface */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] DOMString_t * nodeValue() override;
        [[nodiscard]] DOMString_t textContent() const override;
        [[nodiscard]] size_t length() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] bool isEqualNode( const Node &other ) const override;
        [[nodiscard]] DOMString_t lookupPrefix( const DOMString_t &ns ) const override;
        [[nodiscard]] DOMString_t lookupNamespaceURI( const DOMString_t &prefix ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;

      private:
        NamespaceMap::id_t _namespace_id;
        DOMString_t        _prefix;
        DOMString_t        _name;
        DOMString_t        _value;
        bool               _has_value;
    };

    void swap( Attr &lhs, Attr &rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_ATTRIBUTE_H