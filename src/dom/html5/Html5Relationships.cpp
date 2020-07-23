#include "Html5Relationships.h"

#include <sstream>

#include "properties/relationships.h"
#include "rules/context.h"

//const std::vector<std::vector<bool>>
//    blogator::dom::html5::Html5Relationships::_tag_context_table = blogator::dom::html5::properties::relationships::createTagContextRelationshipTable();

const std::vector<std::vector<bool>>
    blogator::dom::html5::Html5Relationships::_attr2tag_table = blogator::dom::html5::properties::relationships::createAttr2TagRelationshipTable();

const std::vector<std::vector<bool>>
    blogator::dom::html5::Html5Relationships::_tag2cm_membership_table = blogator::dom::html5::properties::relationships::createTag2ContentModelRelationshipTable();

/**
 * Checks whether or not an Attribute is usable with a particular Tag
 * @param attr Attribute
 * @param tag  Tag
 * @return Affiliation state
 * @throws std::out_of_range when enum(s) cast to an out of range index on the lookup map
 */
bool blogator::dom::html5::Html5Relationships::areAffiliated( html5::Attribute attr, html5::Tag tag ) {
    const auto attr_i = static_cast<size_t>( attr );
    const auto tag_i  = static_cast<size_t>( tag );

    if( attr_i >= _attr2tag_table.size() || tag_i >= _attr2tag_table[attr_i].size()) {
        std::stringstream ss;
        ss << "[blogator::dom::html5::rules::Rules::areAffiliated( Attribute=" << attr_i << ", Tag=" << tag_i << " )] "
           << "argument(s) are out of bounds of the lookup table size.";
        throw std::out_of_range( ss.str() );
    }

    return _attr2tag_table[attr_i][tag_i];
}

/**
 * Checks whether or not a Tag belongs to a particular Content Model
 * @param tag   Tag
 * @param model Content Model
 * @return Affiliation state
 * @throws std::out_of_range when enum(s) cast to an out of range index on the lookup map
 */
bool blogator::dom::html5::Html5Relationships::areAffiliated( html5::Tag tag, html5::ContentModel model ) {
    const auto tag_i = static_cast<size_t>( tag );
    const auto cm_i  = static_cast<size_t>( model );

    if( tag_i >= _tag2cm_membership_table.size() || cm_i >= _tag2cm_membership_table[tag_i].size()) {
        std::stringstream ss;
        ss << "[blogator::dom::html5::rules::Rules::areAffiliated( Tag=" << tag_i << ", ContentModel=" << cm_i << " )] "
           << "argument(s) are out of bounds of the lookup table size.";
        throw std::out_of_range( ss.str() );
    }

    return _tag2cm_membership_table[tag_i][cm_i];
}

/**
 *
 * @param node
 * @return
 * @throws
 */
