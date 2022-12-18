#ifndef BLOGATOR_PARSER_DOM_ITERATOR_CONSTNODEITERATOR_H
#define BLOGATOR_PARSER_DOM_ITERATOR_CONSTNODEITERATOR_H

#include "NodeIterator.h"

namespace blogator::parser::dom {
    /**
     * ConstNodeIterator
     * (const wrapper of NodeIterator)
     */
    class ConstNodeIterator {
      public:
        using iterator_category = NodeIterator::iterator_category;
        using difference_type   = NodeIterator::difference_type;
        using value_type        = NodeIterator::value_type;
        using pointer           = NodeIterator::pointer;
        using const_pointer     = NodeIterator::const_pointer const;
        using reference         = NodeIterator::const_reference;

      public:
        ConstNodeIterator( const node::Node * root, bool reverse );
        ConstNodeIterator( const node::Node * root, const node::Node * curr, bool reverse = false );
        ConstNodeIterator( const node::Node * root, std::shared_ptr<NodeFilter> filter, bool reverse = false );
        ConstNodeIterator( const node::Node * root, const node::Node * curr, std::shared_ptr<NodeFilter> filter, bool reverse = false );

        friend std::ostream & operator <<( std::ostream &os, const ConstNodeIterator &it );

        bool operator == ( const ConstNodeIterator &rhs ) const;
        bool operator != ( const ConstNodeIterator &rhs ) const;

        reference operator *() const;
        pointer operator ->() const;
        ConstNodeIterator & operator ++();
        const ConstNodeIterator operator ++( int );
        ConstNodeIterator & operator --();
        const ConstNodeIterator operator --( int );

        [[nodiscard]] unsigned long whatToShow() const;
        [[nodiscard]] const std::shared_ptr<NodeFilter> & nodeFilter() const;

        [[nodiscard]] const_pointer node() const;
        [[nodiscard]] const_pointer nextNode();
        [[nodiscard]] const_pointer previousNode();
        [[nodiscard]] bool done() const;

      private:
        NodeIterator _it;
    };

    std::ostream & operator <<( std::ostream &os, const ConstNodeIterator &it );
}

#endif //BLOGATOR_PARSER_DOM_ITERATOR_CONSTNODEITERATOR_H