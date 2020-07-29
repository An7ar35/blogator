#include "Parser.h"

#include "../html5/validator/validator.h"
#include "../../string/helpers.h"
#include "../html5/kb/KnowledgeBase.h"

/**
 * Constructor
 */
blogator::dom::parser::Parser::Parser() :
    _attributes_validation( ComplianceLevel::NONE ),
    _curr_ns( html5::NameSpace::BLOGATOR )
{}

/**
 * [UNIT-TESTING] Constructor
 * @param list Initializer list of TokenType(s) to be pushed on the stack in order
 */
blogator::dom::parser::Parser::Parser( std::initializer_list<html5::Tag> list ) : //TODO change that to Tag_t
    Parser()
{
    for( auto e : list )
        _stack.push_front( e );
}

/**
 * Parse a token
 * @param token           Token to parse
 * @param parent          Parent DOTNode
 * @param global_attr_map Global lookup map DTO
 * @return Current parent node based on what was inserted
 * @throws exception::DOMException when a parsing error occurs
 */
blogator::dom::DOTNode * blogator::dom::parser::Parser::parseToken(
    const dom::parser::Token  & token,
    dom::DOTNode              * parent,
    dom::dto::GlobalMaps      & global_attr_map )
{
    try {
        switch( token.type ) {
            case TokenType::START_TAG:
                return parseOpeningTag( token.content, parent, global_attr_map );
            case TokenType::END_TAG:
                return parseClosingTag( token.content, parent );
            default: //i.e.: TokenType::TEXT
                return parseText( token, parent );
        }

    } catch( exception::DOMException &e ) {
        throw;
    }
}

/**
 * Sets the attribute validation flag
 * @param level Compliance level
 */
void blogator::dom::parser::Parser::setAttributeValidation( dom::ComplianceLevel level ) {
    _attributes_validation = level;
}

/**
 * Gets the compliance level set for attribute validation
 * @return Attribute validation level
 */
blogator::dom::ComplianceLevel blogator::dom::parser::Parser::attributeValidation() const {
    return _attributes_validation;
}

/**
 * Gets the tree depth (i.e.: tag stack size)
 * @return Tree depth
 */
size_t blogator::dom::parser::Parser::treeDepth() const {
    return _stack.size();
}

/**
 * Parse an opening tag
 * @param str             Opening tag string
 * @param parent          Node where to append new child node at
 * @param global_attr_map Global lookup map DTO
 * @return Pointer to the new node
 * @throws exception::DOMException when trying to give a child to an unpaired node
 * @throws exception::DOMException when validation fails
 */
blogator::dom::DOTNode * blogator::dom::parser::Parser::parseOpeningTag(
    const std::u32string & str,
    DOTNode              * parent,
    dto::GlobalMaps      & global_attr_map )
{
    using blogator::dom::html5::validator::validateOpeningTag;
    using blogator::dom::dto::Attribute;

    try {
        auto attr = std::vector<std::pair<std::u32string, Attribute>>();
        auto node = std::make_unique<DOTNode>( validateOpeningTag( str, attr ) ); //TODO edge-case: SVG

        for( const auto &pair : attr ) {
            if( !validateAttributeName( _attributes_validation, node->type(), pair.first ) ) {
                std::stringstream loc, msg;
                loc << "blogator::dom::parser::Parser::parseOpeningTag( " << encoding::encodeToUTF8( pair.first ) << ", ... )";
                msg << "Failed validation of attribute name \""
                    << encoding::encodeToUTF8( pair.first ) << "\" in \""
                    << encoding::encodeToUTF8( str ) << "\".";

                throw exception::DOMException( loc.str(), msg.str(), exception::DOMErrorType::ValidationError );
            }

            node->addAttribute( pair.first, pair.second );
        }

        populateGlobalAttrMap( node, global_attr_map );

        if( dom::html5::kb::KnowledgeBase::isPaired( node->type() ) ) {
            _stack.push_front( node->type() );
            return parent->addChild( std::move( node ) );

        } else {
            parent->addChild( std::move( node ) );
            return parent;
        }

    } catch( exception::DOMException&e ) {
        throw;
    }
}

/**
 * Parse a closing tag
 * @param str  Closing tag string
 * @param curr Current DOTNode
 * @return Pointer to the appropriate parent node
 * @throws exception::DOMException when validation fails
 * @throws exception::DOMException when closing tag has no opening tag match or is not a recognised tag
 */
