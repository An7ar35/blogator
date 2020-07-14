#include "DOT.h"

#include <algorithm>

#include "../../string/helpers.h"

blogator::dom::DOT::DOT( const std::string &html ) :
    _root( std::make_unique<DOTNode>( html5::Tag::NONE ) )
{
//    parse( encoding::encodeToUTF32( html ) );
}

/**
 * Constructor
 */
blogator::dom::DOT::DOT( const std::u32string &html ) :
    _root( std::make_unique<DOTNode>( html5::Tag::NONE ) )
{
//    parse( html );
}

blogator::dom::DOT::DOT( const blogator::dto::Text &html ) :
    _root( std::make_unique<DOTNode>( html5::Tag::NONE ) )
{
    //TODO
}

/**
 * Constructor (for unit-testing)
 * @param root Root DOTNode to inject
 * @throws exception::DOMException when an ID is already in use
 */
blogator::dom::DOT::DOT( std::unique_ptr<DOTNode> root ) :
    _root( std::move( root ) )
{
    /**
     * [LAMBDA] Helper method to populate the css id/class maps when the tree is injected instead of parsed
     */
    std::function<void(DOTNode *)> populateMaps = [&]( DOTNode *curr ) {
        { //IDs
            auto it = curr->_attributes.find( U"id" );

            if( it != curr->_attributes.cend() ) {
                if( !_css_id_map.try_emplace( it->second, curr ).second ) {
                    std::stringstream ss;
                    ss << "[blogator::dom::DOT::parseIDs( ... )] "
                       << "ID '" << encoding::encodeToUTF8( it->second ) << "' already exists.";

                    throw exception::DOMException(
                        ss.str(),
                        exception::DOMErrorType::InUseAttributeError
                    );
                }
            }
        }

        { //Classes
            auto it = curr->_attributes.find( U"class" );

            if( it != curr->_attributes.cend() ) {
                auto classes = string::split( it->second, U' ' );

                for( const auto &c : classes ) {
                    if( !c.empty() ) {
                        auto class_it = _css_class_map.find( c );

                        if( class_it == _css_class_map.end() )
                            class_it = _css_class_map.emplace( c, std::vector<const DOTNode *>() ).first;

                        class_it->second.emplace_back( curr );
                    }
                }
            }
        }

        for( const auto &child : curr->_children ) //recurse through the tree structure
            populateMaps( child.get() );
    };

    populateMaps( _root.get() );
}

/**
 * Move-Constructor
 * @param dot DOT to move over
 */
blogator::dom::DOT::DOT( blogator::dom::DOT &&dot ) noexcept :
    _root( std::move( dot._root ) ),
    _css_id_map( std::move( dot._css_id_map ) ),
    _css_class_map( std::move( dot._css_class_map ) )
{}

/**
 * Move-assignment operator
 * @param dot DOT to move over
 * @return Moved DOT
 */
blogator::dom::DOT &blogator::dom::DOT::operator =( blogator::dom::DOT &&dot ) noexcept {
    _root          = std::move( dot._root );
    _css_id_map    = std::move( dot._css_id_map );
    _css_class_map = std::move( dot._css_class_map );
    return *this;
}

/**
 * Equality operator
 * @param rhs DOT to compare with
 * @return Equality state of the trees (structure, content, attributes but not id/class maps)
 */
bool blogator::dom::DOT::operator ==( const blogator::dom::DOT &rhs ) const {
    return *_root == *rhs._root;
}

/**
 * Not-Equal operator
 * @param rhs DOT to compare with
 * @return !Equal state of the trees (structure, content, attributes but not id/class maps)
 */
bool blogator::dom::DOT::operator !=( const blogator::dom::DOT &rhs ) const {
    return !( _root == rhs._root );
}

/**
 * Gets a DOTNode by its ID
 * @param id ID name
 * @return DOTNode
 * @throws exception::DOMException when no DOTNode exists with specified ID
 */
const blogator::dom::DOTNode &blogator::dom::DOT::getElementById( const std::u32string &id ) const {
    auto it = _css_id_map.find( id );

    if( it != _css_id_map.cend() )
        return *it->second;

    std::stringstream ss;
    ss << "[blogator::dom::DOT::getElementById( " << encoding::encodeToUTF8( id ) << " )] "
       << "ID does not exist in DOT.";
    throw exception::DOMException( ss.str(), exception::DOMErrorType::HierarchyRequestError );
}

