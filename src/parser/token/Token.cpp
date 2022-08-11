#include "Token.h"

#include "../../unicode/utf8.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param ns Context namespace
 * @param position Line:Col position of token
 */
token::Token::Token( reporter::Context ns, TextPos position ) :
    _context( ns ),
    _position( position )
{}

/**
 * Constructor
 * @param ns Context namespace
 * @param text UTF32 Text
 * @param position Line:Col position of token
 */
token::Token::Token( reporter::Context ns, std::u32string text, TextPos position ) :
    _context( ns ),
    _text( std::move( text ) ),
    _position( position )
{}

/**
 * Sets the text string of the token
 * @param text Text
 */
void token::Token::setText( std::u32string text ) {
    _text = std::move( text );
}

/**
 * Gets the context for the token
 * @return Token context/namespace
 */
blogator::reporter::Context token::Token::context() const {
    return _context;
}

/**
 * Gets the text
 * @return UTF32 text string
 */
std::u32string & token::Token::text() {
    return _text;
}

/**
 * Gets the text
 * @return UTF32 text string
 */
const std::u32string & token::Token::text() const {
    return _text;
}

/**
 * Gets the line number of the token
 * @return Line number
 */
size_t token::Token::lineNum() const {
    return _position.line;
}

/**
 * Gets the char number of the token
 * @return Char number on line
 */
size_t token::Token::colPos() const {
    return _position.col;
}

/**
 * Gets the line:col position of the token
 * @return Token position from the source text
 */
blogator::TextPos token::Token::position() const {
    return _position;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void token::Token::toStr( std::ostream &os ) const {
    os << "{ text: \"";
    unicode::utf8::convert( os, this->text() );
    os << "\", position: " << this->position() << " }";
}