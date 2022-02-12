#include "TreeWalker.h"

#include "../node/Node.h"
#include "../../../unicode/unicode.h"

using namespace blogator::parser::dom;

/**
 * Constructor
 * @param root Pointer to root of TreeWalker subtree
 */
TreeWalker::TreeWalker( node::Node * root ) :
    _root( root ),
    _curr( root )
{}

/**
 * Constructor
 * @param root Pointer to root of TreeWalker subtree
 * @param filter NodeFilter to use
 */
TreeWalker::TreeWalker( node::Node * root, const NodeFilter &filter ) :
    _root( root ),
    _curr( root ),
    _filter( std::make_shared<NodeFilter>( filter ) )
{}

/**
 * Constructor
 * @param root Pointer to root of TreeWalker subtree
 * @param filter Pointer to NodeFilter to use
 */
TreeWalker::TreeWalker( node::Node * root, std::shared_ptr<NodeFilter> filter ) :
    _root( root ),
    _curr( root ),
    _filter( std::move( filter ) )
{}

/**
 * [FRIEND] Output stream operator
 * @param os Output stream
 * @param tw TreeWalker
 * @return Output stream
 */
std::ostream & blogator::parser::dom::operator <<( std::ostream &os, const TreeWalker &tw ) {
    if( tw._curr ) {
        os << "[" << blogator::unicode::utf8::convert( tw._curr->nodeName() ) << "]";

        if( tw._curr->nodeValue() ) {
            os << ": " << blogator::unicode::utf8::convert( *tw._curr->nodeValue() );
        }

    } else {
        os << "NULL";
    }

    return os;
}

/**
 * Gets the Root node of the TreeWalker's subtree
 * @return Pointer to root node
 */
const node::Node * TreeWalker::root() const {
    return _root;
}

/**
 * Gets the what to show mask
 * @return What-to-show mask
 */
unsigned long TreeWalker::whatToShow() const {
    return ( _filter ? _filter->whatToShow() : NodeFilter::SHOW_ALL );
}

/**
 * Gets the node filter used for the TreeWalker
 * @return Node filter pointer reference (is null when no filter is used)
 */
const std::shared_ptr<NodeFilter> & TreeWalker::filter() const {
    return _filter;
}

/**
 * Gets the current node
 * @return Pointer to current node
 */
node::Node * TreeWalker::currentNode() {
    return _curr;
}

/**
 * Moves the current Node to the first visible ancestor node in the document order
 * @return Pointer to found node
 */
node::Node *TreeWalker::parentNode() {
    auto * node = _curr;

    while( node && node != _root ) {
        node = node->parentNode();

        if( node && filter( *node ) == NodeFilter::Result::FILTER_ACCEPT ) {
            _curr = node;
            return node; //EARLY RETURN
        }
    }

    return nullptr;
}

/**
 * Moves the current Node to the first visible child of the current node
 * @return Pointer to found child
 */
node::Node * TreeWalker::firstChild() {
    return traverseChildren( false );
}

/**
 * Moves the current Node to the last visible child of the current node
 * @return Pointer to found child
 */
node::Node * TreeWalker::lastChild() {
    return traverseChildren( true );
}

/**
 * Moves the current Node to its previous sibling (if any)
 * @return Pointer to found sibling
 */
node::Node * TreeWalker::previousSibling() {
    return traverseSibling( true );
}

/**
 * Moves the current Node to its next sibling (if any)
 * @return Pointer to found sibling
 */
node::Node * TreeWalker::nextSibling() {
    return traverseSibling( false );
}

/**
 * Moves the current Node to the previous visible node in the document order
 * @return Pointer to found node
 */
node::Node * TreeWalker::previousNode() {
    auto * node = _curr;

    while( node != _root ) {
        auto * sibling = node->previousSibling();

        while( sibling ) {
            node = sibling;

            auto result = filter( *node );

            while( result != NodeFilter::Result::FILTER_REJECT  && node->hasChildNodes() ) {
                node = node->lastChild();

                result = filter( *node );
            }

            if( result == NodeFilter::Result::FILTER_ACCEPT ) {
                _curr = node;
                return node; //EARLY RETURN
            }

            sibling = node->previousSibling();
        }

        if( node == _root || node->parentNode() == nullptr ) {
            return nullptr; //EARLY RETURN
        }

        node = node->parentNode();

        auto result = filter( *node );

        if( result == NodeFilter::Result::FILTER_ACCEPT ) {
            _curr = node;
            return node; //EARLY RETURN
        }
    }

    return nullptr;
}

