#include "Parser.h"

#include <cctype>

#include "../exception/parsing_failure.h"
#include "logging/ParserLog.h"
#include "tokeniser/Blogator.h"
#include "tokeniser/HTML5.h"
#include "tokeniser/Markdown.h"

using namespace blogator::parser;

/**
 * Parse an HTML formatted string (no compliance checking)
 * @param u32text HTML to parse
 * @param ctx Initial context (source type)
 * @return Collection of tokens
 * @throws ParsingFailure on failure
 */
Tokens_t Tokeniser::parse( std::u32string &u32text, specs::Context ctx ) {
    switch( ctx ) {
        case specs::Context::UNKNOWN:
            return parseHTML( u32text );
        case specs::Context::BLOGATOR:
            return parseBlogator( u32text );
        case specs::Context::HTML5:
            return parseHTML( u32text );
        case specs::Context::MARKDOWN:
            return parseMarkdown( u32text );
        default:
            throw exception::parsing_failure( "Unimplemented: 'parser::specs::Context::?'" );
    }

    //BLOGATOR => <HTML/> || <HTML></HTML>
    //HTML => {{BLOGATOR}}
    //MarkDown => HTML > {{BLOGATOR}} || {{BLOGATOR > ( <HTML/> || <HTML></HTML> }}
    //CONFIG => PURE (i.e. no break conditions)

    //TODO pre-process the input either here or elsewhere prior - must be able to trigger parsing errors
    //     -> see section [13.2.3.5 Preprocessing the input stream]
}



/**
 * Parse Blogator text
 * @param u32text Source UTF8 text
 * @return Collection of tokens
 */
Tokens_t Tokeniser::parseBlogator( std::u32string &u32text ) { //TODO
    auto tree_builder       = dom::TreeBuilder();
    auto source_text        = U32Text( u32text );
    auto blogator_tokeniser = tokeniser::Blogator();
    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
    auto tokens             = Tokens_t();
    auto current_context    = specs::Context::BLOGATOR;

    while( !source_text.reachedEnd() ) {
        switch( current_context ) {
            case specs::Context::BLOGATOR:
                current_context = blogator_tokeniser.parse( source_text, tokens, current_context );

                if( current_context == specs::Context::BLOGATOR && !blogator_tokeniser.finished() ) {
                    //TODO LOG error - BLOGATOR namespace not closed (file line:col of opening token)
                }
                break;

            case specs::Context::HTML5:
                current_context = html5_tokeniser.parse( source_text, current_context );
                break;

            default:
                //TODO log context
                throw exception::parsing_failure( "Context returned not supported with HTML." );
        }
    }

    return tokens;
}

/**
 * Parse HTML+Blogator text
 * @param u32text Source UTF8 text
 * @return Collection of tokens
 */
Tokens_t Tokeniser::parseHTML( std::u32string &u32text ) { //TODO
    auto tree_builder       = dom::TreeBuilder();
    auto source_text        = U32Text( preprocess( u32text ) );
    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
    auto blogator_tokeniser = tokeniser::Blogator();
    auto tokens             = Tokens_t();
    auto current_context    = specs::Context::HTML5;

    while( !source_text.reachedEnd() ) {
        switch( current_context ) {
            case specs::Context::BLOGATOR:
                current_context = blogator_tokeniser.parse( source_text, tokens, current_context );

                if( current_context == specs::Context::BLOGATOR && !blogator_tokeniser.finished() ) {
                    //TODO LOG error - BLOGATOR namespace not closed (file line:col of opening token)
                }
                break;

            case specs::Context::HTML5:
                current_context = html5_tokeniser.parse( source_text, current_context );
                break;

            default:
                //TODO log context
                throw exception::parsing_failure( "Context returned not supported with HTML." );
        }
    }

    return tokens;
}

//html.... {{ blogator... }} html...
//html.... {{ blogator... <html... > blogator.... }} html...

/**
 * Parse Markdown+HTML+Blogator text
 * @param u32text Source UTF8 text
 * @return Collection of tokens
 */
Tokens_t Tokeniser::parseMarkdown( std::u32string &u32text ) { //TODO
    auto tree_builder       = dom::TreeBuilder();
    auto source_text        = U32Text( u32text );
    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
    auto blogator_tokeniser = tokeniser::Blogator();
    auto markdown_tokeniser = tokeniser::Markdown();
    auto tokens             = Tokens_t();
    auto current_context    = specs::Context::MARKDOWN;

    while( !source_text.reachedEnd() ) {
        switch( current_context ) {
            case specs::Context::BLOGATOR:
                current_context = blogator_tokeniser.parse( source_text, tokens, current_context );
                break;

            case specs::Context::HTML5:
                current_context = html5_tokeniser.parse( source_text, current_context );
                break;

            case specs::Context::MARKDOWN:
                current_context = markdown_tokeniser.parse( source_text, tokens, current_context );
                break;

            default:
                //TODO log context
                throw exception::parsing_failure( "Context returned not supported with HTML." );
        }
    }

    return tokens;
}

