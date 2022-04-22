#include "Element.h"

#include <set>
#include <unordered_map>
#include <vector>
#include <ostream>
#include <sstream>

#include "../../../sys/terminate.h"

#include "Namespace.h"

using namespace blogator::parser::specs::infra;

struct AmbiguousElements {
    std::set<std::u32string>                          by_str;
    std::set<blogator::parser::specs::infra::Element> by_type;
};

/**
 * [LOCAL] Creates a u32string->Element enum map
 * @param ns Namespace to generate Element map for
 * @return Map
 */
static std::unordered_map<std::u32string , blogator::parser::specs::infra::Element> loadElementMap( Namespace ns ) noexcept {
    try {
        using blogator::parser::specs::infra::Element;

        auto map   = std::unordered_map<std::u32string, Element>();
        int  start = 0;
        int  end   = 0;

        switch( ns ) {
            case Namespace::DEPR: {
                start = static_cast<int>( Element::DEPR_NS_BEGIN );
                end   = static_cast<int>( Element::DEPR_NS_END  );
            } break;

            case Namespace::HTML5: {
                start = static_cast<int>( Element::HTML5_NS_BEGIN );
                end   = static_cast<int>( Element::HTML5_NS_END  );
            } break;

            case Namespace::MATHML: {
                start = static_cast<int>( Element::MATHML_NS_BEGIN );
                end   = static_cast<int>( Element::MATHML_NS_END   );
            } break;

            case Namespace::SVG: {
                start = static_cast<int>( Element::SVG_NS_BEGIN );
                end   = static_cast<int>( Element::SVG_NS_END   );
            } break;

            default: {
                return std::move( map ); //EARLY RETURN
            }
        }

        for( int i = start; i <= end; ++i ) {
            //Since string representation of elements are all in ASCII range
            //conversion to u32 can just be done with direct casting.
            map.emplace( blogator::to_u32string( static_cast<Element>( i ) ), static_cast<Element>( i ) );
        }

        return std::move( map );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::Element::loadElementMap( Namespace )] Failed to load map data." );
    }
}

AmbiguousElements loadAmbiguousElements() noexcept {
    try {
        auto unique_pool        = std::set<std::u32string>();
        auto ambiguous_elements = AmbiguousElements();

        for( int i = 0; i <= static_cast<int>( Element::ENUM_END ); ++i ) {
            auto [it, added] = unique_pool.emplace( blogator::to_u32string( static_cast<Element>( i ) ) );

            if( !added ) {
                ambiguous_elements.by_str.emplace( blogator::to_u32string( static_cast<Element>( i ) ) );
                ambiguous_elements.by_type.emplace( static_cast<Element>( i ) );
            }
        }

        return std::move( ambiguous_elements );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::Element::loadAmbiguousElements()] Failed to load set of ambiguous elements." );
    }
}

static const std::unordered_map<std::u32string , blogator::parser::specs::infra::Element> ELEMENT_DEPR_STR_MAP   = loadElementMap( Namespace::DEPR );
static const std::unordered_map<std::u32string , blogator::parser::specs::infra::Element> ELEMENT_HTML5_STR_MAP  = loadElementMap( Namespace::HTML5 );
static const std::unordered_map<std::u32string , blogator::parser::specs::infra::Element> ELEMENT_MATHML_STR_MAP = loadElementMap( Namespace::MATHML );
static const std::unordered_map<std::u32string , blogator::parser::specs::infra::Element> ELEMENT_SVG_STR_MAP    = loadElementMap( Namespace::SVG );
static const AmbiguousElements                                                            AMBIGUOUS_ELEMENTS     = loadAmbiguousElements();


