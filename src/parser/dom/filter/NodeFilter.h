#ifndef BLOGATOR_PARSER_DOM_FILTER_NODEFILTER_H
#define BLOGATOR_PARSER_DOM_FILTER_NODEFILTER_H

#include <functional>

namespace blogator::parser::dom {
    namespace node {
        class Node;
    }

    /**
     * NodeFilter
     * ref: https://dom.spec.whatwg.org/#interface-nodefilter
     */
    class NodeFilter {
      public:
        enum class Result : unsigned short {
            FILTER_ACCEPT = 1,
            FILTER_REJECT = 2,
            FILTER_SKIP   = 3
        };

        static constexpr unsigned long SHOW_ALL               = 0xFFFFFFFF;
        static constexpr unsigned long SHOW_ELEMENT           = 0x1;
        static constexpr unsigned long SHOW_ATTRIBUTE         = 0x2;
        static constexpr unsigned long SHOW_TEXT              = 0x4;
        static constexpr unsigned long SHOW_CDATA_SECTION     = 0x8;
        static constexpr unsigned long SHOW_COMMENT           = 0x80;
        static constexpr unsigned long SHOW_DOCUMENT          = 0x100;
        static constexpr unsigned long SHOW_DOCUMENT_TYPE     = 0x200;
        static constexpr unsigned long SHOW_DOCUMENT_FRAGMENT = 0x400;

      public:
        explicit NodeFilter( unsigned long what_to_show );
        NodeFilter( unsigned long what_to_show, std::function<Result( const node::Node & )> acceptNode_fn );

        [[nodiscard]] unsigned long whatToShow() const;

        [[nodiscard]] virtual Result acceptNode( const node::Node &node ) const;
        [[nodiscard]] bool validate( const node::Node &node ) const;

      private:
        const unsigned long                         _what_to_show;
        std::function<Result( const node::Node & )> _callback;
    };
}

#endif //BLOGATOR_PARSER_DOM_FILTER_NODEFILTER_H
