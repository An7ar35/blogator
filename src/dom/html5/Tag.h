#ifndef BLOGATOR_DOM_HTML5_TAG_H
#define BLOGATOR_DOM_HTML5_TAG_H

namespace blogator::dom::html5 {
    /**
     * HTML5 tag types
     * ref: https://www.w3schools.com/TAGs/
     */
    enum class Tag {
        NONE = 0, //Not HTML - used internally for the root node and any leaf nodes with untagged data (raw)

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
        S, SAMP, SCRIPT, SECTION, SELECT, SMALL, SOURCE, SPAN, STRONG, STYLE, SUB, SUMMARY, SUP, SVG,
        TABLE, TBODY, TD, TEMPLATE, TEXTAREA, TFOOT, TH, THEAD, TIME, TITLE, TR, TRACK,
        U, UL,
        VAR, VIDEO,
        WBR,
    };

    /**
     * HTML5 tag structure
     */
    enum class TagStructure {
        PAIRED,   //open and closing tag pair
        UNPAIRED, //single self-closing tag
    };


}

#endif //BLOGATOR_DOM_HTML5_TAG_H
