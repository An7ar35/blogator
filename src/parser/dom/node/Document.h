#ifndef BLOGATOR_PARSER_DOM_NODE_DOCUMENT_H
#define BLOGATOR_PARSER_DOM_NODE_DOCUMENT_H

#include <filesystem>
#include <set>

#include "Node.h"
#include "../enum/ValidationState.h"
#include "../filter/NodeFilter.h"
#include "../iterator/TreeWalker.h"
#include "../enum/QuirksMode.h"

namespace blogator::parser::dom {
    class NodeIterator;
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
        Document( USVString_t encoding, specs::infra::ContentType content_type, USVString_t url, std::filesystem::path origin, QuirksMode quirks = QuirksMode::NO_QUIRKS );
        Document( USVString_t encoding, specs::infra::ContentType content_type, USVString_t url, std::filesystem::path origin, Type type, QuirksMode quirks = QuirksMode::NO_QUIRKS );
        Document( const Document &document );
        Document( Document &&document ) noexcept;

        Document & operator =( const Document &document );
        Document & operator =( Document &&document ) noexcept;

        void swap( Node &rhs ) override;
        void swap( Document &rhs );

        [[nodiscard]] Type type() const;
        [[nodiscard]] const std::filesystem::path & filepath() const;
        [[nodiscard]] QuirksMode quirksMode() const;
        void setQuirks( QuirksMode mode );

        Element * createDocumentElement( DOMString_t local_name );
        Element * createDocumentElement( specs::infra::Element element_type );

      public: /* 'Document' interface */
        [[nodiscard]] USVString_t URL() const;
        [[nodiscard]] USVString_t documentURI() const;
        [[nodiscard]] USVString_t compatMode() const;
        [[nodiscard]] USVString_t characterSet() const;
        [[nodiscard]] USVString_t contentType() const;

        [[nodiscard]] const DocumentType * doctype() const;
        [[nodiscard]] Element * documentElement();
        [[nodiscard]] const Element * documentElement() const;
        [[nodiscard]] Element * getElementById( const DOMString_t &id );
        [[nodiscard]] const Element * getElementById( const DOMString_t &id ) const;
        [[nodiscard]] HTMLCollection_t getElementsByTagName( const DOMString_t &qualified_name );
        [[nodiscard]] HTMLCollection_t getElementsByTagNameNS( const DOMString_t &ns, const DOMString_t &local_name );
        [[nodiscard]] HTMLCollection_t getElementsByClassName( std::set<DOMString_t> class_names );

        [[nodiscard]] std::unique_ptr<Element> createElement( DOMString_t local_name, ValidationState validation = ValidationState::ON ) const;
        [[nodiscard]] std::unique_ptr<Element> createElement( specs::infra::Element element_type ) const;
        [[nodiscard]] std::unique_ptr<Element> createElementNS( const DOMString_t & ns, const DOMString_t & qualified_name, ValidationState validation = ValidationState::ON ) const;

        [[nodiscard]] std::unique_ptr<Text> createTextNode( DOMString_t data ) const;
        [[nodiscard]] std::unique_ptr<CDATASection> createCDATASection( DOMString_t data ) const;
        [[nodiscard]] std::unique_ptr<Comment> createComment( DOMString_t data ) const;

        [[nodiscard]] static NodePtr_t importNode( const Node * node, bool deep = false ) = delete;
        [[nodiscard]] NodePtr_t adoptNode( NodePtr_t & node )                             = delete;

        [[nodiscard]] std::unique_ptr<Attr> createAttribute( DOMString_t local_name );
        [[nodiscard]] std::unique_ptr<Attr> createAttributeNS( const DOMString_t & ns, const DOMString_t & qualified_name );

        [[nodiscard]] NodeIterator createNodeIterator( node::Node * root, unsigned long what_to_show = NodeFilter::SHOW_ALL );
        [[nodiscard]] NodeIterator createNodeIterator( node::Node * root, NodeFilter filter );
        [[nodiscard]] TreeWalker createTreeWalker( node::Node * root, unsigned long what_to_show = NodeFilter::SHOW_ALL );
        [[nodiscard]] TreeWalker createTreeWalker( node::Node * root, NodeFilter filter );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
        [[nodiscard]] DOMString_t lookupPrefix( const DOMString_t &ns ) const override;
        [[nodiscard]] DOMString_t lookupNamespaceURI( const DOMString_t &prefix ) const override;
      protected:
        Node * insertNodeBefore( NodePtr_t node, node::Node * child ) override;
        NodePtr_t replaceChildNode( NodePtr_t &node, NodePtr_t &target ) override;
        NodePtr_t removeChildNode( Nodes_t::iterator it ) override;

      private:
        DOMString_t               _encoding;
        specs::infra::ContentType _content_type;
        DOMString_t               _url;
        std::filesystem::path     _origin;
        Type                      _type;
        QuirksMode                _quirks;
        DocumentType *            _doctype; //shortcut to 'DocumentType' child
        Element *                 _element; //shortcut to 'Element' child
    };

    std::ostream & operator <<( std::ostream &os, Document::Type &type );
    void swap( Document &lhs, Document &rhs );
}

#endif //BLOGATOR_PARSER_DOM_NODE_DOCUMENT_H