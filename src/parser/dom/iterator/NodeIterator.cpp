#include "NodeIterator.h"

#include "../node/Node.h"
#include "../../../src/unicode/unicode.h"
#include "../../../logger/Logger.h"

using namespace blogator::parser::dom;

/**
 * Constructor
 * @param root Pointer to root node
 * @param reverse Reverse mode flag
 */
NodeIterator::NodeIterator( node::Node * root, bool reverse ) :
    _reverse( reverse ),
    _done( ( root == nullptr ) ),
    _root( root ),
    _curr( ( reverse ? nullptr : root ) ),
    _filter( nullptr )
{
    if( !_done && reverse ) {
        iterateForward();
    }
}

/**
 * Constructor
 * @param root Pointer to root node
 * @param curr Pointer to current node
 * @param reverse Reverse mode flag
 */
NodeIterator::NodeIterator( node::Node * root, node::Node * curr, bool reverse ) :
    _reverse( reverse ),
    _done( ( curr == nullptr ) ),
    _root( root ),
    _curr( curr ),
    _filter( nullptr )
{}

/**
 * Constructor
 * @param root Pointer to node
 * @param filter Pointer to common NodeFilter
 * @param reverse Reverse mode flag
 */
NodeIterator::NodeIterator( node::Node * root, std::shared_ptr<NodeFilter> filter, bool reverse ) :
    _reverse( reverse ),
    _done( false ),
    _root( root ),
    _curr( ( reverse ? nullptr : root ) ),
    _filter( std::move( filter ) )
{
    if( _filter && _root && _filter->acceptNode( *_root ) != NodeFilter::Result::FILTER_ACCEPT ) {
        ++(*this);
    }
}

/**
 * Constructor
 * @param root Pointer to root node
 * @param curr Pointer to current node
 * @param filter Pointer to common NodeFilter
 * @param reverse Reverse mode flag
 */
NodeIterator::NodeIterator( node::Node * root, node::Node * curr, std::shared_ptr<NodeFilter> filter, bool reverse ) :
    _reverse( reverse ),
    _done( false ),
    _root( root ),
    _curr( curr ),
    _filter( std::move( filter ) )
{
    if( _filter && _curr && _filter->acceptNode( *_curr ) != NodeFilter::Result::FILTER_ACCEPT ) {
        ++(*this);
    }
}

/**
 * Copy-constructor
 * @param it NodeIterator to copy
 */
NodeIterator::NodeIterator( const NodeIterator &it ) :
    _reverse( it._reverse ),
    _done( it._done ),
    _root( it._root ),
    _curr( it._curr ),
    _filter( it._filter )
{}

/**
 * Move-constructor
 * @param it NodeIterator to move
 */
NodeIterator::NodeIterator( NodeIterator &&it ) noexcept :
    _reverse( it._reverse ),
    _done( it._done ),
    _root( it._root ),
    _curr( it._curr ),
    _filter( std::move( it._filter ) )
{}

/**
 * Copy-operator
 * @param it NodeIterator to copy
 * @return Copy
 */
NodeIterator & NodeIterator::operator =( const NodeIterator &it ) {
    _reverse     = it._reverse;
    _done        = it._done;
    _root        = it._root;
    _curr        = it._curr;
    _filter      = it._filter;
    return *this;
}

/**
 * Move-operator
 * @param it NodeIterator to move
 * @return Moved iterator
 */
NodeIterator & NodeIterator::operator =( NodeIterator &&it ) noexcept {
    _reverse     = it._reverse;
    _done        = it._done;
    _root        = it._root;
    _curr        = it._curr;
    _filter      = std::move( it._filter );
    return *this;
}

/**
 * [FRIEND] Output stream operator
 * @param os Output stream
 * @param it Iterator
 * @return Output stream
 */
