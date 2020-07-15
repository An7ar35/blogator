#include "Rules.h"

#include <algorithm>
#include <iostream>

#include "../../../encoding/encoding.h"

/**
 * Checks if a Tag requires closing
 * @param tag Tag type
 * @return Closing required
 */
bool blogator::dom::html5::Rules::isPaired( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.structure == TagStructure::PAIRED;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::Rules::isPaired( dom::html5::Tag )] "
              "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Converts a tag enum type to its string representation
 * @param tag Tag type
 * @return Tag type as a string
 * @throws std::invalid_argument when tag is not found in lookup map
 */
std::u32string blogator::dom::html5::Rules::tagToStr( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.tag_str;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::Rules::tagToStr( dom::html5::Tag )] "
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
blogator::dom::html5::Tag blogator::dom::html5::Rules::strToTag( std::u32string str ) {
    try {
        std::transform( str.begin(), str.end(), str.begin(), []( char32_t c ){ return std::towlower( c ); } );
        return _str2tag_map.at( str );

    } catch( std::out_of_range &e ) {
        std::string tag = encoding::encodeToUTF8( str );
        std::stringstream ss;
        ss << "[dom::html5::strToTag( " << tag << " )] "
              "String \"" << tag << "\" does not match a known tag.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Converts an attribute enum type to its string representation
 * @param attribute Attribute type
 * @return Attribute name type as a string
 * @throws std::invalid_argument when attribute name is not found in lookup map
 */
std::u32string blogator::dom::html5::Rules::attributeToStr( html5::Attribute attribute ) {
    const auto it = _attr2str_map.find( attribute );

    if( it != _attr2str_map.cend() ) {
        return it->second;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::Rules::attributeToStr( dom::html5::Attribute )] "
              "Attribute (" << static_cast<int>( attribute ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Convert a attribute name string into its enum type
 * @param str HTML5 attribute name string (case-insensitive)
 * @return Attribute type
 * @throws std::invalid_argument when string is not a valid attribute
 */
blogator::dom::html5::Attribute blogator::dom::html5::Rules::strToAttribute( std::u32string str ) {
    try {
        std::transform( str.begin(), str.end(), str.begin(), []( char32_t c ){ return std::towlower( c ); } );
        return _str2attr_map.at( str );

    } catch( std::out_of_range &e ) {
        std::string attr = encoding::encodeToUTF8( str );
        std::stringstream ss;
        ss << "[dom::html5::Rules::strToAttribute( " << attr << " )] "
              "String \"" << attr << "\" does not match a known attribute.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Gets the number of unique tags listed in the internal maps
 * @return Number of unique tags
 */
size_t blogator::dom::html5::Rules::tagCount() const {
    return _tag2str_map.size();
}

/**
 * Gets the number of unique attributes listed in the internal maps
 * @return Number of unique attributes
 */
size_t blogator::dom::html5::Rules::attributeCount() const {
    return _attr2str_map.size();
}

/**
 * [PRIVATE] Constructor
 */
blogator::dom::html5::Rules::Rules() :
    _tag2str_map( Factories::createTag2StrMap() ),
    _str2tag_map( Factories::createStr2TagMap( _tag2str_map ) ),
    _attr2str_map( Factories::createAttr2StrMap() ),
    _str2attr_map( Factories::createStr2AttrMap( _attr2str_map ) ),
    _attr2tag_table( Factories::createAttr2TagLookupTable() )
{
    std::cout << "Rules constructor called!!" << std::endl;
}

/**
 * Generates the Tag->{TagStructure, tag str} lookup map
 * @return Lookup map
 */
std::unordered_map<blogator::dom::html5::Tag, blogator::dom::html5::Rules::TagSpecs> blogator::dom::html5::Rules::Factories::createTag2StrMap() {
    return std::unordered_map<Tag, TagSpecs> {
        { Tag::NONE,       { TagStructure::UNPAIRED, U"" } }, //Blogator specific tag for internal use _only_
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
}

/**
 * Generates the string->Tag lookup map from the Tag->string map
 * @param tag2str Tag->string map
 * @return Lookup map
 */
std::unordered_map<std::u32string, blogator::dom::html5::Tag> blogator::dom::html5::Rules::Factories::createStr2TagMap(
    const std::unordered_map<Tag, TagSpecs> &tag2str )
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

/**
 * Generates the Attribute->string lookup map
 * @return Lookup map
 */
std::unordered_map<blogator::dom::html5::Attribute, std::u32string> blogator::dom::html5::Rules::Factories::createAttr2StrMap() {
    return std::unordered_map<Attribute, std::u32string> {
        { html5::Attribute::ACCEPT,           U"accept" },
        { html5::Attribute::ACCEPT_CHARSET,   U"accept-charset" },
        { html5::Attribute::ACCESSKEY,        U"accesskey" },
        { html5::Attribute::ACTION,           U"action" },
        { html5::Attribute::ALT,              U"alt" },
        { html5::Attribute::ASYNC,            U"async" },
        { html5::Attribute::AUTOCOMPLETE,     U"autocomplete" },
        { html5::Attribute::AUTOFOCUS,        U"autofocus" },
        { html5::Attribute::AUTOPLAY,         U"autoplay" },
        { html5::Attribute::CHARSET,          U"charset" },
        { html5::Attribute::CHECKED,          U"checked" },
        { html5::Attribute::CITE,             U"cite" },
        { html5::Attribute::CLASS,            U"class" },
        { html5::Attribute::COLS,             U"cols" },
        { html5::Attribute::COLSPAN,          U"colspan" },
        { html5::Attribute::CONTENT,          U"content" },
        { html5::Attribute::CONTENTEDITABLE,  U"contenteditable" },
        { html5::Attribute::CONTROLS,         U"controls" },
        { html5::Attribute::COORDS,           U"coords" },
        { html5::Attribute::DATA,             U"data" },
        { html5::Attribute::DATA_X,           U"data-*" }, //edge case //TODO implement conditional exception for this when checking validity of attr.
        { html5::Attribute::DATETIME,         U"datetime" },
        { html5::Attribute::DEFAULT,          U"default" },
        { html5::Attribute::DEFER,            U"defer" },
        { html5::Attribute::DIR,              U"dir" },
        { html5::Attribute::DIRNAME,          U"dirname" },
        { html5::Attribute::DISABLED,         U"disabled" },
        { html5::Attribute::DOWNLOAD,         U"download" },
        { html5::Attribute::DRAGGABLE,        U"draggable" },
        { html5::Attribute::ENCTYPE,          U"enctype" },
        { html5::Attribute::FOR,              U"for" },
        { html5::Attribute::FORM,             U"form" },
        { html5::Attribute::FORMACTION,       U"formaction" },
        { html5::Attribute::HEADERS,          U"headers" },
        { html5::Attribute::HEIGHT,           U"height" },
        { html5::Attribute::HIDDEN,           U"hidden" },
        { html5::Attribute::HIGH,             U"high" },
        { html5::Attribute::HREF,             U"href" },
        { html5::Attribute::HREFLANG,         U"hreflang" },
        { html5::Attribute::HTTP_EQUIV,       U"http-equiv" },
        { html5::Attribute::ID,               U"id" },
        { html5::Attribute::ISMAP,            U"ismap" },
        { html5::Attribute::KIND,             U"kind" },
        { html5::Attribute::LABEL,            U"label" },
        { html5::Attribute::LANG,             U"lang" },
        { html5::Attribute::LIST,             U"list" },
        { html5::Attribute::LOOP,             U"loop" },
        { html5::Attribute::LOW,              U"low" },
        { html5::Attribute::MAX,              U"max" },
        { html5::Attribute::MAXLENGTH,        U"maxlength" },
        { html5::Attribute::MEDIA,            U"media" },
        { html5::Attribute::METHOD,           U"method" },
        { html5::Attribute::MIN,              U"min" },
        { html5::Attribute::MULTIPLE,         U"multiple" },
        { html5::Attribute::MUTED,            U"muted" },
        { html5::Attribute::NAME,             U"name" },
        { html5::Attribute::NOVALIDATE,       U"novalidate" },
        { html5::Attribute::ONABORT,          U"onabort" },
        { html5::Attribute::ONAFTERPRINT,     U"onafterprint" },
        { html5::Attribute::ONBEFOREPRINT,    U"onbeforeprint" },
        { html5::Attribute::ONBEFOREUNLOAD,   U"onbeforeunload" },
        { html5::Attribute::ONBLUR,           U"onblur" },
        { html5::Attribute::ONCANPLAY,        U"oncanplay" },
        { html5::Attribute::ONCANPLAYTHROUGH, U"oncanplaythrough" },
        { html5::Attribute::ONCHANGE,         U"onchange" },
        { html5::Attribute::ONCLICK,          U"onclick" },
        { html5::Attribute::ONCONTEXTMENU,    U"oncontextmenu" },
        { html5::Attribute::ONCOPY,           U"oncopy" },
        { html5::Attribute::ONCUECHANGE,      U"oncuechange" },
        { html5::Attribute::ONCUT,            U"oncut" },
        { html5::Attribute::ONDBLCLICK,       U"ondblclick" },
        { html5::Attribute::ONDRAG,           U"ondrag" },
        { html5::Attribute::ONDRAGEND,        U"ondragend" },
        { html5::Attribute::ONDRAGENTER,      U"ondragcenter" },
        { html5::Attribute::ONDRAGLEAVE,      U"ondragleave" },
        { html5::Attribute::ONDRAGOVER,       U"ondragover" },
        { html5::Attribute::ONDRAGSTART,      U"ondragstart" },
        { html5::Attribute::ONDROP,           U"ondrop" },
        { html5::Attribute::ONDURATIONCHANGE, U"ondurationchange" },
        { html5::Attribute::ONEMPTIED,        U"onemptied" },
        { html5::Attribute::ONENDED,          U"onended" },
        { html5::Attribute::ONERROR,          U"onerror" },
        { html5::Attribute::ONFOCUS,          U"onfocus" },
        { html5::Attribute::ONHASHCHANGE,     U"onhashchange" },
        { html5::Attribute::ONINPUT,          U"oninput" },
        { html5::Attribute::ONINVALID,        U"oninvalid" },
        { html5::Attribute::ONKEYDOWN,        U"onkeydown" },
        { html5::Attribute::ONKEYPRESS,       U"onkeypress" },
        { html5::Attribute::ONKEYUP,          U"onkeyup" },
        { html5::Attribute::ONLOAD,           U"onload" },
        { html5::Attribute::ONLOADEDDATA,     U"onloadeddata" },
        { html5::Attribute::ONLOADEDMETADATA, U"onloadedmetadata" },
        { html5::Attribute::ONLOADSTART,      U"onloadstart" },
        { html5::Attribute::ONMOUSEDOWN,      U"onmousedown" },
        { html5::Attribute::ONMOUSEMOVE,      U"onmousemove" },
        { html5::Attribute::ONMOUSEOUT,       U"onmouseout" },
        { html5::Attribute::ONMOUSEOVER,      U"onmouseover" },
        { html5::Attribute::ONMOUSEUP,        U"onmouseup" },
        { html5::Attribute::ONMOUSEWHEEL,     U"onmousewheel" },
        { html5::Attribute::ONOFFLINE,        U"onoffline" },
        { html5::Attribute::ONONLINE,         U"ononline" },
        { html5::Attribute::ONPAGEHIDE,       U"onepagehide" },
        { html5::Attribute::ONPAGESHOW,       U"onpageshow" },
        { html5::Attribute::ONPASTE,          U"onpaste" },
        { html5::Attribute::ONPAUSE,          U"onpause" },
        { html5::Attribute::ONPLAY,           U"onplay" },
        { html5::Attribute::ONPLAYING,        U"onplaying" },
        { html5::Attribute::ONPOPSTATE,       U"onpopstate" },
        { html5::Attribute::ONPROGRESS,       U"onprogress" },
        { html5::Attribute::ONRATECHANGE,     U"onratechange" },
        { html5::Attribute::ONRESET,          U"onreset" },
        { html5::Attribute::ONRESIZE,         U"onresize" },
        { html5::Attribute::ONSCROLL,         U"onscroll" },
        { html5::Attribute::ONSEARCH,         U"onsearch" },
        { html5::Attribute::ONSEEKED,         U"onseeked" },
        { html5::Attribute::ONSEEKING,        U"onseeking" },
        { html5::Attribute::ONSELECT,         U"onselect" },
        { html5::Attribute::ONSTALLED,        U"onstalled" },
        { html5::Attribute::ONSTORAGE,        U"onstorage" },
        { html5::Attribute::ONSUBMIT,         U"onsubmit" },
        { html5::Attribute::ONSUSPEND,        U"onsuspend" },
        { html5::Attribute::ONTIMEUPDATE,     U"ontimeupdate" },
        { html5::Attribute::ONTOGGLE,         U"ontoggle" },
        { html5::Attribute::ONUNLOAD,         U"onunload" },
        { html5::Attribute::ONVOLUMECHANGE,   U"onvolumechange" },
        { html5::Attribute::ONWAITING,        U"onwaiting" },
        { html5::Attribute::ONWHEEL,          U"onwheel" },
        { html5::Attribute::OPEN,             U"open" },
        { html5::Attribute::OPTIMUM,          U"optimum" },
        { html5::Attribute::PATTERN,          U"pattern" },
        { html5::Attribute::PLACEHOLDER,      U"placeholder" },
        { html5::Attribute::POSTER,           U"poster" },
        { html5::Attribute::PRELOAD,          U"preload" },
        { html5::Attribute::READONLY,         U"readonly" },
        { html5::Attribute::REL,              U"rel" },
        { html5::Attribute::REQUIRED,         U"required" },
        { html5::Attribute::REVERSED,         U"reversed" },
        { html5::Attribute::ROWS,             U"rows" },
        { html5::Attribute::ROWSPAN,          U"rowspan" },
        { html5::Attribute::SANDBOX,          U"sandbox" },
        { html5::Attribute::SCOPE,            U"scope" },
        { html5::Attribute::SELECTED,         U"selected" },
        { html5::Attribute::SHAPE,            U"shape" },
        { html5::Attribute::SIZE,             U"size" },
        { html5::Attribute::SIZES,            U"sizes" },
        { html5::Attribute::SPAN,             U"span" },
        { html5::Attribute::SPELLCHECK,       U"spellcheck" },
        { html5::Attribute::SRC,              U"src" },
        { html5::Attribute::SRCDOC,           U"srcdoc" },
        { html5::Attribute::SRCLANG,          U"srclang" },
        { html5::Attribute::SRCSET,           U"srcset" },
        { html5::Attribute::START,            U"start" },
        { html5::Attribute::STEP,             U"step" },
        { html5::Attribute::STYLE,            U"style" },
        { html5::Attribute::TABINDEX,         U"tabindex" },
        { html5::Attribute::TARGET,           U"target" },
        { html5::Attribute::TITLE,            U"title" },
        { html5::Attribute::TRANSLATE,        U"translate" },
        { html5::Attribute::TYPE,             U"type" },
        { html5::Attribute::USEMAP,           U"usemap" },
        { html5::Attribute::VALUE,            U"value" },
        { html5::Attribute::WIDTH,            U"width" },
        { html5::Attribute::WRAP,             U"wrap" }
    };
}

/**
 * Generates the string->Attribute lookup map from the Attribute->string map
 * @param str2attr Attribute->string map
 * @return Lookup map
 */
std::unordered_map<std::u32string, blogator::dom::html5::Attribute> blogator::dom::html5::Rules::Factories::createStr2AttrMap(
    const std::unordered_map<Attribute, std::u32string> &str2attr )
{
    auto map = std::unordered_map<std::u32string, Attribute>();

    std::for_each(
        str2attr.cbegin(),
        str2attr.cend(),
        [&]( const auto &pair ) { map.emplace( pair.second, pair.first ); }
    );

    return map;
}

/**
 * Generates the Attribute/Tag membership lookup table
 * @return Lookup table
 */
std::vector<std::vector<bool>> blogator::dom::html5::Rules::Factories::createAttr2TagLookupTable() {
    const auto attr_count = static_cast<int>( Attribute::ENUM_END ) + 1;
    const auto tag_count  = static_cast<int>( Tag::ENUM_END ) + 1;

    auto v = std::vector<std::vector<bool>>( attr_count, std::vector<bool>( tag_count, false ) );

    //Global attributes - can be used with all HTML elements (tags)
    for( int i = 0; i < tag_count; ++i ) {
        v[ static_cast<int>( Attribute::ACCESSKEY )       ][ i ] = true;
        v[ static_cast<int>( Attribute::CLASS )           ][ i ] = true;
        v[ static_cast<int>( Attribute::CONTENTEDITABLE ) ][ i ] = true;
        v[ static_cast<int>( Attribute::DATA_X )          ][ i ] = true;
        v[ static_cast<int>( Attribute::DIR )             ][ i ] = true;
        v[ static_cast<int>( Attribute::DRAGGABLE )       ][ i ] = true;
        v[ static_cast<int>( Attribute::HIDDEN )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ID )              ][ i ] = true;
        v[ static_cast<int>( Attribute::LANG )            ][ i ] = true;
        v[ static_cast<int>( Attribute::SPELLCHECK )      ][ i ] = true;
        v[ static_cast<int>( Attribute::STYLE )           ][ i ] = true;
        v[ static_cast<int>( Attribute::TABINDEX )        ][ i ] = true;
        v[ static_cast<int>( Attribute::TITLE )           ][ i ] = true;
        v[ static_cast<int>( Attribute::TRANSLATE )       ][ i ] = true;
    }

    //Visible elements //TODO what tag can be visible? <- apply the following to these instead of everything
    for( int i = 0; i < tag_count; ++i ) {
        v[ static_cast<int>( Attribute::ONBLUR )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCHANGE )        ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCLICK )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCONTEXTMENU )   ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCOPY )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCUT )           ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDBLCLICK )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAG )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGEND )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGENTER )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGLEAVE )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGOVER )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGSTART )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDROP )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONFOCUS )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONINPUT )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONINVALID )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONKEYDOWN )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONKEYPRESS )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONKEYUP )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEDOWN )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEMOVE )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEOUT )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEOVER )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEUP )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEWHEEL )    ][ i ] = true;
        v[ static_cast<int>( Attribute::ONPASTE )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONSCROLL )        ][ i ] = true;
        v[ static_cast<int>( Attribute::ONSELECT )        ][ i ] = true;
        v[ static_cast<int>( Attribute::ONWHEEL )         ][ i ] = true;
    }

    //Local attributes - can be used with only some HTML elements (tags)
    v[ static_cast<int>( Attribute::ACCEPT )            ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ACCEPT_CHARSET )    ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ACTION )            ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ALT )               ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::ALT )               ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ALT )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ASYNC )             ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::AUTOCOMPLETE )      ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::AUTOCOMPLETE )      ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::AUTOPLAY )          ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::AUTOPLAY )          ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::CHARSET )           ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::CHARSET )           ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::CHECKED )           ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::BLOCKQUOTE )  ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::DEL )         ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::INS )         ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::Q )           ] = true;
    v[ static_cast<int>( Attribute::COLS )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::COLSPAN )           ][ static_cast<int>( Tag::TD )          ] = true;
    v[ static_cast<int>( Attribute::COLSPAN )           ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::CONTENT )           ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::CONTROLS )          ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::CONTROLS )          ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::COORDS )            ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::DATA )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::DATETIME )          ][ static_cast<int>( Tag::DEL )         ] = true;
    v[ static_cast<int>( Attribute::DATETIME )          ][ static_cast<int>( Tag::INS )         ] = true;
    v[ static_cast<int>( Attribute::DATETIME )          ][ static_cast<int>( Tag::TIME )        ] = true;
    v[ static_cast<int>( Attribute::DEFAULT )           ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::DEFER )             ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::DIRNAME )           ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::DIRNAME )           ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::FIELDSET )    ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::OPTGROUP )    ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::DOWNLOAD )          ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::DOWNLOAD )          ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::ENCTYPE )           ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::FOR )               ][ static_cast<int>( Tag::LABEL )       ] = true;
    v[ static_cast<int>( Attribute::FOR )               ][ static_cast<int>( Tag::OUTPUT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::FIELDSET )    ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::LABEL )       ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::OUTPUT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::FORMACTION )        ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::FORMACTION )        ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::HEADERS )           ][ static_cast<int>( Tag::TD )          ] = true;
    v[ static_cast<int>( Attribute::HEADERS )           ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::CANVAS )      ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::HIGH )              ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::BASE )        ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::HREFLANG )          ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::HREFLANG )          ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::HREFLANG )          ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::HTTP_EQUIV )        ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::ISMAP )             ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::KIND )              ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::LABEL )             ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::LABEL )             ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::LABEL )             ][ static_cast<int>( Tag::OPTGROUP )    ] = true;
    v[ static_cast<int>( Attribute::LIST )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::LOOP )              ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::LOOP )              ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::LOW )               ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::MAX )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MAX )               ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::MAX )               ][ static_cast<int>( Tag::PROGRESS )    ] = true;
    v[ static_cast<int>( Attribute::MAXLENGTH )         ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MAXLENGTH )         ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::METHOD )            ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::MIN )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MIN )               ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::MULTIPLE )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MULTIPLE )          ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::MUTED )             ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::MUTED )             ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::FIELDSET )    ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::MAP )         ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::OUTPUT )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::PARAM )       ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::NOVALIDATE )        ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONAFTERPRINT )      ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONBEFOREPRINT )     ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONBEFOREUNLOAD )    ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAYTHROUGH )  ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAYTHROUGH )  ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONCUECHANGE )       ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::ONDURATIONCHANGE )  ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONDURATIONCHANGE )  ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONEMPTIED )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONEMPTIED )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONENDED )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONENDED )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONHASHCHANGE )      ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDDATA )      ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDDATA )      ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDMETADATA )  ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDMETADATA )  ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADSTART )       ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADSTART )       ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONOFFLINE )         ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONONLINE )          ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPAGEHIDE )        ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPAGESHOW )        ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPAUSE )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPAUSE )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAY )            ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAY )            ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAYING )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAYING )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONPOPSTATE )        ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPROGRESS )        ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPROGRESS )        ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONRATECHANGE )      ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONRATECHANGE )      ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONRESET )           ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ONRESIZE )          ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONSEARCH )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKED )          ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKED )          ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKING )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKING )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONSTALLED )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSTALLED )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONSTORAGE )         ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONSUBMIT )          ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ONSUSPEND )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSUSPEND )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONTIMEUPDATE )      ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONTIMEUPDATE )      ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONTOGGLE )          ][ static_cast<int>( Tag::DETAILS )     ] = true;
    v[ static_cast<int>( Attribute::ONUNLOAD )          ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONVOLUMECHANGE )    ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONVOLUMECHANGE )    ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONWAITING )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONWAITING )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::OPEN )              ][ static_cast<int>( Tag::DETAILS )     ] = true;
    v[ static_cast<int>( Attribute::OPTIMUM )           ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::PATTERN )           ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::PLACEHOLDER )       ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::PLACEHOLDER )       ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::POSTER )            ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::PRELOAD )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::PRELOAD )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::READONLY )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::READONLY )          ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::REQUIRED )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::REQUIRED )          ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::REQUIRED )          ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::REVERSED )          ][ static_cast<int>( Tag::OL )          ] = true;
    v[ static_cast<int>( Attribute::ROWS )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::ROWSPAN )           ][ static_cast<int>( Tag::TD )          ] = true;
    v[ static_cast<int>( Attribute::ROWSPAN )           ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::SANDBOX )           ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::SCOPE )             ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::SELECTED )          ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::SHAPE )             ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::SIZE )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::SIZE )              ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::SIZES )             ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::SIZES )             ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::SIZES )             ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::SPAN )              ][ static_cast<int>( Tag::COL )         ] = true;
    v[ static_cast<int>( Attribute::SPAN )              ][ static_cast<int>( Tag::COLGROUP )    ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::SRCDOC )            ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::SRCLANG )           ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::SRCSET )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::SRCSET )            ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::START )             ][ static_cast<int>( Tag::OL )          ] = true;
    v[ static_cast<int>( Attribute::STEP )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::BASE )        ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::USEMAP )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::USEMAP )            ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::LI )          ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::PROGRESS )    ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::PARAM )       ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::CANVAS )      ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::WRAP )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;

    return v;
}