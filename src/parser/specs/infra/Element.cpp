#include "Element.h"

#include <map>
#include <ostream>
#include <sstream>

#include "Namespace.h"

using namespace blogator::parser::specs::infra;

/**
 * Output stream operator
 * @param os Output stream
 * @param el Element enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, blogator::parser::specs::infra::Element el ) {
    static std::map<Element, std::string> map;

    if( map.empty() ) {
        map.emplace( Element::UNKNOWN, "UNKNOWN" );
        map.emplace( Element::HTML5_A, "a" );
        map.emplace( Element::HTML5_ABBR, "abbr" );
        map.emplace( Element::HTML5_ADDRESS, "address" );
        map.emplace( Element::HTML5_AREA, "area" );
        map.emplace( Element::HTML5_ARTICLE, "article" );
        map.emplace( Element::HTML5_ASIDE, "aside" );
        map.emplace( Element::HTML5_AUDIO, "audio" );
        map.emplace( Element::HTML5_B, "b" );
        map.emplace( Element::HTML5_BASE, "base" );
        map.emplace( Element::HTML5_BDI, "bdi" );
        map.emplace( Element::HTML5_BDO, "bdo" );
        map.emplace( Element::HTML5_BLOCKQUOTE, "blockquote" );
        map.emplace( Element::HTML5_BODY, "body" );
        map.emplace( Element::HTML5_BR, "br" );
        map.emplace( Element::HTML5_BUTTON, "button" );
        map.emplace( Element::HTML5_CANVAS, "canvas" );
        map.emplace( Element::HTML5_CAPTION, "caption" );
        map.emplace( Element::HTML5_CITE, "cite" );
        map.emplace( Element::HTML5_CODE, "code" );
        map.emplace( Element::HTML5_COL, "col" );
        map.emplace( Element::HTML5_COLGROUP, "colgroup" );
        map.emplace( Element::HTML5_COMMENT, "comment" );
        map.emplace( Element::HTML5_DATA, "data" );
        map.emplace( Element::HTML5_DATALIST, "datalist" );
        map.emplace( Element::HTML5_DD, "dd" );
        map.emplace( Element::HTML5_DEL, "del" );
        map.emplace( Element::HTML5_DETAILS, "details" );
        map.emplace( Element::HTML5_DFN, "dfn" );
        map.emplace( Element::HTML5_DIALOG, "dialog" );
        map.emplace( Element::HTML5_DIV, "div" );
        map.emplace( Element::HTML5_DL, "dl" );
        map.emplace( Element::HTML5_DOCTYPE, "doctype" );
        map.emplace( Element::HTML5_DT, "dt" );
        map.emplace( Element::HTML5_EM, "em" );
        map.emplace( Element::HTML5_EMBED, "embed" );
        map.emplace( Element::HTML5_FIELDSET, "fieldset" );
        map.emplace( Element::HTML5_FIGCAPTION, "figcaption" );
        map.emplace( Element::HTML5_FIGURE, "figure" );
        map.emplace( Element::HTML5_FOOTER, "footer" );
        map.emplace( Element::HTML5_FORM, "form" );
        map.emplace( Element::HTML5_H1, "h1" );
        map.emplace( Element::HTML5_H2, "h2" );
        map.emplace( Element::HTML5_H3, "h3" );
        map.emplace( Element::HTML5_H4, "h4" );
        map.emplace( Element::HTML5_H5, "h5" );
        map.emplace( Element::HTML5_H6, "h6" );
        map.emplace( Element::HTML5_HEAD, "head" );
        map.emplace( Element::HTML5_HEADER, "header" );
        map.emplace( Element::HTML5_HGROUP, "hgroup" );
        map.emplace( Element::HTML5_HR, "hr" );
        map.emplace( Element::HTML5_HTML, "html" );
        map.emplace( Element::HTML5_I, "i" );
        map.emplace( Element::HTML5_IFRAME, "iframe" );
        map.emplace( Element::HTML5_IMG, "img" );
        map.emplace( Element::HTML5_INPUT, "input" );
        map.emplace( Element::HTML5_INS, "ins" );
        map.emplace( Element::HTML5_KBD, "kbd" );
        map.emplace( Element::HTML5_LABEL, "label" );
        map.emplace( Element::HTML5_LEGEND, "legend" );
        map.emplace( Element::HTML5_LI, "li" );
        map.emplace( Element::HTML5_LINK, "link" );
        map.emplace( Element::HTML5_MAIN, "main" );
        map.emplace( Element::HTML5_MAP, "map" );
        map.emplace( Element::HTML5_MARK, "mark" );
        map.emplace( Element::HTML5_META, "meta" );
        map.emplace( Element::HTML5_MENU, "menu" );
        map.emplace( Element::HTML5_METER, "meter" );
        map.emplace( Element::HTML5_NAV, "nav" );
        map.emplace( Element::HTML5_NOSCRIPT, "noscript" );
        map.emplace( Element::HTML5_OBJECT, "object" );
        map.emplace( Element::HTML5_OL, "ol" );
        map.emplace( Element::HTML5_OPTGROUP, "optgroup" );
        map.emplace( Element::HTML5_OPTION, "option" );
        map.emplace( Element::HTML5_OUTPUT, "output" );
        map.emplace( Element::HTML5_P, "p" );
        map.emplace( Element::HTML5_PARAM, "param" );
        map.emplace( Element::HTML5_PICTURE, "picture" );
        map.emplace( Element::HTML5_PRE, "pre" );
        map.emplace( Element::HTML5_PROGRESS, "progress" );
        map.emplace( Element::HTML5_Q, "q" );
        map.emplace( Element::HTML5_RP, "rp" );
        map.emplace( Element::HTML5_RT, "rt" );
        map.emplace( Element::HTML5_RUBY, "ruby" );
        map.emplace( Element::HTML5_S, "s" );
        map.emplace( Element::HTML5_SAMP, "samp" );
        map.emplace( Element::HTML5_SCRIPT, "script" );
        map.emplace( Element::HTML5_SECTION, "section" );
        map.emplace( Element::HTML5_SELECT, "select" );
        map.emplace( Element::HTML5_SLOT, "slot" );
        map.emplace( Element::HTML5_SMALL, "small" );
        map.emplace( Element::HTML5_SOURCE, "source" );
        map.emplace( Element::HTML5_SPAN, "span" );
        map.emplace( Element::HTML5_STRONG, "strong" );
        map.emplace( Element::HTML5_STYLE, "style" );
        map.emplace( Element::HTML5_SUB, "sub" );
        map.emplace( Element::HTML5_SUMMARY, "summary" );
        map.emplace( Element::HTML5_SUP, "sup" );
        map.emplace( Element::HTML5_TABLE, "table" );
        map.emplace( Element::HTML5_TBODY, "tbody" );
        map.emplace( Element::HTML5_TD, "td" );
        map.emplace( Element::HTML5_TEMPLATE, "template" );
        map.emplace( Element::HTML5_TEXTAREA, "textarea" );
        map.emplace( Element::HTML5_TFOOT, "tfoot" );
        map.emplace( Element::HTML5_TH, "th" );
        map.emplace( Element::HTML5_THEAD, "thead" );
        map.emplace( Element::HTML5_TIME, "time" );
        map.emplace( Element::HTML5_TITLE, "title" );
        map.emplace( Element::HTML5_TR, "tr" );
        map.emplace( Element::HTML5_TRACK, "track" );
        map.emplace( Element::HTML5_U, "u" );
        map.emplace( Element::HTML5_UL, "ul" );
        map.emplace( Element::HTML5_VAR, "var" );
        map.emplace( Element::HTML5_VIDEO, "video" );
        map.emplace( Element::HTML5_WBR, "wbr" );
        map.emplace( Element::HTML5_WBR, "wbr" );
        map.emplace( Element::MATH, "math" );
        map.emplace( Element::MATHML_MACTION, "maction" );
        map.emplace( Element::MATHML_MALIGNGROUP, "maligngroup" );
        map.emplace( Element::MATHML_MALIGNMARK, "malignmark" );
        map.emplace( Element::MATHML_MENCLOSE, "menclose" );
        map.emplace( Element::MATHML_MERROR, "merror" );
        map.emplace( Element::MATHML_MFENCED, "mfenced" );
        map.emplace( Element::MATHML_MFRAC, "mfrac" );
        map.emplace( Element::MATHML_MGLYPH, "mglyph" );
        map.emplace( Element::MATHML_MI, "mi" );
        map.emplace( Element::MATHML_MLABELEDTR, "mlabeledtr" );
        map.emplace( Element::MATHML_MLONGDIV, "mlongdiv" );
        map.emplace( Element::MATHML_MMULTISCRIPT, "mmultiscript" );
        map.emplace( Element::MATHML_MN, "mn" );
        map.emplace( Element::MATHML_MO, "mo" );
        map.emplace( Element::MATHML_MOVER, "mover" );
        map.emplace( Element::MATHML_MPADDED, "mpadded" );
        map.emplace( Element::MATHML_MPHANTOM, "mphantom" );
        map.emplace( Element::MATHML_MROOT, "mroot" );
        map.emplace( Element::MATHML_MROW, "mrow" );
        map.emplace( Element::MATHML_MS, "ms" );
        map.emplace( Element::MATHML_MSCARRIES, "mscarries" );
        map.emplace( Element::MATHML_MSCARRY, "mscarry" );
        map.emplace( Element::MATHML_MSGROUP, "msgroup" );
        map.emplace( Element::MATHML_MSLINE, "msline" );
        map.emplace( Element::MATHML_MSPACE, "mspace" );
        map.emplace( Element::MATHML_MSQRT, "msqrt" );
        map.emplace( Element::MATHML_MSROW, "msrow" );
        map.emplace( Element::MATHML_MSTACK, "mstack" );
        map.emplace( Element::MATHML_MSTYLE, "mstyle" );
        map.emplace( Element::MATHML_MSUB, "msub" );
        map.emplace( Element::MATHML_MSUP, "msup" );
        map.emplace( Element::MATHML_MSUBSUP, "msubsup" );
        map.emplace( Element::MATHML_MTABLE, "mtable" );
        map.emplace( Element::MATHML_MTD, "mtd" );
        map.emplace( Element::MATHML_MTEXT, "mtext" );
        map.emplace( Element::MATHML_MTR, "mtr" );
        map.emplace( Element::MATHML_MUNDER, "munder" );
        map.emplace( Element::MATHML_MUNDEROVER, "munderover" );
        map.emplace( Element::MATHML_SEMANTICS, "semantics" );
        map.emplace( Element::MATHML_ANNOTATION, "annotation" );
        map.emplace( Element::MATHML_ANNOTATION_XML, "annotation_xml" );
        map.emplace( Element::SVG, "svg" );
        map.emplace( Element::SVG_A, "a" );
        map.emplace( Element::SVG_ANIMATE, "animate" );
        map.emplace( Element::SVG_ANIMATEMOTION, "animatemotion" );
        map.emplace( Element::SVG_ANIMATETRANSFORM, "animatetransform" );
        map.emplace( Element::SVG_CIRCLE, "circle" );
        map.emplace( Element::SVG_CLIPPATH, "clippath" );
        map.emplace( Element::SVG_COLOR_PROFILE, "color_profile" );
        map.emplace( Element::SVG_DEFS, "defs" );
        map.emplace( Element::SVG_DESC, "desc" );
        map.emplace( Element::SVG_DISCARD, "discard" );
        map.emplace( Element::SVG_ELLIPSE, "ellipse" );
        map.emplace( Element::SVG_FEBLEND, "feblend" );
        map.emplace( Element::SVG_FECOLORMATRIX, "fecolormatrix" );
        map.emplace( Element::SVG_FECOMPONENTTRANSFER, "fecomponenttransfer" );
        map.emplace( Element::SVG_FECOMPOSITE, "fecomposite" );
        map.emplace( Element::SVG_FECONVOLVEMATRIX, "feconvolvematrix" );
        map.emplace( Element::SVG_FEDIFFUSELIGHTING, "fediffuselighting" );
        map.emplace( Element::SVG_FEDISPLACEMENTMAP, "fedisplacementmap" );
        map.emplace( Element::SVG_FEDISTANTLIGHT, "fedistantlight" );
        map.emplace( Element::SVG_FEDROPSHADOW, "fedropshadow" );
        map.emplace( Element::SVG_FEFLOOD, "feflood" );
        map.emplace( Element::SVG_FEFUNCA, "fefunca" );
        map.emplace( Element::SVG_FEFUNCB, "fefuncb" );
        map.emplace( Element::SVG_FEFUNCG, "fefuncg" );
        map.emplace( Element::SVG_FEFUNCR, "fefuncr" );
        map.emplace( Element::SVG_FEGAUSSIANBLUR, "fegaussianblur" );
        map.emplace( Element::SVG_FEIMAGE, "feimage" );
        map.emplace( Element::SVG_FEMERGE, "femerge" );
        map.emplace( Element::SVG_FEMERGENODE, "femergenode" );
        map.emplace( Element::SVG_FEMORPHOLOGY, "femorphology" );
        map.emplace( Element::SVG_FEOFFSET, "feoffset" );
        map.emplace( Element::SVG_FEPOINTLIGHT, "fepointlight" );
        map.emplace( Element::SVG_FESPECULARLIGHTING, "fespecularlighting" );
        map.emplace( Element::SVG_FESPOTLIGHT, "fespotlight" );
        map.emplace( Element::SVG_FETILE, "fetile" );
        map.emplace( Element::SVG_FETURBULENCE, "feturbulence" );
        map.emplace( Element::SVG_FILTER, "filter" );
        map.emplace( Element::SVG_FOREIGNOBJECT, "foreignobject" );
        map.emplace( Element::SVG_G, "g" );
        map.emplace( Element::SVG_HATCH, "hatch" );
        map.emplace( Element::SVG_HATCHPATH, "hatchpath" );
        map.emplace( Element::SVG_IMAGE, "image" );
        map.emplace( Element::SVG_LINE, "line" );
        map.emplace( Element::SVG_LINEARGRADIENT, "lineargradient" );
        map.emplace( Element::SVG_MARKER, "marker" );
        map.emplace( Element::SVG_MASK, "mask" );
        map.emplace( Element::SVG_MESH, "mesh" );
        map.emplace( Element::SVG_MESHGRADIENT, "meshgradient" );
        map.emplace( Element::SVG_MESHPATCH, "meshpatch" );
        map.emplace( Element::SVG_MESHROW, "meshrow" );
        map.emplace( Element::SVG_METADATA, "metadata" );
        map.emplace( Element::SVG_MPATH, "mpath" );
        map.emplace( Element::SVG_PATH, "path" );
        map.emplace( Element::SVG_PATTERN, "pattern" );
        map.emplace( Element::SVG_POLYGON, "polygon" );
        map.emplace( Element::SVG_POLYLINE, "polyline" );
        map.emplace( Element::SVG_RADIALGRADIENT, "radialgradient" );
        map.emplace( Element::SVG_RECT, "rect" );
        map.emplace( Element::SVG_SCRIPT, "script" );
        map.emplace( Element::SVG_SET, "set" );
        map.emplace( Element::SVG_SOLIDCOLOR, "solidcolor" );
        map.emplace( Element::SVG_STOP, "stop" );
        map.emplace( Element::SVG_STYLE, "style" );
        map.emplace( Element::SVG_SWITCH, "switch" );
        map.emplace( Element::SVG_SYMBOL, "symbol" );
        map.emplace( Element::SVG_TEXT, "text" );
        map.emplace( Element::SVG_TEXTPATH, "textpath" );
        map.emplace( Element::SVG_TITLE, "title" );
        map.emplace( Element::SVG_TSPAN, "tspan" );
        map.emplace( Element::SVG_UNKNOWN, "unknown" );
        map.emplace( Element::SVG_USE, "use" );
        map.emplace( Element::SVG_VIEW, "view" );
    }

    if( map.contains( el ) ) {
        os << map.at( el );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Gets the an Element's HTML Namespace
 * @param el Element
 * @return HTML namespace
 */