std::ostream & blogator::parser::dom::operator <<( std::ostream &os, const blogator::parser::dom::NodeIterator &it ) {
    if( it._curr ) {
        os << "[" << blogator::unicode::utf8::convert( it._curr->nodeName() ) << "]";

        if( it._curr->nodeValue() ) {
            os << ": " << blogator::unicode::utf8::convert( *it._curr->nodeValue() );
        }

    } else {
        os << ( it.done() ? ( it._reverse ? "rend()" : "end()" ) : "NULL" );
    }

    return os;
}

/**
 * Equality operator
 * @param rhs NodeIterator to compare with
 * @return Equal state
 */
bool blogator::parser::dom::NodeIterator::operator ==( const NodeIterator &rhs ) const {
    if( this->_reverse == rhs._reverse ) {
        return this->_curr == rhs._curr;
    } else {
        return this->_curr == rhs._curr && this->_curr != nullptr;
    }
}

/**
 * Not-equal operator
 * @param rhs NodeIterator to compare with
 * @return !Equal state
 */
bool blogator::parser::dom::NodeIterator::operator !=( const NodeIterator &rhs ) const {
    return !( *this == rhs );
}

/**
 * De-reference operator
 * @return Node object reference
 * @throws std::runtime_error when de-referencing a NULL iterator (i.e.: end())
 */
node::Node & NodeIterator::operator *() const {
    if( !_done && _curr ) {
        return *_curr;
    }

    throw std::runtime_error(
        "[parser::dom::NodeIterator::operator*()] "
        "Iterator is pointing to a nullptr."
    );
}

/**
 * Gets the Node pointer to by the iterator
 * @return Node pointer
 * @throws std::runtime_error when accessing a NULL iterator (i.e.: end())
 */
NodeIterator::pointer NodeIterator::operator ->() const {
    if( !_done && _curr ) {
        return _curr;
    }

    throw std::runtime_error(
        "[parser::dom::NodeIterator::operator->()] "
        "Iterator is pointing to a nullptr."
    );
}

/**
 * Prefix increment operator
 * @return Incremented iterator
 */
NodeIterator & NodeIterator::operator ++() {
    if( !_done ) {
        iterateForward();
    }

    return *this;
}

/**
 * Postfix increment operator
 * @return Iterator pre-incrementation
 */
NodeIterator NodeIterator::operator ++( int ) {
    NodeIterator tmp = *this;
    ++(*this);
    return tmp;
}

/**
 * Prefix decrement operator
 * @return Decremented iterator
 */
NodeIterator & NodeIterator::operator --() {
    iterateBackward();
    return *this;
}

/**
 * Postfix decrement operator
 * @return Iterator pre-decrementation
 */
NodeIterator NodeIterator::operator --( int ) {
    NodeIterator tmp = *this;
    --(*this);
    return std::move( tmp );
}

/**
 * Gets the what to show mask
 * @return What-to-show mask
 */
unsigned long NodeIterator::whatToShow() const {
    return ( _filter ? _filter->whatToShow() : NodeFilter::SHOW_ALL );
}

/**
 * Gets the node filter used for the iterator
 * @return Node filter pointer reference (is null when no filter is used)
 */
const std::shared_ptr<NodeFilter> & NodeIterator::nodeFilter() const {
    return _filter;
}

/**
 * Gets the pointer of the current node pointed to by the iterator
 * @return Pointer to node (or nullptr)
 */
NodeIterator::pointer NodeIterator::node() {
    return _curr;
}

/**
 * Gets the pointer of the current node pointed to by the iterator
 * @return Const pointer to node (or nullptr)
 */
NodeIterator::const_pointer NodeIterator::node() const {
    return _curr;
}

/**
 * Gets the current node and moves the position of the iterator up
 * @return Pointer to current node (or nullptr when none)
 */
NodeIterator::pointer NodeIterator::nextNode() {
    auto * tmp = _curr;
    ++(*this);
    return tmp;
}

/**
 * Gets the current node and moves the position of the iterator down
 * @return Pointer to current node (or nullptr when done/none)
 */
