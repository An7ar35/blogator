#ifndef BLOGATOR_PARSER_DOM_NODE_NODE_H
#define BLOGATOR_PARSER_DOM_NODE_NODE_H

#include <memory>

#include "../defs.h"
#include "../datastructure/NamespaceMap.h"
#include "../enum/NodeType.h"
#include "../iterator/NodeIterator.h"
#include "../iterator/ConstNodeIterator.h"
#include "../exception/exception.h"
#include "../../specs/infra/specifications.h"

namespace blogator::parser::dom::node {
    class Document;

    /**
     * Base class for DOM nodes
     */
    class Node {
        friend class DOM;

      public:
        typedef NodeIterator      iterator;
        typedef ConstNodeIterator const_iterator;

        Node();
        explicit Node( NodeType type );
        Node( Document * document, NodeType type );
        Node( NodeType type, Node * parent, Node * prev_sibling );
        Node( NodeType type, Node * parent, Node * prev_sibling, Node * next_sibling );
        Node( const Node &node );
        Node( Node &&node ) noexcept;
        virtual ~Node();

        Node & operator =( const Node &node );
        Node & operator =( Node &&node ) noexcept;

        friend std::ostream & operator <<( std::ostream &os, const Node &node );

        bool operator ==( const Node &rhs ) const;
        bool operator !=( const Node &rhs ) const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] iterator begin( NodeFilter filter );
        [[nodiscard]] iterator begin( std::shared_ptr<NodeFilter> filter );
        [[nodiscard]] iterator end();

        [[nodiscard]] const_iterator cbegin() const;
        [[nodiscard]] const_iterator cbegin( NodeFilter filter ) const;
        [[nodiscard]] const_iterator cbegin( std::shared_ptr<NodeFilter> filter ) const;
        [[nodiscard]] const_iterator cend() const;

        [[nodiscard]] iterator rbegin();
        [[nodiscard]] iterator rbegin( NodeFilter filter );
        [[nodiscard]] iterator rbegin( std::shared_ptr<NodeFilter> filter );
        [[nodiscard]] iterator rend();

        [[nodiscard]] const_iterator crbegin() const;
        [[nodiscard]] const_iterator crbegin( NodeFilter filter ) const;
        [[nodiscard]] const_iterator crbegin( std::shared_ptr<NodeFilter> filter ) const;
        [[nodiscard]] const_iterator crend() const;

        virtual size_t depth() const;
        virtual void swap( Node &rhs );
        virtual NodePtr_t detach();
        [[nodiscard]] virtual specs::infra::Namespace getNamespaceEnum() const;

        static specs::infra::Namespace getNamespaceEnum( NamespaceMap::id_t id );
        static const DOMString_t & getNamespaceURI( NamespaceMap::id_t id );
        static NamespaceMap & namespaceMap();

      public: /* 'Node' interface */
        [[nodiscard]] virtual NodeType nodeType() const;
        [[nodiscard]] virtual DOMString_t nodeName() const;

        [[nodiscard]] virtual USVString_t baseURI() const;

        [[nodiscard]] virtual bool isConnected() const;
        [[nodiscard]] virtual Document * ownerDocument();
        [[nodiscard]] virtual const Document * ownerDocument() const;
        [[nodiscard]] virtual Node * getRootNode();
        [[nodiscard]] virtual Node * parentNode();
        [[nodiscard]] virtual const Node * parentNode() const;
        [[nodiscard]] virtual Element * parentElement();
        [[nodiscard]] virtual const Element * parentElement() const;
        [[nodiscard]] virtual bool hasChildNodes() const;
        [[nodiscard]] virtual Nodes_t & childNodes();
        [[nodiscard]] virtual const Nodes_t & childNodes() const;
        [[nodiscard]] virtual Node * firstChild();
        [[nodiscard]] virtual const Node * firstChild() const;
        [[nodiscard]] virtual Node * lastChild();
        [[nodiscard]] virtual const Node * lastChild() const;
        [[nodiscard]] virtual Node * previousSibling();
        [[nodiscard]] virtual const Node * previousSibling() const;
        [[nodiscard]] virtual Node * nextSibling();
        [[nodiscard]] virtual const Node * nextSibling() const;

