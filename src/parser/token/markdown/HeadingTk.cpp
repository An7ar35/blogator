#include "HeadingTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
HeadingTk::HeadingTk( TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::HEADING, position ),
    _level( 0 )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
HeadingTk::HeadingTk( std::u32string text, TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::HEADING, std::move( text ), position ),
    _level( ( this->text().size() > 6 ? 6 : this->text().size() ) )
{}

/**
 * Constructor
 * @param level Heading level (1-6)
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
HeadingTk::HeadingTk( uint8_t level, std::u32string text, blogator::parser::TextPos position )  :
    MarkdownTk( specs::markdown::TokenType::HEADING, std::move( text ), position ),
    _level( level )
{}

/**
 * Gets the heading level value
 * @return Heading level
 */
uint8_t HeadingTk::level() const {
    return _level;
}

/**
 * Sets the heading level value
 * @param val Heading level
 */
void HeadingTk::setLevel( uint8_t val ) {
    _level = val;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void HeadingTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type() << R"(", "level": )" << (unsigned) this->level() << R"(, "text": ")";
    blogator::unicode::utf8::convert( os, this->text() );
    os << R"(" })";
//    os << R"(", "position": ")" << this->position() << R"(" })";
}