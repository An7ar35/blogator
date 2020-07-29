#include "KnowledgeBase.h"

#include <algorithm>
#include <sstream>

#include "kb_attributes.h"
#include "kb_relationships.h"
#include "kb_tags.h"
#include "../../../encoding/encoding.h"

const std::unordered_map<blogator::dom::html5::Tag, blogator::dom::dto::TagSpecs>
    blogator::dom::html5::kb::KnowledgeBase::_tag2str_map = blogator::dom::html5::kb::tags::createTag2StrMap();

const std::unordered_map<std::u32string, blogator::dom::html5::Tag>
    blogator::dom::html5::kb::KnowledgeBase::_str2tag_map = blogator::dom::html5::kb::tags::createStr2TagMap( _tag2str_map );

const std::vector<std::u32string>
    blogator::dom::html5::kb::KnowledgeBase::_attr2str_table = blogator::dom::html5::kb::attributes::createAttributeStrTable();

const std::unordered_map<std::u32string, blogator::dom::html5::Attribute>
    blogator::dom::html5::kb::KnowledgeBase::_str2attr_map = blogator::dom::html5::kb::attributes::createStrAttributeMap( _attr2str_table );

const std::vector<std::vector<bool>>
    blogator::dom::html5::kb::KnowledgeBase::_attr2tag_table = blogator::dom::html5::kb::relationships::createAttr2TagRelationshipTable();


/**
 * Converts a tag enum type to its string representation
 * @param tag Tag type
 * @return Tag type as a string
 * @throws std::invalid_argument when tag is not found in lookup map
 */
std::u32string blogator::dom::html5::kb::KnowledgeBase::tagToStr( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.tag_str;
    } else {
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::tagToStr( dom::html5::Tag )] "
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
blogator::dom::html5::Tag blogator::dom::html5::kb::KnowledgeBase::strToTag( std::u32string str ) {
    try {
        std::transform( str.begin(), str.end(), str.begin(), []( char32_t c ){ return std::towlower( c ); } );
        return _str2tag_map.at( str );

    } catch( std::out_of_range &e ) {
        std::string tag = encoding::encodeToUTF8( str );
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::strToTag( " << tag << " )] "
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
std::u32string blogator::dom::html5::kb::KnowledgeBase::attributeToStr( html5::Attribute attribute ) {
    const auto i = static_cast<size_t>( attribute );

    if( i >= _attr2str_table.size() ) {
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::attributeToStr( dom::html5::Attribute )] "
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
blogator::dom::html5::Attribute blogator::dom::html5::kb::KnowledgeBase::strToAttribute( std::u32string str ) {
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
        ss << "[blogator::dom::html5::kb::KnowledgeBase::strToAttribute( \"" << attr << "\" )] "
           << "String \"" << attr << "\" does not match a known attribute.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Checks if a given string is a valid HTML5 attribute
 * @param str UTF-32 string
 * @return Validation state
 */
bool blogator::dom::html5::kb::KnowledgeBase::isValidAttribute( std::u32string str ) {
    try {
        strToAttribute( std::move( str ) );
        return true;

    } catch( std::invalid_argument &e ) {
        return false;
    }
}

/**
 * Checks if a Tag requires closing
 * @param tag Tag type
 * @return Closing required
 * @throws std::invalid_argument when Tag is unknown
 */
bool blogator::dom::html5::kb::KnowledgeBase::isPaired( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.structure == TagStructure::PAIRED;
    } else {
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::isPaired( dom::html5::Tag )] "
           << "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Gets all individual namespaces
 * @return All individual namespaces
 */
std::vector<blogator::dom::html5::NameSpace> blogator::dom::html5::kb::KnowledgeBase::getNamespaces() {
    auto v = std::vector<NameSpace>();

    for( int e = 0; e < static_cast<int>( NameSpace::ENUM_END ); ++e )
        v.emplace_back( static_cast<NameSpace>( e ) );

    return v;
}

/**
 * Gets the namespace(s) in which a given tag belongs to
 * @param tag Tag type
 * @return List of belonging namespace(s)
 * @throws std::invalid_argument when Tag is unknown
 */
std::vector<blogator::dom::html5::NameSpace> blogator::dom::html5::kb::KnowledgeBase::getNamespace( html5::Tag tag ) {
    const auto it = _tag2str_map.find( tag );

    if( it != _tag2str_map.cend() ) {
        return it->second.ns;

    } else {
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::getNamespace( html5::Tag )] "
           << "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Checks if a given tag belongs to a namespace
 * @param tag Tag type
 * @param ns  NameSpace type
 * @return Membership state
 * @throws std::invalid_argument when Tag is unknown
 */
bool blogator::dom::html5::kb::KnowledgeBase::belongsToNamespace( html5::Tag tag, html5::NameSpace ns ) {
    try {
        if( ns == NameSpace::AGNOSTIC )
            return isNamespaceAgnostic( tag );

        auto v = getNamespace( tag );

        for( const auto &el : v ) {
            if( el == ns || el == NameSpace::AGNOSTIC )
                return true;
        }

        return false;

    } catch( std::invalid_argument &e ) {
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::belongsToNamespace( html5::Tag, html5::NameSpace )] "
           << "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Checks if a given tag is namespace-agnostic
 * @param tag Tag type
 * @return Agnosticism state (i.e. can exist in any namespaces )
 * @throws std::invalid_argument when Tag is unknown
 */
bool blogator::dom::html5::kb::KnowledgeBase::isNamespaceAgnostic( html5::Tag tag ) {
    try {
        auto v = getNamespace( tag );

        //check if contains at least all enums prior to AGNOSTIC (assumes there are no duplicates in the kb_tags)
        if( v.size() >= static_cast<size_t>( html5::NameSpace::AGNOSTIC ) )
            return true; //i.e.: all namespaces are declared for that tag's TagSpecs.ns

        for( const auto &ns : v ) {
            if( ns == NameSpace::AGNOSTIC )
                return true;
        }

        return false;

    } catch( std::invalid_argument &e ) {
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::isNameSpaceAgnostic( html5::Tag )] "
           << "Tag (" << static_cast<int>( tag ) << ") unknown.";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Gets the number of unique tags listed in the internal maps
 * @return Number of unique tags
 */
size_t blogator::dom::html5::kb::KnowledgeBase::tagCount() {
    return kb::KnowledgeBase::_tag2str_map.size();
}

/**
 * Gets the number of unique attributes listed in the internal maps
 * @return Number of unique attributes
 */
size_t blogator::dom::html5::kb::KnowledgeBase::attributeCount() {
    return kb::KnowledgeBase::_attr2str_table.size();
}

/**
 * Checks whether or not an Attribute is usable with a particular Tag
 * @param attr Attribute
 * @param tag  Tag
 * @return Affiliation state
 * @throws std::out_of_range when enum(s) cast to an out of range index on the lookup map
 */
bool blogator::dom::html5::kb::KnowledgeBase::areAffiliated( html5::Attribute attr, html5::Tag tag ) {
    const auto attr_i = static_cast<size_t>( attr );
    const auto tag_i  = static_cast<size_t>( tag );

    if( attr_i >= _attr2tag_table.size() || tag_i >= _attr2tag_table[attr_i].size()) {
        std::stringstream ss;
        ss << "[blogator::dom::html5::kb::KnowledgeBase::areAffiliated( Attribute=" << attr_i << ", Tag=" << tag_i << " )] "
           << "argument(s) are out of bounds of the lookup table size.";
        throw std::out_of_range( ss.str() );
    }

    return _attr2tag_table[attr_i][tag_i];
}