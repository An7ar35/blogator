#include "ConfigTk.h"

#include "../../../unicode/utf8.h"

using namespace blogator::configuration::parser;

/**
 * Constructor
 * @param type Token type
 * @param position Position in source
 */
token::ConfigTk::ConfigTk( specs::TokenType type, blogator::TextPos position ) :
    _type( type ),
    _pos( position )
{}

/**
 * Constructor
 * @param type Token type
 * @param c UTF-32 Character
 * @param position Position in source
 */
token::ConfigTk::ConfigTk( specs::TokenType type, char32_t c, blogator::TextPos position ) :
    _type( type ),
    _text( std::u32string( 1, c ) ),
    _pos( position )
{}

/**
 * Constructor
 * @param type Token type
 * @param text Text
 * @param position Position in source
 */
token::ConfigTk::ConfigTk( specs::TokenType type, std::u32string text, blogator::TextPos position ) :
    _type( type ),
    _text( std::move( text ) ),
    _pos( position )
{}

/**
 * Gets the type of the token
 * @return TokenType enum
 */
specs::TokenType token::ConfigTk::type() const {
    return _type;
}

/**
 * Gets the text of the token
 * @return Token text
 */
const std::u32string &token::ConfigTk::text() const {
    return _text;
}

/**
 * Gets the start position of the token in the source
 * @return Start position of token
 */
blogator::TextPos token::ConfigTk::position() const {
    return _pos;
}

/**
 * Output stream operator
 * @param os Output stream
 * @param tk Configuration token to print out
 * @return Output stream
 */
std::ostream & blogator::configuration::parser::token::operator <<( std::ostream &os, const token::ConfigTk &tk ) {
    os << R"({ "type": ")" << tk.type()
       << R"(", "text": ")";
    unicode::utf8::convert( os, tk.text() );
    os << R"(", "position": ")" << tk.position()
       << R"(" })";

    return os;
}