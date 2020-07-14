#include "Specs.h"

#include <stdexcept>
#include <sstream>
#include <algorithm>

#include "../../encoding/encoding.h"

const std::unordered_map<blogator::dom::html5::Tag, blogator::dom::html5::Specs::TagSpecs> blogator::dom::html5::Specs::_tag2str_map = {
    //Blogator specific tag for internal use _only_
    { Tag::NONE,       { TagStructure::UNPAIRED, U"" } },

    { Tag::A,          { TagStructure::PAIRED,   U"a" } },
    { Tag::ABBR,       { TagStructure::PAIRED,   U"abbr" } },
    { Tag::ADDRESS,    { TagStructure::PAIRED,   U"address" } },
    { Tag::AREA,       { TagStructure::PAIRED,   U"area" } },
    { Tag::ARTICLE,    { TagStructure::PAIRED,   U"article" } },
    { Tag::ASIDE,      { TagStructure::PAIRED,   U"aside" } },
    { Tag::AUDIO,      { TagStructure::PAIRED,   U"audio" } },
    { Tag::B,          { TagStructure::PAIRED,   U"b" } },
    { Tag::BASE,       { TagStructure::PAIRED,   U"base" } },
    { Tag::BDI,        { TagStructure::PAIRED,   U"bdi" } },
    { Tag::BDO,        { TagStructure::PAIRED,   U"bdo" } },
    { Tag::BLOCKQUOTE, { TagStructure::PAIRED,   U"blockquote" } },
    { Tag::BODY,       { TagStructure::PAIRED,   U"body" } },
    { Tag::BR,         { TagStructure::PAIRED,   U"br" } },
    { Tag::BUTTON,     { TagStructure::PAIRED,   U"button" } },
    { Tag::CANVAS,     { TagStructure::PAIRED,   U"canvas" } },
    { Tag::CAPTION,    { TagStructure::PAIRED,   U"caption" } },
    { Tag::CITE,       { TagStructure::PAIRED,   U"cite" } },
    { Tag::CODE,       { TagStructure::PAIRED,   U"code" } },
    { Tag::COL,        { TagStructure::PAIRED,   U"col" } },
    { Tag::COLGROUP,   { TagStructure::PAIRED,   U"colgroup" } },
    { Tag::COMMENT,    { TagStructure::UNPAIRED, U"!--" } }, //EDGE CASE (ends with '-->')
    { Tag::DATA,       { TagStructure::PAIRED,   U"data" } },
    { Tag::DATALIST,   { TagStructure::PAIRED,   U"datalist" } },
    { Tag::DD,         { TagStructure::PAIRED,   U"dd" } },
    { Tag::DEL,        { TagStructure::PAIRED,   U"del" } },
    { Tag::DETAILS,    { TagStructure::PAIRED,   U"details" } },
    { Tag::DFN,        { TagStructure::PAIRED,   U"dfn" } },
    { Tag::DIALOG,     { TagStructure::PAIRED,   U"dialog" } },
    { Tag::DIV,        { TagStructure::PAIRED,   U"div" } },
    { Tag::DL,         { TagStructure::PAIRED,   U"dl" } },
    { Tag::DOCTYPE,    { TagStructure::UNPAIRED, U"!DOCTYPE" } },
    { Tag::DT,         { TagStructure::PAIRED,   U"dt" } },
    { Tag::EM,         { TagStructure::PAIRED,   U"em" } },
    { Tag::EMBED,      { TagStructure::UNPAIRED, U"embed" } },
    { Tag::FIELDSET,   { TagStructure::PAIRED,   U"fieldset" } },
    { Tag::FIGCAPTION, { TagStructure::PAIRED,   U"figcaption" } },
    { Tag::FIGURE,     { TagStructure::PAIRED,   U"figure" } },
    { Tag::FOOTER,     { TagStructure::PAIRED,   U"footer" } },
    { Tag::FORM,       { TagStructure::PAIRED,   U"form" } },
    { Tag::H1,         { TagStructure::PAIRED,   U"h1" } },
    { Tag::H2,         { TagStructure::PAIRED,   U"h2" } },
    { Tag::H3,         { TagStructure::PAIRED,   U"h3" } },
    { Tag::H4,         { TagStructure::PAIRED,   U"h4" } },
    { Tag::H5,         { TagStructure::PAIRED,   U"h5" } },
    { Tag::H6,         { TagStructure::PAIRED,   U"h6" } },
    { Tag::HEAD,       { TagStructure::PAIRED,   U"head" } },
    { Tag::HEADER,     { TagStructure::PAIRED,   U"header" } },
    { Tag::HR,         { TagStructure::UNPAIRED, U"hr" } },
    { Tag::HTML,       { TagStructure::PAIRED,   U"html" } },
    { Tag::I,          { TagStructure::PAIRED,   U"i" } },
    { Tag::IFRAME,     { TagStructure::PAIRED,   U"iframe" } },
    { Tag::IMG,        { TagStructure::UNPAIRED, U"img" } },
    { Tag::INPUT,      { TagStructure::UNPAIRED, U"input" } },
    { Tag::INS,        { TagStructure::PAIRED,   U"ins" } },
    { Tag::KBD,        { TagStructure::PAIRED,   U"kbd" } },
    { Tag::LABEL,      { TagStructure::PAIRED,   U"label" } },
    { Tag::LEGEND,     { TagStructure::PAIRED,   U"legend" } },
    { Tag::LI,         { TagStructure::PAIRED,   U"li" } },
    { Tag::LINK,       { TagStructure::PAIRED,   U"link" } },
    { Tag::MAIN,       { TagStructure::PAIRED,   U"main" } },
    { Tag::MAP,        { TagStructure::PAIRED,   U"map" } },
    { Tag::MARK,       { TagStructure::PAIRED,   U"mark" } },
    { Tag::META,       { TagStructure::UNPAIRED, U"meta" } },
    { Tag::METER,      { TagStructure::PAIRED,   U"meter" } },
    { Tag::NAV,        { TagStructure::PAIRED,   U"nav" } },
    { Tag::NOSCRIPT,   { TagStructure::PAIRED,   U"noscript" } },
    { Tag::OBJECT,     { TagStructure::PAIRED,   U"object" } },
    { Tag::OL,         { TagStructure::PAIRED,   U"ol" } },
    { Tag::OPTGROUP,   { TagStructure::PAIRED,   U"optgroup" } },
    { Tag::OPTION,     { TagStructure::PAIRED,   U"option" } },
    { Tag::OUTPUT,     { TagStructure::PAIRED,   U"output" } },
    { Tag::P,          { TagStructure::PAIRED,   U"p" } },
    { Tag::PARAM,      { TagStructure::UNPAIRED, U"param" } },
    { Tag::PICTURE,    { TagStructure::PAIRED,   U"picture" } },
    { Tag::PRE,        { TagStructure::PAIRED,   U"pre" } },
    { Tag::PROGRESS,   { TagStructure::PAIRED,   U"progress" } },
    { Tag::Q,          { TagStructure::PAIRED,   U"q" } },
    { Tag::RP,         { TagStructure::PAIRED,   U"rp" } },
    { Tag::RT,         { TagStructure::PAIRED,   U"rt" } },
    { Tag::RUBY,       { TagStructure::PAIRED,   U"ruby" } },
    { Tag::S,          { TagStructure::PAIRED,   U"s" } },
    { Tag::SAMP,       { TagStructure::PAIRED,   U"samp" } },
    { Tag::SCRIPT,     { TagStructure::PAIRED,   U"script" } },
    { Tag::SECTION,    { TagStructure::PAIRED,   U"section" } },
    { Tag::SELECT,     { TagStructure::PAIRED,   U"select" } },
    { Tag::SMALL,      { TagStructure::PAIRED,   U"small" } },
    { Tag::SOURCE,     { TagStructure::UNPAIRED, U"source" } },
    { Tag::SPAN,       { TagStructure::PAIRED,   U"span" } },
    { Tag::STRONG,     { TagStructure::PAIRED,   U"strong" } },
    { Tag::STYLE,      { TagStructure::PAIRED,   U"style" } },
    { Tag::SUB,        { TagStructure::PAIRED,   U"sub" } },
    { Tag::SUMMARY,    { TagStructure::PAIRED,   U"summary" } },
    { Tag::SUP,        { TagStructure::PAIRED,   U"sup" } },
    { Tag::SVG,        { TagStructure::PAIRED,   U"svg" } },
    { Tag::TABLE,      { TagStructure::PAIRED,   U"table" } },
    { Tag::TBODY,      { TagStructure::PAIRED,   U"tbody" } },
    { Tag::TD,         { TagStructure::PAIRED,   U"td" } },
    { Tag::TEMPLATE,   { TagStructure::PAIRED,   U"template" } },
    { Tag::TEXTAREA,   { TagStructure::PAIRED,   U"textarea" } },
    { Tag::TFOOT,      { TagStructure::PAIRED,   U"tfoot" } },
    { Tag::TH,         { TagStructure::PAIRED,   U"th" } },
    { Tag::THEAD,      { TagStructure::PAIRED,   U"thead" } },
    { Tag::TIME,       { TagStructure::PAIRED,   U"time" } },
    { Tag::TITLE,      { TagStructure::PAIRED,   U"title" } },
    { Tag::TR,         { TagStructure::PAIRED,   U"tr" } },
    { Tag::TRACK,      { TagStructure::UNPAIRED, U"track" } },
    { Tag::U,          { TagStructure::PAIRED,   U"u" } },
    { Tag::UL,         { TagStructure::PAIRED,   U"ul" } },
    { Tag::VAR,        { TagStructure::PAIRED,   U"var" } },
    { Tag::VIDEO,      { TagStructure::PAIRED,   U"video" } },
    { Tag::WBR,        { TagStructure::PAIRED,   U"wbr" } }
};

