#include "Html5Properties.h"

#include <algorithm>

#include "properties/attribute.h"
#include "properties/content_model.h"
#include "properties/tag.h"
#include "../../encoding/encoding.h"

const std::unordered_map<blogator::dom::html5::Tag, blogator::dom::dto::TagSpecs>
    blogator::dom::html5::Html5Properties::_tag2str_map = blogator::dom::html5::properties::tag::createTag2StrMap();

const std::unordered_map<std::u32string, blogator::dom::html5::Tag>
    blogator::dom::html5::Html5Properties::_str2tag_map = blogator::dom::html5::properties::tag::createStr2TagMap( _tag2str_map );

const std::vector<std::u32string>
    blogator::dom::html5::Html5Properties::_attr2str_table = blogator::dom::html5::properties::attribute::createAttributeStrTable();

const std::unordered_map<std::u32string, blogator::dom::html5::Attribute>
    blogator::dom::html5::Html5Properties::_str2attr_map = blogator::dom::html5::properties::attribute::createStrAttributeMap( _attr2str_table );

const std::vector<std::u32string>
    blogator::dom::html5::Html5Properties::_cm2str_table = blogator::dom::html5::properties::content_model::createContentModelStrTable();


/**
 * Converts a tag enum type to its string representation
 * @param tag Tag type
 * @return Tag type as a string
 * @throws std::invalid_argument when tag is not found in lookup map
 */
std::u32string blogator::dom::html5::Html5Properties::tagToStr( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.tag_str;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::rules::Rules::tagToStr( dom::html5::Tag )] "
           << "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Convert a tag string into its enum type
 * @param str HTML5 tag string (case-insensitive)
 * @return Tag type
 * @throws std::invalid_argument when string is not a valid tag
 */
blogator::dom::html5::Tag blogator::dom::html5::Html5Properties::strToTag( std::u32string str ) {
    try {
        std::transform( str.begin(), str.end(), str.begin(), []( char32_t c ){ return std::towlower( c ); } );
        return _str2tag_map.at( str );

    } catch( std::out_of_range &e ) {
        std::string tag = encoding::encodeToUTF8( str );
        std::stringstream ss;
        ss << "[dom::html5::strToTag( " << tag << " )] "
           << "String \"" << tag << "\" does not match a known tag.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Converts an attribute enum type to its string representation
 * @param attribute Attribute type
 * @return Attribute name type as a string
 * @throws std::invalid_argument when attribute name is not found in lookup table
 */
std::u32string blogator::dom::html5::Html5Properties::attributeToStr( html5::Attribute attribute ) {
    const auto i = static_cast<size_t>( attribute );

    if( i >= _attr2str_table.size() ) {
        std::stringstream ss;
        ss << "[dom::html5::rules::Rules::attributeToStr( dom::html5::Attribute )] "
           << "Attribute (" << static_cast<int>( attribute ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }

    return _attr2str_table.at( i );
}

/**
 * Convert a attribute name string into its enum type
 * @param str HTML5 attribute name string (case-insensitive)
 * @return Attribute type
 * @throws std::invalid_argument when string is not a valid attribute
 */
blogator::dom::html5::Attribute blogator::dom::html5::Html5Properties::strToAttribute( std::u32string str ) {
    try {
        std::transform( str.begin(), str.end(), str.begin(), []( char32_t c ){ return std::towlower( c ); } );

        if( str.length() > 5 ) {
            const auto sub_str = str.substr( 0, 5 );

            if( str.substr( 0, 5 ) == U"data-" ) //edge case for custom 'data-*' attributes
                return _str2attr_map.at( sub_str );

            if( str.substr( 0, 5 ) == U"aria-" ) //edge case for ARIA 'aria-*' attributes
                return _str2attr_map.at( sub_str );
        }

        if( str.size() > 6 && str.substr( 0, 6 ) == U"xmlns:" ) //edge case for XML namespace 'xmlns:*' attributes
            return _str2attr_map.at( U"xmlns" );

        return _str2attr_map.at( str );

    } catch( std::out_of_range &e ) {
        std::string attr = encoding::encodeToUTF8( str );
        std::stringstream ss;
        ss << "[dom::html5::rules::Rules::strToAttribute( " << attr << " )] "
           << "String \"" << attr << "\" does not match a known attribute.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Checks if a Tag requires closing
 * @param tag Tag type
 * @return Closing required
 * @throws std::invalid_argument when Tag is unknown
 */
bool blogator::dom::html5::Html5Properties::isPaired( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.structure == TagStructure::PAIRED;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::rules::Rules::isPaired( dom::html5::Tag )] "
           << "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Gets the namespace in which a given tag belongs to
 * @param tag Tag type
 * @return Belonging namespace
 * @throws std::invalid_argument when Tag is unknown
 */
blogator::dom::html5::NameSpace blogator::dom::html5::Html5Properties::getNamespace( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.ns;
    } else {
        std::stringstream ss;
        ss << "[dom::html5::rules::Rules::getNamespace( dom::html5::Tag )] "
           << "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Gets the number of unique tags listed in the internal maps
 * @return Number of unique tags
 */
size_t blogator::dom::html5::Html5Properties::tagCount() {
    return Html5Properties::_tag2str_map.size();
}

/**
 * Gets the number of unique attributes listed in the internal maps
 * @return Number of unique attributes
 */
size_t blogator::dom::html5::Html5Properties::attributeCount() {
    return Html5Properties::_attr2str_table.size();
}

/**
 * Gets the number of unique content models listed in the internal maps
 * @return Number of unique conent models
 */
size_t blogator::dom::html5::Html5Properties::contentModelCount() {
    return Html5Properties::_cm2str_table.size();
}