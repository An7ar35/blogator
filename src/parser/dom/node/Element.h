#ifndef BLOGATOR_PARSER_DOM_NODE_ELEMENT_H
#define BLOGATOR_PARSER_DOM_NODE_ELEMENT_H

#include "Node.h"
#include "Attr.h"
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
        Element( DOMString_t ns, DOMString_t name );
        Element( DOMString_t ns, DOMString_t prefix, DOMString_t name );
        Element( const Element &node );
        Element( Element &&node ) noexcept;

        Element & operator =( const Element &node );
        Element & operator =( Element &&node ) noexcept;

      public: /* Non-standard interface */
        [[nodiscard]] specs::html5::Element elementType() const;

        [[nodiscard]] node::Attr * createAttribute( const DOMString_t & name );
        [[nodiscard]] node::Attr * createAttribute( const DOMString_t & name, const DOMString_t & value );
        [[nodiscard]] node::Attr * createAttribute( const DOMString_t & prefix, const DOMString_t & name, const DOMString_t & value );
        [[nodiscard]] node::CDATASection * createCDATASection(  const DOMString_t & str );
        [[nodiscard]] node::Comment * createComment( const DOMString_t & comment );
        [[nodiscard]] node::Element * createElement( specs::html5::Element type );
        [[nodiscard]] node::Element * createElement( DOMString_t ns, DOMString_t name );
        [[nodiscard]] node::Text * createText( const DOMString_t & txt );

        [[nodiscard]] bool attributeExists( const DOMString_t & local_name ) const;
        [[nodiscard]] bool attributeExists( const DOMString_t & prefix, const DOMString_t & local_name ) const;

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
//        [SameObject] readonly attribute NamedNodeMap attributes;
//        sequence<DOMString> getAttributeNames();
        [[nodiscard]] const DOMString_t & getAttribute( const DOMString_t & qualified_name ) const;
        [[nodiscard]] const DOMString_t & getAttributeNS( const DOMString_t & ns, const DOMString_t & local_name ) const;
        bool setAttribute( const DOMString_t & qualified_name, const DOMString_t & value );
        bool setAttributeNS( const DOMString_t & ns, const DOMString_t & local_name, const DOMString_t & value );
        bool removeAttribute( DOMString_t qualified_name );
        bool removeAttributeNS( const DOMString_t & ns, DOMString_t local_name );
        bool toggleAttribute( const DOMString_t & qualified_name, bool force = false );
        bool hasAttribute( const DOMString_t & qualified_name ) const;
        bool hasAttribute( const DOMString_t & ns, const DOMString_t & local_name ) const;

        Attr * getAttributeNode( const DOMString_t & qualified_name );
        Attr * getAttributeNodeNS( const DOMString_t & ns, const DOMString_t & local_name );
        Attr * setAttributeNode( std::unique_ptr<Attr> attr );
        Attr * setAttributeNodeNS( std::unique_ptr<Attr> attr );
        NodePtr_t removeAttributeNode( std::unique_ptr<Attr> &node );
        NodePtr_t removeAttributeNode( const Attr * node );

//        Element? closest(DOMString selectors);
//        boolean matches(DOMString selectors);
//
//        HTMLCollection getElementsByTagName(DOMString qualifiedName);
//        HTMLCollection getElementsByTagNameNS(DOMString? namespace, DOMString localName);
//        HTMLCollection getElementsByClassName(DOMString classNames);



        [[nodiscard]] Attributes_t & attributes();
        [[nodiscard]] const Attributes_t & attributes() const;

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t textContent() const override;
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] bool isEqualNode( const Node &other ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;

      private:
        [[nodiscard]] DOMString_t qualifiedName() const;

        specs::html5::Namespace _element_ns;
        DOMString_t             _namespace;
        DOMString_t             _prefix; //TODO
        DOMString_t             _local_name;
        Attributes_t            _attributes;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_ELEMENT_H