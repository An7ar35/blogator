#ifndef BLOGATOR_PARSER_SPECS_HTML5_ATTRIBUTE_H
#define BLOGATOR_PARSER_SPECS_HTML5_ATTRIBUTE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::html5 {
    enum class Attribute {
        UNKNOWN = 0,

        /* HTML5-proper attributes */
        BASE_ATTR_BEGIN,
        ACCEPT = BASE_ATTR_BEGIN, ACCEPT_CHARSET, ACCESSKEY, ACTION, ALT, ASYNC, AUTOCOMPLETE, AUTOFOCUS, AUTOPLAY,
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
        BASE_ATTR_END = WRAP,

        /* Accessible Rich Internet Applications (ARIA) attributes
         * https://www.w3.org/TR/wai-ARIA_1.1/#states_and_properties
         */
        ARIA_ATTR_BEGIN,
        ARIA_ACTIVEDESCENDANT = ARIA_ATTR_BEGIN, ARIA_ATOMIC, ARIA_AUTOCOMPLETE,
        ARIA_BUSY /* state */,
        ARIA_CHECKED, ARIA_COLCOUNT, ARIA_COLINDEX, ARIA_COLSPAN, ARIA_CONTROLS, ARIA_CURRENT /* state */,
        ARIA_DESCRIBEDBY, ARIA_DETAILS, ARIA_DISABLED /* state */, ARIA_DROPEFFECT,
        ARIA_ERRORMESSAGE,
        ARIA_FLOWTO,
        ARIA_GRABBED /* state */,
        ARIA_HASPOPUP, ARIA_HIDDEN /* state */,
        ARIA_INVALID /* state */,
        ARIA_KEYSHORTCUTS,
        ARIA_LABEL, ARIA_LABELLEDBY, ARIA_LEVEL, ARIA_LIVE,
        ARIA_MODAL, ARIA_MULTILINE, ARIA_MULTISELECTABLE,
        ARIA_ORIENTATION, ARIA_OWNS,
        ARIA_PLACEHOLDER, ARIA_POSINSET, ARIA_PRESSED,
        ARIA_READONLY, ARIA_RELEVANT, ARIA_REQUIRED, ARIA_ROLEDESCRIPTION, ARIA_ROWCOUNT, ARIA_ROWINDEX, ARIA_ROWSPAN,
        ARIA_SELECTED, ARIA_SETSIZE, ARIA_SORT,
        ARIA_VALUEMAX, ARIA_VALUEMIN, ARIA_VALUENOW, ARIA_VALUETEXT,
        ARIA_ATTR_END = ARIA_VALUETEXT,

        ENUM_END = ARIA_ATTR_END //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, Attribute attr );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::html5::Attribute el );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_ATTRIBUTE_H