const std::unordered_map<std::u32string, blogator::dom::html5::Tag> blogator::dom::html5::Specs::_str2tag_map = {
    { U"a",          html5::Tag::A },
    { U"abbr",       html5::Tag::ABBR },
    { U"address",    html5::Tag::ADDRESS },
    { U"area",       html5::Tag::AREA },
    { U"article",    html5::Tag::ARTICLE },
    { U"aside",      html5::Tag::ASIDE },
    { U"audio",      html5::Tag::AUDIO },
    { U"b",          html5::Tag::B },
    { U"base",       html5::Tag::BASE },
    { U"bdi",        html5::Tag::BDI },
    { U"bdo",        html5::Tag::BDO },
    { U"blockquote", html5::Tag::BLOCKQUOTE },
    { U"body",       html5::Tag::BODY },
    { U"br",         html5::Tag::BR },
    { U"button",     html5::Tag::BUTTON },
    { U"canvas",     html5::Tag::CANVAS },
    { U"caption",    html5::Tag::CAPTION },
    { U"cite",       html5::Tag::CITE },
    { U"code",       html5::Tag::CODE },
    { U"col",        html5::Tag::COL },
    { U"colgroup",   html5::Tag::COLGROUP },
    { U"!--",        html5::Tag::COMMENT },
    { U"data",       html5::Tag::DATA, },
    { U"datalist",   html5::Tag::DATALIST },
    { U"dd",         html5::Tag::DD },
    { U"del",        html5::Tag::DEL },
    { U"details",    html5::Tag::DETAILS },
    { U"dfn",        html5::Tag::DFN },
    { U"dialog",     html5::Tag::DIALOG },
    { U"div",        html5::Tag::DIV },
    { U"dl",         html5::Tag::DL },
    { U"!doctype",   html5::Tag::DOCTYPE },
    { U"dt",         html5::Tag::DT },
    { U"em",         html5::Tag::EM },
    { U"embed",      html5::Tag::EMBED },
    { U"fieldset",   html5::Tag::FIELDSET },
    { U"figcaption", html5::Tag::FIGCAPTION },
    { U"figure",     html5::Tag::FIGURE },
    { U"footer",     html5::Tag::FOOTER },
    { U"form",       html5::Tag::FORM },
    { U"h1",         html5::Tag::H1 },
    { U"h2",         html5::Tag::H2 },
    { U"h3",         html5::Tag::H3 },
    { U"h4",         html5::Tag::H4 },
    { U"h5",         html5::Tag::H5 },
    { U"h6",         html5::Tag::H6 },
    { U"head",       html5::Tag::HEAD },
    { U"header",     html5::Tag::HEADER },
    { U"hr",         html5::Tag::HR },
    { U"html",       html5::Tag::HTML },
    { U"i",          html5::Tag::I },
    { U"iframe",     html5::Tag::IFRAME },
    { U"img",        html5::Tag::IMG },
    { U"input",      html5::Tag::INPUT },
    { U"ins",        html5::Tag::INS },
    { U"kbd",        html5::Tag::KBD },
    { U"label",      html5::Tag::LABEL },
    { U"legend",     html5::Tag::LEGEND },
    { U"li",         html5::Tag::LI },
    { U"link",       html5::Tag::LINK },
    { U"main",       html5::Tag::MAIN },
    { U"map",        html5::Tag::MAP },
    { U"mark",       html5::Tag::MARK },
    { U"meta",       html5::Tag::META },
    { U"meter",      html5::Tag::METER },
    { U"nav",        html5::Tag::NAV },
    { U"noscript",   html5::Tag::NOSCRIPT },
    { U"object",     html5::Tag::OBJECT },
    { U"ol",         html5::Tag::OL },
    { U"optgroup",   html5::Tag::OPTGROUP },
    { U"option",     html5::Tag::OPTION, },
    { U"output",     html5::Tag::OUTPUT },
    { U"p",          html5::Tag::P },
    { U"param",      html5::Tag::PARAM },
    { U"picture",    html5::Tag::PICTURE },
    { U"pre",        html5::Tag::PRE },
    { U"progress",   html5::Tag::PROGRESS },
    { U"q",          html5::Tag::Q },
    { U"rp",         html5::Tag::RP },
    { U"rt",         html5::Tag::RT },
    { U"ruby",       html5::Tag::RUBY },
    { U"s",          html5::Tag::S },
    { U"samp",       html5::Tag::SAMP },
    { U"script",     html5::Tag::SCRIPT },
    { U"section",    html5::Tag::SECTION },
    { U"select",     html5::Tag::SELECT },
    { U"small",      html5::Tag::SMALL },
    { U"source",     html5::Tag::SOURCE },
    { U"span",       html5::Tag::SPAN },
    { U"strong",     html5::Tag::STRONG },
    { U"style",      html5::Tag::STYLE },
    { U"sub",        html5::Tag::SUB },
    { U"summary",    html5::Tag::SUMMARY },
    { U"sup",        html5::Tag::SUP },
    { U"svg",        html5::Tag::SVG },
    { U"table",      html5::Tag::TABLE },
    { U"tbody",      html5::Tag::TBODY },
    { U"td",         html5::Tag::TD },
    { U"template",   html5::Tag::TEMPLATE },
    { U"textarea",   html5::Tag::TEXTAREA },
    { U"tfoot",      html5::Tag::TFOOT },
    { U"th",         html5::Tag::TH },
    { U"thead",      html5::Tag::THEAD },
    { U"time",       html5::Tag::TIME },
    { U"title",      html5::Tag::TITLE },
    { U"tr",         html5::Tag::TR },
    { U"track",      html5::Tag::TRACK },
    { U"u",          html5::Tag::U },
    { U"ul",         html5::Tag::UL },
    { U"var",        html5::Tag::VAR },
    { U"video",      html5::Tag::VIDEO },
    { U"wbr",        html5::Tag::WBR }
};

