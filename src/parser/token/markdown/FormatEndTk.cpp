#include "FormatEndTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
FormatEndTk::FormatEndTk( blogator::parser::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_END, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
FormatEndTk::FormatEndTk( std::u32string text, blogator::parser::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_END, std::move( text ), position )
{
    this->setFormatType( FormattingTk::resolveFormateType( this->text() ) );
}