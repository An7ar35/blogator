#include "U32Text.h"

#include "../../unicode/ascii.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param text Source text
 */
U32Text::U32Text( std::u32string text ) :
    _src( text.begin(), text.end() ),
    _state( { { 1, 1 }, _src.begin(), ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) } ),
    _marker( _state )
{}

/**
 * Constructor
 * @param text Source text
 */
U32Text::U32Text( std::vector<char32_t> text ) :
    _src( std::move( text ) ),
    _state( { { 1, 1 }, _src.begin(), ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) } ),
    _marker( _state )
{}

/**
 * Constructor
 * @param src_path Source file path
 * @param text Source text to reference
 */
U32Text::U32Text( std::filesystem::path src_path, std::u32string text ) :
    _path( std::move( src_path ) ),
    _src( text.begin(), text.end() ),
    _state( { { 1, 1 }, _src.begin(), ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) } ),
    _marker( _state )
{}

/**
 * Constructor
 * @param src_path Source file path
 * @param text Source text to reference
 */
U32Text::U32Text( std::filesystem::path src_path, std::vector<char32_t> text ) :
    _path( std::move( src_path ) ),
    _src( std::move( text ) ),
    _state( { { 1, 1 }, _src.begin(), ( !_src.empty() ? unicode::ascii::isnewline( _src.front() ) : false ) } ),
    _marker( _state )
{}


/**
 * Advance caret and position tracker (it += n, pos.col += n)
 * @param n Number of characters to advance by
 * @return number of code points skipped
 */
size_t U32Text::advanceCol( unsigned int n ) {
    if( !reachedEnd() ) {
        auto remaining = std::distance( _state.iterator, _src.cend() );
        auto advance   = ( remaining > n ? n : remaining );
        std::advance( _state.iterator, advance );
        _state.position.col += advance;
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
            ++_state.iterator;

            if( _state.newline ) {
                _state.line_sizes.emplace_back( _state.position.col );
                _state.position.newline();
                _state.newline = false;
            } else {
                ++_state.position.col;
            }

            _state.newline = unicode::ascii::isnewline( character() );

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

    if( _state.iterator != _src.begin() ) {
        while( _state.iterator != _src.begin() && count < n ) {
            --_state.iterator;

            if( unicode::ascii::isnewline( character() ) ) {
                _state.newline        = true;
                _state.position.line -= 1;
                _state.position.col   = _state.line_sizes.back();
                _state.line_sizes.pop_back();

            } else {
                _state.newline       = false;
                _state.position.col -= 1;
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
    if( _state.reconsume ) {
        _state.reconsume = false;
    } else if( !reachedEnd() ) {
        ++_state.iterator;

        if( _state.newline ) {
            _state.line_sizes.emplace_back( _state.position.col );
            _state.position.newline();
            _state.newline = false;
        } else {
            ++_state.position.col;
        }

        _state.newline = unicode::ascii::isnewline( character() );
    }

    return character();
}

/**
 * Sets the reconsume flag which holds the caret in its current position during the next call to `nextChar()`
 */
void U32Text::reconsume() {
    _state.reconsume = true;
}


/**
 * Gets the current code point as pointed to by the iterator
 * @return Code point or 0x00 if end was reached
 */
char32_t U32Text::character() const {
    if( reachedEnd() )
        return 0x00;
    else
        return *_state.iterator;
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
        auto iterator = std::next( _state.iterator, fwd_n );

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
        auto remaining   = std::distance( _state.iterator, _src.cend() );
        auto code_points = ( remaining > n ? n : remaining );
        return { _state.iterator, std::next( _state.iterator, code_points ) };
    }

    return {};
}

/**
 * Gets the current position
 * @return Position
 */
TextPos U32Text::position() const noexcept {
    return _state.position;
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
    return ( _state.iterator == _src.cend() );
}

/**
 * Sets a marker at the current State (position/iterator/etc)
 */
void U32Text::setMarker() {
    _marker = _state;
}

/**
 * Reloads the previously set state marker as the current state
 */
void U32Text::resetToMarker() {
    _state = _marker;
}

/**
 * Resets the iterator and position back to the beginning
 */
void U32Text::reset() {
    _state.iterator = _src.cbegin();
    _state.position = TextPos();
}

/**
 * [PRIVATE] Constructor
 * @param pos Position
 * @param it Iterator
 * @param nl Newline flag
 */
U32Text::State::State( TextPos pos, TextIterator_t it, bool nl ) :
    position( pos ),
    iterator( it ),
    newline( nl ),
    reconsume( false )
{}

/**
 * Copy-constructor
 * @param state State to copy
 */
U32Text::State::State( const U32Text::State & state ) :
    position( state.position ),
    iterator( state.iterator ),
    newline( state.newline ),
    reconsume( state.reconsume ),
    line_sizes( state.line_sizes )
{}

/**
 * Copy-operator
 * @param state State
 * @return Copied state
 */
U32Text::State & U32Text::State::operator =( const U32Text::State &state ) {
    this->position   = state.position;
    this->iterator   = state.iterator;
    this->line_sizes = state.line_sizes;
    this->newline    = state.newline;
    this->reconsume  = state.reconsume;
    return *this;
}