/**
 * Checks if a Tag requires closing
 * @param tag Tag type
 * @return Closing required
 */
bool blogator::dom::html5::Specs::isPaired( Tag tag ) {
    const auto it = Specs::_tag2str_map.find( tag );

    if( it != Specs::_tag2str_map.cend() ) {
        return it->second.structure == TagStructure::PAIRED;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::Specs::d( Tag ( dom::html5::Tag )] "
              "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Converts a tag enum type to its string representation
 * @param tag Tag type
 * @return Tag type as a string
 */
std::u32string blogator::dom::html5::Specs::tagToStr( Tag tag ) {
    const auto it = Specs::_tag2str_map.find( tag );

    if( it != Specs::_tag2str_map.cend() ) {
        return it->second.tag_str;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::Specs::tagToStr( dom::html5::Tag )] "
              "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Convert a tag string into its enum type
 * @param str HTML5 tag string (case-insensitive)
 * @return Tag type
 * @throws std::invalid_argument when string is not a valid tag
 */
blogator::dom::html5::Tag blogator::dom::html5::Specs::strToTag( std::u32string str ) {
    try {
        std::transform( str.begin(), str.end(), str.begin(), []( char32_t c ){ return std::towlower( c ); } );
        return Specs::_str2tag_map.at( str );

    } catch( std::out_of_range &e ) {
        std::string tag = encoding::encodeToUTF8( str );
        std::stringstream ss;
        ss << "[dom::html5::strToTag( " << tag << " )] "
              "String \"" << tag << "\" does not match a known tag.";
        throw std::invalid_argument( ss.str() );
    }
}