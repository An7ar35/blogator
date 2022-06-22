#include "U32Text.h"

#include "../../unicode/ascii.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param text Source text
 */
U32Text::U32Text( std::u32string text ) :
    _src( text.begin(), text.end() ),
    _position( { 1, 1 } ),
    _newline( ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) ),
    _reconsume( false )
{
    _iterator = _src.begin();
}

/**
 * Constructor
 * @param text Source text
 */
U32Text::U32Text( std::vector<char32_t> text ) :
    _src( std::move( text ) ),
    _position( { 1, 1 } ),
    _newline( ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) ),
    _reconsume( false )
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
    _position( { 1, 1 } ),
    _newline( ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) ),
    _reconsume( false )
{
    _iterator = _src.begin();
}

/**
 * Constructor
 * @param src_path Source file path
 * @param text Source text to reference
 */
U32Text::U32Text( std::filesystem::path src_path, std::vector<char32_t> text ) :
    _path( std::move( src_path ) ),
    _src( std::move( text ) ),
    _position( { 1, 1 } ),
    _newline( ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) ),
    _reconsume( false )
{
    _iterator = _src.begin();
}


/**
 * Advance caret and position tracker (it += n, pos.col += n)
 * @param n Number of characters to advance by
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
 * Fast-forward caret and position tracker (ignores reconsume flag)
 * @param n Number of characters to advance the caret by
 * @return Number of code points skipped
 */
size_t U32Text::advanceCaret( unsigned int n ) {
    unsigned int count = 0;

    if( !reachedEnd() && n > 0 ) {
        while( !reachedEnd() && count < n ) {
            ++_iterator;

            if( _newline ) {
                _line_sizes.emplace_back( _position.col );
                _position.newline();
                _newline = false;
            } else {
                ++_position.col;
            }

            _newline = unicode::ascii::isnewline( character() );

            ++count;
        }
    }

    return count;
}

/**
 * Fast-rewind caret and position tracker (ignores reconsume flag)
 * @param n Number of characters to rewind the caret by
 * @return Number of codepoints skipped
 */
size_t U32Text::reverseCaret( unsigned int n ) {
    unsigned int count = 0;

    if( _iterator != _src.begin() ) {
        while( _iterator != _src.begin() && count < n ) {
            --_iterator;

            if( unicode::ascii::isnewline( character() ) ) {
                _newline        = true;
                _position.line -= 1;
                _position.col   = _line_sizes.back();
                _line_sizes.pop_back();

            } else {
                _newline       = false;
                _position.col -= 1;
            }

            ++count;
        }
    }

    return count;
}

/**
 * Advances the caret and position tracker and gets the code point under the latter
 * @return Code point or 0x00 if end was reached
 */
char32_t U32Text::nextChar() {
    if( _reconsume ) {
        _reconsume = false;
    } else if( !reachedEnd() ) {
        ++_iterator;

        if( _newline ) {
            _line_sizes.emplace_back( _position.col );
            _position.newline();
            _newline = false;
        } else {
            ++_position.col;
        }

        _newline = unicode::ascii::isnewline( character() );
    }

    return character();
}

/**
 * Sets the reconsume flag which holds the caret in its current position during the next call to `nextChar()`
 */
void U32Text::reconsume() {
    _reconsume = true;
}


/**
 * Gets the current code point as pointed to by the iterator
 * @return Code point or 0x00 if end was reached
 */
char32_t U32Text::character() const {
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
std::pair<char32_t, bool> U32Text::character( std::u32string::iterator::difference_type fwd_n ) {
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
 * Resets the iterator and position back to the beginning
 */
void U32Text::reset() {
    _iterator = _src.cbegin();
    _position = TextPos();
}