#include "MarkdownTk.h"

#include "../../../unicode/unicode.h"

using namespace blogator::parser::token;

/**
 * Constructor
 * @param type Markdown token type
 * @param position Line:Col position of token in source text
 */
markdown::MarkdownTk::MarkdownTk( specs::markdown::TokenType type, TextPos position ) :
    Token( specs::Context::HTML5, position ),
    _type( type )
{}

/**
 * Constructor
 * @param type Markdown token type
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
markdown::MarkdownTk::MarkdownTk( specs::markdown::TokenType type, std::u32string text, TextPos position ) :
    Token( specs::Context::HTML5, std::move( text ), position ),
    _type( type )
{}

/**
 * Gets the token type
 * @return Token type
 */
blogator::parser::specs::markdown::TokenType markdown::MarkdownTk::type() const {
    return _type;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void  markdown::MarkdownTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type() << R"(", "text": ")";
    unicode::normalize( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}