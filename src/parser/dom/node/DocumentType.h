#ifndef BLOGATOR_PARSER_DOM_NODE_DOCUMENTTYPE_H
#define BLOGATOR_PARSER_DOM_NODE_DOCUMENTTYPE_H

#include "Node.h"

namespace blogator::parser::dom::node {
    class DocumentType : public Node {
      public:
        DocumentType( DOMString_t name, DOMString_t public_id, DOMString_t system_id );
        DocumentType( DOMString_t name, DOMString_t public_id, DOMString_t system_id, node::Node * parent, node::Node * prev_sibling );
        DocumentType( DOMString_t name, DOMString_t public_id, DOMString_t system_id, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );
        DocumentType( const DocumentType &node );
        DocumentType( DocumentType &&node ) noexcept;

        DocumentType & operator =( const DocumentType &node );
        DocumentType & operator =( DocumentType &&node ) noexcept;

        void swap( Node &rhs ) override;
        void swap( DocumentType &rhs );

      public: /* 'DocumentType' interface */
        [[nodiscard]] const DOMString_t & name() const;
        [[nodiscard]] const DOMString_t & publicId() const;
        [[nodiscard]] const DOMString_t & systemId() const;

      public: /* 'Node' interface override */
        [[nodiscard]] size_t length() const override;
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] bool isEqualNode( const Node &other ) const override;
        [[nodiscard]] DOMString_t lookupPrefix( const DOMString_t &ns ) const override;
        [[nodiscard]] DOMString_t lookupNamespaceURI( const DOMString_t &prefix ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;
        void toUTF8Stream( std::ostream &os ) const override;

      private:
        DOMString_t _name;
        DOMString_t _public_id;
        DOMString_t _system_id;
    };

    void swap( DocumentType &lhs, DocumentType &rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_DOCUMENTTYPE_H