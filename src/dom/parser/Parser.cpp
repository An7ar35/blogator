#include "Parser.h"

#include "../../exception/DOMException.h"

//void blogator::dom::parser::Parser::init( const parser::Tokeniser &tokeniser ) {
////    auto dot = std::make_unique<DOT>();
//    auto token = tokeniser.cbegin();
//
//    while( token != tokeniser.cend() ) {
//        switch( token->type ) {
//            case TokenType::START_TAG: {
////                parseTag( token, )
//                }
//                break;
//
//            case TokenType::END_TAG: {
//
//                }
//                break;
//
//            case TokenType::TEXT: {
//
//                }
//                break;
//        }
//    }
//
//
//}

/**
 * Parse TAG token
 * @param parent Parent DOTNode
 * @param token  Token to parse
 * @return Newly created DOTNode
 * @throws std::invalid_argument when tag string is not valid
 */
blogator::dom::DOTNode & blogator::dom::parser::Parser::parseTag( const parser::Token &token,
                                                                  blogator::dom::DOTNode &parent )
{

    auto it = token.content.cbegin();

    if( it != token.content.cend() && *it == html5::special_char::LESS_THAN )
        ++it;

    if( it != token.content.cend() && *it == html5::special_char::SOLIDUS ) {
        //closing tag, check tag type and return parent of opening tag
    }

    u32stringstream_t utf32_ss;

    while( it != token.content.cend() && iswspace( *it ) ) {
        utf32_ss << *it;
    }

    //TODO attributes

    return parent.addChild(
        std::make_unique<DOTNode>( html5::Rules::getInstance().strToTag( utf32_ss.str() ) )
    );
}

blogator::dom::DOTNode & blogator::dom::parser::Parser::parseText( const parser::Token &token,
                                                                   blogator::dom::DOTNode &parent )
{ //TODO maybe concatenate continuous text content into 1 node?
    return parent.addChild( std::make_unique<DOTNode>( html5::Tag::NONE, token.content ) );
}

/**
 * Parse an opening tag
 * @param str        Opening tag string (w/o attributes)
 * @param attributes Attribute store for the Tag
 * @return HTML5 Tag type
 * @throws std::invalid_argument when given string cannot be converted to a valid HTML5 tag type
 */
blogator::dom::html5::Tag blogator::dom::parser::Parser::parseOpeningTag( const std::u32string &str,
                                                                          std::unordered_map<std::u32string, std::u32string> &attributes )
{
    auto it = str.begin();
    
    return html5::Rules::getInstance().strToTag( str );
}

/**
 * Parse a closing tag
 * @param str Closing tag string
 * @return HTML5 Tag type
 * @throws std::invalid_argument when given string cannot be converted to a valid HTML5 tag type
 */
blogator::dom::html5::Tag blogator::dom::parser::Parser::parseClosingTag( const std::u32string &str ) {
    auto it = str.cbegin();

    while( it != str.cend() && ( *it == html5::special_char::LESS_THAN || *it == html5::special_char::SOLIDUS ) )
        ++it;

    u32stringstream_t utf32_ss;

    while( it != str.cend() && ( *it != html5::special_char::GREATER_THAN || !iswspace( *it ) ) ) {
        utf32_ss << *it;
        ++it;
    }

    //TODO check that there is only space before the '>' char --> ERROR CONTROL
    return html5::Rules::getInstance().strToTag( utf32_ss.str() );


//    try {
//
//
//    } catch( std::invalid_argument &e ) {
//        std::stringstream ss;
//        ss << "blogator::dom::parser::Parser::parseClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )] "
//           << "Invalid HTML5 tag".
//
//        throw exception::DOMException( "", exception::DOMErrorType::SyntaxError );
//    }
}