bool blogator::dom::html5::Html5Relationships::validateNode( const blogator::dom::DOTNode &node ) {
    using blogator::dom::html5::rules::context::isValidContext;
    switch( node.type() ) {
        case Tag::NONE:       return isValidContext<Tag::NONE>( node );
        case Tag::A:          return isValidContext<Tag::A>( node );
        case Tag::ABBR:       return isValidContext<Tag::ABBR>( node );
        case Tag::ADDRESS:    return isValidContext<Tag::ADDRESS>( node );
        case Tag::AREA:       return isValidContext<Tag::AREA>( node );
        case Tag::ARTICLE:    return isValidContext<Tag::ARTICLE>( node );
        case Tag::ASIDE:      return isValidContext<Tag::ASIDE>( node );
        case Tag::AUDIO:      return isValidContext<Tag::AUDIO>( node );
        case Tag::B:          return isValidContext<Tag::B>( node );
        case Tag::BASE:       return isValidContext<Tag::BASE>( node );
        case Tag::BDI:        return isValidContext<Tag::BDI>( node );
        case Tag::BDO:        return isValidContext<Tag::BDO>( node );
        case Tag::BLOCKQUOTE: return isValidContext<Tag::BLOCKQUOTE>( node );
        case Tag::BODY:       return isValidContext<Tag::BODY>( node );
        case Tag::BR:         return isValidContext<Tag::BR>( node );
        case Tag::BUTTON:     return isValidContext<Tag::BUTTON>( node );
        case Tag::CANVAS:     return isValidContext<Tag::CANVAS>( node );
        case Tag::CAPTION:    return isValidContext<Tag::CAPTION>( node );
        case Tag::CITE:       return isValidContext<Tag::CITE>( node );
        case Tag::CODE:       return isValidContext<Tag::CODE>( node );
        case Tag::COL:        return isValidContext<Tag::COL>( node );
        case Tag::COLGROUP:   return isValidContext<Tag::COLGROUP>( node );
        case Tag::COMMENT:    return isValidContext<Tag::COMMENT>( node );
        case Tag::DATA:       return isValidContext<Tag::DATA>( node );
        case Tag::DATALIST:   return isValidContext<Tag::DATALIST>( node );
        case Tag::DD:         return isValidContext<Tag::DD>( node );
        case Tag::DEL:        return isValidContext<Tag::DEL>( node );
        case Tag::DETAILS:    return isValidContext<Tag::DETAILS>( node );
        case Tag::DFN:        return isValidContext<Tag::DFN>( node );
        case Tag::DIALOG:     return isValidContext<Tag::DIALOG>( node );
        case Tag::DIV:        return isValidContext<Tag::DIV>( node );
        case Tag::DL:         return isValidContext<Tag::DL>( node );
        case Tag::DOCTYPE:    return isValidContext<Tag::DOCTYPE>( node );
        case Tag::DT:         return isValidContext<Tag::DT>( node );
        case Tag::EM:         return isValidContext<Tag::EM>( node );
        case Tag::EMBED:      return isValidContext<Tag::EMBED>( node );
        case Tag::FIELDSET:   return isValidContext<Tag::FIELDSET>( node );
        case Tag::FIGCAPTION: return isValidContext<Tag::FIGCAPTION>( node );
        case Tag::FIGURE:     return isValidContext<Tag::FIGURE>( node );
        case Tag::FOOTER:     return isValidContext<Tag::FOOTER>( node );
        case Tag::FORM:       return isValidContext<Tag::FORM>( node );
        case Tag::H1:         return isValidContext<Tag::H1>( node );
        case Tag::H2:         return isValidContext<Tag::H2>( node );
        case Tag::H3:         return isValidContext<Tag::H3>( node );
        case Tag::H4:         return isValidContext<Tag::H4>( node );
        case Tag::H5:         return isValidContext<Tag::H5>( node );
        case Tag::H6:         return isValidContext<Tag::H6>( node );
        case Tag::HEAD:       return isValidContext<Tag::HEAD>( node );
        case Tag::HEADER:     return isValidContext<Tag::HEADER>( node );
        case Tag::HR:         return isValidContext<Tag::HR>( node );
        case Tag::HTML:       return isValidContext<Tag::HTML>( node );
        case Tag::I:          return isValidContext<Tag::I>( node );
        case Tag::IFRAME:     return isValidContext<Tag::IFRAME>( node );
        case Tag::IMG:        return isValidContext<Tag::IMG>( node );
        case Tag::INPUT:      return isValidContext<Tag::INPUT>( node );
        case Tag::INS:        return isValidContext<Tag::INS>( node );
        case Tag::KBD:        return isValidContext<Tag::KBD>( node );
        case Tag::LABEL:      return isValidContext<Tag::LABEL>( node );
        case Tag::LEGEND:     return isValidContext<Tag::LEGEND>( node );
        case Tag::LI:         return isValidContext<Tag::LI>( node );
        case Tag::LINK:       return isValidContext<Tag::LINK>( node );
        case Tag::MAIN:       return isValidContext<Tag::MAIN>( node );
        case Tag::MAP:        return isValidContext<Tag::MAP>( node );
        case Tag::MARK:       return isValidContext<Tag::MARK>( node );
        case Tag::META:       return isValidContext<Tag::META>( node );
        case Tag::METER:      return isValidContext<Tag::METER>( node );
        case Tag::NAV:        return isValidContext<Tag::NAV>( node );
        case Tag::NOSCRIPT:   return isValidContext<Tag::NOSCRIPT>( node );
        case Tag::OBJECT:     return isValidContext<Tag::OBJECT>( node );
        case Tag::OL:         return isValidContext<Tag::OL>( node );
        case Tag::OPTGROUP:   return isValidContext<Tag::OPTGROUP>( node );
        case Tag::OPTION:     return isValidContext<Tag::OPTION>( node );
        case Tag::OUTPUT:     return isValidContext<Tag::OUTPUT>( node );
        case Tag::P:          return isValidContext<Tag::P>( node );
        case Tag::PARAM:      return isValidContext<Tag::PARAM>( node );
        case Tag::PICTURE:    return isValidContext<Tag::PICTURE>( node );
        case Tag::PRE:        return isValidContext<Tag::PRE>( node );
        case Tag::PROGRESS:   return isValidContext<Tag::PROGRESS>( node );
        case Tag::Q:          return isValidContext<Tag::Q>( node );
        case Tag::RP:         return isValidContext<Tag::RP>( node );
        case Tag::RT:         return isValidContext<Tag::RT>( node );
        case Tag::RUBY:       return isValidContext<Tag::RUBY>( node );
        case Tag::S:          return isValidContext<Tag::S>( node );
        case Tag::SAMP:       return isValidContext<Tag::SAMP>( node );
        case Tag::SCRIPT:     return isValidContext<Tag::SCRIPT>( node );
        case Tag::SECTION:    return isValidContext<Tag::SECTION>( node );
        case Tag::SELECT:     return isValidContext<Tag::SELECT>( node );
        case Tag::SMALL:      return isValidContext<Tag::SMALL>( node );
        case Tag::SOURCE:     return isValidContext<Tag::SOURCE>( node );
        case Tag::SPAN:       return isValidContext<Tag::SPAN>( node );
        case Tag::STRONG:     return isValidContext<Tag::STRONG>( node );
        case Tag::STYLE:      return isValidContext<Tag::STYLE>( node );
        case Tag::SUB:        return isValidContext<Tag::SUB>( node );
        case Tag::SUMMARY:    return isValidContext<Tag::SUMMARY>( node );
        case Tag::SUP:        return isValidContext<Tag::SUP>( node );
        case Tag::TABLE:      return isValidContext<Tag::TABLE>( node );
        case Tag::TBODY:      return isValidContext<Tag::TBODY>( node );
        case Tag::TD:         return isValidContext<Tag::TD>( node );
        case Tag::TEMPLATE:   return isValidContext<Tag::TEMPLATE>( node );
        case Tag::TEXTAREA:   return isValidContext<Tag::TEXTAREA>( node );
        case Tag::TFOOT:      return isValidContext<Tag::TFOOT>( node );
        case Tag::TH:         return isValidContext<Tag::TH>( node );
        case Tag::THEAD:      return isValidContext<Tag::THEAD>( node );
        case Tag::TIME:       return isValidContext<Tag::TIME>( node );
        case Tag::TITLE:      return isValidContext<Tag::TITLE>( node );
        case Tag::TR:         return isValidContext<Tag::TR>( node );
        case Tag::TRACK:      return isValidContext<Tag::TRACK>( node );
        case Tag::U:          return isValidContext<Tag::U>( node );
        case Tag::UL:         return isValidContext<Tag::UL>( node );
        case Tag::VAR:        return isValidContext<Tag::VAR>( node );
        case Tag::VIDEO:      return isValidContext<Tag::VIDEO>( node );
        case Tag::WBR:        return isValidContext<Tag::WBR>( node );
        case Tag::MATH:       return isValidContext<Tag::MATH>( node );
        case Tag::SVG:        return isValidContext<Tag::SVG>( node );
        default:
            throw std::invalid_argument(""); //TODO
    }
}

