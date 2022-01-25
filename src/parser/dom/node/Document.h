#ifndef BLOGATOR_PARSER_DOM_NODE_DOCUMENT_H
#define BLOGATOR_PARSER_DOM_NODE_DOCUMENT_H

#include <filesystem>

#include "Node.h"
#include "../filter/NodeFilter.h"
#include "../enum/QuirksMode.h"

namespace blogator::parser::dom {
    class DOMImplementation;
    class NodeIterator;
    class ConstNodeIterator;
    class TreeWalker;

    namespace node {
        class CDATASection;
        class Comment;
        class DocumentType;
        class Element;
        class Text;
    }
}

namespace blogator::parser::dom::node {
    /**
     * 'Document' DOM node
     * ref: https://dom.spec.whatwg.org/#document
     */
    class Document : public Node {
      public:
        enum class Type { XML = 0, HTML };

        Document();
        Document( USVString_t encoding, specs::html5::ContentType content_type, USVString_t url, std::filesystem::path origin, Type type, QuirksMode quirks = QuirksMode::NO_QUIRKS );
        Document( const Document &document );
        Document( Document &&document ) noexcept;

        Document & operator =( const Document &document );
        Document & operator =( Document &&document ) noexcept;

      public: /* 'Document' interface */
        [[nodiscard]] USVString_t URL() const;
        [[nodiscard]] USVString_t documentURI() const;
        [[nodiscard]] USVString_t compatMode() const;
        [[nodiscard]] USVString_t characterSet() const;
        [[nodiscard]] USVString_t contentType() const;

        [[nodiscard]] const DocumentType * doctype() const;
        [[nodiscard]] const Element * documentElement() const;
        [[nodiscard]] HTMLCollection_t getElementsByTagName( const DOMString_t &qualified_name );
        [[nodiscard]] HTMLCollection_t getElementsByTagNameNS( const DOMString_t &ns, const DOMString_t &local_name );
        [[nodiscard]] HTMLCollection_t getElementsByClassName( const DOMString_t &class_name );

        [[nodiscard]] std::unique_ptr<Element> createElement( DOMString_t local_name );
        [[nodiscard]] std::unique_ptr<Element> createElement( specs::html5::Element element_type );
        [[nodiscard]] static std::unique_ptr<Element> createElementNS( DOMString_t ns, DOMString_t prefix, DOMString_t local_name );
        [[nodiscard]] static std::unique_ptr<Text> createTextNode( DOMString_t data );
        [[nodiscard]] static std::unique_ptr<CDATASection> createCDATASection( DOMString_t data );
        [[nodiscard]] static std::unique_ptr<Comment> createComment( DOMString_t data );

        [[nodiscard]] static NodePtr_t importNode( const Node * node, bool deep = false );
        [[nodiscard]] Node * adoptNode( NodePtr_t & node );

        [[nodiscard]] std::unique_ptr<Attr> createAttribute( DOMString_t local_name );
        [[nodiscard]] std::unique_ptr<Attr> createAttributeNS( DOMString_t ns, DOMString_t qualified_name );

//        NodeIterator createNodeIterator( node::Node * root, unsigned long what_to_show = NodeFilter::SHOW_ALL );
//        NodeIterator createNodeIterator( node::Node * root, NodeFilter filter );
//        TreeWalker createTreeWalker( node::Node * root, unsigned long what_to_show = NodeFilter::SHOW_ALL );
//        TreeWalker createTreeWalker( node::Node * root, NodeFilter filter );

      public: /* Non-standard 'Document' interface */
        [[nodiscard]] Type type() const;
        [[nodiscard]] const std::filesystem::path & filepath() const;
        void setQuirks( QuirksMode mode );

      public: /* 'Node' interface override */
        Node * insertBefore( NodePtr_t &node, Node * child ) override;
        Node * insertBefore( NodePtr_t &&node, Node * child ) override;
        Node * appendChild( NodePtr_t &node_ptr ) override;
        Node * appendChild( NodePtr_t &&node_ptr ) override;
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        NodePtr_t replaceChild( NodePtr_t &node, NodePtr_t &child ) override;
        NodePtr_t removeChild( NodePtr_t &child ) override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;

      private:
        DOMString_t               _encoding;
        specs::html5::ContentType _content_type;
        DOMString_t               _url;
        std::filesystem::path     _origin;
        Type                      _type;
        QuirksMode                _quirks;
        DocumentType *            _doctype; //shortcut to 'DocumentType' child
        Element *                 _element; //shortcut to 'Element' child


        std::unordered_map<DOMString_t, NodeList_t>           _by_name;
        std::unordered_map<DOMString_t, NodeList_t>           _by_tag_name;
        std::unordered_map<specs::html5::Element, NodeList_t> _by_tag_enum;
        std::unordered_map<DOMString_t, NodeList_t>           _by_class_name;
        std::unordered_map<DOMString_t, node::Node *>         _by_id;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_DOCUMENT_H
