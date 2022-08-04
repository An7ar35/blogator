#include "HtmlBlockType.h"

#include <set>

using namespace blogator::parser::specs::markdown;

/**
 * Output stream operator
 * @param os Output stream
 * @param attr HtmlBlockType enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, HtmlBlockType type ) {
    if( type == HtmlBlockType::UNKNOWN ) {
        os << "UNKNOWN";
    } else {
        using T = std::underlying_type_t<HtmlBlockType>;
        os << std::to_string( static_cast<T>( type ) );
    }

    return os;
}

/**
 * Checks if a tag name matches a type 1 HTML block start condition
 * @param tag_name Tag name string (lowercase)
 * @return Type 1 start condition state
 */
bool blogator::parser::specs::markdown::isHtmlBlockType1( const std::u32string &tag_name ) {
    static const auto TYPE_1 = std::set<std::u32string>( {
        U"pre",
        U"script",
        U"style",
        U"textarea",
    } );

    return TYPE_1.contains( tag_name );
}

/**
 * Checks if a tag name matches a type 6 HTML block start condition
 * @param tag_name Tag name string (lowercase)
 * @return Type 6 start condition state
 */
bool blogator::parser::specs::markdown::isHtmlBlockType6( const std::u32string &tag_name ) {
    static const auto TYPE_6 = std::set<std::u32string>( {
        U"address", U"article", U"aside",
        U"base", U"basefont", U"blockquote", U"body",
        U"caption", U"center", U"col", U"colgroup",
        U"dd", U"details", U"dialog", U"dir", U"div", U"dl", U"dt",
        U"fieldset", U"figcaption", U"figure", U"footer", U"form", U"frame", U"frameset",
        U"h1", U"h2", U"h3", U"h4", U"h5", U"h6", U"head", U"header", U"hr", U"html",
        U"iframe",
        U"legend", U"li", U"link",
        U"main", U"menu", U"menuitem",
        U"nav", U"noframes",
        U"ol", U"optgroup", U"option",
        U"p", U"param",
        U"section", U"source",
        U"summary",
        U"table", U"tbody", U"td", U"tfoot", U"th", U"thead", U"title", U"tr", U"track",
        U"ul",
    } );

    return TYPE_6.contains( tag_name );
}
