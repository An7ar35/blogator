#ifndef BLOGATOR_PARSER_DOM_NODE_CDATASECTION_H
#define BLOGATOR_PARSER_DOM_NODE_CDATASECTION_H

#include "Text.h"

namespace blogator::parser::dom::node {
    /**
     * 'CDATASection' DOM node
     * ref: https://dom.spec.whatwg.org/#interface-cdatasection
     */
    class CDATASection : public Text {
      public:
        explicit CDATASection( DOMString_t str );
        CDATASection( DOMString_t str, node::Node * parent, node::Node * prev_sibling );
        CDATASection( DOMString_t str, node::Node * parent, node::Node * prev_sibling, node::Node * next_sibling );

      public: /* 'Node' interface override */
        [[nodiscard]] DOMString_t nodeName() const override;
        [[nodiscard]] NodePtr_t cloneNode( bool deep ) const override;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_CDATASECTION_HD