/**
 * Output stream operator
 * @param os Output stream
 * @param el Element enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, blogator::parser::specs::infra::Element el ) {
    static const auto map = std::unordered_map<Element, std::string>( {
        { Element::UNKNOWN,                 "UNKNOWN" },
        { Element::OTHER,                   "OTHER" },
        { Element::DEPR_HTML_APPLET,        "applet" },
        { Element::DEPR_HTML_BASEFONT,      "basefont" },
        { Element::DEPR_HTML_BGSOUND,       "bgsound" },
        { Element::DEPR_HTML_BIG,           "big" },
        { Element::DEPR_HTML_CENTER,        "center" },
        { Element::DEPR_HTML_DIR,           "dir" },
        { Element::DEPR_HTML_FONT,          "font" },
        { Element::DEPR_HTML_FRAME,         "frame" },
        { Element::DEPR_HTML_FRAMESET,      "frameset" },
        { Element::DEPR_HTML_IMAGE,         "image" }, //strictly not a depreciated tag but an invalid one
        { Element::DEPR_HTML_KEYGEN,        "keygen" },
        { Element::DEPR_HTML_LISTING,       "listing" },
        { Element::DEPR_HTML_MARQUEE,       "marquee" },
        { Element::DEPR_HTML_MENUITEM,      "menuitem" },
        { Element::DEPR_HTML_NOBR,          "nobr" },
        { Element::DEPR_HTML_NOEMEBED,      "noembed" },
        { Element::DEPR_HTML_NOFRAMES,      "noframes" },
        { Element::DEPR_HTML_PLAINTEXT,     "plaintext" },
        { Element::DEPR_HTML_RB,            "rb" },
        { Element::DEPR_HTML_RTC,           "rtc" },
        { Element::DEPR_HTML_STRIKE,        "strike" },
        { Element::DEPR_HTML_TT,            "tt" },
        { Element::DEPR_HTML_XMP,           "xmp" },
        { Element::HTML5_A,                 "a" },
        { Element::HTML5_ABBR,              "abbr" },
        { Element::HTML5_ADDRESS,           "address" },
        { Element::HTML5_AREA,              "area" },
        { Element::HTML5_ARTICLE,           "article" },
        { Element::HTML5_ASIDE,             "aside" },
        { Element::HTML5_AUDIO,             "audio" },
        { Element::HTML5_B,                 "b" },
        { Element::HTML5_BASE,              "base" },
        { Element::HTML5_BDI,               "bdi" },
        { Element::HTML5_BDO,               "bdo" },
        { Element::HTML5_BLOCKQUOTE,        "blockquote" },
        { Element::HTML5_BODY,              "body" },
        { Element::HTML5_BR,                "br" },
        { Element::HTML5_BUTTON,            "button" },
        { Element::HTML5_CANVAS,            "canvas" },
        { Element::HTML5_CAPTION,           "caption" },
        { Element::HTML5_CITE,              "cite" },
        { Element::HTML5_CODE,              "code" },
        { Element::HTML5_COL,               "col" },
        { Element::HTML5_COLGROUP,          "colgroup" },
        { Element::HTML5_COMMENT,           "comment" },
        { Element::HTML5_DATA,              "data" },
        { Element::HTML5_DATALIST,          "datalist" },
        { Element::HTML5_DD,                "dd" },
        { Element::HTML5_DEL,               "del" },
        { Element::HTML5_DETAILS,           "details" },
        { Element::HTML5_DFN,               "dfn" },
        { Element::HTML5_DIALOG,            "dialog" },
        { Element::HTML5_DIV,               "div" },
        { Element::HTML5_DL,                "dl" },
        { Element::HTML5_DOCTYPE,           "doctype" },
        { Element::HTML5_DT,                "dt" },
        { Element::HTML5_EM,                "em" },
        { Element::HTML5_EMBED,             "embed" },
        { Element::HTML5_FIELDSET,          "fieldset" },
        { Element::HTML5_FIGCAPTION,        "figcaption" },
        { Element::HTML5_FIGURE,            "figure" },
        { Element::HTML5_FOOTER,            "footer" },
        { Element::HTML5_FORM,              "form" },
        { Element::HTML5_H1,                "h1" },
        { Element::HTML5_H2,                "h2" },
        { Element::HTML5_H3,                "h3" },
        { Element::HTML5_H4,                "h4" },
        { Element::HTML5_H5,                "h5" },
        { Element::HTML5_H6,                "h6" },
        { Element::HTML5_HEAD,              "head" },
        { Element::HTML5_HEADER,            "header" },
        { Element::HTML5_HGROUP,            "hgroup" },
        { Element::HTML5_HR,                "hr" },
        { Element::HTML5_HTML,              "html" },
        { Element::HTML5_I,                 "i" },
        { Element::HTML5_IFRAME,            "iframe" },
        { Element::HTML5_IMG,               "img" },
        { Element::HTML5_INPUT,             "input" },
        { Element::HTML5_INS,               "ins" },
        { Element::HTML5_KBD,               "kbd" },
        { Element::HTML5_LABEL,             "label" },
        { Element::HTML5_LEGEND,            "legend" },
        { Element::HTML5_LI,                "li" },
        { Element::HTML5_LINK,              "link" },
        { Element::HTML5_MAIN,              "main" },
        { Element::HTML5_MAP,               "map" },
        { Element::HTML5_MARK,              "mark" },
        { Element::HTML5_META,              "meta" },
        { Element::HTML5_MENU,              "menu" },
        { Element::HTML5_METER,             "meter" },
        { Element::HTML5_NAV,               "nav" },
        { Element::HTML5_NOSCRIPT,          "noscript" },
        { Element::HTML5_OBJECT,            "object" },
        { Element::HTML5_OL,                "ol" },
        { Element::HTML5_OPTGROUP,          "optgroup" },
        { Element::HTML5_OPTION,            "option" },
        { Element::HTML5_OUTPUT,            "output" },
        { Element::HTML5_P,                 "p" },
        { Element::HTML5_PARAM,             "param" },
        { Element::HTML5_PICTURE,           "picture" },
        { Element::HTML5_PRE,               "pre" },
        { Element::HTML5_PROGRESS,          "progress" },
        { Element::HTML5_Q,                 "q" },
        { Element::HTML5_RP,                "rp" },
        { Element::HTML5_RT,                "rt" },
        { Element::HTML5_RUBY,              "ruby" },
        { Element::HTML5_S,                 "s" },
        { Element::HTML5_SAMP,              "samp" },
        { Element::HTML5_SCRIPT,            "script" },
        { Element::HTML5_SECTION,           "section" },
        { Element::HTML5_SELECT,            "select" },
        { Element::HTML5_SLOT,              "slot" },
        { Element::HTML5_SMALL,             "small" },
        { Element::HTML5_SOURCE,            "source" },
        { Element::HTML5_SPAN,              "span" },
        { Element::HTML5_STRONG,            "strong" },
        { Element::HTML5_STYLE,             "style" },
        { Element::HTML5_SUB,               "sub" },
        { Element::HTML5_SUMMARY,           "summary" },
        { Element::HTML5_SUP,               "sup" },
        { Element::HTML5_TABLE,             "table" },
        { Element::HTML5_TBODY,             "tbody" },
        { Element::HTML5_TD,                "td" },
        { Element::HTML5_TEMPLATE,          "template" },
        { Element::HTML5_TEXTAREA,          "textarea" },
        { Element::HTML5_TFOOT,             "tfoot" },
        { Element::HTML5_TH,                "th" },
        { Element::HTML5_THEAD,             "thead" },
        { Element::HTML5_TIME,              "time" },
        { Element::HTML5_TITLE,             "title" },
        { Element::HTML5_TR,                "tr" },
        { Element::HTML5_TRACK,             "track" },
        { Element::HTML5_U,                 "u" },
        { Element::HTML5_UL,                "ul" },
        { Element::HTML5_VAR,               "var" },
        { Element::HTML5_VIDEO,             "video" },
        { Element::HTML5_WBR,               "wbr" },
        { Element::HTML5_WBR,               "wbr" },
        { Element::MATH,                    "math" },
        { Element::MATHML_MACTION,          "maction" },
        { Element::MATHML_MALIGNGROUP,      "maligngroup" },
        { Element::MATHML_MALIGNMARK,       "malignmark" },
        { Element::MATHML_MENCLOSE,         "menclose" },
        { Element::MATHML_MERROR,           "merror" },
        { Element::MATHML_MFENCED,          "mfenced" },
        { Element::MATHML_MFRAC,            "mfrac" },
        { Element::MATHML_MGLYPH,           "mglyph" },
        { Element::MATHML_MI,               "mi" },
        { Element::MATHML_MLABELEDTR,       "mlabeledtr" },
        { Element::MATHML_MLONGDIV,         "mlongdiv" },
        { Element::MATHML_MMULTISCRIPT,     "mmultiscript" },
        { Element::MATHML_MN,               "mn" },
        { Element::MATHML_MO,               "mo" },
        { Element::MATHML_MOVER,            "mover" },
        { Element::MATHML_MPADDED,          "mpadded" },
        { Element::MATHML_MPHANTOM,         "mphantom" },
        { Element::MATHML_MROOT,            "mroot" },
        { Element::MATHML_MROW,             "mrow" },
        { Element::MATHML_MS,               "ms" },
        { Element::MATHML_MSCARRIES,        "mscarries" },
        { Element::MATHML_MSCARRY,          "mscarry" },
        { Element::MATHML_MSGROUP,          "msgroup" },
        { Element::MATHML_MSLINE,           "msline" },
        { Element::MATHML_MSPACE,           "mspace" },
        { Element::MATHML_MSQRT,            "msqrt" },
        { Element::MATHML_MSROW,            "msrow" },
        { Element::MATHML_MSTACK,           "mstack" },
        { Element::MATHML_MSTYLE,           "mstyle" },
        { Element::MATHML_MSUB,             "msub" },
        { Element::MATHML_MSUP,             "msup" },
        { Element::MATHML_MSUBSUP,          "msubsup" },
        { Element::MATHML_MTABLE,           "mtable" },
        { Element::MATHML_MTD,              "mtd" },
        { Element::MATHML_MTEXT,            "mtext" },
        { Element::MATHML_MTR,              "mtr" },
        { Element::MATHML_MUNDER,           "munder" },
        { Element::MATHML_MUNDEROVER,       "munderover" },
        { Element::MATHML_SEMANTICS,        "semantics" },
        { Element::MATHML_ANNOTATION,       "annotation" },
        { Element::MATHML_ANNOTATION_XML,   "annotation-xml" },
        { Element::SVG,                     "svg" },
        { Element::SVG_A,                   "a" },
        { Element::SVG_ALTGLYPH,            "altGlyph" },
        { Element::SVG_ALTGLYPHDEF,         "altGlyphDef" },
        { Element::SVG_ALTGLYPHITEM,        "altGlyphItem" },
        { Element::SVG_ANIMATE,             "animate" },
        { Element::SVG_ANIMATECOLOR,        "animateColor" },
        { Element::SVG_ANIMATEMOTION,       "animateMotion" },
        { Element::SVG_ANIMATETRANSFORM,    "animateTransform" },
        { Element::SVG_CIRCLE,              "circle" },
        { Element::SVG_CLIPPATH,            "clipPath" },
        { Element::SVG_COLOR_PROFILE,       "color-profile" },
        { Element::SVG_DEFS,                "defs" },
        { Element::SVG_DESC,                "desc" },
        { Element::SVG_DISCARD,             "discard" },
        { Element::SVG_ELLIPSE,             "ellipse" },
        { Element::SVG_FEBLEND,             "feBlend" },
        { Element::SVG_FECOLORMATRIX,       "feColorMatrix" },
        { Element::SVG_FECOMPONENTTRANSFER, "feComponentTransfer" },
        { Element::SVG_FECOMPOSITE,         "feComposite" },
        { Element::SVG_FECONVOLVEMATRIX,    "feConvolveMatrix" },
        { Element::SVG_FEDIFFUSELIGHTING,   "feDiffuseLighting" },
        { Element::SVG_FEDISPLACEMENTMAP,   "feDisplacementMap" },
        { Element::SVG_FEDISTANTLIGHT,      "feDistantLight" },
        { Element::SVG_FEDROPSHADOW,        "feDropShadow" },
        { Element::SVG_FEFLOOD,             "feFlood" },
        { Element::SVG_FEFUNCA,             "feFuncA" },
        { Element::SVG_FEFUNCB,             "feFuncB" },
        { Element::SVG_FEFUNCG,             "feFuncG" },
        { Element::SVG_FEFUNCR,             "feFuncR" },
        { Element::SVG_FEGAUSSIANBLUR,      "feGaussianBlur" },
        { Element::SVG_FEIMAGE,             "feImage" },
        { Element::SVG_FEMERGE,             "feMerge" },
        { Element::SVG_FEMERGENODE,         "feMergeNode" },
        { Element::SVG_FEMORPHOLOGY,        "feMorphology" },
        { Element::SVG_FEOFFSET,            "feOffset" },
        { Element::SVG_FEPOINTLIGHT,        "fePointLight" },
        { Element::SVG_FESPECULARLIGHTING,  "feSpecularLighting" },
        { Element::SVG_FESPOTLIGHT,         "feSpotLight" },
        { Element::SVG_FETILE,              "feTile" },
        { Element::SVG_FETURBULENCE,        "feTurbulence" },
        { Element::SVG_FILTER,              "filter" },
        { Element::SVG_FOREIGNOBJECT,       "foreignObject" },
        { Element::SVG_G,                   "g" },
        { Element::SVG_GLYPHREF,            "glyphRef" },
        { Element::SVG_HATCH,               "hatch" },
        { Element::SVG_HATCHPATH,           "hatchpath" },
        { Element::SVG_IMAGE,               "image" },
        { Element::SVG_LINE,                "line" },
        { Element::SVG_LINEARGRADIENT,      "linearGradient" },
        { Element::SVG_MARKER,              "marker" },
        { Element::SVG_MASK,                "mask" },
        { Element::SVG_MESH,                "mesh" },
        { Element::SVG_MESHGRADIENT,        "meshgradient" },
        { Element::SVG_MESHPATCH,           "meshpatch" },
        { Element::SVG_MESHROW,             "meshrow" },
        { Element::SVG_METADATA,            "metadata" },
        { Element::SVG_MPATH,               "mpath" },
        { Element::SVG_PATH,                "path" },
        { Element::SVG_PATTERN,             "pattern" },
        { Element::SVG_POLYGON,             "polygon" },
        { Element::SVG_POLYLINE,            "polyline" },
        { Element::SVG_RADIALGRADIENT,      "radialGradient" },
        { Element::SVG_RECT,                "rect" },
        { Element::SVG_SCRIPT,              "script" },
        { Element::SVG_SET,                 "set" },
        { Element::SVG_SOLIDCOLOR,          "solidcolor" },
        { Element::SVG_STOP,                "stop" },
        { Element::SVG_STYLE,               "style" },
        { Element::SVG_SWITCH,              "switch" },
        { Element::SVG_SYMBOL,              "symbol" },
        { Element::SVG_TEXT,                "text" },
        { Element::SVG_TEXTPATH,            "textPath" },
        { Element::SVG_TITLE,               "title" },
        { Element::SVG_TSPAN,               "tspan" },
        { Element::SVG_UNKNOWN,             "unknown" },
        { Element::SVG_USE,                 "use" },
        { Element::SVG_VIEW,                "view" },
    } );

    if( map.contains( el ) ) {
        os << map.at( el );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Gets the an Element's Namespace
 * @param el Element
 * @return Namespace type
 */
