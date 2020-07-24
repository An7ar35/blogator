#include "DOTNode.h"

#include "../html5/special_char.h"
#include "../../string/helpers.h"

/**
 * Constructor
 */
blogator::dom::DOTNode::DOTNode() :
    DOTNode( 0, html5::Tag::NONE )
{}

/**
 * Constructor
 * @param tag  Node's html tag type
 * @param text (optional) Text content
 */
blogator::dom::DOTNode::DOTNode( blogator::dom::html5::Tag tag, std::u32string text ) :
    DOTNode( 0, tag, std::move( text ) )
{}

/**
 * Constructor
 * @param i_pos Index position in the list of children in the parent node
 * @param tag   Node's html tag type
 * @param text  (optional) Text content
 */
blogator::dom::DOTNode::DOTNode( size_t i_pos, html5::Tag tag, std::u32string text ) :
    _tag( tag ),
    _text( std::move( text ) ),
    _nth_pos( i_pos ),
    _parent( nullptr ),
    _children( std::vector<std::unique_ptr<DOTNode>>() )
{}

/**
 * Move-Constructor
 * @param node DOTNode to move over
 */
blogator::dom::DOTNode::DOTNode( DOTNode &&node ) noexcept :
    _tag( node._tag ),
    _text( std::move( node._text ) ),
    _nth_pos( node._nth_pos ),
    _parent( node._parent ),
    _children( std::move( node._children ) )
{}

/**
 * Equality operator
 * @param rhs DOTNode to compare with
 * @return Equality state of the nodes, their tree structure and content (except the * address to parents)
 */
bool blogator::dom::DOTNode::operator ==( const blogator::dom::DOTNode &rhs ) const {
    return _tag == rhs._tag
           && _text == rhs._text
           && _attributes.size() == rhs._attributes.size()
           && std::equal( _attributes.cbegin(), _attributes.cend(), rhs._attributes.cbegin(),
               []( const auto &a, const auto &b ) { return a.second == b.second; })
           && _children.size() == rhs._children.size()
           && std::equal( _children.cbegin(), _children.cend(), rhs._children.cbegin(),
               []( const auto &a, const auto &b ) { return *a == *b; } );
}

/**
 * Not-Equal operator
 * @param rhs DOTNode to compare with
 * @return !Equal state of the nodes, their tree structure and content
 */
bool blogator::dom::DOTNode::operator !=( const blogator::dom::DOTNode &rhs ) const {
    return !( *this == rhs );
}

/**
 * Explicitly copies the node and its children tree structure/content
 * NOTE: copies the root node's parent address so that both subtrees can link back to their parent
 *       tree - It's better to use this copy method from the root to avoid invalid parent pointers.
 * @return Pointer to the node as the root of the copied tree structure
 */
std::unique_ptr<blogator::dom::DOTNode> blogator::dom::DOTNode::recursiveCopy() const {
    auto p = std::make_unique<DOTNode>( _nth_pos, _tag );

    p->_text   = _text;
    p->_parent = _parent;
    p->_attributes.insert( _attributes.cbegin(), _attributes.cend() );

    for( const auto & child : _children )
        p->_children.emplace_back( child->recursiveCopy( p.get() ) );

    return p ;
}

/**
 * Helper method for recursive copy
 * @param parent Parent pointer
 * @return Pointer to the node as the root of the copied tree structure
 */
std::unique_ptr<blogator::dom::DOTNode> blogator::dom::DOTNode::recursiveCopy( blogator::dom::DOTNode *parent ) const {
    auto p = std::make_unique<DOTNode>( _nth_pos, _tag );

    p->_text   = _text;
    p->_parent = parent;
    p->_attributes.insert( _attributes.cbegin(), _attributes.cend() );

    for( const auto & child : _children )
        p->_children.emplace_back( child->recursiveCopy( p.get() ) );

    return p ;
}

/**
 * Sets the text content of the node
 * @param text Text content
 */
void blogator::dom::DOTNode::setTextContent( std::u32string text ) {
    _text = std::move( text );
}

/**
 * Add child node
 * @param node Node to add as a child
 * @return Pointer to the new child
 * @throws HtmlParseLib::DOMException when attempt is made of giving a child to an unpaired node
 */
