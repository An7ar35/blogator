#include "LineBreakTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
LineBreakTk::LineBreakTk( blogator::parser::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::LINE_BREAK, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
LineBreakTk::LineBreakTk( std::u32string text, blogator::parser::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::LINE_BREAK, std::move( text ), position )
{}