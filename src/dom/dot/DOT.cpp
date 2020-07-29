#include "DOT.h"

#include <algorithm>
#include <fstream>

#include "../../string/helpers.h"
#include "../parser/Tokeniser.h"
#include "../parser/Parser.h"

/**
 * Constructor
 * @param config Configuration
 */
blogator::dom::DOT::DOT( DOTConfig config ) :
    _display( cli::MsgInterface::getInstance() ),
    _config( config ),
    _root( std::make_unique<DOTNode>( html5::Tag::NONE ) )
{}

/**
 * Constructor
 * @param html UTF-8 HTML string
 */
blogator::dom::DOT::DOT( DOTConfig config, const std::string &html ) :
    DOT( config )
{
    auto text = dto::Text();
    text._lines.emplace_back( encoding::encodeToUTF32( html ) );
    parse( text );
}

/**
 * Constructor
 * @param html UTF-32 HTML string
 */
blogator::dom::DOT::DOT( DOTConfig config, const std::u32string &html ) :
    DOT( config )
{
    auto text = dto::Text();
    text._lines.emplace_back( html );
    parse( text );
}

/**
 * Constructor
 * @param html HTML based Text DTO
 */
blogator::dom::DOT::DOT( DOTConfig config, const dto::Text &html ) :
    DOT( config )
{
    parse( html );
}

/**
 * Constructor
 * @param src Source file path for the HTML to import
 */
blogator::dom::DOT::DOT( DOTConfig config, const std::filesystem::path &src ) :
    DOT( config )
{
    auto text = DOT::readFile( src );
    parse( text );
}

/**
 * Constructor (for unit-testing)
 * @param root Root DOTNode to inject
 * @throws exception::DOMException when an ID is already in use
 */
blogator::dom::DOT::DOT( std::unique_ptr<DOTNode> root ) :
    _display( cli::MsgInterface::getInstance() ),
    _root( std::move( root ) )
{
    /**
     * [LAMBDA] Helper method to populate the css id/class maps when the tree is injected instead of parsed
     */
    std::function<void(DOTNode *)> populateMaps = [&]( DOTNode *curr ) {
        { //IDs
            auto it = curr->_attributes.find( U"id" );

            if( it != curr->_attributes.cend() ) {
                if( !_css_id_map.try_emplace( it->second.value, curr ).second ) {
                    std::stringstream msg;
                    msg << "ID '" << encoding::encodeToUTF8( it->second.value ) << "' already exists.";

                    throw exception::DOMException(
                        "blogator::dom::DOT::DOT( std::unique_ptr<DOTNode> )",
                        msg.str(),
                        exception::DOMErrorType::InUseAttributeError
                    );
                }
            }
        }

        { //Classes
            auto it = curr->_attributes.find( U"class" );

            if( it != curr->_attributes.cend() ) {
                auto classes = string::split( it->second.value, U' ' );

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
    _display( dot._display ),
    _config( dot._config ),
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
    _config        = dot._config ;
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
 * Processes text into a DOT
 * @param text Text DTO
 */
void blogator::dom::DOT::parse( const dto::Text &text ) {
    auto tokeniser = parser::Tokeniser( text );
    auto parser    = parser::Parser();
    auto curr_node = _root.get();

    parser.setAttributeValidation( _config.html5.attribute_compliance );

    for( const auto &token : tokeniser ) {
        try {
            curr_node = parser.parseToken( token, curr_node, _global_maps );

        } catch( exception::DOMException &e ) {
            std::stringstream ss;

            if( _display.showDebug() )
                ss << "[" << e.where() << "] ";

            ss << "(" << token.line_i << ":" << token.char_i << ") " << e.what();
            _display.error( ss.str() );

            throw;
        }
    }
}

/**
 * Read into a Text DTO a text-based file
 * @param path Path for file
 * @return Text DTO
 * @throws std::filesystem::filesystem_error when target file at given path cannot be opened
 */
blogator::dom::dto::Text blogator::dom::DOT::readFile( const std::filesystem::path &path ) {
    auto text = dto::Text();
    std::string   line;
    std::ifstream file( path );

    if( !file.is_open() )
        throw std::filesystem::filesystem_error( "Could not open: " + path.string(), std::error_code() );

    while( getline( file, line ) ) {
        text._lines.emplace_back( blogator::encoding::encodeToUTF32( line ) );
    }

    file.close();

    return text;
}