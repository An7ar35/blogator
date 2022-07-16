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
    this->setFormatType( FormattingTk::resolveFormatType( this->text() ) );
}

/**
 * Constructor
 * @param c UTF32 character
 * @param position Line:Col position of token in source text
 */
FormatEndTk::FormatEndTk( uint32_t c, blogator::parser::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_END, { c }, position )
{
    this->setFormatType( FormattingTk::resolveFormatType( this->text() ) );
}

/**
 * Constructor
 * @param c1 UTF32 character 1
 * @param c2 UTF32 character 2
 * @param position Line:Col position of token in source text
 */
FormatEndTk::FormatEndTk( uint32_t c1, uint32_t c2, blogator::parser::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_END, { c1, c2 }, position )
{
    this->setFormatType( FormattingTk::resolveFormatType( this->text() ) );
}