blogator::dom::DOTNode * blogator::dom::DOTNode::addChild( std::unique_ptr<DOTNode> node ) {
    if( !html5::Html5Properties::isPaired( _tag ) && parent() ) //rule: Only paired structs can have nested children
        throw exception::DOMException(
            "[blogator::dom::DOTNode::addChild( std::unique_ptr<DOTNode> )] "
            "Cannot add children to an unpaired node type.",
            blogator::exception::DOMErrorType::HierarchyRequestError
        );

    const auto i = _children.size();
    _children.emplace_back( std::move( node ) );
    _children.back()->_parent = this;
    _children.back()->_nth_pos = i;
    return _children.back().get();
}

/**
 * Gets the node's raw data if any
 * @return Raw data
 */
const std::u32string & blogator::dom::DOTNode::content() const {
    return _text;
}

/**
 * Gets the index position of the node in its parent
 * @return Index position
 */
size_t blogator::dom::DOTNode::indexInParent() const {
    return _nth_pos;
}

/**
 * Get's the node's parent
 * @return Pointer to parent of node or nullptr if orphaned
 */
blogator::dom::DOTNode *blogator::dom::DOTNode::parent() {
    return _parent;
}

/**
 * Get's the node's parent
 * @return Pointer to parent of node or nullptr if orphaned
 */
const blogator::dom::DOTNode * blogator::dom::DOTNode::parent() const {
    return _parent;
}

/**
 * Gets the list of the node's children
 * @return Collection of children in order of insertion
 */
const std::vector<std::unique_ptr<blogator::dom::DOTNode>> &blogator::dom::DOTNode::children() const {
    return _children;
}

/**
 * Gets an attribute's value content
 * @param key Attribute key
 * @return Value string of the given attribute
 * @throws exception::DOMException when attribute key does not exist for the node
 */
std::u32string blogator::dom::DOTNode::attribute( const std::u32string &key ) const {
    try {
        auto attr = _attributes.at( key );
        return attr.value;

    } catch ( std::out_of_range &e ) {
        std::stringstream ss;
        ss << "[blogator::dom::DOTNode::attribute( \""
           << encoding::encodeToUTF8( key )  << "\" )] "
           << "Attribute key not found.";

        throw exception::DOMException( ss.str(), exception::DOMErrorType::NotFoundError );
    }
}

/**
 * Check if node has any attributes
 * @return Attribute existence
 */
bool blogator::dom::DOTNode::hasAttribute() const {
    return !_attributes.empty();
}

/**
 * Checks if node has a particular attribute
 * @param key Attribute key
 * @return Attribute existence
 */
bool blogator::dom::DOTNode::hasAttribute( const std::u32string &key ) const {
    return ( _attributes.find( key ) != _attributes.cend() );
}

/**
 * Adds a new attribute
 * @param key       Attribute key
 * @param attribute Attribute DTO (value, boundary char)
 * @return true if success, false if key already exists
 */
bool blogator::dom::DOTNode::addAttribute( const std::u32string &key, dom::dto::Attribute attribute ) {
    auto it = _attributes.find( key );

    if( it != _attributes.end() )
        return false;

    _attributes.emplace( key, std::move( attribute ) );
    return true;
}

/**
 * Appends a value to an existing attribute that can support this operation (i.e.: with a boundary character specified)
 * @param key   Attribute key
 * @param value Attribute value to append
 * @return Success of operation (i.e. key was found and its value appended to)
 * @throws exception::DOMException when value has a nested boundary character that is already in used as a boundary character
 */
