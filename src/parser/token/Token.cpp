#include "Token.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param ns Context namespace
 * @param line_number Line position of token
 * @param col_number Char position of token on line
 */
token::Token::Token( specs::Context ns, size_t line_number, size_t col_number ) :
    _context( ns ),
    _line_pos( line_number ),
    _col_pos( col_number )
{}

/**
 * Constructor
 * @param ns Context namespace
 * @param text Text
 * @param line_number Line position of token
 * @param col_number Char position of token on line
 */
token::Token::Token( specs::Context ns, std::string text, size_t line_number, size_t col_number ) :
    _context( ns ),
    _text( std::move( text ) ),
    _line_pos( line_number ),
    _col_pos( col_number )
{}

/**
 * Sets the text string of the token
 * @param text Text
 */
void token::Token::setText( std::string text ) {
    _text = std::move( text );
}

/**
 * Gets the context for the token
 * @return Token context/namespace
 */
specs::Context token::Token::context() const {
    return _context;
}

/**
 * Gets the text
 * @return Text string
 */
std::string token::Token::text() const {
    return _text;
}

/**
 * Gets the line number of the token
 * @return Line number
 */
size_t token::Token::lineNum() const {
    return _line_pos;
}

/**
 * Gets the char number of the token
 * @return Char number on line
 */
size_t token::Token::colPos() const {
    return _col_pos;
}