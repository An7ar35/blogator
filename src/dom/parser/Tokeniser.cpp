#include "Tokeniser.h"

#include "../html5/html5.h"

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
 * Constructor
 * @param utf32_src Source html text
 */
blogator::dom::parser::Tokeniser::Tokeniser( const blogator::dto::Text &utf32_src ) :
    _source( utf32_src )
{
    dto::Text::LineIndex_t line_index { 1 };

    for( const auto & line : _source._lines ) {
        processLine( line, line_index );
        ++line_index;
    }

    for( const auto &t : _tokens )
        std::cout << t << std::endl;
}

///**
// * Initialise the token parser
// * @return No-Error state
// */
//bool blogator::dom::parser::Tokeniser::init() {
//    dto::Text::LineIndex_t line_index { 1 };
//
//    for( const auto & line : _source._lines ) {
////        std::cout << "RAW-LINE: " << encoding::encodeToUTF8( line ) << std::endl; //DEBUG
//        processLine( line, line_index );
//        ++line_index;
//    }
//
//    for( const auto &t : _tokens ) //DEBUG
//        std::cout << t << std::endl;
//
//    return true;
//}

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
void blogator::dom::parser::Tokeniser::processLine( const dto::Text::Line_t &line, const dto::Text::LineIndex_t &line_i ) {
    auto                   it     = line.cbegin();
    dto::Text::CharIndex_t char_i = 1;

    while( it != line.cend() ) {

        if( it != line.cend() ) {
            if( *it == html5::special_char::LESS_THAN && !iswspace( *std::next( it ) ) ) { //open tag
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
    auto starting_char_i = char_info.index;
    auto tag_type        = TokenType::START_TAG;

    if( *std::next( char_info.it ) == html5::special_char::SOLIDUS )
        tag_type = TokenType::END_TAG;

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
    /**
     * [LAMBDA] checks for end of text content
     */
    auto isText = [&]() {
        return !( char_info.it == line_info.line.cend()
                  || ( *char_info.it == html5::special_char::LESS_THAN
                       && std::next( char_info.it ) != line_info.line.cend()
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

    if( !( !_tokens.empty() && _tokens.back().type == TokenType::END_TAG && is_space ) ) {
        _tokens.emplace_back( line_info.index, start_char_i, TokenType::TEXT, ss.str() );
    } //i.e. If previous node is a END_TAG and text is just space then omit from insertion
}
