#ifndef BLOGATOR_PARSER_DOM_NODE_NODE_H
#define BLOGATOR_PARSER_DOM_NODE_NODE_H

#include <memory>

#include "../defs.h"
#include "../enum/NodeType.h"
#include "../exception/exception.h"
#include "../../specs/html5/specifications.h"

namespace blogator::parser::dom::node {
    class Document;

    /**
     * Base class for DOM nodes
     */
    class Node {
        friend class DOM;

      public:
        Node();
        explicit Node( NodeType type );
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

      public: /* 'Node' interface */
        [[nodiscard]] virtual NodeType nodeType() const;
        [[nodiscard]] virtual DOMString_t nodeName() const;

        [[nodiscard]] virtual USVString_t baseURI() const;

        [[nodiscard]] virtual bool isConnected() const;
        [[nodiscard]] virtual Document * ownerDocument();
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

        [[nodiscard]] virtual NodePtr_t cloneNode( bool deep ) const; //TODO test realistic DOM tree
        [[nodiscard]] bool isEqualNode( const NodePtr_t &other ) const;
        [[nodiscard]] virtual bool isEqualNode( const Node &other ) const;
        [[nodiscard]] bool isSameNode( const NodePtr_t &other ) const;
        [[nodiscard]] bool isSameNode( const Node &other ) const;

        static const unsigned short DOCUMENT_POSITION_DISCONNECTED;
        static const unsigned short DOCUMENT_POSITION_PRECEDING;
        static const unsigned short DOCUMENT_POSITION_FOLLOWING;
        static const unsigned short DOCUMENT_POSITION_CONTAINS;
        static const unsigned short DOCUMENT_POSITION_CONTAINED_BY;
        static const unsigned short DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC;
        [[nodiscard]] virtual unsigned short compareDocumentPosition( const Node &other );
        [[nodiscard]] bool contains( const Node &other ) const;

        [[nodiscard]] virtual DOMString_t lookupPrefix( specs::html5::Namespace ns );
        [[nodiscard]] virtual DOMString_t lookupNamespaceURI( const DOMString_t &prefix );
        [[nodiscard]] virtual bool isDefaultNamespace( const DOMString_t &ns ) const;
        [[nodiscard]] virtual bool isDefaultNamespace( specs::html5::Namespace ns ) const;

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
        [[nodiscard]] virtual NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target );
        [[nodiscard]] virtual NodePtr_t removeChildNode( Nodes_t::iterator it );
        void setParent( Node * node );
        void setPrevSibling( Node * node );
        void setNextSibling( Node * node );

        NodeType _node_type;
        Node *   _parent;
        Node *   _prev_sibling;
        Node *   _next_sibling;
        Nodes_t  _children;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_NODE_H