/**
 * Gets all DOTNode belonging to a class
 * @param class_name Class name
 * @return List of DOTNodes
 * @throws exception::DOMException when no DOTNode exists with specified class name
 */
std::vector<const blogator::dom::DOTNode *> blogator::dom::DOT::getElementByClass( const std::u32string &class_name ) const {
    auto v  = std::vector<const dom::DOTNode *>();
    auto it = _css_class_map.find( class_name );

    if( it != _css_class_map.end() ) {
        std::copy( it->second.cbegin(), it->second.cend(), std::back_inserter( v ) );
        return v;
    }

    std::stringstream ss;
    ss << "[blogator::dom::DOT::getElementByClass( " << encoding::encodeToUTF8( class_name ) << " )] "
       << "Class does not exist in DOT.";
    throw exception::DOMException( ss.str(), exception::DOMErrorType::HierarchyRequestError );
}

/**
 * Checks character is a boundary defining character
 * @param c Character to check
 * @return Boundary defining state
 */
bool blogator::dom::DOT::isBoundaryChar( char32_t c ) {
    return ( iswspace( c ) || c == U'\"' || c == '\'' || c == U'>' || c == U'<' || c == '/' );
}

std::u32string blogator::dom::DOT::consumeToken( const std::u32string &src, std::u32string::const_iterator &it ) {
    u32stringstream_t ss;

    while( it != src.cend() && iswspace( *it ) )
        ++it;

    while( it != src.cend() && !DOT::isBoundaryChar( *it ) ) {
        ss << *it;
        ++it;
    }

    return ss.str();
}


/**
 * Adds an attribute to a node
 * @param node       DOTNode
 * @param name       Attribute name
 * @param definition Attribute value(s)
 */
void blogator::dom::DOT::addAttribute( std::unique_ptr<DOTNode> &node,
                                       std::u32string name,
                                       std::u32string definition )
{
    using blogator::string::split;

    std::transform( name.begin(), name.end(), name.begin(), []( char32_t c ){ return std::towlower( c ); } );

    if( name == U"class" ) {
        auto defs = string::split<char32_t>( definition, U' ' );

        for( const auto & def : defs ) {
            if( !def.empty() ) {
                auto it = _css_class_map.find( def );

                if( it != _css_class_map.end() )
                    it->second.emplace_back( node.get() );
            }
        }

        node->addAttribute( name , std::move( definition ) );

        auto it = _css_class_map.find( name );

        if( it == _css_class_map.end() )
            it = _css_class_map.emplace( name, std::vector<const dom::DOTNode *>() ).first;

        it->second.emplace_back( node.get() );

    } else if( name == U"id" ) {
        auto defs     = string::split<char32_t>( definition, U' ' );
        auto filtered = std::vector<std::u32string>();

        std::copy_if( defs.begin(), defs.end(), std::back_inserter( filtered ), []( auto str ){ return !str.empty(); } );

        if( filtered.empty() ) { //ERROR
            u32stringstream_t ss;
            ss << "Missing value in 'id' attribute.";
            throw exception::DOMException( encoding::encodeToUTF8( ss.str() ), exception::DOMErrorType::SyntaxError );
        }

        if( filtered.size() > 1 ) { //ERROR
            u32stringstream_t ss;
            ss << "Multiple values given in 'id' attribute: ";
            for( const auto & id : filtered )
                ss << "'" << id << "' ";
            throw exception::DOMException( encoding::encodeToUTF8( ss.str() ), exception::DOMErrorType::SyntaxError );
        }

        if( _css_id_map.find( filtered.at( 0 ) ) != _css_id_map.end() ) { //ERROR
            u32stringstream_t ss;
            ss << "'id' value '" << filtered.at( 0 ) << "' already in use.";
            throw exception::DOMException( encoding::encodeToUTF8( ss.str() ), exception::DOMErrorType::InUseAttributeError );
        }

        node->addAttribute( name, filtered.at( 0 ) );
        _css_id_map.emplace( name, node.get() );

    } else {
        node->addAttribute( name, std::move( definition ) );
    }

}