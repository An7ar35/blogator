#include "HorizontalRuleTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param text Text
 * @param position Line:Col position of token in source text
 */
HorizontalRuleTk::HorizontalRuleTk( std::u32string text, TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::HORIZONTAL_RULE, std::move( text ), position )
{}
