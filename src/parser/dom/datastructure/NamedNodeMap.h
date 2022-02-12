#ifndef BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMEDNODEMAP_H
#define BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMEDNODEMAP_H

#include <cstddef>
#include <map>

#include "../defs.h"

namespace blogator::parser::dom {
    /**
     * NamedNodeMap
     * ref: https://dom.spec.whatwg.org/#namednodemap
     */
    class NamedNodeMap {
        friend node::Element;

      public:
        explicit NamedNodeMap( node::Node * parent );
        NamedNodeMap( const NamedNodeMap &nnp );
        NamedNodeMap( NamedNodeMap &&nnp ) noexcept;

        NamedNodeMap & operator =( const NamedNodeMap &nnp );
        NamedNodeMap & operator =( NamedNodeMap &&nnp ) noexcept;

        void swap( NamedNodeMap &rhs );

        [[nodiscard]] const Attributes_t & list() const;
        [[nodiscard]] bool attributeExists( DOMString_t qualified_name ) const;
        [[nodiscard]] bool empty() const;
        [[nodiscard]] size_t length() const;
        [[nodiscard]] const node::Attr * item( size_t index ) const;
        [[nodiscard]] const node::Attr * getNamedItem( DOMString_t qualified_name ) const;

        const node::Attr * setNamedItem( const node::Attr & attr );
        const node::Attr * setNode( AttrPtr_t attr );
        AttrPtr_t removeItem( size_t index );
        AttrPtr_t removeNode( const node::Attr * node );
        AttrPtr_t removeNamedItem( DOMString_t qualified_name );

        [[nodiscard]] const node::Node * ownerNode() const;
        [[nodiscard]] const node::Element * ownerElement() const;

      private:
        node::Node *                        _parent;
        std::map<DOMString_t, node::Attr *> _map;
        Attributes_t                        _nodes;

        void setOwnerDocument( node::Document * document );
        void setParent( node::Node * parent );
        [[nodiscard]] Attributes_t::iterator getIterator( const node::Attr * ptr );
        node::Attr * appendAttribute( AttrPtr_t &&attr );
    };

    void swap( NamedNodeMap &lhs, NamedNodeMap &rhs );
}

#endif //BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMEDNODEMAP_H