/**
 * Moves the current Node to the next visible node in the document order
 * @return Pointer to found node
 */
node::Node * TreeWalker::nextNode() {
    auto * node   = _curr;
    auto   result = NodeFilter::Result::FILTER_ACCEPT;

    while( true ) {
        while( result != NodeFilter::Result::FILTER_REJECT && node->hasChildNodes() ) {
            node   = node->firstChild();
            result = filter( *node );

            if( result == NodeFilter::Result::FILTER_ACCEPT ) {
                _curr = node;
                return node; //EARLY RETURN
            }
        }

        node::Node * sibling = nullptr;
        node::Node * temp    = node;

        while( temp ) {
            if( temp == _root ) {
                return nullptr; //EARLY RETURN
            }

            sibling = temp->nextSibling();

            if( sibling ) {
                node = sibling;
                break;
            }

            temp = temp->parentNode();
        }

        result = filter( *node );

        if( result == NodeFilter::Result::FILTER_ACCEPT ) {
            _curr = node;
            return node; //EARLY RETURN
        }
    }
}

/**
 * [PRIVATE] Filters a Node through the set Filter
 * @param node Node to check
 * @return Result of filtering
 */
NodeFilter::Result TreeWalker::filter( node::Node &node ) {
    return ( _filter ? _filter->acceptNode( node ) : NodeFilter::Result::FILTER_ACCEPT );
}

/**
 * [PRIVATE] Traverses a child list
 * @param last Flag (false=first, true=last)
 * @return Next child node matching the filter
 */
node::Node * TreeWalker::traverseChildren( bool last ) {
    auto * node = _curr;

    node = ( last ? node->lastChild() : node->firstChild() );

    while( node ) {
        auto result = filter( *node );

        if( result == NodeFilter::Result::FILTER_ACCEPT ) {
            _curr = node;
            return node; //EARLY RETURN
        }

        if( result == NodeFilter::Result::FILTER_SKIP ) {
            auto * child = ( last ? node->lastChild() : node->firstChild() );

            if( child ) {
                node = child;
                continue;
            }
        }

        while( node ) {
            auto * sibling = ( last ? node->previousSibling() : node->nextSibling() );

            if( sibling ) {
                node = sibling;
                break;
            }

            auto * parent = node->parentNode();

            if( parent == nullptr || parent == _root || parent == _curr ) {
                return nullptr; //EARLY RETURN
            }

            node = parent;
        }
    }

    return nullptr;
}

/**
 * [PRIVATE] Traverse node siblings
 * @param previous  Flag (false=next, true=previous)
 * @return Next child node matching the filter
 */
node::Node *TreeWalker::traverseSibling( bool previous ) {
    auto * node = _curr;

    if( node == _root ) {
        return nullptr; //EARLY RETURN
    }

    while( true ) {
        auto * sibling = ( previous ? node->previousSibling() : node->nextSibling() );

        while( sibling ) {
            node = sibling;

            auto result = filter( *node );

            if( result == NodeFilter::Result::FILTER_ACCEPT ) {
                _curr = node;
                return node; //EARLY RETURN
            }

            sibling = ( previous ? node->lastChild() : node->firstChild() );

            if( result == NodeFilter::Result::FILTER_REJECT || sibling == nullptr ) {
                sibling = ( previous ? node->previousSibling() : node->nextSibling() );
            }
        }

        node = node->parentNode();

        if( node == nullptr || node == _root ) {
            return nullptr; //EARLY RETURN
        }

        auto result = filter( *node );

        if( result == NodeFilter::Result::FILTER_ACCEPT ) {
            return nullptr; //EARLY RETURN;
        }
    }
}