blogator::dom::DOTNode * blogator::dom::parser::Parser::parseClosingTag(
    const std::u32string & str,
    DOTNode              * curr )
{
    using blogator::dom::html5::validator::validateClosingTag;

    auto tag = html5::validator::validateClosingTag( str );

    if( _stack.empty() ) { //tree struct ERROR
        std::stringstream loc, msg;
        loc << "blogator::dom::parser::Parser::parseClosingTag( \"" << encoding::encodeToUTF8( str ) << "\", DOTNode * )";
        msg << "Closing tag (" << encoding::encodeToUTF8( html5::kb::KnowledgeBase::tagToStr( tag ) ) << ") does not match an opening tag.";
        throw exception::DOMException( loc.str(), msg.str(), exception::DOMErrorType::ValidationError );
    }

    //TODO allow for the allowed exceptions as outlined in the standards?

    if( _stack.front() != tag ) {
        //missing closing tag somewhere (or missing opening tag?)
        std::stringstream loc, msg;
        loc << "blogator::dom::parser::Parser::parseClosingTag( \"" << encoding::encodeToUTF8( str ) << "\", DOTNode * )";
        msg << "Closing tag ("
            << encoding::encodeToUTF8( html5::kb::KnowledgeBase::tagToStr( tag ) ) << ") "
            << "does not match last opened tag ("
            << encoding::encodeToUTF8( html5::kb::KnowledgeBase::tagToStr( _stack.front() ) ) << ").";
        throw exception::DOMException( loc.str(), msg.str(), exception::DOMErrorType::ValidationError );
    }

    _stack.pop_front();
    return curr->parent();
}

/**
 *
 * @param token
 * @param parent
 * @return
 */
blogator::dom::DOTNode * blogator::dom::parser::Parser::parseText(
    const parser::Token    & token,
    blogator::dom::DOTNode * parent )
{ //TODO maybe concatenate continuous text content into 1 node?
    parent->addChild( std::make_unique<DOTNode>( html5::Tag::NONE, token.content ) );
    return parent;
}

/**
 * Add any 'class' and 'id' attribute values found in a given node to a global map
 * @param node            DOTNode
 * @param global_attr_map Global attribute map for ids and classes
 */
void blogator::dom::parser::Parser::populateGlobalAttrMap(
    std::unique_ptr<DOTNode> & node,
    dto::GlobalMaps          & global_attr_map )
{
    if( node->hasAttribute( U"id") ) {
        auto id_value = node->attribute( U"id" );

        if( !global_attr_map.id_map.try_emplace( id_value, node.get() ).second ) {
            std::stringstream ss;
            ss << "[blogator::dom::DOT::parseIDs( ... )] "
               << "ID '" << encoding::encodeToUTF8( id_value ) << "' already exists.";

            throw exception::DOMException(
                ss.str(),
                exception::DOMErrorType::InUseAttributeError
            );
        }
    }

    if( node->hasAttribute( U"class" ) ) {
        auto class_values = string::split( node->attribute( U"class" ), U' ' );

        for( const auto &val : class_values ) {
            if( !val.empty() ) {
                auto class_it = global_attr_map.class_map.find( val );

                if( class_it == global_attr_map.class_map.end() )
                    class_it = global_attr_map.class_map.emplace( val, std::vector<const DOTNode *>() ).first;

                class_it->second.emplace_back( node.get() );
            }
        }
    }
}