bool blogator::dom::html5::Html5Relationships::validateNode( const blogator::dom::DOTNode &node,
                                                             blogator::dom::html5::InsertMode &insert_mode ) {
    return false;
}

/**
 * Checks the descendants of a node match a particular Tag type
 * @param root Root node
 * @param tag  Tag type
 * @return Has descendants matching a tag
 */
bool blogator::dom::html5::Html5Relationships::hasDescendant( const DOTNode &root, Tag tag ) {
    for( const auto &child : root.children() ) {
        if( hasDescendant_( *child, tag ) )
            return true;
    }

    return false;
}

/**
 * Checks if a node or its descendants match a particular Tag type
 * @param root Root node
 * @param tag  Tag type
 * @return Is/has descendants matching a tag
 */
bool blogator::dom::html5::Html5Relationships::hasDescendant_( const DOTNode &root, Tag tag ) {
    if( root.type() == tag )
        return true;

    for( const auto &child : root.children() ) {
        if( hasDescendant_( *child, tag ) )
            return true;
    }

    return false;
}

/**
 * Checks the descendants of a node match a particular Content Model type
 * @param root  Root node
 * @param model Content Model type
 * @return Has descendants matching a Content Model
 */
bool blogator::dom::html5::Html5Relationships::hasDescendant( const DOTNode &root, ContentModel model ) {
    for( const auto &child : root.children() ) {
        if( hasDescendant_( *child, model ) )
            return true;
    }

    return false;
}

