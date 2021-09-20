#include "Tokeniser.h"

#include "../exception/parsing_failure.h"

using namespace blogator::parser;

/**
 * Parse an HTML formatted string (no compliance checking)
 * @param u8text HTML to parse
 * @param ctx Initial context (source type)
 * @return Collection of tokens
 * @throws ParsingFailure on failure
 */
std::vector<token::Token> Tokeniser::parse( const std::string &u8text, specs::Context ctx ) {
    switch( ctx ) {
        case specs::Context::UNKNOWN:
            return parseHTML( u8text );
        case specs::Context::BLOGATOR:
            return parseBlogator( u8text );
        case specs::Context::HTML5:
            return parseHTML( u8text );
        case specs::Context::MARKDOWN:
            return parseMarkdown( u8text );
        default:
            throw exception::parsing_failure( "Unimplemented: 'parser::specs::Context::?'" );
    }
}

/**
 * Parse Blogator text
 * @param u8text Source UTF8 text
 * @return Collection of tokens
 */
std::vector<token::Token> Tokeniser::parseBlogator( const std::string &u8text ) { //TODO
    std::vector<token::Token> tokens;

    auto token_start = u8text.cbegin();
    auto token_end   = token_start;



    return std::vector<token::Token>();
}

/**
 * Parse HTML+Blogator text
 * @param u8text Source UTF8 text
 * @return Collection of tokens
 */
std::vector<token::Token> Tokeniser::parseHTML( const std::string &u8text ) { //TODO
    using blogator::parser::specs::Context;

    std::vector<token::Token> tokens;

    auto                   token_start = u8text.cbegin();
    auto                   token_end   = token_start;
    auto                   current_ctx = Context::UNKNOWN;
    std::string::size_type col         = 0;
    size_t                 line        = 0;

    while( token_end != u8text.cend() ) {

        switch( current_ctx ) {
            case Context::UNKNOWN: {
                //TODO check first chars and decide
            } break;

            case Context::BLOGATOR: {
                //TODO '{{' begins blogator context
                //TODO '}}' ends blogator context
            } break;

            case Context::HTML5: {

            } break;

            default: {
                //log error
                throw exception::parsing_failure( "Unimplemented: Context::?" );
            }
        }

        tokens.emplace_back( Context::BLOGATOR, col, line );

        //{{
        //<

        if( *token_start == '\n' ) {
            ++line;
        }

        ++token_end;
    }

    return tokens;
}

/**
 * Parse Markdown+HTML+Blogator text
 * @param u8text Source UTF8 text
 * @return Collection of tokens
 */
std::vector<token::Token> Tokeniser::parseMarkdown( const std::string &u8text ) { //TODO
    return std::vector<token::Token>();
}

/**
 * Parse Blogator text
 * @param u8text Source UTF8 text
 * @param tokens Collection of tokens
 * @param iterator Iterator to the start
 * @return Iterator to end
 */
TextIterator_t Tokeniser::parseBlogator( const std::string &u8text, std::vector<token::Token> &tokens, TextIterator_t iterator ) {
    return std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::const_iterator();
}

/**
 * Parse HTML text
 * @param u8text Source UTF8 text
 * @param tokens Collection of tokens
 * @param iterator Iterator to the start
 * @return Iterator to end
 */
TextIterator_t Tokeniser::parseHTML( const std::string &u8text, std::vector<token::Token> &tokens, TextIterator_t iterator ) {
    return std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::const_iterator();
}

/**
 * Parse Markdown text
 * @param u8text Source UTF8 text
 * @param tokens Collection of tokens
 * @param iterator Iterator to the start
 * @return Iterator to end
 */
TextIterator_t Tokeniser::parseMarkdown( const std::string &u8text, std::vector<token::Token> &tokens, TextIterator_t iterator ) {
    return std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::const_iterator();
}


