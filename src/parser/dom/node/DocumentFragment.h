#ifndef BLOGATOR_PARSER_DOM_NODE_DOCUMENTFRAGMENT_H
#define BLOGATOR_PARSER_DOM_NODE_DOCUMENTFRAGMENT_H

#include "Node.h"

namespace blogator::parser::dom::node {
    /**
     * 'DocumentFragment' DOM node
     * ref: https://dom.spec.whatwg.org/#documentfragment
     */
    class DocumentFragment : public Node {
      public:
        DocumentFragment();
        DocumentFragment( const DocumentFragment &frag ) = delete;
        DocumentFragment( DocumentFragment &&frag ) noexcept = default;

        DocumentFragment & operator =( const DocumentFragment &frag ) = delete;
        DocumentFragment & operator =( DocumentFragment &&frag ) noexcept = default;

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t lookupPrefix( const DOMString_t &ns ) const override;
        [[nodiscard]] DOMString_t lookupNamespaceURI( const DOMString_t &prefix ) const override;

      public: /* Non-standard 'DocumentFragment' interface */
        node::Node * host();
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_DOCUMENTFRAGMENT_H