blogator::parser::specs::infra::Namespace blogator::parser::specs::infra::getNamespace( Element el ) {
    using blogator::parser::specs::infra::Element;
    using blogator::parser::specs::infra::Namespace;

    if( el >= Element::DEPR_NS_BEGIN && el <= Element::DEPR_NS_END ) {
        return Namespace::DEPR;
    } else if( el >= Element::HTML5_NS_BEGIN && el <= Element::HTML5_NS_END ) {
        return Namespace::HTML5;
    } else if( el >= Element::MATHML_NS_BEGIN && el <= Element::MATHML_NS_END ) {
        return Namespace::MATHML;
    } else if( el >= Element::SVG_NS_BEGIN && el <= Element::SVG_NS_END ) {
        return Namespace::SVG;
    } else {
        return Namespace::UNKNOWN;
    }
}

/**
 * Resolves an Element's namespace
 * @param el Element to resolve
 * @return Namespace (Guaranteed to be one of HTML, MATHML, SVG or UNKNOWN)
 */
blogator::parser::specs::infra::Namespace blogator::parser::specs::infra::resolveNamespace( Element el ) {
    using blogator::parser::specs::infra::Element;
    using blogator::parser::specs::infra::Namespace;

    if( el >= Element::DEPR_NS_BEGIN && el <= Element::DEPR_NS_END ) {
        return Namespace::HTML;
    } else if( el >= Element::HTML5_NS_BEGIN && el <= Element::HTML5_NS_END ) {
        return Namespace::HTML;
    } else if( el >= Element::MATHML_NS_BEGIN && el <= Element::MATHML_NS_END ) {
        return Namespace::MATHML;
    } else if( el >= Element::SVG_NS_BEGIN && el <= Element::SVG_NS_END ) {
        return Namespace::SVG;
    } else {
        return Namespace::UNKNOWN;
    }
}

