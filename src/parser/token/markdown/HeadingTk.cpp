#include "HeadingTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
HeadingTk::HeadingTk( TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::HEADING, position ),
    _indent( 0 )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
HeadingTk::HeadingTk( std::u32string text, TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::HEADING, std::move( text ), position ),
    _indent( ( text.size() > 6 ? 6 : text.size() ) )
{}

/**
 * Gets the indent value
 * @return Indent depth
 */
uint8_t HeadingTk::indent() const {
    return _indent;
}

/**
 * Sets the indent value
 * @param val Indent depth
 */
void HeadingTk::setIndent( uint8_t val ) {
    _indent = val;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void HeadingTk::toStr( std::ostream &os ) const {
    os << "MarkdownTk={ type: " << this->type() << ", indent: " << _indent << ", text: \"";
    blogator::unicode::utf8::convert( os, this->text() );
    os << "\", position: " << this->position() << " }";
}
