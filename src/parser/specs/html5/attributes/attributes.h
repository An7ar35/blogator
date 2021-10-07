#ifndef BLOGATOR_PARSER_SPECS_HTML5_ATTRIBUTES_H
#define BLOGATOR_PARSER_SPECS_HTML5_ATTRIBUTES_H

namespace blogator::parser::specs::html5::attributes {
    /* HTML5-proper attributes */
    enum class HTML5 {
        UNKNOWN_ATTRIBUTE = 0,

        ACCEPT, ACCEPT_CHARSET, ACCESSKEY, ACTION, ALT, ASYNC, AUTOCOMPLETE, AUTOFOCUS, AUTOPLAY,
        CHARSET, CHECKED, CITE, CLASS, COLS, COLSPAN, CONTENT, CONTENTEDITABLE, CONTROLS, COORDS,
        DATA, DATA_X /* data-* */, DATETIME, DEFAULT, DEFER, DIR, DIRNAME, DISABLED, DOWNLOAD, DRAGGABLE,
        ENCTYPE,
        FOR, FORM, FORMACTION,
        HEADERS, HEIGHT, HIDDEN, HIGH, HREF, HREFLANG, HTTP_EQUIV,
        ID, ISMAP,
        KIND,
        LABEL, LANG, LIST, LOOP, LOW,
        MAX, MAXLENGTH, MEDIA, METHOD, MIN, MULTIPLE, MUTED,
        NAME, NOVALIDATE,
        ONABORT, ONAFTERPRINT,
        ONBEFOREPRINT, ONBEFOREUNLOAD, ONBLUR,
        ONCANPLAY, ONCANPLAYTHROUGH, ONCHANGE, ONCLICK, ONCONTEXTMENU, ONCOPY, ONCUECHANGE, ONCUT,
        ONDBLCLICK, ONDRAG, ONDRAGEND, ONDRAGENTER, ONDRAGLEAVE, ONDRAGOVER, ONDRAGSTART, ONDROP, ONDURATIONCHANGE,
        ONEMPTIED, ONENDED, ONERROR,
        ONFOCUS,
        ONHASHCHANGE,
        ONINPUT, ONINVALID,
        ONKEYDOWN, ONKEYPRESS, ONKEYUP,
        ONLOAD, ONLOADEDDATA, ONLOADEDMETADATA, ONLOADSTART,
        ONMOUSEDOWN, ONMOUSEMOVE, ONMOUSEOUT, ONMOUSEOVER, ONMOUSEUP, ONMOUSEWHEEL,
        ONOFFLINE, ONONLINE,
        ONPAGEHIDE, ONPAGESHOW, ONPASTE, ONPAUSE, ONPLAY, ONPLAYING, ONPOPSTATE, ONPROGRESS,
        ONRATECHANGE, ONRESET, ONRESIZE,
        ONSCROLL, ONSEARCH, ONSEEKED, ONSEEKING, ONSELECT, ONSTALLED, ONSTORAGE, ONSUBMIT, ONSUSPEND,
        ONTIMEUPDATE, ONTOGGLE,
        ONUNLOAD,
        ONVOLUMECHANGE,
        ONWAITING, ONWHEEL,
        OPEN, OPTIMUM,
        PATTERN, PLACEHOLDER, POSTER, PRELOAD,
        READONLY, REL, REQUIRED, REVERSED, ROWS, ROWSPAN,
        SANDBOX, SCOPE, SELECTED, SHAPE, SIZE, SIZES, SPAN, SPELLCHECK, SRC, SRCDOC, SRCLANG, SRCSET, START, STEP, STYLE,
        TABINDEX, TARGET, TITLE, TRANSLATE, TYPE,
        USEMAP,
        VALUE,
        WIDTH, WRAP,

        ENUM_END //used for iterating enums
    };

    /* Accessible Rich Internet Applications (ARIA) attributes */
    //https://www.w3.org/TR/wai-ARIA_1.1/#states_and_properties
    enum class Aria {
        UNKNOWN_ATTRIBUTE = 0,
        //TODO complete list
        ROLE,

        ARIA_atomic,
        ARIA_busy, //(state)
        ARIA_controls,
        ARIA_current, //(state)
        ARIA_describedby,
        ARIA_details,
        ARIA_disabled, //(state)
        ARIA_dropeffect,
        ARIA_errormessage,
        ARIA_flowto,
        ARIA_grabbed, //(state)
        ARIA_haspopup,
        ARIA_hidden, //(state)
        ARIA_invalid, //(state)
        ARIA_keyshortcuts,
        ARIA_label,
        ARIA_labelledby,
        ARIA_live,
        ARIA_owns,
        ARIA_relevant,
        ARIA_roledescription,

        //Widget attributes
        ARIA_autocomplete,
        ARIA_checked,
        ARIA_disabled,
        ARIA_errormessage,
        ARIA_expanded,
        ARIA_haspopup,
        ARIA_hidden,
        ARIA_invalid,
        ARIA_label,
        ARIA_level,
        ARIA_modal,
        ARIA_multiline,
        ARIA_multiselectable,
        ARIA_orientation,
        ARIA_placeholder,
        ARIA_pressed,
        ARIA_readonly,
        ARIA_required,
        ARIA_selected,
        ARIA_sort,
        ARIA_valuemax,
        ARIA_valuemin,
        ARIA_valuenow,
        ARIA_valuetext,

        ARIA_atomic,
        ARIA_busy,
        ARIA_live,
        ARIA_relevant,

        ARIA_dropeffect,
        ARIA_grabbed,

        ARIA_activedescendant,
        ARIA_colcount,
        ARIA_colindex,
        ARIA_colspan,
        ARIA_controls,
        ARIA_describedby,
        ARIA_details,
        ARIA_errormessage,
        ARIA_flowto,
        ARIA_labelledby,
        ARIA_owns,
        ARIA_posinset,
        ARIA_rowcount,
        ARIA_rowindex,
        ARIA_rowspan,
        ARIA_setsize,

        ENUM_END //used for iterating enums
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_ATTRIBUTES_H
