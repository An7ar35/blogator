#ifndef BLOGATOR_PARSER_DOM_NODE_NODE_H
#define BLOGATOR_PARSER_DOM_NODE_NODE_H

#include <string>
#include <memory>
#include <list>
#include <vector>

#include "NodeType.h"

namespace blogator::parser::dom {
    class DOM;

    namespace node {
        class Node;
        class Document;
        class Element;
    }

    typedef std::list<const node::Node &>            NodeList_t;
    typedef std::vector<std::unique_ptr<node::Node>> Nodes_t;
}

namespace blogator::parser::dom::node {
    /**
     * Abstract base class for DOM nodes
     */
    class Node {
        friend class DOM;
      public:
        //Bit masks
        static const int DOCUMENT_POSITION_DISCONNECTED            { 0b0000'0001 }; // 1
        static const int DOCUMENT_POSITION_PRECEDING               { 0b0000'0010 }; // 2
        static const int DOCUMENT_POSITION_FOLLOWING               { 0b0000'0100 }; // 4
        static const int DOCUMENT_POSITION_CONTAINS                { 0b0000'1000 }; // 8
        static const int DOCUMENT_POSITION_CONTAINED_BY            { 0b0001'0000 }; //16
        static const int DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC { 0b0010'0000 }; //32

        //Constructor/Destructor
        explicit Node( NodeType type );
        Node( NodeType type, Node * parent, Node * prev_sibling );
        Node( NodeType type, Node * parent, Node * prev_sibling, Node * next_sibling );
        virtual ~Node();

        //Operators
        virtual bool operator ==( const Node &rhs ) const;
        virtual bool operator !=( const Node &rhs ) const;

        //Concrete methods
        [[nodiscard]] virtual NodeType nodeType() const final;
        [[nodiscard]] virtual bool hasParent() const final;
        [[nodiscard]] virtual bool hasSiblingPrev() const final;
        [[nodiscard]] virtual bool hasSiblingNext() const final;
        [[nodiscard]] virtual const Node & parentNode() const final;
        [[nodiscard]] virtual const Node & previousSibling() const final;
        [[nodiscard]] virtual const Node & nextSibling() const final;
        [[nodiscard]] virtual bool isSameNode( const std::unique_ptr<Node> & node ) const;
        [[nodiscard]] virtual bool hasChildNodes() const;
        [[nodiscard]] virtual bool hasValue() const;

        //Abstract methods
        [[nodiscard]] virtual std::string baseURI() const = delete;
        [[nodiscard]] virtual NodeList_t childNodes() const = delete;
        [[nodiscard]] virtual const Node * firstChild() const = delete;
        [[nodiscard]] virtual bool isConnected() const = delete;
        [[nodiscard]] virtual const Node * lastChild() const = delete;
        [[nodiscard]] virtual std::u32string nodeName() const = delete;
        virtual std::u32string & nodeValue() = delete;
        [[nodiscard]] virtual const Document & ownerDocument() const = delete;
        [[nodiscard]] virtual bool hasParentElement() const  = delete;
        [[nodiscard]] virtual const Element & parentElement() const = delete;
        [[nodiscard]] virtual bool hasTextContent() const = delete;
        [[nodiscard]] virtual std::u32string textContent() const = delete;
        virtual void setTextContent( const std::u32string & content ) = delete;
        virtual void appendChild( std::unique_ptr<Node> node ) = delete;
        [[nodiscard]] virtual std::unique_ptr<Node> cloneNode() const = delete;
        [[nodiscard]] virtual int compareDocumentPosition( Node & node ) const = delete;
        [[nodiscard]] virtual bool contains( Node & node ) const = delete;
        [[nodiscard]] virtual Node & getRootNode( bool composed ) = delete;
        [[nodiscard]] virtual Node & insertBefore( std::unique_ptr<Node> new_node, Node & ref_node ) = delete;
        [[nodiscard]] virtual bool isDefaultNamespace() const = delete;
        [[nodiscard]] virtual bool isEqualNode( const std::unique_ptr<Node> & node ) const = delete;
        [[nodiscard]] virtual std::pair<std::u32string, bool> lookupPrefix( const std::u32string & prefix ) = delete;
        [[nodiscard]] virtual std::pair<std::u32string, bool> lookupNamespaceURI( const std::u32string & prefix ) = delete;
        virtual void normalize() = delete;
        virtual void removeChild( Node & node ) = delete;
        [[nodiscard]] virtual std::unique_ptr<Node> replaceChild( Node & new_child, Node & old_child ) = delete;

      private:
        Node *   _parent;
        Node *   _prev_sibling;
        Node *   _next_sibling;
        NodeType _type;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_NODE_H