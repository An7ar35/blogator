#include "Element.h"

#include <map>
#include <ostream>
#include <sstream>

using namespace blogator::parser::specs::html5;

/**
 * Output stream operator
 * @param os Output stream
 * @param el Element enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::html5::operator <<( std::ostream &os, blogator::parser::specs::html5::Element el ) {
    static std::map<Element, std::string> map;

    if( map.empty() ) {
        map.emplace( Element::UNKNOWN, "UNKNOWN" );
        map.emplace( Element::HTML5_A, "HTML5_A" );
        map.emplace( Element::HTML5_ABBR, "HTML5_ABBR" );
        map.emplace( Element::HTML5_ADDRESS, "HTML5_ADDRESS" );
        map.emplace( Element::HTML5_AREA, "HTML5_AREA" );
        map.emplace( Element::HTML5_ARTICLE, "HTML5_ARTICLE" );
        map.emplace( Element::HTML5_ASIDE, "HTML5_ASIDE" );
        map.emplace( Element::HTML5_AUDIO, "HTML5_AUDIO" );
        map.emplace( Element::HTML5_B, "HTML5_B" );
        map.emplace( Element::HTML5_BASE, "HTML5_BASE" );
        map.emplace( Element::HTML5_BDI, "HTML5_BDI" );
        map.emplace( Element::HTML5_BDO, "HTML5_BDO" );
        map.emplace( Element::HTML5_BLOCKQUOTE, "HTML5_BLOCKQUOTE" );
        map.emplace( Element::HTML5_BODY, "HTML5_BODY" );
        map.emplace( Element::HTML5_BR, "HTML5_BR" );
        map.emplace( Element::HTML5_BUTTON, "HTML5_BUTTON" );
        map.emplace( Element::HTML5_CANVAS, "HTML5_CANVAS" );
        map.emplace( Element::HTML5_CAPTION, "HTML5_CAPTION" );
        map.emplace( Element::HTML5_CITE, "HTML5_CITE" );
        map.emplace( Element::HTML5_CODE, "HTML5_CODE" );
        map.emplace( Element::HTML5_COL, "HTML5_COL" );
        map.emplace( Element::HTML5_COLGROUP, "HTML5_COLGROUP" );
        map.emplace( Element::HTML5_COMMENT, "HTML5_COMMENT" );
        map.emplace( Element::HTML5_DATA, "HTML5_DATA" );
        map.emplace( Element::HTML5_DATALIST, "HTML5_DATALIST" );
        map.emplace( Element::HTML5_DD, "HTML5_DD" );
        map.emplace( Element::HTML5_DEL, "HTML5_DEL" );
        map.emplace( Element::HTML5_DETAILS, "HTML5_DETAILS" );
        map.emplace( Element::HTML5_DFN, "HTML5_DFN" );
        map.emplace( Element::HTML5_DIALOG, "HTML5_DIALOG" );
        map.emplace( Element::HTML5_DIV, "HTML5_DIV" );
        map.emplace( Element::HTML5_DL, "HTML5_DL" );
        map.emplace( Element::HTML5_DOCTYPE, "HTML5_DOCTYPE" );
        map.emplace( Element::HTML5_DT, "HTML5_DT" );
        map.emplace( Element::HTML5_EM, "HTML5_EM" );
        map.emplace( Element::HTML5_EMBED, "HTML5_EMBED" );
        map.emplace( Element::HTML5_FIELDSET, "HTML5_FIELDSET" );
        map.emplace( Element::HTML5_FIGCAPTION, "HTML5_FIGCAPTION" );
        map.emplace( Element::HTML5_FIGURE, "HTML5_FIGURE" );
        map.emplace( Element::HTML5_FOOTER, "HTML5_FOOTER" );
        map.emplace( Element::HTML5_FORM, "HTML5_FORM" );
        map.emplace( Element::HTML5_H1, "HTML5_H1" );
        map.emplace( Element::HTML5_H2, "HTML5_H2" );
        map.emplace( Element::HTML5_H3, "HTML5_H3" );
        map.emplace( Element::HTML5_H4, "HTML5_H4" );
        map.emplace( Element::HTML5_H5, "HTML5_H5" );
        map.emplace( Element::HTML5_H6, "HTML5_H6" );
        map.emplace( Element::HTML5_HEAD, "HTML5_HEAD" );
        map.emplace( Element::HTML5_HEADER, "HTML5_HEADER" );
        map.emplace( Element::HTML5_HGROUP, "HTML5_HGROUP" );
        map.emplace( Element::HTML5_HR, "HTML5_HR" );
        map.emplace( Element::HTML5_HTML, "HTML5_HTML" );
        map.emplace( Element::HTML5_I, "HTML5_I" );
        map.emplace( Element::HTML5_IFRAME, "HTML5_IFRAME" );
        map.emplace( Element::HTML5_IMG, "HTML5_IMG" );
        map.emplace( Element::HTML5_INPUT, "HTML5_INPUT" );
        map.emplace( Element::HTML5_INS, "HTML5_INS" );
        map.emplace( Element::HTML5_KBD, "HTML5_KBD" );
        map.emplace( Element::HTML5_LABEL, "HTML5_LABEL" );
        map.emplace( Element::HTML5_LEGEND, "HTML5_LEGEND" );
        map.emplace( Element::HTML5_LI, "HTML5_LI" );
        map.emplace( Element::HTML5_LINK, "HTML5_LINK" );
        map.emplace( Element::HTML5_MAIN, "HTML5_MAIN" );
        map.emplace( Element::HTML5_MAP, "HTML5_MAP" );
        map.emplace( Element::HTML5_MARK, "HTML5_MARK" );
        map.emplace( Element::HTML5_META, "HTML5_META" );
        map.emplace( Element::HTML5_MENU, "HTML5_MENU" );
        map.emplace( Element::HTML5_METER, "HTML5_METER" );
        map.emplace( Element::HTML5_NAV, "HTML5_NAV" );
        map.emplace( Element::HTML5_NOSCRIPT, "HTML5_NOSCRIPT" );
        map.emplace( Element::HTML5_OBJECT, "HTML5_OBJECT" );
        map.emplace( Element::HTML5_OL, "HTML5_OL" );
        map.emplace( Element::HTML5_OPTGROUP, "HTML5_OPTGROUP" );
        map.emplace( Element::HTML5_OPTION, "HTML5_OPTION" );
        map.emplace( Element::HTML5_OUTPUT, "HTML5_OUTPUT" );
        map.emplace( Element::HTML5_P, "HTML5_P" );
        map.emplace( Element::HTML5_PARAM, "HTML5_PARAM" );
        map.emplace( Element::HTML5_PICTURE, "HTML5_PICTURE" );
        map.emplace( Element::HTML5_PRE, "HTML5_PRE" );
        map.emplace( Element::HTML5_PROGRESS, "HTML5_PROGRESS" );
        map.emplace( Element::HTML5_Q, "HTML5_Q" );
        map.emplace( Element::HTML5_RP, "HTML5_RP" );
        map.emplace( Element::HTML5_RT, "HTML5_RT" );
        map.emplace( Element::HTML5_RUBY, "HTML5_RUBY" );
        map.emplace( Element::HTML5_S, "HTML5_S" );
        map.emplace( Element::HTML5_SAMP, "HTML5_SAMP" );
        map.emplace( Element::HTML5_SCRIPT, "HTML5_SCRIPT" );
        map.emplace( Element::HTML5_SECTION, "HTML5_SECTION" );
        map.emplace( Element::HTML5_SELECT, "HTML5_SELECT" );
        map.emplace( Element::HTML5_SLOT, "HTML5_SLOT" );
        map.emplace( Element::HTML5_SMALL, "HTML5_SMALL" );
        map.emplace( Element::HTML5_SOURCE, "HTML5_SOURCE" );
        map.emplace( Element::HTML5_SPAN, "HTML5_SPAN" );
        map.emplace( Element::HTML5_STRONG, "HTML5_STRONG" );
        map.emplace( Element::HTML5_STYLE, "HTML5_STYLE" );
        map.emplace( Element::HTML5_SUB, "HTML5_SUB" );
        map.emplace( Element::HTML5_SUMMARY, "HTML5_SUMMARY" );
        map.emplace( Element::HTML5_SUP, "HTML5_SUP" );
        map.emplace( Element::HTML5_TABLE, "HTML5_TABLE" );
        map.emplace( Element::HTML5_TBODY, "HTML5_TBODY" );
        map.emplace( Element::HTML5_TD, "HTML5_TD" );
        map.emplace( Element::HTML5_TEMPLATE, "HTML5_TEMPLATE" );
        map.emplace( Element::HTML5_TEXTAREA, "HTML5_TEXTAREA" );
        map.emplace( Element::HTML5_TFOOT, "HTML5_TFOOT" );
        map.emplace( Element::HTML5_TH, "HTML5_TH" );
        map.emplace( Element::HTML5_THEAD, "HTML5_THEAD" );
        map.emplace( Element::HTML5_TIME, "HTML5_TIME" );
        map.emplace( Element::HTML5_TITLE, "HTML5_TITLE" );
        map.emplace( Element::HTML5_TR, "HTML5_TR" );
        map.emplace( Element::HTML5_TRACK, "HTML5_TRACK" );
        map.emplace( Element::HTML5_U, "HTML5_U" );
        map.emplace( Element::HTML5_UL, "HTML5_UL" );
        map.emplace( Element::HTML5_VAR, "HTML5_VAR" );
        map.emplace( Element::HTML5_VIDEO, "HTML5_VIDEO" );
        map.emplace( Element::HTML5_WBR, "HTML5_WBR" );
        map.emplace( Element::HTML5_WBR, "HTML5_WBR" );
        map.emplace( Element::MATH, "MATH" );
        map.emplace( Element::MATHML_MACTION, "MATHML_MACTION" );
        map.emplace( Element::MATHML_MALIGNGROUP, "MATHML_MALIGNGROUP" );
        map.emplace( Element::MATHML_MALIGNMARK, "MATHML_MALIGNMARK" );
        map.emplace( Element::MATHML_MENCLOSE, "MATHML_MENCLOSE" );
        map.emplace( Element::MATHML_MERROR, "MATHML_MERROR" );
        map.emplace( Element::MATHML_MFENCED, "MATHML_MFENCED" );
        map.emplace( Element::MATHML_MFRAC, "MATHML_MFRAC" );
        map.emplace( Element::MATHML_MGLYPH, "MATHML_MGLYPH" );
        map.emplace( Element::MATHML_MI, "MATHML_MI" );
        map.emplace( Element::MATHML_MLABELEDTR, "MATHML_MLABELEDTR" );
        map.emplace( Element::MATHML_MLONGDIV, "MATHML_MLONGDIV" );
        map.emplace( Element::MATHML_MMULTISCRIPT, "MATHML_MMULTISCRIPT" );
        map.emplace( Element::MATHML_MN, "MATHML_MN" );
        map.emplace( Element::MATHML_MO, "MATHML_MO" );
        map.emplace( Element::MATHML_MOVER, "MATHML_MOVER" );
        map.emplace( Element::MATHML_MPADDED, "MATHML_MPADDED" );
        map.emplace( Element::MATHML_MPHANTOM, "MATHML_MPHANTOM" );
        map.emplace( Element::MATHML_MROOT, "MATHML_MROOT" );
        map.emplace( Element::MATHML_MROW, "MATHML_MROW" );
        map.emplace( Element::MATHML_MS, "MATHML_MS" );
        map.emplace( Element::MATHML_MSCARRIES, "MATHML_MSCARRIES" );
        map.emplace( Element::MATHML_MSCARRY, "MATHML_MSCARRY" );
        map.emplace( Element::MATHML_MSGROUP, "MATHML_MSGROUP" );
        map.emplace( Element::MATHML_MSLINE, "MATHML_MSLINE" );
        map.emplace( Element::MATHML_MSPACE, "MATHML_MSPACE" );
        map.emplace( Element::MATHML_MSQRT, "MATHML_MSQRT" );
        map.emplace( Element::MATHML_MSROW, "MATHML_MSROW" );
        map.emplace( Element::MATHML_MSTACK, "MATHML_MSTACK" );
        map.emplace( Element::MATHML_MSTYLE, "MATHML_MSTYLE" );
        map.emplace( Element::MATHML_MSUB, "MATHML_MSUB" );
        map.emplace( Element::MATHML_MSUP, "MATHML_MSUP" );
        map.emplace( Element::MATHML_MSUBSUP, "MATHML_MSUBSUP" );
        map.emplace( Element::MATHML_MTABLE, "MATHML_MTABLE" );
        map.emplace( Element::MATHML_MTD, "MATHML_MTD" );
        map.emplace( Element::MATHML_MTEXT, "MATHML_MTEXT" );
        map.emplace( Element::MATHML_MTR, "MATHML_MTR" );
        map.emplace( Element::MATHML_MUNDER, "MATHML_MUNDER" );
        map.emplace( Element::MATHML_MUNDEROVER, "MATHML_MUNDEROVER" );
        map.emplace( Element::MATHML_SEMANTICS, "MATHML_SEMANTICS" );
        map.emplace( Element::MATHML_ANNOTATION, "MATHML_ANNOTATION" );
        map.emplace( Element::MATHML_ANNOTATION_XML, "MATHML_ANNOTATION_XML" );
        map.emplace( Element::SVG, "SVG" );
        map.emplace( Element::SVG_A, "SVG_A" );
        map.emplace( Element::SVG_ANIMATE, "SVG_ANIMATE" );
        map.emplace( Element::SVG_ANIMATEMOTION, "SVG_ANIMATEMOTION" );
        map.emplace( Element::SVG_ANIMATETRANSFORM, "SVG_ANIMATETRANSFORM" );
        map.emplace( Element::SVG_CIRCLE, "SVG_CIRCLE" );
        map.emplace( Element::SVG_CLIPPATH, "SVG_CLIPPATH" );
        map.emplace( Element::SVG_COLOR_PROFILE, "SVG_COLOR_PROFILE" );
        map.emplace( Element::SVG_DEFS, "SVG_DEFS" );
        map.emplace( Element::SVG_DESC, "SVG_DESC" );
        map.emplace( Element::SVG_DISCARD, "SVG_DISCARD" );
        map.emplace( Element::SVG_ELLIPSE, "SVG_ELLIPSE" );
        map.emplace( Element::SVG_FEBLEND, "SVG_FEBLEND" );
        map.emplace( Element::SVG_FECOLORMATRIX, "SVG_FECOLORMATRIX" );
        map.emplace( Element::SVG_FECOMPONENTTRANSFER, "SVG_FECOMPONENTTRANSFER" );
        map.emplace( Element::SVG_FECOMPOSITE, "SVG_FECOMPOSITE" );
        map.emplace( Element::SVG_FECONVOLVEMATRIX, "SVG_FECONVOLVEMATRIX" );
        map.emplace( Element::SVG_FEDIFFUSELIGHTING, "SVG_FEDIFFUSELIGHTING" );
        map.emplace( Element::SVG_FEDISPLACEMENTMAP, "SVG_FEDISPLACEMENTMAP" );
        map.emplace( Element::SVG_FEDISTANTLIGHT, "SVG_FEDISTANTLIGHT" );
        map.emplace( Element::SVG_FEDROPSHADOW, "SVG_FEDROPSHADOW" );
        map.emplace( Element::SVG_FEFLOOD, "SVG_FEFLOOD" );
        map.emplace( Element::SVG_FEFUNCA, "SVG_FEFUNCA" );
        map.emplace( Element::SVG_FEFUNCB, "SVG_FEFUNCB" );
        map.emplace( Element::SVG_FEFUNCG, "SVG_FEFUNCG" );
        map.emplace( Element::SVG_FEFUNCR, "SVG_FEFUNCR" );
        map.emplace( Element::SVG_FEGAUSSIANBLUR, "SVG_FEGAUSSIANBLUR" );
        map.emplace( Element::SVG_FEIMAGE, "SVG_FEIMAGE" );
        map.emplace( Element::SVG_FEMERGE, "SVG_FEMERGE" );
        map.emplace( Element::SVG_FEMERGENODE, "SVG_FEMERGENODE" );
        map.emplace( Element::SVG_FEMORPHOLOGY, "SVG_FEMORPHOLOGY" );
        map.emplace( Element::SVG_FEOFFSET, "SVG_FEOFFSET" );
        map.emplace( Element::SVG_FEPOINTLIGHT, "SVG_FEPOINTLIGHT" );
        map.emplace( Element::SVG_FESPECULARLIGHTING, "SVG_FESPECULARLIGHTING" );
        map.emplace( Element::SVG_FESPOTLIGHT, "SVG_FESPOTLIGHT" );
        map.emplace( Element::SVG_FETILE, "SVG_FETILE" );
        map.emplace( Element::SVG_FETURBULENCE, "SVG_FETURBULENCE" );
        map.emplace( Element::SVG_FILTER, "SVG_FILTER" );
        map.emplace( Element::SVG_FOREIGNOBJECT, "SVG_FOREIGNOBJECT" );
        map.emplace( Element::SVG_G, "SVG_G" );
        map.emplace( Element::SVG_HATCH, "SVG_HATCH" );
        map.emplace( Element::SVG_HATCHPATH, "SVG_HATCHPATH" );
        map.emplace( Element::SVG_IMAGE, "SVG_IMAGE" );
        map.emplace( Element::SVG_LINE, "SVG_LINE" );
        map.emplace( Element::SVG_LINEARGRADIENT, "SVG_LINEARGRADIENT" );
        map.emplace( Element::SVG_MARKER, "SVG_MARKER" );
        map.emplace( Element::SVG_MASK, "SVG_MASK" );
        map.emplace( Element::SVG_MESH, "SVG_MESH" );
        map.emplace( Element::SVG_MESHGRADIENT, "SVG_MESHGRADIENT" );
        map.emplace( Element::SVG_MESHPATCH, "SVG_MESHPATCH" );
        map.emplace( Element::SVG_MESHROW, "SVG_MESHROW" );
        map.emplace( Element::SVG_METADATA, "SVG_METADATA" );
        map.emplace( Element::SVG_MPATH, "SVG_MPATH" );
        map.emplace( Element::SVG_PATH, "SVG_PATH" );
        map.emplace( Element::SVG_PATTERN, "SVG_PATTERN" );
        map.emplace( Element::SVG_POLYGON, "SVG_POLYGON" );
        map.emplace( Element::SVG_POLYLINE, "SVG_POLYLINE" );
        map.emplace( Element::SVG_RADIALGRADIENT, "SVG_RADIALGRADIENT" );
        map.emplace( Element::SVG_RECT, "SVG_RECT" );
        map.emplace( Element::SVG_SCRIPT, "SVG_SCRIPT" );
        map.emplace( Element::SVG_SET, "SVG_SET" );
        map.emplace( Element::SVG_SOLIDCOLOR, "SVG_SOLIDCOLOR" );
        map.emplace( Element::SVG_STOP, "SVG_STOP" );
        map.emplace( Element::SVG_STYLE, "SVG_STYLE" );
        map.emplace( Element::SVG_SWITCH, "SVG_SWITCH" );
        map.emplace( Element::SVG_SYMBOL, "SVG_SYMBOL" );
        map.emplace( Element::SVG_TEXT, "SVG_TEXT" );
        map.emplace( Element::SVG_TEXTPATH, "SVG_TEXTPATH" );
        map.emplace( Element::SVG_TITLE, "SVG_TITLE" );
        map.emplace( Element::SVG_TSPAN, "SVG_TSPAN" );
        map.emplace( Element::SVG_UNKNOWN, "SVG_UNKNOWN" );
        map.emplace( Element::SVG_USE, "SVG_USE" );
        map.emplace( Element::SVG_VIEW, "SVG_VIEW" );
    }

    if( map.contains( el ) ) {
        os << map.at( el );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::html5::Element enum to a string representation
 * @param el Element enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::html5::Element el ) {
    std::stringstream ss;
    ss << el;
    return ss.str();
}
