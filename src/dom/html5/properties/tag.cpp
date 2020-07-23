#include "tag.h"

#include <functional>
#include <cwctype>

/**
 * Generates the Tag->{TagStructure, tag str} lookup map
 * @return Lookup map
 */
std::unordered_map<blogator::dom::html5::Tag, blogator::dom::dto::TagSpecs>
    blogator::dom::html5::properties::tag::createTag2StrMap()
{
    return std::unordered_map<Tag, dto::TagSpecs> {
        { Tag::NONE,       { TagStructure::UNPAIRED, NameSpace::BLOGATOR, U"" } }, //Blogator specific tag for internal use _only_
        { Tag::A,          { TagStructure::PAIRED,   NameSpace::HTML5,    U"a" } },
        { Tag::ABBR,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"abbr" } },
        { Tag::ADDRESS,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"address" } },
        { Tag::AREA,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"area" } },
        { Tag::ARTICLE,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"article" } },
        { Tag::ASIDE,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"aside" } },
        { Tag::AUDIO,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"audio" } },
        { Tag::B,          { TagStructure::PAIRED,   NameSpace::HTML5,    U"b" } },
        { Tag::BASE,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"base" } },
        { Tag::BDI,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"bdi" } },
        { Tag::BDO,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"bdo" } },
        { Tag::BLOCKQUOTE, { TagStructure::PAIRED,   NameSpace::HTML5,    U"blockquote" } },
        { Tag::BODY,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"body" } },
        { Tag::BR,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"br" } },
        { Tag::BUTTON,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"button" } },
        { Tag::CANVAS,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"canvas" } },
        { Tag::CAPTION,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"caption" } },
        { Tag::CITE,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"cite" } },
        { Tag::CODE,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"code" } },
        { Tag::COL,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"col" } },
        { Tag::COLGROUP,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"colgroup" } },
        { Tag::COMMENT,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"!--" } }, //EDGE CASE (ends with '-->')
        { Tag::DATA,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"data" } },
        { Tag::DATALIST,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"datalist" } },
        { Tag::DD,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"dd" } },
        { Tag::DEL,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"del" } },
        { Tag::DETAILS,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"details" } },
        { Tag::DFN,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"dfn" } },
        { Tag::DIALOG,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"dialog" } },
        { Tag::DIV,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"div" } },
        { Tag::DL,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"dl" } },
        { Tag::DOCTYPE,    { TagStructure::UNPAIRED, NameSpace::HTML5,    U"!DOCTYPE" } },
        { Tag::DT,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"dt" } },
        { Tag::EM,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"em" } },
        { Tag::EMBED,      { TagStructure::UNPAIRED, NameSpace::HTML5,    U"embed" } },
        { Tag::FIELDSET,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"fieldset" } },
        { Tag::FIGCAPTION, { TagStructure::PAIRED,   NameSpace::HTML5,    U"figcaption" } },
        { Tag::FIGURE,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"figure" } },
        { Tag::FOOTER,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"footer" } },
        { Tag::FORM,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"form" } },
        { Tag::H1,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"h1" } },
        { Tag::H2,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"h2" } },
        { Tag::H3,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"h3" } },
        { Tag::H4,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"h4" } },
        { Tag::H5,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"h5" } },
        { Tag::H6,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"h6" } },
        { Tag::HEAD,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"head" } },
        { Tag::HEADER,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"header" } },
        { Tag::HR,         { TagStructure::UNPAIRED, NameSpace::HTML5,    U"hr" } },
        { Tag::HTML,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"html" } },
        { Tag::I,          { TagStructure::PAIRED,   NameSpace::HTML5,    U"i" } },
        { Tag::IFRAME,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"iframe" } },
        { Tag::IMG,        { TagStructure::UNPAIRED, NameSpace::HTML5,    U"img" } },
        { Tag::INPUT,      { TagStructure::UNPAIRED, NameSpace::HTML5,    U"input" } },
        { Tag::INS,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"ins" } },
        { Tag::KBD,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"kbd" } },
        { Tag::LABEL,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"label" } },
        { Tag::LEGEND,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"legend" } },
        { Tag::LI,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"li" } },
        { Tag::LINK,       { TagStructure::UNPAIRED, NameSpace::HTML5,    U"link" } },
        { Tag::MAIN,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"main" } },
        { Tag::MAP,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"map" } },
        { Tag::MARK,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"mark" } },
        { Tag::MATH,       { TagStructure::PAIRED,   NameSpace::MATHML,   U"math" } },
        { Tag::META,       { TagStructure::UNPAIRED, NameSpace::HTML5,    U"meta" } },
        { Tag::METER,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"meter" } },
        { Tag::NAV,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"nav" } },
        { Tag::NOSCRIPT,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"noscript" } },
        { Tag::OBJECT,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"object" } },
        { Tag::OL,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"ol" } },
        { Tag::OPTGROUP,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"optgroup" } },
        { Tag::OPTION,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"option" } },
        { Tag::OUTPUT,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"output" } },
        { Tag::P,          { TagStructure::PAIRED,   NameSpace::HTML5,    U"p" } },
        { Tag::PARAM,      { TagStructure::UNPAIRED, NameSpace::HTML5,    U"param" } },
        { Tag::PICTURE,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"picture" } },
        { Tag::PRE,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"pre" } },
        { Tag::PROGRESS,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"progress" } },
        { Tag::Q,          { TagStructure::PAIRED,   NameSpace::HTML5,    U"q" } },
        { Tag::RP,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"rp" } },
        { Tag::RT,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"rt" } },
        { Tag::RUBY,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"ruby" } },
        { Tag::S,          { TagStructure::PAIRED,   NameSpace::HTML5,    U"s" } },
        { Tag::SAMP,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"samp" } },
        { Tag::SCRIPT,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"script" } },
        { Tag::SECTION,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"section" } },
        { Tag::SELECT,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"select" } },
        { Tag::SMALL,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"small" } },
        { Tag::SOURCE,     { TagStructure::UNPAIRED, NameSpace::HTML5,    U"source" } },
        { Tag::SPAN,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"span" } },
        { Tag::STRONG,     { TagStructure::PAIRED,   NameSpace::HTML5,    U"strong" } },
        { Tag::STYLE,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"style" } },
        { Tag::SUB,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"sub" } },
        { Tag::SUMMARY,    { TagStructure::PAIRED,   NameSpace::HTML5,    U"summary" } },
        { Tag::SUP,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"sup" } },
        { Tag::SVG,        { TagStructure::PAIRED,   NameSpace::SVG,      U"svg" } },
        { Tag::TABLE,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"table" } },
        { Tag::TBODY,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"tbody" } },
        { Tag::TD,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"td" } },
        { Tag::TEMPLATE,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"template" } },
        { Tag::TEXTAREA,   { TagStructure::PAIRED,   NameSpace::HTML5,    U"textarea" } },
        { Tag::TFOOT,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"tfoot" } },
        { Tag::TH,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"th" } },
        { Tag::THEAD,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"thead" } },
        { Tag::TIME,       { TagStructure::PAIRED,   NameSpace::HTML5,    U"time" } },
        { Tag::TITLE,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"title" } },
        { Tag::TR,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"tr" } },
        { Tag::TRACK,      { TagStructure::UNPAIRED, NameSpace::HTML5,    U"track" } },
        { Tag::U,          { TagStructure::PAIRED,   NameSpace::HTML5,    U"u" } },
        { Tag::UL,         { TagStructure::PAIRED,   NameSpace::HTML5,    U"ul" } },
        { Tag::VAR,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"var" } },
        { Tag::VIDEO,      { TagStructure::PAIRED,   NameSpace::HTML5,    U"video" } },
        { Tag::WBR,        { TagStructure::PAIRED,   NameSpace::HTML5,    U"wbr" } }
    };
}

/**
 * Generates the string->Tag lookup map from the Tag->string map
 * @param tag2str Tag->string map
 * @return Lookup map
 */
std::unordered_map<std::u32string, blogator::dom::html5::Tag> blogator::dom::html5::properties::tag::createStr2TagMap(
    const std::unordered_map<Tag, dto::TagSpecs> &tag2str )
{
    auto map = std::unordered_map<std::u32string, Tag>();

    std::for_each(
        tag2str.cbegin(),
        tag2str.cend(),
        [&]( const auto &pair ) {
            if( pair.first == Tag::DOCTYPE ) { //edge case -> just to keep XML correctness
                auto str = pair.second.tag_str;
                std::transform( str.begin(), str.end(), str.begin(), []( char32_t c ){ return std::towlower( c ); } );
                map.emplace( str, pair.first );
            } else {
                map.emplace( pair.second.tag_str, pair.first );
            }
        }
    );

    return map;
}