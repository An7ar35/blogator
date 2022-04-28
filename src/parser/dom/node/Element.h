#ifndef BLOGATOR_PARSER_DOM_NODE_ELEMENT_H
#define BLOGATOR_PARSER_DOM_NODE_ELEMENT_H

#include <set>

#include "Node.h"
#include "Attr.h"
#include "../datastructure/NamedNodeMap.h"
#include "../enum/ValidationState.h"
#include "../../specs/infra/Namespace.h"
#include "../../specs/infra/Element.h"

namespace blogator::parser::dom::node {
    class CDATASection;
    class Comment;
    class Text;

    /**
     * 'Element' DOM node
     * ref: https://dom.spec.whatwg.org/#element
     */
    class Element : public node::Node {
      public:
        explicit Element( DOMString_t local_name );
        explicit Element( specs::infra::Element element );
        Element( const DOMString_t & ns, DOMString_t name );
        Element( Document * document, const DOMString_t & ns, DOMString_t name );
        Element( NamespaceMap::id_t ns_id, DOMString_t name );
        Element( Document * document, NamespaceMap::id_t ns_id, DOMString_t name );
        Element( const DOMString_t & ns, DOMString_t prefix, DOMString_t name );
        Element( Document * document, const DOMString_t & ns, DOMString_t prefix, DOMString_t name );
        Element( NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t name );
        Element( Document * document, NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t name );

        Element( const Element &node );
        Element( Element &&node ) noexcept;

        Element & operator =( const Element &node );
        Element & operator =( Element &&node ) noexcept;

        void swap( Node &rhs ) override;
        void swap( Element &rhs );
        [[nodiscard]] specs::infra::Namespace getNamespaceEnum() const override;

      public: /* Non-standard interface */
        specs::infra::Element elementType();
        [[nodiscard]] DOMString_t qualifiedName() const;
        [[nodiscard]] NamespaceMap::id_t namespaceID() const;
        [[nodiscard]] bool isHtmlNative() const;

        node::Attr * createAttribute( DOMString_t local_name, ValidationState validation = ValidationState::ON );
        node::Attr * createAttribute( DOMString_t local_name, const DOMString_t & value, ValidationState validation = ValidationState::ON );
        node::Attr * createAttributeNS( const DOMString_t & ns, const DOMString_t & qualified_name, const DOMString_t & value, ValidationState validation = ValidationState::ON );
        node::CDATASection * createCDATASection(  const DOMString_t & str );
        node::Comment * createComment( const DOMString_t & comment );
        node::Element * createElement( specs::infra::Element type );
        node::Element * createElement( DOMString_t local_name );
        node::Element * createElementNS( const DOMString_t & ns, const DOMString_t & qualified_name );
        node::Text * createText( const DOMString_t & txt );

      public: /* 'Element' interface */
        [[nodiscard]] DOMString_t namespaceURI() const;
        [[nodiscard]] DOMString_t prefix() const;
        [[nodiscard]] DOMString_t localName() const;
        [[nodiscard]] DOMString_t tagName() const;

        [[nodiscard]] DOMString_t id() const;
        [[nodiscard]] DOMString_t className() const;
        [[nodiscard]] DOMTokenList_t classList() const;
        [[nodiscard]] DOMString_t slot() const;

        [[nodiscard]] bool hasAttributes() const;
        [[nodiscard]] NamedNodeMap & attributes();
        [[nodiscard]] const NamedNodeMap & attributes() const;
        [[nodiscard]] Sequence_t<DOMString_t> getAttributeNames();
        [[nodiscard]] const DOMString_t * getAttribute( const DOMString_t & qualified_name ) const;
        [[nodiscard]] const DOMString_t * getAttributeNS( const DOMString_t & ns, const DOMString_t & local_name ) const;
        bool setAttribute( const DOMString_t & qualified_name, ValidationState validation = ValidationState::ON );
        bool setAttribute( const DOMString_t & qualified_name, const DOMString_t & value, ValidationState validation = ValidationState::ON );
        bool setAttributeNS( const DOMString_t & ns, const DOMString_t & qualified_name, const DOMString_t & value, ValidationState validation = ValidationState::ON );
        bool removeAttribute( const DOMString_t & qualified_name );
        bool removeAttributeNS( const DOMString_t & ns, const DOMString_t & local_name );
        bool toggleAttribute( DOMString_t qualified_name );
        bool toggleAttribute( DOMString_t qualified_name, bool force );
        [[nodiscard]] bool hasAttribute( const DOMString_t & qualified_name ) const;
        [[nodiscard]] bool hasAttributeNS( const DOMString_t & ns, const DOMString_t & local_name ) const;

        const Attr * getAttributeNode( const DOMString_t & qualified_name );
        const Attr * getAttributeNodeNS( const DOMString_t & ns, const DOMString_t & local_name );
        const Attr * setAttributeNode( const node::Attr & attr );
        const Attr * setAttributeNodeNS( const node::Attr & attr );
        NodePtr_t removeAttributeNode( const Attr * node );

        [[nodiscard]] HTMLCollection_t getElementsByTagName( DOMString_t qualified_name );
        [[nodiscard]] HTMLCollection_t getElementsByTagNameNS( const DOMString_t & ns, const DOMString_t & local_name );
        [[nodiscard]] HTMLCollection_t getElementsByClassName( std::set<DOMString_t> class_names );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t textContent() const override;
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] bool isEqualNode( const Node &other ) const override;
        [[nodiscard]] DOMString_t lookupPrefix( const DOMString_t &ns ) const override;
        [[nodiscard]] DOMString_t lookupNamespaceURI( const DOMString_t &prefix ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;
        void setOwnerDocument( Document * document ) override;
        void toUTF8Stream( std::ostream &os ) const override;

      private:
        NamespaceMap::id_t    _namespace_id;
        specs::infra::Element _element_id;
        DOMString_t           _prefix;
        DOMString_t           _local_name;
        NamedNodeMap          _attributes;
    };

    void swap( Element &lhs, Element &rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_ELEMENT_H