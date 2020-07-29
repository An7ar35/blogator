#include "Tokeniser.h"

#include "../html5/special_char.h"

/**
 * Constructor
 * @param line_number  Line index
 * @param char_number  Starting character index
 * @param token_type   Type of content
 * @param text_content Token content
 */
blogator::dom::parser::Token::Token( dto::Text::LineIndex_t line_number,
                                     dto::Text::CharIndex_t char_number,
                                     TokenType              token_type,
                                     std::u32string         text_content ) :
    line_i( line_number ),
    char_i( char_number ),
    type( token_type ),
    content( std::move( text_content ) )
{}

/**
 * Equality operator
 * @param rhs Token object to compare to
 * @return result
 */
bool blogator::dom::parser::Token::operator ==( const blogator::dom::parser::Token &rhs ) const {
    return line_i  == rhs.line_i
        && char_i  == rhs.char_i
        && type    == rhs.type
        && content == rhs.content;
}

/**
 * Inequality operator
 * @param rhs Token object to compare to
 * @return result
 */
bool blogator::dom::parser::Token::operator !=( const blogator::dom::parser::Token &rhs ) const {
    return !( *this == rhs );
}

/**
 * Constructor
 * @param utf32_src Source html text
 */
blogator::dom::parser::Tokeniser::Tokeniser( const dto::Text &utf32_src ) :
    _source( utf32_src ),
    _inside_comment_flag( false )
{
    dto::Text::LineIndex_t line_index { 1 };

    for( const auto & line : _source._lines ) {
        processLine( line, line_index );
        ++line_index;
    }
}

/**
 * iterator begin() to the list of tokens
 * @return iterator
 */
blogator::dom::parser::Tokeniser::iterator blogator::dom::parser::Tokeniser::begin() {
    return _tokens.begin();
}

/**
 * iterator end() to the list of tokens
 * @return iterator
 */
blogator::dom::parser::Tokeniser::iterator blogator::dom::parser::Tokeniser::end() {
    return _tokens.end();
}

/**
 * const iterator begin() to the list of tokens
 * @return iterator
 */
blogator::dom::parser::Tokeniser::const_iterator blogator::dom::parser::Tokeniser::cbegin() const {
    return _tokens.cbegin();
}

/**
 * const iterator end() to the list of tokens
 * @return iterator
 */
blogator::dom::parser::Tokeniser::const_iterator blogator::dom::parser::Tokeniser::cend() const {
    return _tokens.cend();
}

/**
 * Returns a reference to the element at specified location pos, with bounds checking.
 * @param pos position of the Token to return
 * @return Reference to the Token at position
 * @throws std::out_of_range when pos is not within the element range of the container
 */
blogator::dom::parser::Token &blogator::dom::parser::Tokeniser::at( unsigned long pos ) {
    return _tokens.at( pos );
}

/**
 * Returns a reference to the element at specified location pos, with bounds checking.
 * @param pos position of the Token to return
 * @return Reference to the Token at position
 * @throws std::out_of_range when pos is not within the element range of the container
 */
const blogator::dom::parser::Token &blogator::dom::parser::Tokeniser::at( std::vector<Token>::size_type pos ) const {
    return _tokens.at( pos );
}

/**
 * Gets the number of currently held tokens
 * @return Token count
 */
std::vector<blogator::dom::parser::Token>::size_type blogator::dom::parser::Tokeniser::tokenCount() const {
    return _tokens.size();
}

/**
 * Process a line of text into tokens
 * @param line   Line of text from Text DTO
 * @param line_i Index of line in Text DTO
 */
void blogator::dom::parser::Tokeniser::processLine( const dto::Text::Line_t &line,
                                                    const dto::Text::LineIndex_t &line_i )
{ //TODO test this mofo hardcore-like!
    using blogator::dom::dto::Text;

    auto                   it     = line.cbegin();
    dto::Text::CharIndex_t char_i = 1;

    while( it != line.cend() ) {
        if( it != line.cend() ) {
            if( _inside_comment_flag ) {
                processCommentText( { line, line_i }, { it, char_i } );
            } else if( *it == html5::special_char::LESS_THAN && !Text::isEOL( it, line ) && !iswspace( *std::next( it ) ) ) { //a tag looks to begin...
                processTag( { line, line_i }, { it, char_i } );
            } else {
                processText( { line, line_i }, { it, char_i } );
            }
        }
    }
}

/**
 * Processes an HTML tag and adds it to the token store
 * @param line_info Line information container (const ref to line content and index)
 * @param char_info Character information container (ref to char iterator and index)
 */
void blogator::dom::parser::Tokeniser::processTag( Tokeniser::LineInfo line_info,
                                                   Tokeniser::CharInfo char_info )
{
    using blogator::dom::dto::Text;

    auto starting_char_i = char_info.index;
    auto tag_type        = TokenType::START_TAG;

    if( _inside_comment_flag && isCommentClose( line_info, char_info ) ) {
        _inside_comment_flag = false;
        return;
    }

    if( !Text::isEOL( std::next( char_info.it ), line_info.line ) && *std::next( char_info.it ) == html5::special_char::SOLIDUS )
        tag_type = TokenType::END_TAG;

    if( *std::next( char_info.it ) == html5::special_char::EXCLAMATION_MARK ) { //Edge case - will process comment open/close tags + text
        if( processCommentOpen( line_info, char_info ) ) {
            _inside_comment_flag = true;
            return;
        }
    }

    u32stringstream_t ss;

    while( char_info.it != line_info.line.cend() && *char_info.it != html5::special_char::GREATER_THAN ) {
        ss << *char_info.it;
        ++char_info.it;
        ++char_info.index;
    }

    if( char_info.it != line_info.line.cend() ) {
        ss << *char_info.it; //include '>' character
        ++char_info.it;
        ++char_info.index;
    }

    _tokens.emplace_back( line_info.index, starting_char_i, tag_type, ss.str() );
}

