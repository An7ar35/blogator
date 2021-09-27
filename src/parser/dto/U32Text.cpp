#include "U32Text.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param text Source text to reference
 */
U32Text::U32Text( const std::u32string &text ) :
    _src( text ),
    _position( { 1, 1 } ),
    _iterator( text.cbegin() )
{}

/**
 * Advances the column position only (pos.col += n)
 * @param n Number of code points to advance by (default=1)
 */
void U32Text::advanceCol( size_t n ) {
    ++_position.col;
}

/**
 * Advances the position by 1 line (pos.line++, pos.col = 1)
 */
void U32Text::advanceLine() {
    _position.col = 0;
    ++_position.line;
}

/**
 * Advance caret  (it += n, pos.col += n)
 * @param n Number of characters to skipCol by
 */
void U32Text::skipCol( size_t n ) {
    std::advance( _iterator, n );
    advanceCol( n );
}

/**
 * Advance caret to the next line (it++, pos.line++, pos.col = 1)
 */
void U32Text::skipLine() {
    std::advance( _iterator, 1 );
    advanceLine();
}

/**
 * [UNCHECKED] Gets the current code point as pointed to by the iterator
 * @return Code point
 */
uint32_t U32Text::character() const {
    return *_iterator;
}

/**
 * Gets the current position
 * @return Position
 */
TextPos U32Text::position() const noexcept {
    return _position;
}

/**
 * Check if the iterator has reached the end of the text
 * @return End of text reached
 */
bool U32Text::reachedEnd() const {
    return ( _iterator == _src.cend() );
}

/**
 * Checks if the given iterator has reached the end of the text
 * @param it Iterator to check
 * @return End of text reached by iterator
 */
bool U32Text::reachedEnd( const TextIterator_t &it ) const {
    return ( it == _src.cend() );
}
