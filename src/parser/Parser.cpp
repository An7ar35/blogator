#include "Parser.h"

#include <cctype>

#include "../logger/Logger.h"
#include "../exception/parsing_failure.h"
#include "../reporter/ParseReporter.h"
#include "../encoding/Transcode.h"
#include "tokeniser/Native.h"
#include "tokeniser/HTML5.h"
#include "tokeniser/Markdown.h"
#include "dom/node/Document.h"

using namespace blogator::parser;

/**
 * Parse stream
 * @param source Source byte stream
 * @param ctx Starting context
 * @return DOM
 * @throws exception::parsing_failure when the process failed
 */
std::unique_ptr<dom::node::Document> Parser::parse( encoding::Source &source, reporter::Context ctx ) {
//    auto codepoints = std::vector<uint32_t>();
//
//    if( !encoding::Transcode::convert( source, codepoints ) ) {
//        //TODO log error
//        throw exception::parsing_failure( "Failed transcoding." );
//    }
//
//    auto u32text      = U32Text( codepoints );
//    auto tree_builder = dom::TreeBuilder();
//
//    if( !parse( u32text, tree_builder, ctx ) ) {
//        //TODO error
//        throw exception::parsing_failure( "Failed parsing." );
//    }
//
//    return std::move( tree_builder.reset() );
    return std::make_unique<dom::node::Document>();
}

///**
// * Parser a UTF-32 encoded text
// * @param u32text UTF-32 encoded text
// * @param tree_builder DOM tree builder instance
// * @param ctx Starting context
// * @return Success
// */
//bool Parser::parse( U32Text &u32text, dom::TreeBuilder &tree_builder, reporter::Context ctx ) {
//    switch( ctx ) {
//        case reporter::Context::UNKNOWN:
//            return parseHTML( u32text, tree_builder );
//        case reporter::Context::NATIVE:
//            return parseNative( u32text, tree_builder );
//        case reporter::Context::HTML5:
//            return parseHTML( u32text, tree_builder );
//        case reporter::Context::MARKDOWN:
//            return parseMarkdown( u32text, tree_builder );
//        default:
//            throw exception::parsing_failure( "Unimplemented: 'parser::reporter::Context::?'" );
//    }
//
//    //BLOGATOR => <HTML/> || <HTML></HTML>
//    //HTML => {{BLOGATOR}}
//    //MarkDown => HTML > {{BLOGATOR}} || {{BLOGATOR > ( <HTML/> || <HTML></HTML> }}
//    //CONFIG => PURE (i.e. no break conditions)
//
//    //TODO pre-process the input either here or elsewhere prior - must be able to trigger parsing errors
//    //     -> see section [13.2.3.5 Preprocessing the input stream]
//
//    //html.... {{ blogator... }} html...
//    //html.... {{ blogator... <html... > blogator.... }} html...
//}
//
//
//bool Parser::parseNative( U32Text &u32text, dom::TreeBuilder &tree_builder ) {
//    auto blogator_tokeniser = tokeniser::Native( tree_builder );
//    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
//    auto tokens             = Tokens_t();
//    auto current_context    = reporter::Context::NATIVE;
//
//    while( !u32text.reachedEnd() ) {
//        switch( current_context ) {
//            case reporter::Context::NATIVE:
////                current_context = blogator_tokeniser.parse( u32text, tokens, current_context );
//
//                if( current_context == reporter::Context::NATIVE && !blogator_tokeniser.finished() ) {
//                    //TODO LOG error - BLOGATOR namespace not closed (file line:col of opening token)
//                }
//                break;
//
//            case reporter::Context::HTML5:
//                current_context = html5_tokeniser.parse( u32text, current_context );
//                break;
//
//            default:
//                //TODO log context
//                throw exception::parsing_failure( "Context returned not supported with HTML." );
//        }
//    }
//
//    return true; //TODO
//}
//
//bool Parser::parseHTML( U32Text &u32text, dom::TreeBuilder &tree_builder ) {
//    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
//    auto blogator_tokeniser = tokeniser::Native( tree_builder );
//    auto current_context    = reporter::Context::HTML5;
//
//    while( !u32text.reachedEnd() ) {
//        switch( current_context ) {
//            case reporter::Context::NATIVE:
////                current_context = blogator_tokeniser.parse( u32text, current_context );
//
//                if( current_context == reporter::Context::NATIVE && !blogator_tokeniser.finished() ) {
//                    //TODO LOG error - BLOGATOR namespace not closed (file line:col of opening token)
//                }
//                break;
//
//            case reporter::Context::HTML5:
//                current_context = html5_tokeniser.parse( u32text, current_context );
//                break;
//
//            default:
//                //TODO log context
//                throw exception::parsing_failure( "Context returned not supported with HTML." );
//        }
//    }
//
//    return true; //TODO
//}
//
//bool Parser::parseMarkdown( U32Text &u32text, dom::TreeBuilder &tree_builder ) {
//    auto html5_tokeniser    = tokeniser::HTML5( tree_builder );
//    auto blogator_tokeniser = tokeniser::Native( tree_builder );
//    auto markdown_tokeniser = tokeniser::Markdown( tree_builder );
//    auto tokens             = Tokens_t();
//    auto current_context    = reporter::Context::MARKDOWN;
//
//    while( !u32text.reachedEnd() ) {
//        switch( current_context ) {
//            case reporter::Context::NATIVE:
////                current_context = blogator_tokeniser.parse( u32text, current_context );
//                break;
//
//            case reporter::Context::HTML5:
//                current_context = html5_tokeniser.parse( u32text, current_context );
//                break;
//
//            case reporter::Context::MARKDOWN:
////                current_context = markdown_tokeniser.parse( u32text, tokens, current_context );
//                break;
//
//            default:
//                //TODO log context
//                throw exception::parsing_failure( "Context returned not supported with HTML." );
//        }
//    }
//
//    return true; //TODO
//}