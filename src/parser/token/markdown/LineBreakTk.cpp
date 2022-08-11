#include "LineBreakTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
LineBreakTk::LineBreakTk( blogator::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::LINE_BREAK, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
LineBreakTk::LineBreakTk( std::u32string text, blogator::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::LINE_BREAK, std::move( text ), position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void LineBreakTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}