#include "Attribute.h"

#include <sstream>
#include <map>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr Attribute enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, blogator::parser::specs::infra::Attribute attr ) {
    static std::map<Attribute, std::string> map;

    if( map.empty() ) {
        map.emplace( Attribute::UNKNOWN, "UNKNOWN" );
        map.emplace( Attribute::ACCEPT, "accept" );
        map.emplace( Attribute::ACCEPT_CHARSET, "accept-charset" );
        map.emplace( Attribute::ACCESSKEY, "accesskey" );
        map.emplace( Attribute::ACTION, "action" );
        map.emplace( Attribute::ALT, "alt" );
        map.emplace( Attribute::ASYNC, "async" );
        map.emplace( Attribute::AUTOCOMPLETE, "autocomplete" );
        map.emplace( Attribute::AUTOFOCUS, "autofocus" );
        map.emplace( Attribute::AUTOPLAY, "autoplay" );
        map.emplace( Attribute::CHARSET, "charset" );
        map.emplace( Attribute::CHECKED, "checked" );
        map.emplace( Attribute::CITE, "cite" );
        map.emplace( Attribute::CLASS, "class" );
        map.emplace( Attribute::COLS, "cols" );
        map.emplace( Attribute::COLSPAN, "colspan" );
        map.emplace( Attribute::CONTENT, "content" );
        map.emplace( Attribute::CONTENTEDITABLE, "contenteditable" );
        map.emplace( Attribute::CONTROLS, "controls" );
        map.emplace( Attribute::COORDS, "coords" );
        map.emplace( Attribute::DATA, "data" );
        map.emplace( Attribute::DATA_X, "data-*" );
        map.emplace( Attribute::DATETIME, "datetime" );
        map.emplace( Attribute::DEFAULT, "default" );
        map.emplace( Attribute::DEFER, "defer" );
        map.emplace( Attribute::DIR, "dir" );
        map.emplace( Attribute::DIRNAME, "dirname" );
        map.emplace( Attribute::DISABLED, "disabled" );
        map.emplace( Attribute::DOWNLOAD, "download" );
        map.emplace( Attribute::DRAGGABLE, "draggable" );
        map.emplace( Attribute::ENCTYPE, "enctype" );
        map.emplace( Attribute::FOR, "for" );
        map.emplace( Attribute::FORM, "form" );
        map.emplace( Attribute::FORMACTION, "formaction" );
        map.emplace( Attribute::HEADERS, "headers" );
        map.emplace( Attribute::HEIGHT, "height" );
        map.emplace( Attribute::HIDDEN, "hidden" );
        map.emplace( Attribute::HIGH, "high" );
        map.emplace( Attribute::HREF, "href" );
        map.emplace( Attribute::HREFLANG, "hreflang" );
        map.emplace( Attribute::HTTP_EQUIV, "http-equiv" );
        map.emplace( Attribute::ID, "id" );
        map.emplace( Attribute::ISMAP, "ismap" );
        map.emplace( Attribute::KIND, "kind" );
        map.emplace( Attribute::LABEL, "label" );
        map.emplace( Attribute::LANG, "lang" );
        map.emplace( Attribute::LIST, "list" );
        map.emplace( Attribute::LOOP, "loop" );
        map.emplace( Attribute::LOW, "low" );
        map.emplace( Attribute::MAX, "max" );
        map.emplace( Attribute::MAXLENGTH, "maxlength" );
        map.emplace( Attribute::MEDIA, "media" );
        map.emplace( Attribute::METHOD, "method" );
        map.emplace( Attribute::MIN, "min" );
        map.emplace( Attribute::MULTIPLE, "multiple" );
        map.emplace( Attribute::MUTED, "muted" );
        map.emplace( Attribute::NAME, "name" );
        map.emplace( Attribute::NOVALIDATE, "novalidate" );
        map.emplace( Attribute::ONABORT, "onabort" );
        map.emplace( Attribute::ONAFTERPRINT, "onafterprint" );
        map.emplace( Attribute::ONBEFOREPRINT, "onbeforeprint" );
        map.emplace( Attribute::ONBEFOREUNLOAD, "onbeforeunload" );
        map.emplace( Attribute::ONBLUR, "onblur" );
        map.emplace( Attribute::ONCANPLAY, "oncanplay" );
        map.emplace( Attribute::ONCANPLAYTHROUGH, "oncanplaythrough" );
        map.emplace( Attribute::ONCHANGE, "onchange" );
        map.emplace( Attribute::ONCLICK, "onclick" );
        map.emplace( Attribute::ONCONTEXTMENU, "oncontextmenu" );
        map.emplace( Attribute::ONCOPY, "oncopy" );
        map.emplace( Attribute::ONCUECHANGE, "oncuechange" );
        map.emplace( Attribute::ONCUT, "oncut" );
        map.emplace( Attribute::ONDBLCLICK, "ondblclick" );
        map.emplace( Attribute::ONDRAG, "ondrag" );
        map.emplace( Attribute::ONDRAGEND, "ondragend" );
        map.emplace( Attribute::ONDRAGENTER, "ondragenter" );
        map.emplace( Attribute::ONDRAGLEAVE, "ondragleave" );
        map.emplace( Attribute::ONDRAGOVER, "ondragover" );
        map.emplace( Attribute::ONDRAGSTART, "ondragstart" );
        map.emplace( Attribute::ONDROP, "ondrop" );
        map.emplace( Attribute::ONDURATIONCHANGE, "ondurationchange" );
        map.emplace( Attribute::ONEMPTIED, "onemptied" );
        map.emplace( Attribute::ONENDED, "onended" );
        map.emplace( Attribute::ONERROR, "onerror" );
        map.emplace( Attribute::ONFOCUS, "onfocus" );
        map.emplace( Attribute::ONHASHCHANGE, "onhashchange" );
        map.emplace( Attribute::ONINPUT, "oninput" );
        map.emplace( Attribute::ONINVALID, "oninvalid" );
        map.emplace( Attribute::ONKEYDOWN, "onkeydown" );
        map.emplace( Attribute::ONKEYPRESS, "onkeypress" );
        map.emplace( Attribute::ONKEYUP, "onkeyup" );
        map.emplace( Attribute::ONLOAD, "onload" );
        map.emplace( Attribute::ONLOADEDDATA, "onloadeddata" );
        map.emplace( Attribute::ONLOADEDMETADATA, "onloadedmetadata" );
        map.emplace( Attribute::ONLOADSTART, "onloadstart" );
        map.emplace( Attribute::ONMOUSEDOWN, "onmousedown" );
        map.emplace( Attribute::ONMOUSEMOVE, "onmousemove" );
        map.emplace( Attribute::ONMOUSEOUT, "onmouseout" );
        map.emplace( Attribute::ONMOUSEOVER, "onmouseover" );
        map.emplace( Attribute::ONMOUSEUP, "onmouseup" );
        map.emplace( Attribute::ONMOUSEWHEEL, "onmousewheel" );
        map.emplace( Attribute::ONOFFLINE, "onoffline" );
        map.emplace( Attribute::ONONLINE, "ononline" );
        map.emplace( Attribute::ONPAGEHIDE, "onpagehide" );
        map.emplace( Attribute::ONPAGESHOW, "onpageshow" );
        map.emplace( Attribute::ONPASTE, "onpaste" );
        map.emplace( Attribute::ONPAUSE, "onpause" );
        map.emplace( Attribute::ONPLAY, "onplay" );
        map.emplace( Attribute::ONPLAYING, "onplaying" );
        map.emplace( Attribute::ONPOPSTATE, "onpopstate" );
        map.emplace( Attribute::ONPROGRESS, "onprogress" );
        map.emplace( Attribute::ONRATECHANGE, "onratechange" );
        map.emplace( Attribute::ONRESET, "onreset" );
        map.emplace( Attribute::ONRESIZE, "onresize" );
        map.emplace( Attribute::ONSCROLL, "onscroll" );
        map.emplace( Attribute::ONSEARCH, "onsearch" );
        map.emplace( Attribute::ONSEEKED, "onseeked" );
        map.emplace( Attribute::ONSEEKING, "onseeking" );
        map.emplace( Attribute::ONSELECT, "onselect" );
        map.emplace( Attribute::ONSTALLED, "onstalled" );
        map.emplace( Attribute::ONSTORAGE, "onstorage" );
        map.emplace( Attribute::ONSUBMIT, "onsubmit" );
        map.emplace( Attribute::ONSUSPEND, "onsuspend" );
        map.emplace( Attribute::ONTIMEUPDATE, "ontimeupdate" );
        map.emplace( Attribute::ONTOGGLE, "ontoggle" );
        map.emplace( Attribute::ONUNLOAD, "onunload" );
        map.emplace( Attribute::ONVOLUMECHANGE, "onvolumechange" );
        map.emplace( Attribute::ONWAITING, "onwaiting" );
        map.emplace( Attribute::ONWHEEL, "onwheel" );
        map.emplace( Attribute::OPEN, "open" );
        map.emplace( Attribute::OPTIMUM, "optimum" );
        map.emplace( Attribute::PATTERN, "pattern" );
        map.emplace( Attribute::PLACEHOLDER, "placeholder" );
        map.emplace( Attribute::POSTER, "poster" );
        map.emplace( Attribute::PRELOAD, "preload" );
        map.emplace( Attribute::READONLY, "readonly" );
        map.emplace( Attribute::REL, "rel" );
        map.emplace( Attribute::REQUIRED, "required" );
        map.emplace( Attribute::REVERSED, "reversed" );
        map.emplace( Attribute::ROWS, "rows" );
        map.emplace( Attribute::ROWSPAN, "rowspan" );
        map.emplace( Attribute::SANDBOX, "sandbox" );
        map.emplace( Attribute::SCOPE, "scope" );
        map.emplace( Attribute::SELECTED, "selected" );
        map.emplace( Attribute::SHAPE, "shape" );
        map.emplace( Attribute::SIZE, "size" );
        map.emplace( Attribute::SIZES, "sizes" );
        map.emplace( Attribute::SPAN, "span" );
        map.emplace( Attribute::SPELLCHECK, "spellcheck" );
        map.emplace( Attribute::SRC, "src" );
        map.emplace( Attribute::SRCDOC, "srcdoc" );
        map.emplace( Attribute::SRCLANG, "srclang" );
        map.emplace( Attribute::SRCSET, "srcset" );
        map.emplace( Attribute::START, "start" );
        map.emplace( Attribute::STEP, "step" );
        map.emplace( Attribute::STYLE, "style" );
        map.emplace( Attribute::TABINDEX, "tabindex" );
        map.emplace( Attribute::TARGET, "target" );
        map.emplace( Attribute::TITLE, "title" );
        map.emplace( Attribute::TRANSLATE, "translate" );
        map.emplace( Attribute::TYPE, "type" );
        map.emplace( Attribute::USEMAP, "usemap" );
        map.emplace( Attribute::VALUE, "value" );
        map.emplace( Attribute::WIDTH, "width" );
        map.emplace( Attribute::WRAP, "wrap" );
        map.emplace( Attribute::ARIA_ACTIVEDESCENDANT, "activedescendant" );
        map.emplace( Attribute::ARIA_ATOMIC, "atomic" );
        map.emplace( Attribute::ARIA_AUTOCOMPLETE, "autocomplete" );
        map.emplace( Attribute::ARIA_BUSY, "busy" );
        map.emplace( Attribute::ARIA_CHECKED, "checked" );
        map.emplace( Attribute::ARIA_COLCOUNT, "colcount" );
        map.emplace( Attribute::ARIA_COLINDEX, "colindex" );
        map.emplace( Attribute::ARIA_COLSPAN, "colspan" );
        map.emplace( Attribute::ARIA_CONTROLS, "controls" );
        map.emplace( Attribute::ARIA_CURRENT, "current" );
        map.emplace( Attribute::ARIA_DESCRIBEDBY, "describedby" );
        map.emplace( Attribute::ARIA_DETAILS, "details" );
        map.emplace( Attribute::ARIA_DISABLED, "disabled" );
        map.emplace( Attribute::ARIA_DROPEFFECT, "dropeffect" );
        map.emplace( Attribute::ARIA_ERRORMESSAGE, "errormessage" );
        map.emplace( Attribute::ARIA_FLOWTO, "flowto" );
        map.emplace( Attribute::ARIA_GRABBED, "grabbed" );
        map.emplace( Attribute::ARIA_HASPOPUP, "haspopup" );
        map.emplace( Attribute::ARIA_HIDDEN, "hidden" );
        map.emplace( Attribute::ARIA_INVALID, "invalid" );
        map.emplace( Attribute::ARIA_KEYSHORTCUTS, "keyshortcuts" );
        map.emplace( Attribute::ARIA_LABEL, "label" );
        map.emplace( Attribute::ARIA_LABELLEDBY, "labelledby" );
        map.emplace( Attribute::ARIA_LEVEL, "level" );
        map.emplace( Attribute::ARIA_LIVE, "live" );
        map.emplace( Attribute::ARIA_MODAL, "modal" );
        map.emplace( Attribute::ARIA_MULTILINE, "multiline" );
        map.emplace( Attribute::ARIA_MULTISELECTABLE, "multiselectable" );
        map.emplace( Attribute::ARIA_ORIENTATION, "orientation" );
        map.emplace( Attribute::ARIA_OWNS, "owns" );
        map.emplace( Attribute::ARIA_PLACEHOLDER, "placeholder" );
        map.emplace( Attribute::ARIA_POSINSET, "posinset" );
        map.emplace( Attribute::ARIA_PRESSED, "pressed" );
        map.emplace( Attribute::ARIA_READONLY, "readonly" );
        map.emplace( Attribute::ARIA_RELEVANT, "relevant" );
        map.emplace( Attribute::ARIA_REQUIRED, "required" );
        map.emplace( Attribute::ARIA_ROLEDESCRIPTION, "roledescription" );
        map.emplace( Attribute::ARIA_ROWCOUNT, "rowcount" );
        map.emplace( Attribute::ARIA_ROWINDEX, "rowindex" );
        map.emplace( Attribute::ARIA_ROWSPAN, "rowspan" );
        map.emplace( Attribute::ARIA_SELECTED, "selected" );
        map.emplace( Attribute::ARIA_SETSIZE, "setsize" );
        map.emplace( Attribute::ARIA_SORT, "sort" );
        map.emplace( Attribute::ARIA_VALUEMAX, "valuemax" );
        map.emplace( Attribute::ARIA_VALUEMIN, "valuemin" );
        map.emplace( Attribute::ARIA_VALUENOW, "valuenow" );
        map.emplace( Attribute::ARIA_VALUETEXT, "valuetext" );
    }

    if( map.contains( attr ) ) {
        os << map.at( attr );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::infra::Attribute enum to a string representation
 * @param attr Attribute enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::infra::Attribute attr ) {
    std::stringstream ss;
    ss << attr;
    return ss.str();
}