        [[nodiscard]] virtual DOMString_t * nodeValue();
        [[nodiscard]] virtual DOMString_t textContent() const;
        [[nodiscard]] virtual size_t length() const;
        virtual void normalize();

        [[nodiscard]] virtual NodePtr_t cloneNode( bool deep ) const;
        [[nodiscard]] bool isEqualNode( const NodePtr_t &other ) const;
        [[nodiscard]] virtual bool isEqualNode( const Node &other ) const;
        [[nodiscard]] bool isSameNode( const NodePtr_t &other ) const;
        [[nodiscard]] bool isSameNode( const Node &other ) const;

        static constexpr unsigned short DOCUMENT_POSITION_DISCONNECTED            = 0x01; //0b00'0001 (1)
        static constexpr unsigned short DOCUMENT_POSITION_PRECEDING               = 0x02; //0b00'0010 (2)
        static constexpr unsigned short DOCUMENT_POSITION_FOLLOWING               = 0x04; //0b00'0100 (4)
        static constexpr unsigned short DOCUMENT_POSITION_CONTAINS                = 0x08; //0b00'1000 (8)
        static constexpr unsigned short DOCUMENT_POSITION_CONTAINED_BY            = 0x10; //0b01'0000 (16)
        static constexpr unsigned short DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = 0x20; //0b10'0000 (32)
        [[nodiscard]] virtual unsigned short compareDocumentPosition( const Node &other );
        [[nodiscard]] bool contains( const Node &other ) const;

        [[nodiscard]] virtual DOMString_t lookupPrefix( const DOMString_t &ns ) const;
        [[nodiscard]] virtual DOMString_t lookupNamespaceURI( const DOMString_t &prefix ) const;
        [[nodiscard]] bool isDefaultNamespace( const DOMString_t &ns ) const;

        virtual Node * insertBefore( NodePtr_t &node, Node * child );
        virtual Node * insertBefore( NodePtr_t &&node, Node * child );
        virtual Node * appendChild( NodePtr_t &node_ptr );
        virtual Node * appendChild( NodePtr_t &&node_ptr );
        [[nodiscard]] virtual NodePtr_t replaceChild( NodePtr_t &node, NodePtr_t &child );
        [[nodiscard]] virtual NodePtr_t replaceChild( NodePtr_t &node, size_t index );
        [[nodiscard]] virtual NodePtr_t removeChild( NodePtr_t &child );
        [[nodiscard]] virtual NodePtr_t removeChild( size_t index );

      protected:
        [[nodiscard]] Nodes_t::iterator getParentChildListIterator( const Node * node );
        [[nodiscard]] Nodes_t::iterator getChildListIterator( const Node * child );
        [[nodiscard]] virtual Node * insertNodeBefore( NodePtr_t node, node::Node * child );
        [[nodiscard]] virtual NodePtr_t replaceChildNode( NodePtr_t &nodes, NodePtr_t &target );
        [[nodiscard]] virtual NodePtr_t removeChildNode( Nodes_t::iterator it );
        virtual void setOwnerDocument( Document * document );
        void setParent( Node * node );
        void setPrevSibling( Node * node );
        void setNextSibling( Node * node );

        static dom::NamespaceMap namespace_map;

        NodeType   _node_type;
        Document * _document;
        Node *     _parent;
        Node *     _prev_sibling;
        Node *     _next_sibling;
        Nodes_t    _children;

      private:
        Node * appendNode( NodePtr_t &node );
        Node * appendNodes( Nodes_t &nodes );
        Node * insertNodeBefore( NodePtr_t &node, Nodes_t::iterator it );
        Node * insertNodesBefore( Nodes_t &nodes, Nodes_t::iterator it );
    };

    std::ostream & operator <<( std::ostream &os, const Node &node );
    void swap( Node & lhs, Node & rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_NODE_H