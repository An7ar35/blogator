#include "NativeTk.h"

#include "../../../exception/parsing_failure.h"

using namespace blogator::parser::token::native;
using namespace blogator::parser::specs::native;

/**
 * Constructor
 * @param ns Context namespace
 * @param element Element type
 * @param position Line:Col position of token
 */
NativeTk::NativeTk( specs::Context ns, specs::native::Element element, TextPos position ) :
    Token( ns, position ),
    _element( element )
{}

/**
 * Constructor
 * @param ns Context namespace
 * @param element Element type
 * @param text UTF32 Text
 * @param position Line:Col position of token
 */
NativeTk::NativeTk( specs::Context ns, specs::native::Element element, std::u32string text, TextPos position ) :
    Token( ns, std::move( text ), position ),
    _element( element )
{}

/**
 * Gets the Blogator element type
 * @return Blogator element
 */
Element NativeTk::element() const {
    return _element;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void NativeTk::toStr( std::ostream &os ) const {
    os << "NativeTk={ element: ";
    specs::native::Specifications::printElement( os, element() );
    os << ", text: \"";
    unicode::utf8::convert( os, text() );
    os << "\", position: " << lineNum() << ":" << colPos() << " }";
}


///**
// * Parse a Blogator token
// * @param u32text Source UTF32 text
// * @param iterator Text iterator
// * @param curr_pos Current line:col position in the text
// * @return Token
// * @throws parsing_failure if parsing fails
// */
//Blogator Blogator::parseToken( const std::u32string &u32text, std::u32string::const_iterator &iterator, TextPos &curr_pos ) {
//    if( iterator == u32text.cend() ) {
//        //TODO log
//        throw exception::parsing_failure( "[parser::tokeniser::Blogator::parseToken(..)] Iterator is already at end of text." );
//    }
//
//    using blogator::parser::specs::blogator::Element;
//
//    Element    element   = Element::UNKNOWN;
//    TextPos    start_pos = curr_pos;
//    const auto start_it  = iterator;
//
//    if( *iterator == '{' ) { //'{{'
//        auto fwd_iterator = std::next( iterator, 1 );
//
//        if( fwd_iterator != u32text.cend() && *fwd_iterator == '{' ) {
//            std::advance( iterator, 2 );
//            element       = Element::BEGIN;
//            curr_pos.col += 2;
//        }
//
//    } else if( *iterator == '}' ) { //'}}'
//        auto fwd_iterator = std::next( iterator, 1 );
//
//        if( fwd_iterator != u32text.cend() && *fwd_iterator == '}' ) {
//            std::advance( iterator, 2 );
//            element       = Element::END;
//            curr_pos.col += 2;
//        }
//
//    } else {
//        while( iterator != u32text.cend() ) {
//            if( unicode::ascii::isdigit( *iterator ) ) {
//
//            } else if( unicode::ascii::isalpha( *iterator ))
//        }
//
//        if( unicode::ascii::isalnum( *iterator ) ) {
//            auto fwd_iterator = std::next( iterator, 1 );
//
//
//
//
//            //TODO iterate through ASCII text for 'BLOGATOR::*' and check if matches element
//        }
//    }
//
//    return { specs::Context::BLOGATOR, element, std::u32string( start_it, iterator - 1 ), start_pos.line, start_pos.col };
//}

