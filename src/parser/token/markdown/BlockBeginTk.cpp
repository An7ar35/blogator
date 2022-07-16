#include "BlockBeginTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param type TokenType enum
 * @param position Line:Col position of token in source text
 */
BlockBeginTk::BlockBeginTk( specs::markdown::TokenType type, TextPos position ) :
    MarkdownTk( type, position )
{}

/**
 * Constructor
 * @param type TokenType enum
 * @param text Text
 * @param position Line:Col position of token in source text
 */
BlockBeginTk::BlockBeginTk( specs::markdown::TokenType type, std::u32string text, TextPos position ) :
    MarkdownTk( type, std::move( text ), position )
{}

/**
 * Gets the block space indent level
 * @return Block space indent count
 */
unsigned BlockBeginTk::indent() const {
    return ( this->position().col > 0 ? this->position().col - 1 : 0 );
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void BlockBeginTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "text": ")";
    unicode::utf8::convert( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}
