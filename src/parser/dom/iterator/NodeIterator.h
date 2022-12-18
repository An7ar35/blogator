#ifndef BLOGATOR_PARSER_DOM_ITERATOR_NODEITERATOR_H
#define BLOGATOR_PARSER_DOM_ITERATOR_NODEITERATOR_H

#include <iterator>
#include <memory>
#include <ostream>

#include "../filter/NodeFilter.h"

namespace blogator::parser::dom {
    /**
     * NodeIterator
     * ref: https://dom.spec.whatwg.org/#nodeiterator
     */
    class NodeIterator {
      public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = node::Node;
        using pointer           = node::Node *;
        using const_pointer     = const pointer;
        using reference         = node::Node &;
        using const_reference   = const node::Node &;

      public:
        NodeIterator( node::Node * root, bool reverse );
        NodeIterator( node::Node * root, node::Node * curr, bool reverse = false );
        NodeIterator( node::Node * root, std::shared_ptr<NodeFilter> filter, bool reverse = false );
        NodeIterator( node::Node * root, node::Node * curr, std::shared_ptr<NodeFilter> filter, bool reverse = false );
        NodeIterator( const NodeIterator &it );
        NodeIterator( NodeIterator &&it ) noexcept;
        ~NodeIterator() = default;

        NodeIterator & operator =( const NodeIterator &it );
        NodeIterator & operator =( NodeIterator &&it ) noexcept;

        friend std::ostream & operator <<( std::ostream &os, const NodeIterator &it );

        bool operator == ( const NodeIterator &rhs ) const;
        bool operator != ( const NodeIterator &rhs ) const;

        reference operator *() const;
        pointer operator ->() const;
        NodeIterator & operator ++();
        const NodeIterator operator ++( int );
        NodeIterator & operator --();
        const NodeIterator operator --( int );

        [[nodiscard]] unsigned long whatToShow() const;
        [[nodiscard]] const std::shared_ptr<NodeFilter> & nodeFilter() const;

        [[nodiscard]] pointer node();
        [[nodiscard]] const_pointer node() const;
        [[nodiscard]] pointer nextNode();
        [[nodiscard]] pointer previousNode();
        [[nodiscard]] bool done() const;

      private:
        bool                        _reverse;
        bool                        _done;
        pointer                     _root;
        pointer                     _curr;
        std::shared_ptr<NodeFilter> _filter;

        void iterateForward();
        void iterateBackward();
        [[nodiscard]] pointer nextNode( pointer node ) const;
        [[nodiscard]] pointer prevNode( pointer node ) const;
        [[nodiscard]] pointer lastNode() const;
    };

    std::ostream & operator <<( std::ostream &os, const NodeIterator &it );
}

#endif //BLOGATOR_PARSER_DOM_ITERATOR_NODEITERATOR_H
