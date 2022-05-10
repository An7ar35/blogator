#include "U32Text.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param text Source text
 */
U32Text::U32Text( std::u32string text ) :
    _src( text.begin(), text.end() ),
    _position( { 1, 1 } )
{
    _iterator = _src.begin();
}

/**
 * Constructor
 * @param text Source text
 */
U32Text::U32Text( std::vector<uint32_t> text ) :
    _src( std::move( text ) ),
    _position( { 1, 1 } )
{
    _iterator = _src.begin();
}

/**
 * Constructor
 * @param src_path Source file path
 * @param text Source text to reference
 */
U32Text::U32Text( std::filesystem::path src_path, std::u32string text ) :
    _path( std::move( src_path ) ),
    _src( text.begin(), text.end() ),
    _position( { 1, 1 } )
{
    _iterator = _src.begin();
}

/**
 * Constructor
 * @param src_path Source file path
 * @param text Source text to reference
 */
U32Text::U32Text( std::filesystem::path src_path, std::vector<uint32_t> text ) :
    _path( std::move( src_path ) ),
    _src( std::move( text ) ),
    _position( { 1, 1 } )
{
    _iterator = _src.begin();
}

/**
 * Blindly advances the column position tracker only (pos.col += n)
 * @param n Number of code points to advance by (default=1)
 */
void U32Text::advanceColTracker( unsigned int n ) {
    _position.col += n;
}

/**
 * Blindly advances the position tracker by 1 line (pos.line++, pos.col = 1)
 */
void U32Text::advanceLineTracker() {
    _position.newline();
}

/**
 * Advance caret and position tracker (it += n, pos.col += n)
 * @param n Number of characters to advanceCol by
 * @return number of code points skipped
 */
size_t U32Text::advanceCol( unsigned int n ) {
    if( !reachedEnd() ) {
        auto remaining = std::distance( _iterator, _src.cend() );
        auto advance   = ( remaining > n ? n : remaining );
        std::advance( _iterator, advance );
        _position.col += advance;
        return advance;
    }

    return 0;
}

/**
 * Advance caret and position tracker to the next line (it++, pos.line++, pos.col = 1)
 */
void U32Text::advanceLine() {
    if( !reachedEnd() ) {
        std::advance( _iterator, 1 );
        advanceLineTracker();
    }
}

/**
 * Gets the current code point as pointed to by the iterator
 * @return Code point or 0x00 if end was reached
 */
uint32_t U32Text::character() const {
    if( reachedEnd() )
        return 0x00;
    else
        return *_iterator;
}

/**
 * Gets the code point n code points away from current position
 * @param fwd_n Number of code points to skip
 * @return Code point and reached end flag
 */
std::pair<uint32_t, bool> U32Text::character( std::u32string::iterator::difference_type fwd_n ) {
    if( reachedEnd() ) {
        return { 0x00, true };
    } else if ( fwd_n > 0 ) {
        auto iterator = std::next( _iterator, fwd_n );

        if( iterator == _src.cend() )
            return { 0x00, true };
        else
            return { *iterator, false };
    } else {
        return { character(), false };
    }
}

/**
 * Gets a range of character code points starting at, and including, the current position
 * @param n Number of code points
 * @return String of size range or less if end was reached
 */
std::u32string U32Text::characters( std::u32string::iterator::difference_type n ) {
    if( !reachedEnd() ) {
        auto remaining   = std::distance( _iterator, _src.cend() );
        auto code_points = ( remaining > n ? n : remaining );
        return { _iterator, std::next( _iterator, code_points ) };
    }

    return {};
}

/**
 * Gets the current position
 * @return Position
 */
TextPos U32Text::position() const noexcept {
    return _position;
}

/**
 * Gets the file path for the associated text
 * @return File path
 */
std::filesystem::path U32Text::path() const noexcept {
    return _path;
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

/**
 * Gets an iterator to the currently held position
 * @return Iterator
 */
std::vector<uint32_t>::const_iterator U32Text::iteratorCurr() const {
    return _iterator;
}

/**
 * Gets an iterator to the end position
 * @return Iterator
 */
std::vector<uint32_t>::const_iterator U32Text::iteratorEnd() const {
    return _src.cend();
}

/**
 * Resets the iterator and position back to the beginning
 */
void U32Text::reset() {
    _iterator = _src.cbegin();
    _position = TextPos();
}