#include "FormatBeginTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
FormatBeginTk::FormatBeginTk( blogator::parser::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_BEGIN, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
FormatBeginTk::FormatBeginTk( std::u32string text, blogator::parser::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_BEGIN, std::move( text ), position )
{}