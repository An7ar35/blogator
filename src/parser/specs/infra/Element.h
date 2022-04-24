#ifndef BLOGATOR_PARSER_SPECS_HTML5_ELEMENTS_H
#define BLOGATOR_PARSER_SPECS_HTML5_ELEMENTS_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::infra {
    enum class Namespace;

    enum class Element {
        UNKNOWN = 0, //not resolved
        OTHER   = 1, //not typed

        /**
         * Deprecated tags (backward compatibility)
         */
        DEPR_HTML_APPLET,
        HTML_NS_BEGIN = DEPR_HTML_APPLET, //Not HTML - used for iterating enums
        DEPR_NS_BEGIN = HTML_NS_BEGIN,    //Not HTML - used for iterating enums
        DEPR_HTML_BASEFONT,
        DEPR_HTML_BGSOUND,
        DEPR_HTML_BIG,
        DEPR_HTML_CENTER,
        DEPR_HTML_DIR,
        DEPR_HTML_FONT,
        DEPR_HTML_FRAME,
        DEPR_HTML_FRAMESET,
        DEPR_HTML_IMAGE,
        DEPR_HTML_KEYGEN,
        DEPR_HTML_LISTING,
        DEPR_HTML_MARQUEE,
        DEPR_HTML_MENUITEM,
        DEPR_HTML_NOBR,
        DEPR_HTML_NOEMEBED,
        DEPR_HTML_NOFRAMES,
        DEPR_HTML_PLAINTEXT,
        DEPR_HTML_RB,
        DEPR_HTML_RTC,
        DEPR_HTML_STRIKE,
        DEPR_HTML_TT,
        DEPR_HTML_XMP,
        DEPR_NS_END = DEPR_HTML_XMP, //Not HTML - used for iterating enums

        /**
         * HTML5::core elements
         */

        HTML5_A,
        HTML5_NS_BEGIN = HTML5_A, //Not HTML - used for iterating enums
        HTML5_ABBR, HTML5_ADDRESS, HTML5_AREA, HTML5_ARTICLE, HTML5_ASIDE, HTML5_AUDIO,
        HTML5_B, HTML5_BASE, HTML5_BDI, HTML5_BDO, HTML5_BLOCKQUOTE, HTML5_BODY, HTML5_BR, HTML5_BUTTON,
        HTML5_CANVAS, HTML5_CAPTION, HTML5_CITE, HTML5_CODE, HTML5_COL, HTML5_COLGROUP, HTML5_COMMENT,
        HTML5_DATA, HTML5_DATALIST, HTML5_DD, HTML5_DEL, HTML5_DETAILS, HTML5_DFN, HTML5_DIALOG,
            HTML5_DIV, HTML5_DL, HTML5_DOCTYPE, HTML5_DT,
        HTML5_EM, HTML5_EMBED,
        HTML5_FIELDSET, HTML5_FIGCAPTION, HTML5_FIGURE, HTML5_FOOTER, HTML5_FORM,
        HTML5_H1, HTML5_H2, HTML5_H3, HTML5_H4, HTML5_H5, HTML5_H6, HTML5_HEAD, HTML5_HEADER, HTML5_HGROUP, HTML5_HR, HTML5_HTML,
        HTML5_I, HTML5_IFRAME, HTML5_IMG, HTML5_INPUT, HTML5_INS,
        HTML5_KBD,
        HTML5_LABEL, HTML5_LEGEND, HTML5_LI, HTML5_LINK,
        HTML5_MAIN, HTML5_MAP, HTML5_MARK, HTML5_META, HTML5_MENU, HTML5_METER,
        HTML5_NAV, HTML5_NOSCRIPT,
        HTML5_OBJECT, HTML5_OL, HTML5_OPTGROUP, HTML5_OPTION, HTML5_OUTPUT,
        HTML5_P, HTML5_PARAM, HTML5_PICTURE, HTML5_PRE, HTML5_PROGRESS,
        HTML5_Q,
        HTML5_RP, HTML5_RT, HTML5_RUBY,
        HTML5_S, HTML5_SAMP, HTML5_SCRIPT, HTML5_SEARCH, HTML5_SECTION, HTML5_SELECT, HTML5_SLOT, HTML5_SMALL,
            HTML5_SOURCE, HTML5_SPAN, HTML5_STRONG, HTML5_STYLE, HTML5_SUB, HTML5_SUMMARY, HTML5_SUP,
        HTML5_TABLE, HTML5_TBODY, HTML5_TD, HTML5_TEMPLATE, HTML5_TEXTAREA, HTML5_TFOOT, HTML5_TH,
            HTML5_THEAD, HTML5_TIME, HTML5_TITLE, HTML5_TR, HTML5_TRACK,
        HTML5_U, HTML5_UL,
        HTML5_VAR, HTML5_VIDEO,
        HTML5_WBR,
        HTML5_NS_END = HTML5_WBR,   //Not HTML - used for iterating enums
        HTML_NS_END = HTML5_NS_END, //Not HTML - used for iterating enums

        /**
         * HTML5::MathML3.0 elements
         * (https://developer.mozilla.org/en-US/docs/Web/MathML/Element)
         */
        MATH,
        MATHML_NS_BEGIN = MATH, //Not HTML - used for iterating enums
        MATHML_MACTION, MATHML_MALIGNGROUP, MATHML_MALIGNMARK,
        MATHML_MENCLOSE, MATHML_MERROR,
        MATHML_MFENCED, MATHML_MFRAC,
        MATHML_MGLYPH,
        MATHML_MI,
        MATHML_MLABELEDTR, MATHML_MLONGDIV,
        MATHML_MMULTISCRIPT,
        MATHML_MN,
        MATHML_MO, MATHML_MOVER,
        MATHML_MPADDED, MATHML_MPHANTOM,
        MATHML_MROOT, MATHML_MROW,
        MATHML_MS, MATHML_MSCARRIES, MATHML_MSCARRY, MATHML_MSGROUP, MATHML_MSLINE, MATHML_MSPACE,
            MATHML_MSQRT, MATHML_MSROW, MATHML_MSTACK, MATHML_MSTYLE, MATHML_MSUB, MATHML_MSUP, MATHML_MSUBSUP,
        MATHML_MTABLE, MATHML_MTD, MATHML_MTEXT, MATHML_MTR,
        MATHML_MUNDER, MATHML_MUNDEROVER,
        MATHML_SEMANTICS, MATHML_ANNOTATION, MATHML_ANNOTATION_XML,
        MATHML_NS_END = MATHML_ANNOTATION_XML, //Not HTML - used for iterating enums

        /*
         * HTML::SVG elements
         * https://developer.mozilla.org/en-US/docs/Web/SVG/Element
         */
        SVG,
        SVG_NS_BEGIN = SVG, //Not HTML - used for iterating enums
        SVG_A, SVG_ALTGLYPH, SVG_ALTGLYPHDEF, SVG_ALTGLYPHITEM, SVG_ANIMATE, SVG_ANIMATECOLOR, SVG_ANIMATEMOTION, SVG_ANIMATETRANSFORM,
        SVG_CIRCLE, SVG_CLIPPATH, SVG_COLOR_PROFILE,
        SVG_DEFS, SVG_DESC, SVG_DISCARD,
        SVG_ELLIPSE,
        SVG_FEBLEND,
        SVG_FECOLORMATRIX, SVG_FECOMPONENTTRANSFER, SVG_FECOMPOSITE, SVG_FECONVOLVEMATRIX,
        SVG_FEDIFFUSELIGHTING, SVG_FEDISPLACEMENTMAP, SVG_FEDISTANTLIGHT, SVG_FEDROPSHADOW,
        SVG_FEFLOOD, SVG_FEFUNCA, SVG_FEFUNCB, SVG_FEFUNCG, SVG_FEFUNCR,
        SVG_FEGAUSSIANBLUR,
        SVG_FEIMAGE,
        SVG_FEMERGE, SVG_FEMERGENODE, SVG_FEMORPHOLOGY,
        SVG_FEOFFSET,
        SVG_FEPOINTLIGHT,
        SVG_FESPECULARLIGHTING, SVG_FESPOTLIGHT,
        SVG_FETILE, SVG_FETURBULENCE,
        SVG_FILTER,
        SVG_FOREIGNOBJECT,
        SVG_G, SVG_GLYPHREF,
        SVG_HATCH, SVG_HATCHPATH,
        SVG_IMAGE,
        SVG_LINE, SVG_LINEARGRADIENT,
        SVG_MARKER, SVG_MASK, SVG_MESH, SVG_MESHGRADIENT, SVG_MESHPATCH, SVG_MESHROW, SVG_METADATA, SVG_MPATH,
        SVG_PATH, SVG_PATTERN, SVG_POLYGON, SVG_POLYLINE,
        SVG_RADIALGRADIENT, SVG_RECT,
        SVG_SCRIPT, SVG_SET, SVG_SOLIDCOLOR, SVG_STOP, SVG_STYLE, SVG_SWITCH, SVG_SYMBOL,
        SVG_TEXT, SVG_TEXTPATH, SVG_TITLE, SVG_TSPAN,
        SVG_UNKNOWN, SVG_USE,
        SVG_VIEW,
        SVG_NS_END = SVG_VIEW, //Not HTML - used for iterating enums

        ENUM_END = SVG_NS_END//Not HTML - used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, Element el );
    specs::infra::Namespace getNamespace( specs::infra::Element el );
    specs::infra::Namespace resolveNamespace( specs::infra::Element el );
    specs::infra::Element resolveElementType( Namespace ns_hint, std::u32string & el );
    bool isAmbiguousElement( const std::u32string & el );
    bool isAmbiguousElement( specs::infra::Element el );
    std::u32string & adjustSVGElementTagName( std::u32string & name );
    //TODO create paired, self closing lookup
}

namespace blogator {
    std::string to_string( blogator::parser::specs::infra::Element el );
    std::u32string to_u32string( blogator::parser::specs::infra::Element el );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_ELEMENTS_H