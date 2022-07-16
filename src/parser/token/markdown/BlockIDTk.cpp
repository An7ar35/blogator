#include "BlockIDTk.h"

#include "../../../unicode/ascii.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
BlockIDTk::BlockIDTk( blogator::parser::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::BLOCK_ID, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
BlockIDTk::BlockIDTk( std::u32string text, blogator::parser::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::BLOCK_ID, std::move( text ), position )
{}

/**
 * Validate an name ID
 * @param name ID
 * @return Validation state
 */
bool BlockIDTk::validateName( const std::u32string &name ) {
    auto it = name.cbegin();

    if( it == name.cend() ) {
        return false; //EARLY RETURN
    }

    if( unicode::ascii::isdigit( *it ) ) {
        return false; //EARLY RETURN
    }

    if( *it++ == unicode::HYPHEN_MINUS && ( it == name.cend() || unicode::ascii::isdigit( *it ) ) ) {
        return false; //EARLY RETURN
    }

    while( it != name.cend() ) {
        if( !( unicode::ascii::isalnum( *it ) || *it == unicode::HYPHEN_MINUS || *it == unicode::UNDERSCORE ) ) {
            return false; //EARLY RETURN
        }

        ++it;
    }

    return true;
}