/**
 * Processes HTML text and adds it to the token store
 * @param line_info Line information container (const ref to line content and index)
 * @param char_info Character information container (ref to char iterator and index)
 */
void blogator::dom::parser::Tokeniser::processText( Tokeniser::LineInfo line_info,
                                                    Tokeniser::CharInfo char_info )
{
    using blogator::dom::dto::Text;
    /**
     * [LAMBDA] checks for end of text content
     */
    auto isText = [&]() {
        return !( char_info.it == line_info.line.cend()
                  || ( *char_info.it == html5::special_char::LESS_THAN
                       && !Text::isEOL( std::next( char_info.it ), line_info.line )
                       && !iswspace( *std::next( char_info.it ) ) ) );
    };

    auto start_char_i = char_info.index;
    auto is_space     = true;
    u32stringstream_t ss;

    while( isText() ) {
        ss << *char_info.it;

        if( is_space && !iswspace( *char_info.it ) )
            is_space = false;

        ++char_info.it;
        ++char_info.index;
    }

//    if( !( !_tokens.empty() && _tokens.back().type == TokenType::END_TAG && is_space ) ) {
        _tokens.emplace_back( line_info.index, start_char_i, TokenType::TEXT, ss.str() );
//    } //i.e. If previous node is a END_TAG and text is just space then omit from insertion
}

/**
 * Checks existence of an opening comment tag and processes it
 * @param line_info Line information container (const ref to line content and index)
 * @param char_info Character information container (ref to char iterator and index)
 * @return Existence && processed state
 */
bool blogator::dom::parser::Tokeniser::processCommentOpen( Tokeniser::LineInfo line_info,
                                                           Tokeniser::CharInfo char_info )
{
    using blogator::dom::dto::Text;

    auto it = char_info.it + 1; //assumes the '<' was already checked thus the function call

    if( *(it++) != html5::special_char::EXCLAMATION_MARK )
        return false;

    if( Text::isEOL( it, line_info.line ) || *(it++) != html5::special_char::HYPHEN_MINUS )
        return false;

    if( Text::isEOL( it, line_info.line ) || *(it++) != html5::special_char::HYPHEN_MINUS )
        return false;

    //confirmed to be an opening comment tag ('!--')
    _tokens.emplace_back( line_info.index, char_info.index, TokenType::START_TAG, U"<!--" );

    _inside_comment_flag = true;
    char_info.it         = it;
    char_info.index     += 4;

    return true;
}

/**
 * Processes text content within a comment
 * @param line_info Line information container (const ref to line content and index)
 * @param char_info Character information container (ref to char iterator and index)
 */
void blogator::dom::parser::Tokeniser::processCommentText( Tokeniser::LineInfo line_info,
                                                           Tokeniser::CharInfo char_info )
{
    /**
     * [LAMBDA] checks for end of text content
     */
    auto isCommentText = [&]( bool &flag ) {
        if( char_info.it == line_info.line.cend() )
            return false;
        if( *char_info.it == html5::special_char::HYPHEN_MINUS && isCommentClose( line_info, char_info ) ) {
            flag = true;
            return false;
        }
        return true;
    };

    bool closing_tag_flag = false;
    auto start_char_i     = char_info.index;
    auto is_space         = true;
    u32stringstream_t ss;

    while( isCommentText( closing_tag_flag ) ) {
        ss << *char_info.it;

        if( is_space && !iswspace( *char_info.it ) )
            is_space = false;

        ++char_info.it;
        ++char_info.index;
    }

    _tokens.emplace_back( line_info.index, start_char_i, TokenType::TEXT, ss.str() );

    if( closing_tag_flag )
        processCommentClose( line_info, char_info );
}

/**
 * Processes a comment closing tag
 * @param line_info Line information container (const ref to line content and index)
 * @param char_info Character information container (ref to char iterator and index)
 */
void blogator::dom::parser::Tokeniser::processCommentClose( Tokeniser::LineInfo line_info,
                                                            Tokeniser::CharInfo char_info )
{
    char_info.it         = std::next( char_info.it, 3 );
    char_info.index     += 3;
    _inside_comment_flag = false;

    _tokens.emplace_back( line_info.index, char_info.index, TokenType::END_TAG, U"-->" );
}

/**
 * Checks for a comment closing tag
 * @param line_info Line information container (const ref to line content and index)
 * @param char_info Character information container (ref to char iterator and index)
 * @return Comment closing tag existence
 */
bool blogator::dom::parser::Tokeniser::isCommentClose( Tokeniser::LineInfo line_info,
                                                       Tokeniser::CharInfo char_info )
{
    using blogator::dom::dto::Text;

    auto it = char_info.it + 1; //assumes the first '-' was already checked thus this function being called

    if( Text::isEOL( it, line_info.line ) || *(it++) != html5::special_char::HYPHEN_MINUS )
        return false;

    return ( !Text::isEOL( it, line_info.line ) && *( it++ ) == html5::special_char::GREATER_THAN );
}