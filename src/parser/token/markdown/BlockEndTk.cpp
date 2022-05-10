#include "BlockEndTk.h"

#include "../../../unicode/unicode.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param type TokenType of the block
 * @param position Line:Col position of token in source text
 */
BlockEndTk::BlockEndTk( specs::markdown::TokenType type, TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::BLOCK_END, position ),
    _block_type( type )
{}

/**
 * Gets the token type of the block
 * @return TokenType of block
 */
blogator::parser::specs::markdown::TokenType BlockEndTk::blockType() const {
    return _block_type;
}

#ifdef TESTING

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void  BlockEndTk::toStr( std::ostream &os ) const {
    os << "[" << this->type() << ", " << this->blockType() << ", \"";
    unicode::normalize( os, this->text() );
    os << "\"]";
}

#else

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void BlockEndTk::toStr( std::ostream &os ) const {
    os << "MarkdownTk={ type: " << this->type() << ", block: " << this->blockType() << ", text: \"";
    blogator::unicode::utf8::convert( os, this->text() );
    os << "\", position: " << this->position() << " }";
}

#endif