/**
 * Checks if a node or its descendants match a particular Content Model type
 * @param root  Root node
 * @param model Content Model type
 * @return Is/has descendants matching a Content Model
 */
bool blogator::dom::html5::Html5Relationships::hasDescendant_( const DOTNode &root, ContentModel model ) {
    if( Html5Relationships::areAffiliated( root.type(), model ) )
        return true;

    for( const auto &child : root.children() ) {
        if( hasDescendant_( *child, model ) )
            return true;
    }

    return false;
}

/**
 * Checks the descendants of a node match any of a list of Content Model types
 * @param root   Root node
 * @param models Content Model types
 * @return Has descendants matching any of the Content Models
 */
bool blogator::dom::html5::Html5Relationships::hasDescendant( const DOTNode &root, const std::vector<ContentModel> &models ) {
    for( const auto &child : root.children() ) {
        if( hasDescendant_( *child, models ) )
            return true;
    }

    return false;
}

/**
 * Checks if a node or its descendants match any of a list of Content Model types
 * @param root   Root node
 * @param models Content Model types
 * @return Is/has descendants matching any of the Content Models
 */
bool blogator::dom::html5::Html5Relationships::hasDescendant_( const DOTNode &root, const std::vector<ContentModel> &models ) {
    for( const auto &model : models ) {
        if( Html5Relationships::areAffiliated( root.type(), model ) )
            return true;
    }

    for( const auto &child : root.children() ) {
        if( hasDescendant_( *child, models ) )
            return true;
    }

    return false;
}


///**
// * Gets the validity of a Tag context
// * @param child_tag  Child tag to check against a parent
// * @param parent_tag Parent Tag
// * @return State of 'child Tag is allowed to be a child of parent Tag'
// */
//bool blogator::dom::html5::Html5Relationships::isContextValid( html5::Tag child_tag, html5::Tag parent_tag ) {
//    const auto child_i  = static_cast<size_t>( child_tag );
//    const auto parent_i = static_cast<size_t>( parent_tag );
//
//    if( child_i >= Html5Relationships::_tag_context_table.size() || parent_i >= Html5Relationships::_tag_context_table[child_i].size()) {
//        std::stringstream ss;
//        ss << "[blogator::dom::html5::rules::Rules::areAffiliated( Tag=" << child_i << ", Tag=" << parent_i << " )] "
//           << "argument(s) are out of bounds of the lookup table size.";
//        throw std::out_of_range( ss.str() );
//    }
//
//    return _tag_context_table[child_i][parent_i];
//}