#ifndef BLOGATOR_PARSER_DOM_NODE_CHARACTERDATA_H
#define BLOGATOR_PARSER_DOM_NODE_CHARACTERDATA_H

#include "Node.h"

namespace blogator::parser::dom::node {
    /**
     * 'CharacterData' DOM node
     * ref: https://dom.spec.whatwg.org/#interface-characterdata
     */
    class CharacterData : public node::Node {
      public:
        CharacterData( NodeType type, DOMString_t data );
        CharacterData( Document * document, NodeType type, DOMString_t data );
        CharacterData( const CharacterData & node );
        CharacterData( CharacterData && node ) noexcept;
        ~CharacterData() override = default;

        CharacterData & operator =( const CharacterData & node );
        CharacterData & operator =( CharacterData && node ) noexcept;

        void swap( Node &rhs ) override;
        void swap( CharacterData & rhs );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t textContent() const override;
        [[nodiscard]] size_t length() const override;
        [[nodiscard]] DOMString_t * nodeValue() override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] bool isEqualNode( const Node &other ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;
        void toUTF8Stream( std::ostream &os ) const override;

      public: /* 'CharacterData' interface */
        virtual DOMString_t & data();
        [[nodiscard]] virtual const DOMString_t & data() const;
        [[nodiscard]] virtual DOMString_t substringData( size_t offset, size_t length ) const;
        virtual const DOMString_t & appendData( const DOMString_t & data );
        virtual const DOMString_t & insertData( size_t offset, const DOMString_t & data );
        virtual const DOMString_t & deleteData( size_t offset, size_t count );
        virtual const DOMString_t & replaceData( size_t offset, size_t count, const DOMString_t & replacement_data );

      private:
        DOMString_t _data;
    };

    void swap( CharacterData & lhs, CharacterData & rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_CHARACTERDATA_H