/**
 * [PRIVATE/LOCAL] Resolves an element string into its enum type
 * @param el Element string
 * @param ns Namespace to resolve element in (must be either of HTML, MATHML, SVG)
 * @return Element type ('OTHER' is un-resolved into a type)
 */
blogator::parser::specs::infra::Element resolveElementType_( std::u32string & el, Namespace ns ) {
    switch( ns ) {
        case Namespace::HTML: [[fallthrough]];
        case Namespace::HTML5: {
            auto it = ELEMENT_HTML5_STR_MAP.find( el );

            if( it != ELEMENT_HTML5_STR_MAP.cend() ) {
                return it->second; //EARLY RETURN
            }
        } [[fallthrough]];

        case Namespace::DEPR: {
            auto it = ELEMENT_DEPR_STR_MAP.find( el );

            if( it != ELEMENT_DEPR_STR_MAP.cend() ) {
                return it->second; //EARLY RETURN
            }
        } break;

        case Namespace::MATHML: {
            auto it = ELEMENT_MATHML_STR_MAP.find( el );

            if( it != ELEMENT_MATHML_STR_MAP.cend() ) {
                return it->second; //EARLY RETURN
            }
        } break;

        case Namespace::SVG: {
            auto adjusted = blogator::parser::specs::infra::adjustSVGElementTagName( el );
            auto it       = ELEMENT_SVG_STR_MAP.find( adjusted );

            if( it != ELEMENT_SVG_STR_MAP.cend() ) {
                return it->second; //EARLY RETURN
            }
        } break;

        default: break;
    }

    return Element::OTHER;
}

