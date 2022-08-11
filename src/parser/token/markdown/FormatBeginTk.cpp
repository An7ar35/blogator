#include "FormatBeginTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
FormatBeginTk::FormatBeginTk( blogator::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_BEGIN, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
FormatBeginTk::FormatBeginTk( std::u32string text, blogator::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_BEGIN, std::move( text ), position )
{
    this->setFormatType( FormattingTk::resolveFormatType( this->text() ) );
}


/**
 * Constructor
 * @param c UTF32 character
 * @param position Line:Col position of token in source text
 */
FormatBeginTk::FormatBeginTk( uint32_t c, blogator::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_BEGIN, { c }, position )
{
    this->setFormatType( FormattingTk::resolveFormatType( this->text() ) );
}

/**
 * Constructor
 * @param c1 UTF32 character 1
 * @param c2 UTF32 character 2
 * @param position Line:Col position of token in source text
 */
FormatBeginTk::FormatBeginTk( uint32_t c1, uint32_t c2, blogator::TextPos position ) :
    FormattingTk( specs::markdown::TokenType::FORMAT_BEGIN, { c1, c2 }, position )
{
    this->setFormatType( FormattingTk::resolveFormatType( this->text() ) );
}