///**
// * Parse Blogator text
// * @param u32text Source UTF8 text
// * @param tokens Collection of tokens
// * @param iterator Iterator to the start
// * @return Iterator to end
// */
//TextIterator_t Tokeniser::parseBlogator( U32Text &text, Tokens_t &tokens ) {
//    using namespace specs;
//
//    Context current_ctx = Context::UNKNOWN;
//
//    while( text.iterator != text.src.cend() ) {
//
//        switch( *text.iterator ) {
//            case '{': [[fallthrough]];
//            case '}': {
//                auto fwd_iterator = std::next( text.iterator, 1 );
//
//                if( fwd_iterator != text.src.cend() && *fwd_iterator == *text.iterator ) {
//                    current_ctx = Context::BLOGATOR;
//
//                    tokens.emplace_back( std::make_unique<Blogator>(
//                        current_ctx,
//                        ( *text.iterator == '{' ? specs::blogator::Element::BEGIN : specs::blogator::Element::END ),
//                        std::u32string( text.iterator, fwd_iterator ),
//                        text.position
//                    ) );
//
//                    std::advance( text.iterator, 2 );
//                    text.position.col += 2;
//
//                } else {
//                    //TODO
//                }
//            } break;
//
//            case '<': {
//
//            } break;
//
//
//        }
//
//
//
//
//
//        while( text.iterator != text.src.cend() && std::isspace( *( text.iterator++ ) ) );
//
//        tokens.emplace_back( std::make_unique<Blogator>( Blogator::parseToken( text.src, text.iterator, text.position ) ) );
//
//        auto token = dynamic_cast<Blogator *>( tokens.back().get() );
//
//        if( token->element() == specs::blogator::Element::UNKNOWN ) {
//            //TODO error
//        }
//
//        while( text.iterator != text.src.cend() ) {
//
//
//        }
//
//
//        while( text.iterator != text.src.cend() && std::isspace( *( text.iterator++ ) ) );
//    }
//
//    return std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::const_iterator();
//}
//
///**
// * Parse HTML text
// * @param u32text Source UTF8 text
// * @param tokens Collection of tokens
// * @param iterator Iterator to the start
// * @return Iterator to end
// */
//TextIterator_t Tokeniser::parseHTML( U32Text &text, Tokens_t &tokens ) {
//    return std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::const_iterator();
//}
//
///**
// * Parse Markdown text
// * @param u32text Source UTF8 text
// * @param tokens Collection of tokens
// * @param iterator Iterator to the start
// * @return Iterator to end
// */
//TextIterator_t Tokeniser::parseMarkdown( U32Text &text, Tokens_t &tokens ) {
//    return std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::const_iterator();
//}

std::u32string blogator::parser::Tokeniser::preprocess( std::u32string &u32str, const std::filesystem::path & path ) {
    //Any occurrences of surrogates are surrogate-in-input-stream parse errors.
    //Any occurrences of noncharacters are noncharacter-in-input-stream parse errors
    // and any occurrences of controls other than ASCII whitespace and U+0000 NULL characters are control-character-in-input-stream parse errors.

    //Before the tokenization stage, the input stream must be preprocessed by normalizing newlines.
    // Thus, newlines in HTML DOMs are represented by U+000A LF characters, and there are never
    // any U+000D CR characters in the input to the tokenization stage.

    using blogator::parser::specs::Context;
    using blogator::parser::specs::html5::ErrorCode;

    std::u32string::size_type line  = 1;
    std::u32string::size_type col   = 1;
    std::vector<uint32_t>     buffer;

    for( auto it = u32str.begin(); it != u32str.end(); ++it ) {
        if( unicode::utf32::issurrogate( *it ) ) {
            logging::ParserLog::log( path, Context::HTML5, ErrorCode::SURROGATE_IN_INPUT_STREAM, { line, col } );
            buffer.emplace_back( *it );

        } else if( unicode::utf32::isnonchar( *it ) ) {
            logging::ParserLog::log( path, Context::HTML5, ErrorCode::NONCHARACTER_IN_INPUT_STREAM, { line, col } );
            buffer.emplace_back( *it );

        } else if( *it != 0x00 && unicode::ascii::iscntrl( *it ) && !unicode::ascii::iswspace( *it  ) ) {
            logging::ParserLog::log( path, Context::HTML5, ErrorCode::CONTROL_CHARACTER_IN_INPUT_STREAM, { line, col } );
            buffer.emplace_back( *it );

        } else if( unicode::ascii::isnewline( *it ) ) {
            auto next = std::next( it );

            if( *it == unicode::CR && next != u32str.cend() && *next == unicode::LF ) {
                it = next;
            }

            buffer.emplace_back( unicode::LF );
            ++line;
            col = 0;

        } else {
            buffer.emplace_back( *it );
        }

        ++col;
    }

    return std::u32string( buffer.begin(), buffer.end() );
}