NodeIterator::pointer NodeIterator::previousNode() {
    auto * tmp = _curr;
    --(*this);
    return ( tmp == _curr ? nullptr : _curr );
}

/**
 * Checks if iterator has reached its end
 * @return End/Done state
 */
bool NodeIterator::done() const {
    return _done;
}

/**
 * [PRIVATE] Moves the iterator position in the tree forward
 */
inline void NodeIterator::iterateForward() {
    if( _reverse ) {
        _curr = prevNode( _curr );

        if( _filter ) {
            while( _curr && _filter->acceptNode( *_curr ) != NodeFilter::Result::FILTER_ACCEPT ) {
                _curr = prevNode( _curr );
            }
        }

    } else { //forward iterator
        _curr = nextNode( _curr );

        if( _filter ) {
            while( _curr && _filter->acceptNode( *_curr ) != NodeFilter::Result::FILTER_ACCEPT ) {
                _curr = nextNode( _curr );
            }
        }
    }

    _done = ( _curr == nullptr );
}

/**
 * [PRIVATE] Moves the iterator position in the tree backwards
 */
inline void NodeIterator::iterateBackward() {
    auto * prev = _curr;

    if( _reverse ) {
        _curr = nextNode( _curr );

        if( _filter ) {
            while( _curr && _filter->acceptNode( *_curr ) != NodeFilter::Result::FILTER_ACCEPT ) {
                _curr = nextNode( _curr );
            }
        }

    } else { //forward iterator
        _curr = prevNode( _curr );

        if( _filter ) {
            while( _curr && _filter->acceptNode( *_curr ) != NodeFilter::Result::FILTER_ACCEPT ) {
                _curr = prevNode( _curr );
            }
        }
    }

    if( _curr == nullptr ) {
        _done = true;
        _curr = prev;
    }
}

/**
 * [PRIVATE] Gets the next node in the tree pre-order
 * @param node Current node pointer
 * @return Next node pre-order (or nullptr when end is reached)
 */
inline NodeIterator::pointer NodeIterator::nextNode( NodeIterator::pointer node ) const {
    if( node == nullptr ) {
        return _root; //EARLY RETURN
    }

    if( node->hasChildNodes() ) {
        return node->firstChild(); //EARLY RETURN
    }

    if( node->nextSibling() ) {
        return node->nextSibling(); //EARLY RETURN
    }

    auto * parent = node->parentNode();

    while( parent && !parent->nextSibling() ) {
        if( parent == _root ) {
            return nullptr; //EARLY RETURN

        } else {
            parent = parent->parentNode();
        }
    }

    if( parent && parent->nextSibling() ){
        return parent->nextSibling(); //EARLY RETURN
    }

    return nullptr;
}

/**
 * [PRIVATE] Gets the previous node in the tree pre-order
 * @param node Current node pointer (or nullptr to get the last pre-order node in the tree)
 * @return Previous node pre-order (or nullptr if already at root or it has been lost)
 */
inline NodeIterator::pointer NodeIterator::prevNode( NodeIterator::pointer node ) const {
    if( node == nullptr ) {
        return lastNode();
    }

    if( node == _root ) {
        return nullptr; //EARLY RETURN
    }

    if( node->previousSibling() ) {
        auto * curr = node->previousSibling();

        while( curr && curr->hasChildNodes() ) {
            curr = curr->lastChild();
        }

        return curr; //EARLY RETURN
    }

    if( node->parentNode() ){
        return node->parentNode(); //EARLY RETURN
    }

    LOG_ERROR(
        "[parser::dom::NodeIterator::back( Node * )] "
        "Root has been lost (iterator has been orphaned)"
    );

    return nullptr;
}

/**
 * [PRIVATE] Gets the last node in the pre-ordered tree
 * @return Last pre-ordered node in tree
 */
inline NodeIterator::pointer NodeIterator::lastNode() const {
    auto * curr = _root;

    while( curr && curr->hasChildNodes() ) {
        curr = curr->lastChild();
    }

    return curr;
}