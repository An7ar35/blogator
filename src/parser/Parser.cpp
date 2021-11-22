#include "Parser.h"

#include <cctype>

#include "../exception/parsing_failure.h"
#include "logging/ParserLog.h"
#include "encoding/Transcode.h"
#include "tokeniser/Blogator.h"
#include "tokeniser/HTML5.h"
#include "tokeniser/Markdown.h"

using namespace blogator::parser;

/**
 * Parse stream
 * @param source Source byte stream
 * @param ctx Starting context
 * @return DOM
 * @throws exception::parsing_failure when the process failed
 */
std::unique_ptr<dom::DOM> Parser::parse( Source &source, specs::Context ctx ) {
    auto codepoints = std::vector<uint32_t>();

    if( !encoding::Transcode::convert( source, codepoints ) ) {
        //TODO log error
        throw exception::parsing_failure( "Failed transcoding." );
    }

    auto u32text      = U32Text( codepoints );
    auto dom          = std::make_unique<dom::DOM>();
    auto tree_builder = dom::TreeBuilder( *dom );

    if( !parse( u32text, tree_builder, ctx ) ) {
        //TODO error
        throw exception::parsing_failure( "Failed parsing." );
    }

    return std::move( dom );
}

/**
 * Parser a UTF-32 encoded text
 * @param u32text UTF-32 encoded text
 * @param tree_builder DOM tree builder instance
 * @param ctx Starting context
 * @return Success
 */
bool Parser::parse( U32Text &u32text, dom::TreeBuilder &tree_builder, specs::Context ctx ) {
    switch( ctx ) {
        case specs::Context::UNKNOWN:
            return parseHTML( u32text, tree_builder );
        case specs::Context::BLOGATOR:
            return parseBlogator( u32text, tree_builder );
        case specs::Context::HTML5:
            return parseHTML( u32text, tree_builder );
        case specs::Context::MARKDOWN:
            return parseMarkdown( u32text, tree_builder );
        default:
            throw exception::parsing_failure( "Unimplemented: 'parser::specs::Context::?'" );
    }

    //BLOGATOR => <HTML/> || <HTML></HTML>
    //HTML => {{BLOGATOR}}
    //MarkDown => HTML > {{BLOGATOR}} || {{BLOGATOR > ( <HTML/> || <HTML></HTML> }}
    //CONFIG => PURE (i.e. no break conditions)

    //TODO pre-process the input either here or elsewhere prior - must be able to trigger parsing errors
    //     -> see section [13.2.3.5 Preprocessing the input stream]

    //html.... {{ blogator... }} html...
    //html.... {{ blogator... <html... > blogator.... }} html...
}


bool Parser::parseBlogator( U32Text &u32text, dom::TreeBuilder &tree_builder ) {
    auto blogator_tokeniser = tokeniser::Blogator( tree_builder );
    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
    auto tokens             = Tokens_t();
    auto current_context    = specs::Context::BLOGATOR;

    while( !u32text.reachedEnd() ) {
        switch( current_context ) {
            case specs::Context::BLOGATOR:
//                current_context = blogator_tokeniser.parse( u32text, tokens, current_context );

                if( current_context == specs::Context::BLOGATOR && !blogator_tokeniser.finished() ) {
                    //TODO LOG error - BLOGATOR namespace not closed (file line:col of opening token)
                }
                break;

            case specs::Context::HTML5:
                current_context = html5_tokeniser.parse( u32text, current_context );
                break;

            default:
                //TODO log context
                throw exception::parsing_failure( "Context returned not supported with HTML." );
        }
    }

    return true; //TODO
}

bool Parser::parseHTML( U32Text &u32text, dom::TreeBuilder &tree_builder ) {
    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
    auto blogator_tokeniser = tokeniser::Blogator( tree_builder );
    auto current_context    = specs::Context::HTML5;

    while( !u32text.reachedEnd() ) {
        switch( current_context ) {
            case specs::Context::BLOGATOR:
//                current_context = blogator_tokeniser.parse( u32text, current_context );

                if( current_context == specs::Context::BLOGATOR && !blogator_tokeniser.finished() ) {
                    //TODO LOG error - BLOGATOR namespace not closed (file line:col of opening token)
                }
                break;

            case specs::Context::HTML5:
                current_context = html5_tokeniser.parse( u32text, current_context );
                break;

            default:
                //TODO log context
                throw exception::parsing_failure( "Context returned not supported with HTML." );
        }
    }

    return true; //TODO
}

bool Parser::parseMarkdown( U32Text &u32text, dom::TreeBuilder &tree_builder ) {
    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
    auto blogator_tokeniser = tokeniser::Blogator( tree_builder );
    auto markdown_tokeniser = tokeniser::Markdown( tree_builder );
    auto tokens             = Tokens_t();
    auto current_context    = specs::Context::MARKDOWN;

    while( !u32text.reachedEnd() ) {
        switch( current_context ) {
            case specs::Context::BLOGATOR:
//                current_context = blogator_tokeniser.parse( u32text, current_context );
                break;

            case specs::Context::HTML5:
                current_context = html5_tokeniser.parse( u32text, current_context );
                break;

            case specs::Context::MARKDOWN:
//                current_context = markdown_tokeniser.parse( u32text, tokens, current_context );
                break;

            default:
                //TODO log context
                throw exception::parsing_failure( "Context returned not supported with HTML." );
        }
    }

    return true; //TODO
}