/**
 * Gets a element type from a string
 * @param ns_hint Namespace to look for the element (can be any of UNKNOWN (all), HTML, MATHML, SVG)
 * @param el Element string (expects lowercase for HTML, SVG* or MATHML - *SVG names are auto-adjusted)
 * @return Element type (Element::OTHER if not found)
 */
blogator::parser::specs::infra::Element blogator::parser::specs::infra::resolveElementType( Namespace ns_hint, std::u32string & el ) {
    auto resolved = Element::UNKNOWN;

    if( ns_hint == Namespace::UNKNOWN ) {
        for( auto & ns : { Namespace::HTML, Namespace::SVG, Namespace::MATHML } ) {
            if( ( resolved = resolveElementType_( el, ns ) ) != Element::OTHER ) {
                return resolved; //EARLY RETURN;
            }
        }

    } else {
        return resolveElementType_( el, ns_hint );
    }

    return resolved;
}

/**
 * Checks if an element name is found in more than one of the HTML/SVG/MATHML namespaces
 * @param el Element name in lowercase
 * @return Ambiguity state
 */
bool blogator::parser::specs::infra::isAmbiguousElement( const std::u32string &el ) { //TODO unit test
    return AMBIGUOUS_ELEMENTS.by_str.contains( el );
}

/**
 * Checks if an element name is found in more than one of the HTML/SVG/MATHML namespaces
 * @param el Element type
 * @return Ambiguity state
 */
