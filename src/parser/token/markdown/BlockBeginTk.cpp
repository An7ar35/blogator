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
 * Check the block has an ID set
 * @return ID state
 */
bool BlockBeginTk::hasID() const {
    return !_id.empty();
}

/**
 * Gets the block's ID string
 * @return ID string
 */
const std::u32string & BlockBeginTk::id() const {
    return _id;
}

/**
 * Sets the block's ID string
 * @param id ID string
 */
void BlockBeginTk::setID( const std::u32string &id ) {
    _id = id;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void BlockBeginTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "text": ")";
    unicode::utf8::convert( os, this->text() );
    os << R"(", "id": ")";
    unicode::utf8::convert( os, this->id() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}