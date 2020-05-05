#include "OrderedPostInsertion.h"

void blogator::output::helper::OrderedPostInsertion::push( helper::InsertPositionType type,
                                                           helper::OrderedPostInsertion::VariantPosition_t value )
{
    _min_heap.push( { type, std::move( value ) } );
}

/**
 * Pushes the given element value to the heap.
 * @param value Path position
 */
void blogator::output::helper::OrderedPostInsertion::pushPath( const OrderedPostInsertion::PathPosition_t &value ) {
    _min_heap.push( { InsertPositionType::PATH, value } );
}

/**
 * Pushes the given element value to the heap.
 * @param value Table of Content position
 */
void blogator::output::helper::OrderedPostInsertion::pushToC( const OrderedPostInsertion::ToCPosition_t &value ) {
    _min_heap.push( { InsertPositionType::TOC, value } );
}

/**
 * Pushes the given element value to the heap.
 * @param value Heading ID position
 */
void blogator::output::helper::OrderedPostInsertion::pushHeading( const OrderedPostInsertion::HeadingPosition_t &value ) {
    _min_heap.push( { InsertPositionType::HEADING_ID, value } );
}

/**
 * Returns a read-only (constant) reference to the data at the first element of the data-structure
 * @return Element at the top of the heap
 */
const std::pair<blogator::output::helper::InsertPositionType, blogator::output::helper::OrderedPostInsertion::VariantPosition_t> &
    blogator::output::helper::OrderedPostInsertion::top() const
{
    return _min_heap.top();
}

/**
 * Removes the first element in the heap
 */
void blogator::output::helper::OrderedPostInsertion::pop() {
    _min_heap.pop();
}

/**
 * Gets the empty state of the heap
 * @return Empty state
 */
bool blogator::output::helper::OrderedPostInsertion::empty() const {
    return _min_heap.empty();
}

/**
 * Gets the InsertPosition of a variant node
 * @param node Variant HeapNode_t
 * @return InsertPosition
 * throw std::invalid_argument when variant node has an unknown label
 */
const blogator::dto::InsertPosition &
    blogator::output::helper::OrderedPostInsertion::getInsertPosition( const OrderedPostInsertion::HeapNode_t &node )
{
    switch( node.first ) {
        case InsertPositionType::PATH:
            return std::get<PathPosition_t>( node.second ).first;
        case InsertPositionType::TOC:
            return std::get<ToCPosition_t>( node.second );
        case InsertPositionType::HEADING_ID:
            return std::get<HeadingPosition_t>( node.second ).first;
    }
    throw std::invalid_argument(
        "[blogator::output::helper::OrderedPostInsertion::getInsertPosition( const OrderedPostInsertion::HeapNode_t & )] Variant label unknown."
    );
}
