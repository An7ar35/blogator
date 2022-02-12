#ifndef BLOGATOR_PARSER_DOM_NODE_ELEMENT_H
#define BLOGATOR_PARSER_DOM_NODE_ELEMENT_H

#include <set>

#include "Node.h"
#include "Attr.h"
#include "../datastructure/NamedNodeMap.h"
#include "../../specs/html5/Namespace.h"
#include "../../specs/html5/Element.h"

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
        explicit Element( specs::html5::Element element );
        Element( const DOMString_t & ns, DOMString_t name );
        Element( NamespaceMap::id_t ns_id, DOMString_t name );
        Element( const DOMString_t & ns, DOMString_t prefix, DOMString_t name );
        Element( NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t name );
        Element( const Element &node );
        Element( Element &&node ) noexcept;

        Element & operator =( const Element &node );
        Element & operator =( Element &&node ) noexcept;

        void swap( Element &rhs );

      public: /* Non-standard interface */
        [[nodiscard]] specs::html5::Element elementType() const;
        [[nodiscard]] DOMString_t qualifiedName() const;
        [[nodiscard]] NamespaceMap::id_t namespaceID() const;
        [[nodiscard]] bool isHtmlNative() const;

        node::Attr * createAttribute( const DOMString_t & name );
        node::Attr * createAttribute( const DOMString_t & prefix, const DOMString_t & name );
        node::Attr * createAttribute( const DOMString_t & prefix, const DOMString_t & name, const DOMString_t & value );
        node::CDATASection * createCDATASection(  const DOMString_t & str );
        node::Comment * createComment( const DOMString_t & comment );
        node::Element * createElement( specs::html5::Element type );
        node::Element * createElement( DOMString_t ns, DOMString_t name );
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
        bool setAttribute( const DOMString_t & qualified_name );
        bool setAttribute( const DOMString_t & qualified_name, const DOMString_t & value );
        bool removeAttribute( const DOMString_t & qualified_name );
        bool toggleAttribute( DOMString_t qualified_name );
        bool toggleAttribute( DOMString_t qualified_name, bool force );
        [[nodiscard]] bool hasAttribute( const DOMString_t & qualified_name ) const;

        const Attr * getAttributeNode( const DOMString_t & qualified_name );
        const Attr * setAttributeNode( const node::Attr &attr );
        NodePtr_t removeAttributeNode( const Attr * node );

        [[nodiscard]] HTMLCollection_t getElementsByTagName( DOMString_t qualified_name );
        [[nodiscard]] HTMLCollection_t getElementsByClassName( std::set<DOMString_t> class_names );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t textContent() const override;
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] bool isEqualNode( const Node &other ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;

      private:
        NamespaceMap::id_t _namespace_id;
        DOMString_t        _prefix;
        DOMString_t        _local_name;
        NamedNodeMap       _attributes;
    };

    void swap( Element &lhs, Element &rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_ELEMENT_H