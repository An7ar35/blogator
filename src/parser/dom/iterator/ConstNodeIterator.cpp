#include "ConstNodeIterator.h"

#include "../node/Node.h"
#include "../../../unicode/unicode.h"
#include "../../../logger/Logger.h"

using namespace blogator::parser::dom;

/**
 * Constructor
 * @param root Pointer to root node
 * @param reverse Reverse mode flag
 */
ConstNodeIterator::ConstNodeIterator( const node::Node * const root, bool reverse ) :
    _it( const_cast<NodeIterator::pointer>( root ),
         reverse )
{}

/**
 * Constructor
 * @param root Pointer to root node
 * @param curr Pointer to current node
 * @param reverse Reverse mode flag
 */
ConstNodeIterator::ConstNodeIterator( const node::Node * const root, const node::Node * const curr, bool reverse ) :
    _it( const_cast<NodeIterator::pointer>( root ),
         const_cast<NodeIterator::pointer>( curr ),
         reverse )
{}

/**
 * Constructor
 * @param root Pointer to node
 * @param filter Pointer to common NodeFilter
 * @param reverse Reverse mode flag
 */
ConstNodeIterator::ConstNodeIterator( const node::Node * const root, std::shared_ptr<NodeFilter> filter, bool reverse ) :
    _it( const_cast<NodeIterator::pointer>( root ),
         std::move( filter ),
         reverse )
{}

/**
 * Constructor
 * @param root Pointer to root node
 * @param curr Pointer to current node
 * @param filter Pointer to common NodeFilter
 * @param reverse Reverse mode flag
 */
ConstNodeIterator::ConstNodeIterator( const node::Node * const root, const node::Node * const curr, std::shared_ptr<NodeFilter> filter, bool reverse ) :
    _it( const_cast<NodeIterator::pointer>( root ),
         const_cast<NodeIterator::pointer>( curr ),
         std::move( filter ),
         reverse )
{}

/**
 * [FRIEND] Output stream operator
 * @param os Output stream
 * @param it Iterator
 * @return Output stream
 */
std::ostream & blogator::parser::dom::operator <<( std::ostream &os, const blogator::parser::dom::ConstNodeIterator &it ) {
    os << it._it;
    return os;
}

/**
 * Equality operator
 * @param rhs ConstNodeIterator to compare with
 * @return Equal state
 */
bool blogator::parser::dom::ConstNodeIterator::operator ==( const ConstNodeIterator &rhs ) const {
    return ( this->_it == rhs._it );
}

/**
 * Not-equal operator
 * @param rhs ConstNodeIterator to compare with
 * @return !Equal state
 */
bool blogator::parser::dom::ConstNodeIterator::operator !=( const ConstNodeIterator &rhs ) const {
    return !( *this == rhs );
}

/**
 * De-reference operator
 * @return Node object reference
 * @throws std::runtime_error when de-referencing a NULL iterator (i.e.: end())
 */
const node::Node & ConstNodeIterator::operator *() const {
    try {
        return _it.operator*();

    } catch( const std::runtime_error &e ) {
        throw std::runtime_error(
            "[parser::dom::ConstNodeIterator::operator*()] "
            "Iterator is pointing to a nullptr."
        );
    }
}

/**
 * Gets the Node pointer to by the iterator
 * @return Node pointer
 * @throws std::runtime_error when accessing a NULL iterator (i.e.: end())
 */
ConstNodeIterator::pointer ConstNodeIterator::operator ->() const {
    try {
        return const_cast<pointer>( _it.operator->() );

    } catch( const std::runtime_error &e ) {
        throw std::runtime_error(
            "[parser::dom::ConstNodeIterator::operator->()] "
            "Iterator is pointing to a nullptr."
        );
    }
}

/**
 * Prefix increment operator
 * @return Incremented iterator
 */
ConstNodeIterator & ConstNodeIterator::operator ++() {
    _it.operator++();
    return *this;
}

/**
 * Postfix increment operator
 * @return Iterator pre-incrementation
 */
const ConstNodeIterator ConstNodeIterator::operator ++( int ) {
    ConstNodeIterator tmp = *this;
    ++(*this);
    return tmp;
}

/**
 * Prefix decrement operator
 * @return Decremented iterator
 */
ConstNodeIterator & ConstNodeIterator::operator --() {
    _it.operator--();
    return *this;
}

/**
 * Postfix decrement operator
 * @return Iterator pre-decrementation
 */
const ConstNodeIterator ConstNodeIterator::operator --( int ) {
    ConstNodeIterator tmp = *this;
    --(*this);
    return tmp;
}

/**
 * Gets the what to show mask
 * @return What-to-show mask
 */
unsigned long ConstNodeIterator::whatToShow() const {
    return _it.whatToShow();
}

/**
 * Gets the node filter used for the iterator
 * @return Node filter pointer reference (is null when no filter is used)
 */
const std::shared_ptr<NodeFilter> & ConstNodeIterator::nodeFilter() const {
    return _it.nodeFilter();
}

/**
 * Gets the pointer of the current node pointed to by the iterator
 * @return Const pointer to node (or nullptr)
 */
ConstNodeIterator::const_pointer ConstNodeIterator::node() const {
    return _it.node();
}

/**
 * Gets the current node and moves the position of the iterator up
 * @return Pointer to current node (or nullptr when none)
 */
ConstNodeIterator::const_pointer ConstNodeIterator::nextNode() {
    return _it.nextNode();
}

/**
 * Gets the current node and moves the position of the iterator down
 * @return Pointer to current node (or nullptr when done/none)
 */
ConstNodeIterator::const_pointer ConstNodeIterator::previousNode() {
    return _it.previousNode();
}

/**
 * Checks if iterator has reached its end
 * @return End/Done state
 */
bool ConstNodeIterator::done() const {
    return _it.done();
}