blogator::parser::specs::infra::Namespace blogator::parser::specs::infra::getNamespace( Element el ) {
    using blogator::parser::specs::infra::Element;
    using blogator::parser::specs::infra::Namespace;

    if( el >= Element::HTML5_NS_BEGIN && el <= Element::HTML5_NS_END ) {
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
 * Gets a element type from a string
 * @param el Element string
 * @return Element type (Element::UNKNOWN if not found)
 */
blogator::parser::specs::infra::Element blogator::parser::specs::infra::getElementType( const std::u32string &el ) {
    static std::map<std::u32string , specs::infra::Element> map;

    if( map.empty() ) {
        //i = 1 as 'Element::UNKNOWN' (0) can be skipped
        for( int i = 1; i <= static_cast<int>( blogator::parser::specs::infra::Element::ENUM_END ); ++i ) {
            //Since string representation of elements are all in ASCII range
            //conversion to u32 can just be done with direct casting.
            auto u8str = blogator::to_string( static_cast<specs::infra::Element>( i ) );
            map.emplace( std::u32string( u8str.cbegin(), u8str.cend() ), static_cast<specs::infra::Element>( i ) );
        }
    }

    auto it = map.find( el );

    if( it != map.cend() ) {
        return it->second;
    } else {
        return specs::infra::Element::UNKNOWN;
    }
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
