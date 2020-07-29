#ifndef BLOGATOR_DOM_HTML5_RULES_CONTEXT_H
#define BLOGATOR_DOM_HTML5_RULES_CONTEXT_H

#include "../enums/Tag.h"
#include "../enums/InsertMode.h"
#include "../../dot/DOTNode.h"

namespace blogator::dom::html5::rules::context {
    /**
     * Template method for checking validity of a node within the context of a tree based on the rules outlined in the HTML5.3 standard
     * @param node DOTNode
     * @return Validation state
     */
    template<Tag> static bool isValidContext( const DOTNode &node );
}

/* ======================================= IMPLEMENTATION ======================================= */
/* Yeah, i know... but as the HTML element all have their quirks it's "simpler" to just have 1 method
 * for each individual tag so that any changes to the validation standard can be addressed in a testable
 * way. Since each tag has a singular isolated method it saves from more headaches in the long run with
 * any arising edge cases or modifications to the standard.
 */

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::NONE>( const DOTNode &node ) {
    return node.parent()
        || node.childrenCount() == 0; //Root or leaf node
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::A>( const DOTNode &node ) {
    //Categories: Flow content. Phrasing content. If the element has an href attribute: Interactive content. Palpable content.
    //Contexts in which this element can be used: Where phrasing content is expected.
    //Transparent, but there must be no interactive content or a element descendants.


    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::ABBR>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::ADDRESS>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::AREA>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::ARTICLE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::ASIDE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::AUDIO>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::B>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::BASE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::BDI>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::BDO>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::BLOCKQUOTE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::BODY>( const DOTNode &node ) {
    
    if( node.parent() && node.parent()->childrenCount() > 0 ) {
        const auto &children = node.parent()->children();

        if( children.back()->type() == Tag::HEAD ) {
            return true;
        }

        if( children.back()->type() == Tag::COMMENT ) {
            auto rit = children.crbegin();
            while( (*rit++)->type() == Tag::COMMENT );
            return ( (*rit)->type() == Tag::HEAD );
        }
    }
    //A head element followed by a body element.
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::BR>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::BUTTON>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::CANVAS>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::CAPTION>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::CITE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::CODE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::COL>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::COLGROUP>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::COMMENT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DATA>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DATALIST>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DD>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DEL>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DETAILS>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DFN>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DIALOG>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DIV>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DL>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DOCTYPE>( const DOTNode &node ) {
    if( !node.parent()->parent() ) //parent needs to be root
        return false;

    if( node.indexInParent() > 0 ) { //any previous siblings must be comments only
        for( size_t i = 0; i <  node.indexInParent(); ++i ) {
            if( node.parent()->children().at( i )->type() != Tag::COMMENT )
                return false;
        }
    }

    return true;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::DT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::EM>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::EMBED>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::FIELDSET>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::FIGCAPTION>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::FIGURE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::FOOTER>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::FORM>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::H1>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::H2>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::H3>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::H4>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::H5>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::H6>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::HEAD>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::HEADER>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::HR>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::HTML>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::I>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::IFRAME>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::IMG>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::INPUT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::INS>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::KBD>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::LABEL>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::LEGEND>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::LI>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::LINK>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::MAIN>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::MAP>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::MARK>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::META>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::METER>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::NAV>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::NOSCRIPT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::OBJECT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::OL>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::OPTGROUP>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::OPTION>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::OUTPUT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::P>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::PARAM>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::PICTURE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::PRE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::PROGRESS>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::Q>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::RP>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::RT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::RUBY>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::S>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SAMP>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SCRIPT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SECTION>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SELECT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SMALL>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SOURCE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SPAN>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::STRONG>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::STYLE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SUB>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SUMMARY>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SUP>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TABLE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TBODY>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TD>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TEMPLATE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TEXTAREA>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TFOOT>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TH>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::THEAD>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TIME>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TITLE>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TR>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::TRACK>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::U>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::UL>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::VAR>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::VIDEO>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::WBR>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::MATH>( const DOTNode &node ) {
    return false;
}

template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::SVG>( const DOTNode &node ) {
    return false;
}


#endif //BLOGATOR_DOM_HTML5_RULES_CONTEXT_H
