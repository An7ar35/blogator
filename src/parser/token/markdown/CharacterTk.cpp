#include "CharacterTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
CharacterTk::CharacterTk( TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::CHARACTER, position )
{}

/**
 * Constructor
 * @param c Character
 * @param position Line:Col position of token in source text
 */
CharacterTk::CharacterTk( char32_t c, blogator::parser::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::CHARACTER, std::u32string( 1, c ), position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
CharacterTk::CharacterTk( std::u32string text, TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::CHARACTER, std::move( text ), position )
{}