bool blogator::parser::specs::infra::isAmbiguousElement( Element el ) { //TODO unit test
    return AMBIGUOUS_ELEMENTS.by_type.contains( el );
}

/**
 * Adjusts a SVG element tag name
 * @param name Element tag name in lowercase to adjust
 * @return Adjusted tag name name
 */
std::u32string & blogator::parser::specs::infra::adjustSVGElementTagName( std::u32string & name ) {
    static const auto map = std::unordered_map<std::u32string , Element>( {
        { U"altglyph",            Element::SVG_ALTGLYPH },
        { U"altglyphdef",         Element::SVG_ALTGLYPHDEF },
        { U"altglyphitem",        Element::SVG_ALTGLYPHITEM },
        { U"animatecolor",        Element::SVG_ANIMATECOLOR },
        { U"animatemotion",       Element::SVG_ANIMATEMOTION },
        { U"animatetransform",    Element::SVG_ANIMATETRANSFORM },
        { U"clippath",            Element::SVG_CLIPPATH },
        { U"feblend",             Element::SVG_FEBLEND },
        { U"fecolormatrix",       Element::SVG_FECOLORMATRIX },
        { U"fecomponenttransfer", Element::SVG_FECOMPONENTTRANSFER },
        { U"fecomposite",         Element::SVG_FECOMPOSITE },
        { U"feconvolvematrix",    Element::SVG_FECONVOLVEMATRIX },
        { U"fediffuselighting",   Element::SVG_FEDIFFUSELIGHTING },
        { U"fedisplacementmap",   Element::SVG_FEDISPLACEMENTMAP },
        { U"fedistantlight",      Element::SVG_FEDISTANTLIGHT },
        { U"fedropshadow",        Element::SVG_FEDROPSHADOW },
        { U"feflood",             Element::SVG_FEFLOOD },
        { U"fefunca",             Element::SVG_FEFUNCA },
        { U"fefuncb",             Element::SVG_FEFUNCB },
        { U"fefuncg",             Element::SVG_FEFUNCG },
        { U"fefuncr",             Element::SVG_FEFUNCR },
        { U"fegaussianblur",      Element::SVG_FEGAUSSIANBLUR },
        { U"feimage",             Element::SVG_FEIMAGE },
        { U"femerge",             Element::SVG_FEMERGE },
        { U"femergenode",         Element::SVG_FEMERGENODE },
        { U"femorphology",        Element::SVG_FEMORPHOLOGY },
        { U"feoffset",            Element::SVG_FEOFFSET },
        { U"fepointlight",        Element::SVG_FEPOINTLIGHT },
        { U"fespecularlighting",  Element::SVG_FESPECULARLIGHTING },
        { U"fespotlight",         Element::SVG_FESPOTLIGHT },
        { U"fetile",              Element::SVG_FETILE },
        { U"feturbulence",        Element::SVG_FETURBULENCE },
        { U"foreignobject",       Element::SVG_FOREIGNOBJECT },
        { U"glyphref",            Element::SVG_GLYPHREF },
        { U"lineargradient",      Element::SVG_LINEARGRADIENT },
        { U"radialgradient",      Element::SVG_RADIALGRADIENT },
        { U"textpath",            Element::SVG_TEXTPATH },
    } );

    const auto it = map.find( name );

    if( it != map.cend() ) {
        name = blogator::to_u32string( it->second );
    }

    return name;
}

/**
 * Converts a parser::specs::infra::Element enum to a string representation
 * @param el Element enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::infra::Element el ) {
    std::stringstream ss;
    ss << el;
    return ss.str();
}

/**
 * Converts a parser::specs::infra::Element enum to a string representation
 * @param el Element enum
 * @return String representation
 */
std::u32string blogator::to_u32string( blogator::parser::specs::infra::Element el ) {
    const auto u8str = blogator::to_string( el );
    return { u8str.cbegin(), u8str.cend() };
}
