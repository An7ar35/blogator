#include "Token.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param ns Context namespace
 * @param position Line:Col position of token
 */
token::Token::Token( specs::Context ns, TextPos position ) :
    _context( ns ),
    _position( position )
{}

/**
 * Constructor
 * @param ns Context namespace
 * @param text UTF32 Text
 * @param position Line:Col position of token
 */
token::Token::Token( specs::Context ns, std::u32string text, TextPos position ) :
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
specs::Context token::Token::context() const {
    return _context;
}

/**
 * Gets the text
 * @return UTF32 text string
 */
std::u32string token::Token::text() const {
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
 * Prints out a string representation of the token
 * @param os Output stream
 */
void token::Token::toStr( std::ostream &os ) const {
    os << "token::Token={ text: \"";
    unicode::utf8::convert( os, text() );
    os << "\", position: " << lineNum() << ":" << colPos() << " }";
}
