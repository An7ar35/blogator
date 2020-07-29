#ifndef BLOGATOR_DOM_HTML5_TAG_H
#define BLOGATOR_DOM_HTML5_TAG_H

namespace blogator::dom::html5 {
    /**
     * Allowed tag types
     */
    enum class Tag {
        /* Blogator (for internal use only) */
        NONE = 0, //for the root node and any leaf nodes with untagged data (raw)

        /* HTML5 */
        A, ABBR, ADDRESS, AREA, ARTICLE, ASIDE, AUDIO,
        B, BASE, BDI, BDO, BLOCKQUOTE, BODY, BR, BUTTON,
        CANVAS, CAPTION, CITE, CODE, COL, COLGROUP, COMMENT,
        DATA, DATALIST, DD, DEL, DETAILS, DFN, DIALOG, DIV, DL, DOCTYPE, DT,
        EM, EMBED,
        FIELDSET, FIGCAPTION, FIGURE, FOOTER, FORM,
        H1, H2, H3, H4, H5, H6, HEAD, HEADER, HR, HTML,
        I, IFRAME, IMG, INPUT, INS,
        KBD,
        LABEL, LEGEND, LI, LINK,
        MAIN, MAP, MARK, META, METER,
        NAV, NOSCRIPT,
        OBJECT, OL, OPTGROUP, OPTION, OUTPUT,
        P, PARAM, PICTURE, PRE, PROGRESS,
        Q,
        RP, RT, RUBY,
        S, SAMP, SCRIPT, SECTION, SELECT, SMALL, SOURCE, SPAN, STRONG, STYLE, SUB, SUMMARY, SUP,
        TABLE, TBODY, TD, TEMPLATE, TEXTAREA, TFOOT, TH, THEAD, TIME, TITLE, TR, TRACK,
        U, UL,
        VAR, VIDEO,
        WBR,

        /* MathML 3.0 (https://developer.mozilla.org/en-US/docs/Web/MathML/Element) */
        MATH,
        MACTION, MALIGNGROUP, MALIGNMARK,
        MENCLOSE, MERROR,
        MFENCED, MFRAC,
        MGLYPH,
        MI,
        MLABELEDTR, MLONGDIV,
        MMULTISCRIPT,
        MN,
        MO, MOVER,
        MPADDED, MPHANTOM,
        MROOT, MROW,
        MS, MSCARRIES, MSCARRY, MSGROUP, MSLINE, MSPACE, MSQRT, MSROW, MSTACK, MSTYLE, MSUB, MSUP, MSUBSUP,
        MTABLE, MTD, MTEXT, MTR,
        MUNDER, MUNDEROVER,
        SEMANTICS, ANNOTATION, ANNOTATION_XML,

        /* SVG (https://developer.mozilla.org/en-US/docs/Web/SVG/Element) */
        SVG,
        /* A, */ ANIMATE, ANIMATEMOTION, ANIMATETRANSFORM,
        CIRCLE, CLIPPATH, COLOR_PROFILE,
        DEFS, DESC, DISCARD,
        ELLIPSE,
        FEBLEND,
            FECOLORMATRIX, FECOMPONENTTRANSFER, FECOMPOSITE, FECONVOLVEMATRIX,
            FEDIFFUSELIGHTING, FEDISPLACEMENTMAP, FEDISTANTLIGHT, FEDROPSHADOW,
            FEFLOOD, FEFUNCA, FEFUNCB, FEFUNCG, FEFUNCR,
            FEGAUSSIANBLUR,
            FEIMAGE,
            FEMERGE, FEMERGENODE, FEMORPHOLOGY,
            FEOFFSET,
            FEPOINTLIGHT,
            FESPECULARLIGHTING, FESPOTLIGHT,
            FETILE, FETURBULENCE,
            FILTER,
            FOREIGNOBJECT,
        G,
        HATCH, HATCHPATH,
        IMAGE,
        LINE, LINEARGRADIENT,
        MARKER, MASK, MESH, MESHGRADIENT, MESHPATCH, MESHROW, METADATA, MPATH,
        PATH, PATTERN, POLYGON, POLYLINE,
        RADIALGRADIENT, RECT,
        /* SCRIPT, */ SET, SOLIDCOLOR, STOP, /* STYLE, */ SWITCH, SYMBOL,
        TEXT, TEXTPATH, /* TITLE, */ TSPAN,
        UNKNOWN, USE,
        VIEW,


        ENUM_END //Not HTML - used for iterating enums
    };
}

#endif //BLOGATOR_DOM_HTML5_TAG_H