bool blogator::dom::DOTNode::appendAttribute( const std::u32string &key, std::u32string value ) {
    auto it = _attributes.find( key );

    if( value.empty() || it == _attributes.end() )
        return false;

    try {
        auto nested_bc = dom::dto::Attribute::checkNestedBoundaryChar( value ); //can throw

        if( it->second.boundary == html5::AttrBoundaryChar::NONE ) {
            switch( nested_bc ) {
                case html5::AttrBoundaryChar::NONE:
                case html5::AttrBoundaryChar::APOSTROPHE:
                    it->second.boundary = html5::AttrBoundaryChar::QUOTATION_MARK;
                    break;
                case html5::AttrBoundaryChar::QUOTATION_MARK:
                    it->second.boundary = html5::AttrBoundaryChar::APOSTROPHE;
                    break;
            }

            it->second.value += ( it->second.value.empty() ? std::move( value ) : U" " + std::move( value ) );
            return true;
        }

        if( it->second.boundary == html5::AttrBoundaryChar::APOSTROPHE ) {
            if( nested_bc == html5::AttrBoundaryChar::APOSTROPHE ) { //cannot nest (') into (')
                std::stringstream msg;
                msg << "Nested boundary char (') already in use by the attribute \"" << encoding::encodeToUTF8( key ) << "\".";
                throw std::invalid_argument( msg.str() );
            }

            it->second.value += ( it->second.value.empty() ? std::move( value ) : U" " + std::move( value ) );
            return true;
        }

        if( it->second.boundary == html5::AttrBoundaryChar::QUOTATION_MARK ) {
            if( nested_bc == html5::AttrBoundaryChar::QUOTATION_MARK ) { //cannot nest (") into (")
                std::stringstream msg;
                msg << "Nested boundary char (\") already in use by the attribute \"" << encoding::encodeToUTF8( key ) << "\".";
                throw std::invalid_argument( msg.str() );
            }

            it->second.value += ( it->second.value.empty() ? std::move( value ) : U" " + std::move( value ) );
            return true;
        }

    } catch( std::invalid_argument &e ) {
        std::stringstream loc;
        loc << "blogator::dom::DOTNode::appendAttribute( \"" << encoding::encodeToUTF8( key ) << "\", \""
                                                             << encoding::encodeToUTF8( value ) << "\" )";
        throw exception::DOMException(
            loc.str(),
            e.what(),
            exception::DOMErrorType::ValidationError
        );
    }
}

/**
 * Replaces the value of a given attribute
 * @param key       Attribute key
 * @param attribute Attribute DTO
 */
void blogator::dom::DOTNode::replaceAttribute( const std::u32string &key, dom::dto::Attribute attribute ) {
    auto it = _attributes.find( key );

    if( it != _attributes.end() ) {
        it->second = attribute;
        return;
    }

    std::stringstream loc;
    loc << "blogator::dom::DOTNode::replaceAttribute( \""
        << encoding::encodeToUTF8( key ) << "\" , \""
        << encoding::encodeToUTF8( attribute.value ) << "\" )";

    throw exception::DOMException( loc.str(), "Attribute key not found.", exception::DOMErrorType::NotFoundError );
}

/**
 * Gets the attribute boundary character type in use with a given attribute
 * @param attr_key Attribute key
 * @return Boundary character type
 */
blogator::dom::html5::AttrBoundaryChar blogator::dom::DOTNode::boundaryChar( const std::u32string &attr_key ) {
    auto it = _attributes.find( attr_key );

    if( it != _attributes.end() )
        return it->second.boundary;

    std::stringstream loc;
    loc << "[blogator::dom::DOTNode::boundaryChar( \"" << encoding::encodeToUTF8( attr_key ) << " )";

    throw exception::DOMException( loc.str(), "Attribute key not found.", exception::DOMErrorType::NotFoundError );
}

/**
 * Gets the number of children nodes
 * @return Child node count
 */
size_t blogator::dom::DOTNode::childrenCount() const {
    return _children.size();
}

/**
 * Gets the node's HTML type
 * @return HTML type
 */
blogator::dom::html5::Tag blogator::dom::DOTNode::type() const {
    return _tag;
}

/**
 * Prints the attributes of the node
 * @param os Output stream
 */
void blogator::dom::DOTNode::printAttributes( u32ostream_t &os ) const {
    if( !_attributes.empty() )
        os << U" ";
    for( auto it = _attributes.cbegin(); it != _attributes.cend(); ++it ) {
        os << it->first;

        if( !it->second.value.empty() ) {
            os << U"=";

            switch( it->second.boundary ) {
                case html5::AttrBoundaryChar::APOSTROPHE:
                    os << html5::special_char::APOSTROPHE
                       << it->second.value
                       << html5::special_char::APOSTROPHE;
                    break;
                case html5::AttrBoundaryChar::QUOTATION_MARK:
                    os << html5::special_char::QUOTATION_MARK
                       << it->second.value
                       << html5::special_char::QUOTATION_MARK;
                    break;
                default:
                    os << it->second.value;
                    break;
            }
        }

        if( it != std::prev( _attributes.cend() ) )
            os << U" ";
    }
}