bool blogator::dom::parser::Parser::autoClosePrevTag(
    blogator::dom::html5::Tag prev,
    blogator::dom::html5::Tag next )
{
    switch( prev ) {
        case html5::Tag::NONE:
            break;
        case html5::Tag::A:
            break;
        case html5::Tag::ABBR:
            break;
        case html5::Tag::ADDRESS:
            break;
        case html5::Tag::AREA:
            break;
        case html5::Tag::ARTICLE:
            break;
        case html5::Tag::ASIDE:
            break;
        case html5::Tag::AUDIO:
            break;
        case html5::Tag::B:
            break;
        case html5::Tag::BASE:
            break;
        case html5::Tag::BDI:
            break;
        case html5::Tag::BDO:
            break;
        case html5::Tag::BLOCKQUOTE:
            break;
        case html5::Tag::BODY:
            break;
        case html5::Tag::BR:
            break;
        case html5::Tag::BUTTON:
            break;
        case html5::Tag::CANVAS:
            break;
        case html5::Tag::CAPTION:
            break;
        case html5::Tag::CITE:
            break;
        case html5::Tag::CODE:
            break;
        case html5::Tag::COL:
            break;
        case html5::Tag::COLGROUP:


            break;
        case html5::Tag::COMMENT:
            break;
        case html5::Tag::DATA:
            break;
        case html5::Tag::DATALIST:
            break;
        case html5::Tag::DD:
            return ( next == html5::Tag::DD || next == html5::Tag::DT );

        case html5::Tag::DEL:
            break;
        case html5::Tag::DETAILS:
            break;
        case html5::Tag::DFN:
            break;
        case html5::Tag::DIALOG:
            break;
        case html5::Tag::DIV:
            break;
        case html5::Tag::DL:
            break;
        case html5::Tag::DOCTYPE:
            break;
        case html5::Tag::DT:
            return ( next == html5::Tag::DD || next == html5::Tag::DT );

        case html5::Tag::EM:
            break;
        case html5::Tag::EMBED:
            break;
        case html5::Tag::FIELDSET:
            break;
        case html5::Tag::FIGCAPTION:
            break;
        case html5::Tag::FIGURE:
            break;
        case html5::Tag::FOOTER:
            break;
        case html5::Tag::FORM:
            break;
        case html5::Tag::H1:
            break;
        case html5::Tag::H2:
            break;
        case html5::Tag::H3:
            break;
        case html5::Tag::H4:
            break;
        case html5::Tag::H5:
            break;
        case html5::Tag::H6:
            break;
        case html5::Tag::HEAD:
            break;
        case html5::Tag::HEADER:
            break;
        case html5::Tag::HR:
            break;
        case html5::Tag::HTML:
            break;
        case html5::Tag::I:
            break;
        case html5::Tag::IFRAME:
            break;
        case html5::Tag::IMG:
            break;
        case html5::Tag::INPUT:
            break;
        case html5::Tag::INS:
            break;
        case html5::Tag::KBD:
            break;
        case html5::Tag::LABEL:
            break;
        case html5::Tag::LEGEND:
            break;
        case html5::Tag::LI:
            return ( next == html5::Tag::LI );

        case html5::Tag::LINK:
            break;
        case html5::Tag::MAIN:
            break;
        case html5::Tag::MAP:
            break;
        case html5::Tag::MARK:
            break;
        case html5::Tag::META:
            break;
        case html5::Tag::METER:
            break;
        case html5::Tag::NAV:
            break;
        case html5::Tag::NOSCRIPT:
            break;
        case html5::Tag::OBJECT:
            break;
        case html5::Tag::OL:
            break;
        case html5::Tag::OPTGROUP:
            return ( next == html5::Tag::OPTGROUP );
            //TODO ...or if there is no more content in the parent element.
        case html5::Tag::OPTION:
            return ( next == html5::Tag::OPTION || next == html5::Tag::OPTGROUP );
            //TODO ...or if there is no more content in the parent element.
            break;
        case html5::Tag::OUTPUT:
            break;
        case html5::Tag::P:
            return ( next == html5::Tag::ADDRESS    || next == html5::Tag::ARTICLE      ||
                     next == html5::Tag::ASIDE      || next == html5::Tag::BLOCKQUOTE   ||
                     next == html5::Tag::DETAILS    || next == html5::Tag::DIV          ||
                     next == html5::Tag::DL         || next == html5::Tag::FIELDSET     ||
                     next == html5::Tag::FIGCAPTION || next == html5::Tag::FIGURE       ||
                     next == html5::Tag::FOOTER     || next == html5::Tag::FORM         ||
                     next == html5::Tag::H1         || next == html5::Tag::H2           ||
                     next == html5::Tag::H3         || next == html5::Tag::H4           ||
                     next == html5::Tag::H5         || next == html5::Tag::H6           ||
                     next == html5::Tag::HEADER     || next == html5::Tag::HR           ||
                     next == html5::Tag::MAIN       || next == html5::Tag::NAV          ||
                     next == html5::Tag::OL         || next == html5::Tag::P            ||
                     next == html5::Tag::PRE        || next == html5::Tag::SECTION      ||
                     next == html5::Tag::TABLE      || next == html5::Tag::UL
            );
            /*TODO A p element's end tag may be omitted ..., or if there is no more content in
             * the parent element and the parent element is an HTML element that is not an
             * a, audio, del, ins, map, noscript, or video element, or an autonomous custom element. */
        case html5::Tag::PARAM:
            break;
        case html5::Tag::PICTURE:
            break;
        case html5::Tag::PRE:
            break;
        case html5::Tag::PROGRESS:
            break;
        case html5::Tag::Q:
            break;
        case html5::Tag::RP:
            return ( next == html5::Tag::RT || next == html5::Tag::RP );
            //TODO ...or if there is no more content in the parent element.
        case html5::Tag::RT:
            return ( next == html5::Tag::RT || next == html5::Tag::RP );
            //TODO ...or if there is no more content in the parent element.
        case html5::Tag::RUBY:
            break;
        case html5::Tag::S:
            break;
        case html5::Tag::SAMP:
            break;
        case html5::Tag::SCRIPT:
            break;
        case html5::Tag::SECTION:
            break;
        case html5::Tag::SELECT:
            break;
        case html5::Tag::SMALL:
            break;
        case html5::Tag::SOURCE:
            break;
        case html5::Tag::SPAN:
            break;
        case html5::Tag::STRONG:
            break;
        case html5::Tag::STYLE:
            break;
        case html5::Tag::SUB:
            break;
        case html5::Tag::SUMMARY:
            break;
        case html5::Tag::SUP:
            break;
        case html5::Tag::TABLE:
            break;
        case html5::Tag::TBODY:
            break;
        case html5::Tag::TD:
            break;
        case html5::Tag::TEMPLATE:
            break;
        case html5::Tag::TEXTAREA:
            break;
        case html5::Tag::TFOOT:
            break;
        case html5::Tag::TH:
            break;
        case html5::Tag::THEAD:
            break;
        case html5::Tag::TIME:
            break;
        case html5::Tag::TITLE:
            break;
        case html5::Tag::TR:
            break;
        case html5::Tag::TRACK:
            break;
        case html5::Tag::U:
            break;
        case html5::Tag::UL:
            break;
        case html5::Tag::VAR:
            break;
        case html5::Tag::VIDEO:
            break;
        case html5::Tag::WBR:
            break;
        case html5::Tag::MATH:
            break;
        case html5::Tag::SVG:
            break;
        case html5::Tag::ENUM_END:
            break;
        default:
            return false;
    }

    return false;
}

