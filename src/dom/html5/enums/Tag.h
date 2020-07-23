#ifndef BLOGATOR_DOM_HTML5_TAG_H
#define BLOGATOR_DOM_HTML5_TAG_H

namespace blogator::dom::html5 {
    /**
     * Allowed tag types
     */
    enum class Tag {
        NONE = 0, //Not HTML - used internally for the root node and any leaf nodes with untagged data (raw)

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

        /* SVG */
        SVG,

        ENUM_END //Not HTML - used for iterating enums
    };
}

#endif //BLOGATOR_DOM_HTML5_TAG_H
