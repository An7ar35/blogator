#ifndef BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMEDNODEMAP_H
#define BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMEDNODEMAP_H

#include <cstddef>
#include <map>

#include "../defs.h"
#include "../enum/ValidationState.h"

namespace blogator::parser::dom {
    /**
     * NamedNodeMap
     * ref: https://dom.spec.whatwg.org/#namednodemap
     */
    class NamedNodeMap {
        friend node::Element;

      public:
        typedef std::map<DOMString_t, node::Attr *>::const_iterator const_iterator;

        explicit NamedNodeMap( node::Node * parent );
        NamedNodeMap( const NamedNodeMap &nnp );
        NamedNodeMap( NamedNodeMap &&nnp ) noexcept;

        NamedNodeMap & operator =( const NamedNodeMap &nnp );
        NamedNodeMap & operator =( NamedNodeMap &&nnp ) noexcept;

        bool operator ==( const NamedNodeMap &rhs ) const;
        bool operator !=( const NamedNodeMap &rhs ) const;

        [[nodiscard]] const_iterator cbegin() const;
        [[nodiscard]] const_iterator cend() const;

        void swap( NamedNodeMap &rhs );

        [[nodiscard]] const Attributes_t & list() const;
        [[nodiscard]] bool attributeExists( DOMString_t qualified_name ) const;
        [[nodiscard]] bool attributeExistsNS( const DOMString_t &ns, const DOMString_t &local_name ) const;
        [[nodiscard]] bool empty() const;
        [[nodiscard]] size_t length() const;
        [[nodiscard]] const node::Attr * item( size_t index ) const;
        [[nodiscard]] const node::Attr * getNamedItem( DOMString_t qualified_name ) const;
        [[nodiscard]] const node::Attr * getNamedItemNS( const DOMString_t & ns, const DOMString_t & local_name ) const;

        const node::Attr * setNamedItem( const node::Attr & attr, ValidationState validation = ValidationState::ON );
        const node::Attr * setNamedItemNS( const node::Attr & attr, ValidationState validation = ValidationState::ON );
        const node::Attr * setNode( AttrPtr_t attr );
        AttrPtr_t removeItem( size_t index );
        AttrPtr_t removeNode( const node::Attr * node );
        AttrPtr_t removeNamedItem( DOMString_t qualified_name );
        AttrPtr_t removeNamedItemNS( const DOMString_t & ns, const DOMString_t & local_name );

        [[nodiscard]] const node::Node * ownerNode() const;
        [[nodiscard]] const node::Element * ownerElement() const;

      private:
        node::Node *                        _parent;
        std::map<DOMString_t, node::Attr *> _map;
        Attributes_t                        _nodes;

        void setOwnerDocument( node::Document * document );
        void setParent( node::Node * parent );
        [[nodiscard]] Attributes_t::iterator getIterator( const node::Attr * ptr );
        [[nodiscard]] Attributes_t::iterator getIteratorNS( const DOMString_t & ns, const DOMString_t & local_name );
        node::Attr * appendAttribute( AttrPtr_t &&attr );
    };

    void swap( NamedNodeMap &lhs, NamedNodeMap &rhs );
}

#endif //BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMEDNODEMAP_H
