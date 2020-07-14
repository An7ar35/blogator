#include "DOTNode.h"

/**
 * Constructor
 */
blogator::dom::DOTNode::DOTNode() :
    DOTNode( html5::Tag::NONE )
{}

/**
 * Constructor
 * @param tag  Node's html tag type
 * @param text (optional) Text content
 */
blogator::dom::DOTNode::DOTNode( html5::Tag tag, std::u32string text ) :
    _tag( tag ),
    _text( std::move( text ) ),
    _parent( nullptr ),
    _children( std::vector<std::unique_ptr<DOTNode>>() )
{}

/**
 * Move-Constructor
 * @param node DOTNode to move over
 */
blogator::dom::DOTNode::DOTNode( blogator::dom::DOTNode &&node ) noexcept :
    _tag( node._tag ),
    _text( std::move( node._text ) ),
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
           && std::equal( _attributes.cbegin(), _attributes.cend(), rhs._attributes.cbegin() )
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
    auto p = std::make_unique<DOTNode>( _tag );

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
    auto p = std::make_unique<DOTNode>( _tag );

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
 * @return Reference to the new child
 * @throws HtmlParseLib::DOMException when attempt is made of giving a child to an unpaired node
 */
blogator::dom::DOTNode & blogator::dom::DOTNode::addChild( std::unique_ptr<DOTNode> node ) {
    if( !html5::Specs::isPaired( _tag ) ) //rule: Only paired structs can have nested children
        throw exception::DOMException(
            "[blogator::dom::DOTNode::addChild( std::unique_ptr<DOTNode> )] "
            "Cannot add children to an unpaired node type.",
            blogator::exception::DOMErrorType::HierarchyRequestError
        );

    _children.emplace_back( std::move( node ) );
    _children.back()->_parent = this;
    return *_children.back();
}

/**
 * Gets the node's raw data if any
 * @return Raw data
 */
const std::u32string & blogator::dom::DOTNode::content() const {
    return _text;
}

/**
 * Get's the node's parent
 * @return Pointer to parent of node or nullptr if orphaned
 */
const blogator::dom::DOTNode *blogator::dom::DOTNode::parent() const {
    return _parent;
}

/**
 * Gets an attribute's value
 * @param key Attribute key
 * @return Value string of the given attribute
 * @throws exception::DOMException when attribute key does not exist for the node
 */
std::u32string blogator::dom::DOTNode::attribute( const std::u32string &key ) const {
    try {
        return _attributes.at( key );
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
 * Adds a new attribute if key not found otherwise appends to the existing value
 * @param key   Attribute key
 * @param value Attribute value to add/append (adds a space prior to appending when value is !empty)
 * @return `true` key added, `false` if key already existed
 */
bool blogator::dom::DOTNode::addAttribute( const std::u32string &key, std::u32string value ) {
    auto it = _attributes.find( key );

    if( it != _attributes.end() ) {
        it->second += ( it->second.empty() ? std::move( value ) : U" " + std::move( value ) );
        return false;
    } else {
        _attributes.emplace( key, std::move( value ) );
        return true;
    }
}

/**
 * Replaces the value of a given attribute
 * @param key   Attribute key
 * @param value Attribute value
 * @throws exception::DOMException when key could not be found in the node's attributes
 */
void blogator::dom::DOTNode::replaceAttribute( const std::u32string &key, const std::u32string &value ) {
    auto it = _attributes.find( key );

    if( it != _attributes.end() ) {
        it->second = value;
        return;
    }

    std::stringstream ss;
    ss << "[blogator::dom::DOTNode::replaceAttribute( \""
       << encoding::encodeToUTF8( key ) << "\" , \""
       << encoding::encodeToUTF8( value ) << "\" )] "
       << "Attribute key not found.";

    throw exception::DOMException( ss.str(), exception::DOMErrorType::NotFoundError );
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
        os << it->first << U"=\"" << it->second << U"\"";
        if( it != std::prev( _attributes.cend() ) )
            os << U" ";
    }
}