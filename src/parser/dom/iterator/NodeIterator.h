#ifndef BLOGATOR_PARSER_DOM_ITERATOR_NODEITERATOR_H
#define BLOGATOR_PARSER_DOM_ITERATOR_NODEITERATOR_H

#include <stack>
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
        using reference         = node::Node &;

      public:
        NodeIterator( node::Node * root, bool reverse );
        NodeIterator( node::Node * root, node::Node * curr, bool reverse = false );
        NodeIterator( node::Node * root, std::shared_ptr<NodeFilter> filter, bool reverse = false );
        NodeIterator( node::Node * root, node::Node * curr, std::shared_ptr<NodeFilter> filter, bool reverse = false );
        NodeIterator( const NodeIterator &it );
        NodeIterator( NodeIterator &&it ) noexcept;

        NodeIterator & operator =( const NodeIterator &it );
        NodeIterator & operator =( NodeIterator &&it ) noexcept;

        friend std::ostream & operator <<( std::ostream &os, const NodeIterator &it );

        bool operator == ( const NodeIterator &rhs ) const;
        bool operator != ( const NodeIterator &rhs ) const;

        reference operator *() const;
        pointer operator ->();
        NodeIterator & operator ++();
        NodeIterator operator ++( int );
        NodeIterator & operator --();
        NodeIterator operator --( int );

        [[nodiscard]] unsigned long whatToShow() const;
        [[nodiscard]] const std::shared_ptr<NodeFilter> & nodeFilter() const;

        [[nodiscard]] node::Node * node();
        [[nodiscard]] node::Node * nextNode();
        [[nodiscard]] node::Node * previousNode();
        [[nodiscard]] bool done() const;

      private:
        bool                        _reverse;
        bool                        _done;
        node::Node *                _root;
        node::Node *                _curr;
        std::shared_ptr<NodeFilter> _filter;

        void iterateForward();
        void iterateBackward();
        [[nodiscard]] node::Node * nextNode( node::Node * node ) const;
        [[nodiscard]] node::Node * prevNode( node::Node * node ) const;
        [[nodiscard]] node::Node * lastNode() const;
    };

    std::ostream & operator <<( std::ostream &os, const NodeIterator &it );
}

#endif //BLOGATOR_PARSER_DOM_ITERATOR_NODEITERATOR_H
