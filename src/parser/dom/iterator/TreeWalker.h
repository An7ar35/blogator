#ifndef BLOGATOR_PARSER_DOM_ITERATOR_TREEWALKER_H
#define BLOGATOR_PARSER_DOM_ITERATOR_TREEWALKER_H

#include <memory>
#include <ostream>
#include "../filter/NodeFilter.h"

namespace blogator::parser::dom {
    namespace node {
        class Node;
    }

    /**
     * TreeWalker
     * ref: https://dom.spec.whatwg.org/#treewalker
     */
    class TreeWalker {
      public:
        explicit TreeWalker( node::Node * root );
        TreeWalker( node::Node * root, const NodeFilter &filter );
        TreeWalker( node::Node * root, std::shared_ptr<NodeFilter> filter );

        friend std::ostream & operator <<( std::ostream &os, const TreeWalker &tw );

      public:
        [[nodiscard]] const node::Node * root() const;
        [[nodiscard]] unsigned long whatToShow() const;
        [[nodiscard]] const std::shared_ptr<NodeFilter> & filter() const;
        [[nodiscard]] node::Node * currentNode();

        [[nodiscard]] node::Node * parentNode();
        [[nodiscard]] node::Node * firstChild();
        [[nodiscard]] node::Node * lastChild();
        [[nodiscard]] node::Node * previousSibling();
        [[nodiscard]] node::Node * nextSibling();
        [[nodiscard]] node::Node * previousNode();
        [[nodiscard]] node::Node * nextNode();

      private:
        node::Node *                _root;
        node::Node *                _curr;
        std::shared_ptr<NodeFilter> _filter;

        NodeFilter::Result filter( node::Node &node );
        node::Node * traverseChildren( bool last = false );
        node::Node * traverseSibling( bool previous = false );
    };

    std::ostream & operator <<( std::ostream &os, const TreeWalker &tw );
}

#endif //BLOGATOR_PARSER_DOM_ITERATOR_TREEWALKER_H