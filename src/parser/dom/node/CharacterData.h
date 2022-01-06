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
        CharacterData( NodeType type, DOMString_t data, node::Node * parent, node::Node * prev_sibling );
        CharacterData( NodeType type, DOMString_t data, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );
        CharacterData( const CharacterData & node );
        CharacterData( CharacterData && node ) noexcept;

        CharacterData & operator =( const CharacterData & node );
        CharacterData & operator =( CharacterData && node ) noexcept;

      public: /* 'Node' interface override */
        [[nodiscard]] size_t length() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;

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
}

#endif //BLOGATOR_PARSER_DOM_NODE_CHARACTERDATA_H