blogator::dom::html5::Tag
blogator::dom::parser::Parser::autoOpenTag( blogator::dom::html5::Tag prev,
                                            blogator::dom::html5::Tag next ) {
    switch( next ) {

        case html5::Tag::NONE:
            break;
        case html5::Tag::A:
            break;
        case html5::Tag::ABBR:
            break;
        case html5::Tag::ADDRESS:
            break;
        case html5::Tag::AREA:
            break;
        case html5::Tag::ARTICLE:
            break;
        case html5::Tag::ASIDE:
            break;
        case html5::Tag::AUDIO:
            break;
        case html5::Tag::B:
            break;
        case html5::Tag::BASE:
            break;
        case html5::Tag::BDI:
            break;
        case html5::Tag::BDO:
            break;
        case html5::Tag::BLOCKQUOTE:
            break;
        case html5::Tag::BODY:
            break;
        case html5::Tag::BR:
            break;
        case html5::Tag::BUTTON:
            break;
        case html5::Tag::CANVAS:
            break;
        case html5::Tag::CAPTION:
            break;
        case html5::Tag::CITE:
            break;
        case html5::Tag::CODE:
            break;
        case html5::Tag::COL:
            break;
        case html5::Tag::COLGROUP:
            //TODO A colgroup element's start tag may be omitted if the first thing inside the colgroup element is a col element, and if the element is not immediately preceded by another colgroup element whose end tag has been omitted. (It can't be omitted if the element is empty.)
            break;
        case html5::Tag::COMMENT:
            break;
        case html5::Tag::DATA:
            break;
        case html5::Tag::DATALIST:
            break;
        case html5::Tag::DD:
            break;
        case html5::Tag::DEL:
            break;
        case html5::Tag::DETAILS:
            break;
        case html5::Tag::DFN:
            break;
        case html5::Tag::DIALOG:
            break;
        case html5::Tag::DIV:
            break;
        case html5::Tag::DL:
            break;
        case html5::Tag::DOCTYPE:
            break;
        case html5::Tag::DT:
            break;
        case html5::Tag::EM:
            break;
        case html5::Tag::EMBED:
            break;
        case html5::Tag::FIELDSET:
            break;
        case html5::Tag::FIGCAPTION:
            break;
        case html5::Tag::FIGURE:
            break;
        case html5::Tag::FOOTER:
            break;
        case html5::Tag::FORM:
            break;
        case html5::Tag::H1:
            break;
        case html5::Tag::H2:
            break;
        case html5::Tag::H3:
            break;
        case html5::Tag::H4:
            break;
        case html5::Tag::H5:
            break;
        case html5::Tag::H6:
            break;
        case html5::Tag::HEAD:
            break;
        case html5::Tag::HEADER:
            break;
        case html5::Tag::HR:
            break;
        case html5::Tag::HTML:
            break;
        case html5::Tag::I:
            break;
        case html5::Tag::IFRAME:
            break;
        case html5::Tag::IMG:
            break;
        case html5::Tag::INPUT:
            break;
        case html5::Tag::INS:
            break;
        case html5::Tag::KBD:
            break;
        case html5::Tag::LABEL:
            break;
        case html5::Tag::LEGEND:
            break;
        case html5::Tag::LI:
            break;
        case html5::Tag::LINK:
            break;
        case html5::Tag::MAIN:
            break;
        case html5::Tag::MAP:
            break;
        case html5::Tag::MARK:
            break;
        case html5::Tag::META:
            break;
        case html5::Tag::METER:
            break;
        case html5::Tag::NAV:
            break;
        case html5::Tag::NOSCRIPT:
            break;
        case html5::Tag::OBJECT:
            break;
        case html5::Tag::OL:
            break;
        case html5::Tag::OPTGROUP:
            break;
        case html5::Tag::OPTION:
            break;
        case html5::Tag::OUTPUT:
            break;
        case html5::Tag::P:
            break;
        case html5::Tag::PARAM:
            break;
        case html5::Tag::PICTURE:
            break;
        case html5::Tag::PRE:
            break;
        case html5::Tag::PROGRESS:
            break;
        case html5::Tag::Q:
            break;
        case html5::Tag::RP:
            break;
        case html5::Tag::RT:
            break;
        case html5::Tag::RUBY:
            break;
        case html5::Tag::S:
            break;
        case html5::Tag::SAMP:
            break;
        case html5::Tag::SCRIPT:
            break;
        case html5::Tag::SECTION:
            break;
        case html5::Tag::SELECT:
            break;
        case html5::Tag::SMALL:
            break;
        case html5::Tag::SOURCE:
            break;
        case html5::Tag::SPAN:
            break;
        case html5::Tag::STRONG:
            break;
        case html5::Tag::STYLE:
            break;
        case html5::Tag::SUB:
            break;
        case html5::Tag::SUMMARY:
            break;
        case html5::Tag::SUP:
            break;
        case html5::Tag::TABLE:
            break;
        case html5::Tag::TBODY:
            break;
        case html5::Tag::TD:
            break;
        case html5::Tag::TEMPLATE:
            break;
        case html5::Tag::TEXTAREA:
            break;
        case html5::Tag::TFOOT:
            break;
        case html5::Tag::TH:
            break;
        case html5::Tag::THEAD:
            break;
        case html5::Tag::TIME:
            break;
        case html5::Tag::TITLE:
            break;
        case html5::Tag::TR:
            break;
        case html5::Tag::TRACK:
            break;
        case html5::Tag::U:
            break;
        case html5::Tag::UL:
            break;
        case html5::Tag::VAR:
            break;
        case html5::Tag::VIDEO:
            break;
        case html5::Tag::WBR:
            break;
        case html5::Tag::MATH:
            break;
        case html5::Tag::SVG:
            break;
        case html5::Tag::ENUM_END:
            break;
    }
}

/**
 * Validates an attribute's name in a tag based on a compliance level
 * @param level     Validation compliance level
 * @param tag_type  Parent tag type for attribute
 * @param attr_name Attribute name
 * @return Validation state
 */
bool blogator::dom::parser::Parser::validateAttributeName(
    dom::ComplianceLevel   level,
    html5::Tag             tag_type,
    const std::u32string & attr_name )
{
    using blogator::dom::html5::kb::KnowledgeBase;

    switch( level ) {
        case ComplianceLevel::NONE:
            break;

        case ComplianceLevel::PARTIAL: //only checks the relationship of a recognised attribute with tag
            if( !html5::kb::KnowledgeBase::isValidAttribute( attr_name ) )
                break;
            [[fallthrough]];

        case ComplianceLevel::STRICT:
            try {
                return html5::kb::KnowledgeBase::areAffiliated(
                    html5::kb::KnowledgeBase::strToAttribute( attr_name ), tag_type
                );

            } catch( std::invalid_argument &e ) {
                return false;
            }
    }

    return true;
}