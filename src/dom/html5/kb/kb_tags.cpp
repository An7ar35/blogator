#include "kb_tags.h"

#include <functional>
#include <cwctype>

/**
 * Generates the Tag->{TagStructure, tag str} lookup map
 * @return Lookup map
 */
std::unordered_map<blogator::dom::html5::Tag, blogator::dom::dto::TagSpecs>
    blogator::dom::html5::kb::tags::createTag2StrMap()
{
    return std::unordered_map<Tag, dto::TagSpecs> {
        /* Blogator namespace */
        { Tag::NONE,                { TagStructure::UNPAIRED, { NameSpace::AGNOSTIC },              U"" } },
        /* HTML5 namespace */
        { Tag::A,                   { TagStructure::PAIRED,   { NameSpace::HTML5, NameSpace::SVG }, U"a" } },
        { Tag::ABBR,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"abbr" } },
        { Tag::ADDRESS,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"address" } },
        { Tag::AREA,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"area" } },
        { Tag::ARTICLE,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"article" } },
        { Tag::ASIDE,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"aside" } },
        { Tag::AUDIO,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"audio" } },
        { Tag::B,                   { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"b" } },
        { Tag::BASE,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"base" } },
        { Tag::BDI,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"bdi" } },
        { Tag::BDO,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"bdo" } },
        { Tag::BLOCKQUOTE,          { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"blockquote" } },
        { Tag::BODY,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"body" } },
        { Tag::BR,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"br" } },
        { Tag::BUTTON,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"button" } },
        { Tag::CANVAS,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"canvas" } },
        { Tag::CAPTION,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"caption" } },
        { Tag::CITE,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"cite" } },
        { Tag::CODE,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"code" } },
        { Tag::COL,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"col" } },
        { Tag::COLGROUP,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"colgroup" } },
        { Tag::COMMENT,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"!--" } }, //EDGE CASE (ends with '-->')
        { Tag::DATA,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"data" } },
        { Tag::DATALIST,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"datalist" } },
        { Tag::DD,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"dd" } },
        { Tag::DEL,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"del" } },
        { Tag::DETAILS,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"details" } },
        { Tag::DFN,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"dfn" } },
        { Tag::DIALOG,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"dialog" } },
        { Tag::DIV,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"div" } },
        { Tag::DL,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"dl" } },
        { Tag::DOCTYPE,             { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"!DOCTYPE" } },
        { Tag::DT,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"dt" } },
        { Tag::EM,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"em" } },
        { Tag::EMBED,               { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"embed" } },
        { Tag::FIELDSET,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"fieldset" } },
        { Tag::FIGCAPTION,          { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"figcaption" } },
        { Tag::FIGURE,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"figure" } },
        { Tag::FOOTER,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"footer" } },
        { Tag::FORM,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"form" } },
        { Tag::H1,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"h1" } },
        { Tag::H2,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"h2" } },
        { Tag::H3,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"h3" } },
        { Tag::H4,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"h4" } },
        { Tag::H5,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"h5" } },
        { Tag::H6,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"h6" } },
        { Tag::HEAD,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"head" } },
        { Tag::HEADER,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"header" } },
        { Tag::HR,                  { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"hr" } },
        { Tag::HTML,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"html" } },
        { Tag::I,                   { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"i" } },
        { Tag::IFRAME,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"iframe" } },
        { Tag::IMG,                 { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"img" } },
        { Tag::INPUT,               { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"input" } },
        { Tag::INS,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"ins" } },
        { Tag::KBD,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"kbd" } },
        { Tag::LABEL,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"label" } },
        { Tag::LEGEND,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"legend" } },
        { Tag::LI,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"li" } },
        { Tag::LINK,                { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"link" } },
        { Tag::MAIN,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"main" } },
        { Tag::MAP,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"map" } },
        { Tag::MARK,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"mark" } },
        { Tag::META,                { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"meta" } },
        { Tag::METER,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"meter" } },
        { Tag::NAV,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"nav" } },
        { Tag::NOSCRIPT,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"noscript" } },
        { Tag::OBJECT,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"object" } },
        { Tag::OL,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"ol" } },
        { Tag::OPTGROUP,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"optgroup" } },
        { Tag::OPTION,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"option" } },
        { Tag::OUTPUT,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"output" } },
        { Tag::P,                   { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"p" } },
        { Tag::PARAM,               { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"param" } },
        { Tag::PICTURE,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"picture" } },
        { Tag::PRE,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"pre" } },
        { Tag::PROGRESS,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"progress" } },
        { Tag::Q,                   { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"q" } },
        { Tag::RP,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"rp" } },
        { Tag::RT,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"rt" } },
        { Tag::RUBY,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"ruby" } },
        { Tag::S,                   { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"s" } },
        { Tag::SAMP,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"samp" } },
        { Tag::SCRIPT,              { TagStructure::PAIRED,   { NameSpace::HTML5, NameSpace::SVG }, U"script" } },
        { Tag::SECTION,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"section" } },
        { Tag::SELECT,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"select" } },
        { Tag::SMALL,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"small" } },
        { Tag::SOURCE,              { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"source" } },
        { Tag::SPAN,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"span" } },
        { Tag::STRONG,              { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"strong" } },
        { Tag::STYLE,               { TagStructure::PAIRED,   { NameSpace::HTML5, NameSpace::SVG }, U"style" } },
        { Tag::SUB,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"sub" } },
        { Tag::SUMMARY,             { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"summary" } },
        { Tag::SUP,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"sup" } },
        { Tag::TABLE,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"table" } },
        { Tag::TBODY,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"tbody" } },
        { Tag::TD,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"td" } },
        { Tag::TEMPLATE,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"template" } },
        { Tag::TEXTAREA,            { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"textarea" } },
        { Tag::TFOOT,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"tfoot" } },
        { Tag::TH,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"th" } },
        { Tag::THEAD,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"thead" } },
        { Tag::TIME,                { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"time" } },
        { Tag::TITLE,               { TagStructure::PAIRED,   { NameSpace::HTML5, NameSpace::SVG }, U"title" } },
        { Tag::TR,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"tr" } },
        { Tag::TRACK,               { TagStructure::UNPAIRED, { NameSpace::HTML5 },                 U"track" } },
        { Tag::U,                   { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"u" } },
        { Tag::UL,                  { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"ul" } },
        { Tag::VAR,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"var" } },
        { Tag::VIDEO,               { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"video" } },
        { Tag::WBR,                 { TagStructure::PAIRED,   { NameSpace::HTML5 },                 U"wbr" } },
        /* MathML namespace */ //TODO check TagStructures
        { Tag::MATH,                { TagStructure::PAIRED,   { NameSpace::MathML },                U"math" } },
        { Tag::MACTION,             { TagStructure::PAIRED,   { NameSpace::MathML },                U"maction" } },
        { Tag::MALIGNGROUP,         { TagStructure::PAIRED,   { NameSpace::MathML },                U"maligngroup" } },
        { Tag::MALIGNMARK,          { TagStructure::PAIRED,   { NameSpace::MathML },                U"malignmark" } },
        { Tag::MENCLOSE,            { TagStructure::PAIRED,   { NameSpace::MathML },                U"menclose" } },
        { Tag::MERROR,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"merror" } },
        { Tag::MFENCED,             { TagStructure::PAIRED,   { NameSpace::MathML },                U"mfenced" } },
        { Tag::MFRAC,               { TagStructure::PAIRED,   { NameSpace::MathML },                U"mfrac" } },
        { Tag::MGLYPH,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"mglyph" } },
        { Tag::MI,                  { TagStructure::PAIRED,   { NameSpace::MathML },                U"mi" } },
        { Tag::MLABELEDTR,          { TagStructure::PAIRED,   { NameSpace::MathML },                U"mlabeledtr" } },
        { Tag::MLONGDIV,            { TagStructure::PAIRED,   { NameSpace::MathML },                U"mlongdiv" } },
        { Tag::MMULTISCRIPT,        { TagStructure::PAIRED,   { NameSpace::MathML },                U"mmultiscripts" } },
        { Tag::MN,                  { TagStructure::PAIRED,   { NameSpace::MathML },                U"mn" } },
        { Tag::MO,                  { TagStructure::PAIRED,   { NameSpace::MathML },                U"mo" } },
        { Tag::MOVER,               { TagStructure::PAIRED,   { NameSpace::MathML },                U"mover" } },
        { Tag::MPADDED,             { TagStructure::PAIRED,   { NameSpace::MathML },                U"mpadded" } },
        { Tag::MPHANTOM,            { TagStructure::PAIRED,   { NameSpace::MathML },                U"mphantom" } },
        { Tag::MROOT,               { TagStructure::PAIRED,   { NameSpace::MathML },                U"mroot" } },
        { Tag::MROW,                { TagStructure::PAIRED,   { NameSpace::MathML },                U"mrow" } },
        { Tag::MS,                  { TagStructure::PAIRED,   { NameSpace::MathML },                U"ms" } },
        { Tag::MSCARRIES,           { TagStructure::PAIRED,   { NameSpace::MathML },                U"mscarries" } },
        { Tag::MSCARRY,             { TagStructure::PAIRED,   { NameSpace::MathML },                U"mscarry" } },
        { Tag::MSGROUP,             { TagStructure::PAIRED,   { NameSpace::MathML },                U"msgroup" } },
        { Tag::MSLINE,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"msline" } },
        { Tag::MSPACE,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"mspace" } },
        { Tag::MSQRT,               { TagStructure::PAIRED,   { NameSpace::MathML },                U"msqrt" } },
        { Tag::MSROW,               { TagStructure::PAIRED,   { NameSpace::MathML },                U"msrow" } },
        { Tag::MSTACK,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"mstack" } },
        { Tag::MSTYLE,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"mstyle" } },
        { Tag::MSUB,                { TagStructure::PAIRED,   { NameSpace::MathML },                U"msub" } },
        { Tag::MSUP,                { TagStructure::PAIRED,   { NameSpace::MathML },                U"msup" } },
        { Tag::MSUBSUP,             { TagStructure::PAIRED,   { NameSpace::MathML },                U"msubsup" } },
        { Tag::MTABLE,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"mtable" } },
        { Tag::MTD,                 { TagStructure::PAIRED,   { NameSpace::MathML },                U"mtd" } },
        { Tag::MTEXT,               { TagStructure::PAIRED,   { NameSpace::MathML },                U"mtext" } },
        { Tag::MTR,                 { TagStructure::PAIRED,   { NameSpace::MathML },                U"mtr" } },
        { Tag::MUNDER,              { TagStructure::PAIRED,   { NameSpace::MathML },                U"munder" } },
        { Tag::MUNDEROVER,          { TagStructure::PAIRED,   { NameSpace::MathML },                U"munderover" } },
        { Tag::SEMANTICS,           { TagStructure::PAIRED,   { NameSpace::MathML },                U"semantics" } },
        { Tag::ANNOTATION,          { TagStructure::PAIRED,   { NameSpace::MathML },                U"annotation" } },
        { Tag::ANNOTATION_XML,      { TagStructure::PAIRED,   { NameSpace::MathML },                U"annotation-xml" } },
        /* SVG namespace */ //TODO check TagStructures
        { Tag::SVG,                 { TagStructure::PAIRED,   { NameSpace::SVG },                   U"svg" } },
        { Tag::ANIMATE,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"animate" } },
        { Tag::ANIMATEMOTION,       { TagStructure::PAIRED,   { NameSpace::SVG },                   U"animateMotion" } },
        { Tag::ANIMATETRANSFORM,    { TagStructure::PAIRED,   { NameSpace::SVG },                   U"animateTransform" } },
        { Tag::CIRCLE,              { TagStructure::PAIRED,   { NameSpace::SVG },                   U"circle" } },
        { Tag::CLIPPATH,            { TagStructure::PAIRED,   { NameSpace::SVG },                   U"clipPath" } },
        { Tag::COLOR_PROFILE,       { TagStructure::PAIRED,   { NameSpace::SVG },                   U"color-profile" } },
        { Tag::DEFS,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"defs" } },
        { Tag::DESC,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"desc" } },
        { Tag::DISCARD,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"discard" } },
        { Tag::ELLIPSE,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"ellipse" } },
        { Tag::FEBLEND,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feBlend" } },
        { Tag::FECOLORMATRIX,       { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feColorMatrix" } },
        { Tag::FECOMPONENTTRANSFER, { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feComponentTransfer" } },
        { Tag::FECOMPOSITE,         { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feComposite" } },
        { Tag::FECONVOLVEMATRIX,    { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feConvolveMatrix" } },
        { Tag::FEDIFFUSELIGHTING,   { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feDiffuseLighting" } },
        { Tag::FEDISPLACEMENTMAP,   { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feDisplacementMap" } },
        { Tag::FEDISTANTLIGHT,      { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feDistantLight" } },
        { Tag::FEDROPSHADOW,        { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feDropShadow" } },
        { Tag::FEFLOOD,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feFlood" } },
        { Tag::FEFUNCA,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feFuncA" } },
        { Tag::FEFUNCB,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feFuncB" } },
        { Tag::FEFUNCG,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feFuncG" } },
        { Tag::FEFUNCR,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feFuncR" } },
        { Tag::FEGAUSSIANBLUR,      { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feGaussianBlur" } },
        { Tag::FEIMAGE,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feImage" } },
        { Tag::FEMERGE,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feMerge" } },
        { Tag::FEMERGENODE,         { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feMergeNode" } },
        { Tag::FEMORPHOLOGY,        { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feMorphology" } },
        { Tag::FEOFFSET,            { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feOffset" } },
        { Tag::FEPOINTLIGHT,        { TagStructure::PAIRED,   { NameSpace::SVG },                   U"fePointLight" } },
        { Tag::FESPECULARLIGHTING,  { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feSpecularLighting" } },
        { Tag::FESPOTLIGHT,         { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feSpotLight" } },
        { Tag::FETILE,              { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feTile" } },
        { Tag::FETURBULENCE,        { TagStructure::PAIRED,   { NameSpace::SVG },                   U"feTurbulence" } },
        { Tag::FILTER,              { TagStructure::PAIRED,   { NameSpace::SVG },                   U"filter" } },
        { Tag::FOREIGNOBJECT,       { TagStructure::PAIRED,   { NameSpace::SVG },                   U"foreignObject" } },
        { Tag::G,                   { TagStructure::PAIRED,   { NameSpace::SVG },                   U"g" } },
        { Tag::HATCH,               { TagStructure::PAIRED,   { NameSpace::SVG },                   U"hatch" } },
        { Tag::HATCHPATH,           { TagStructure::PAIRED,   { NameSpace::SVG },                   U"hatchpath" } },
        { Tag::IMAGE,               { TagStructure::PAIRED,   { NameSpace::SVG },                   U"image" } },
        { Tag::LINE,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"line" } },
        { Tag::LINEARGRADIENT,      { TagStructure::PAIRED,   { NameSpace::SVG },                   U"linearGradient" } },
        { Tag::MARKER,              { TagStructure::PAIRED,   { NameSpace::SVG },                   U"marker" } },
        { Tag::MASK,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"mask" } },
        { Tag::MESH,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"mesh" } },
        { Tag::MESHGRADIENT,        { TagStructure::PAIRED,   { NameSpace::SVG },                   U"meshgradient" } },
        { Tag::MESHPATCH,           { TagStructure::PAIRED,   { NameSpace::SVG },                   U"meshpatch" } },
        { Tag::MESHROW,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"meshrow" } },
        { Tag::METADATA,            { TagStructure::PAIRED,   { NameSpace::SVG },                   U"metadata" } },
        { Tag::MPATH,               { TagStructure::PAIRED,   { NameSpace::SVG },                   U"mpath" } },
        { Tag::PATH,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"path" } },
        { Tag::PATTERN,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"pattern" } },
        { Tag::POLYGON,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"polygon" } },
        { Tag::POLYLINE,            { TagStructure::PAIRED,   { NameSpace::SVG },                   U"polyline" } },
        { Tag::RADIALGRADIENT,      { TagStructure::PAIRED,   { NameSpace::SVG },                   U"radialGradient" } },
        { Tag::RECT,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"rect" } },
        { Tag::SET,                 { TagStructure::PAIRED,   { NameSpace::SVG },                   U"set" } },
        { Tag::SOLIDCOLOR,          { TagStructure::PAIRED,   { NameSpace::SVG },                   U"solidcolor" } },
        { Tag::STOP,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"stop" } },
        { Tag::SWITCH,              { TagStructure::PAIRED,   { NameSpace::SVG },                   U"switch" } },
        { Tag::SYMBOL,              { TagStructure::PAIRED,   { NameSpace::SVG },                   U"symbol" } },
        { Tag::TEXT,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"text" } },
        { Tag::TEXTPATH,            { TagStructure::PAIRED,   { NameSpace::SVG },                   U"textPath" } },
        { Tag::TSPAN,               { TagStructure::PAIRED,   { NameSpace::SVG },                   U"tspan" } },
        { Tag::UNKNOWN,             { TagStructure::PAIRED,   { NameSpace::SVG },                   U"unknown" } },
        { Tag::USE,                 { TagStructure::PAIRED,   { NameSpace::SVG },                   U"use" } },
        { Tag::VIEW,                { TagStructure::PAIRED,   { NameSpace::SVG },                   U"view" } }
    };
}

/**
 * Generates the string->Tag lookup map from the Tag->string map
 * @param tag2str Tag->string map
 * @return Lookup map
 */
std::unordered_map<std::u32string, blogator::dom::html5::Tag> blogator::dom::html5::kb::tags::createStr2TagMap(
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