#include "Node.h"

#include <stdexcept>
#include <sstream>

#include "Attr.h"
#include "CDATASection.h"
#include "CharacterData.h"
#include "Comment.h"
#include "Document.h"
#include "DocumentType.h"
#include "Element.h"
#include "Text.h"
#include "../../../unicode/unicode.h"
#include "../../../logger/Logger.h"
#include "../../../exception/failed_expectation.h"

using namespace blogator::parser::dom::node;

const unsigned short Node::DOCUMENT_POSITION_DISCONNECTED            = 0x01; //0b00'0001 (1)
const unsigned short Node::DOCUMENT_POSITION_PRECEDING               = 0x02; //0b00'0010 (2)
const unsigned short Node::DOCUMENT_POSITION_FOLLOWING               = 0x04; //0b00'0100 (4)
const unsigned short Node::DOCUMENT_POSITION_CONTAINS                = 0x08; //0b00'1000 (8)
const unsigned short Node::DOCUMENT_POSITION_CONTAINED_BY            = 0x10; //0b01'0000 (16)
const unsigned short Node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = 0x20; //0b10'0000 (32)

std::ostream & blogator::parser::dom::node::operator <<( std::ostream &os, const Node &node ) {
    //TODO

    return os;
}

/**
 * Constructor
 */
Node::Node() :
    _parent( nullptr ),
    _prev_sibling( nullptr ),
    _next_sibling( nullptr ),
    _node_type( NodeType::UNDEFINED )
{}

/**
 * Constructor
 * @param type Node type
 */
Node::Node( NodeType type ) :
    _parent( nullptr ),
    _prev_sibling( nullptr ),
    _next_sibling( nullptr ),
    _node_type( type )
{}

/**
 * Constructor
 * @param type Node type
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
Node::Node( NodeType type, Node * parent, Node * prev_sibling ) :
    _parent( parent ),
    _prev_sibling( prev_sibling ),
    _next_sibling( nullptr ),
    _node_type( type )
{
    if( _prev_sibling != nullptr ) {
        this->setNextSibling( _prev_sibling->nextSibling() );
        _prev_sibling->setNextSibling( this );

        if( _next_sibling != nullptr ) {
            _next_sibling->setPrevSibling( this );
        }
    }
}

/**
 * Constructor
 * @param type Node type
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
Node::Node( NodeType type, Node * parent, Node * prev_sibling, Node * next_sibling ) :
    _parent( parent ),
    _prev_sibling( prev_sibling ),
    _next_sibling( next_sibling ),
    _node_type( type )
{
    if( _prev_sibling != nullptr ) {
        _prev_sibling->setNextSibling( this );
    }

    if( _next_sibling != nullptr ) {
        _next_sibling->setPrevSibling( this );
    }
}

/**
 * Copy-constructor (copies as detached node)
 * @param node Node to copy
 */
Node::Node( const Node &node ) :
    _parent( nullptr ),
    _prev_sibling( nullptr ),
    _next_sibling( nullptr ),
    _node_type( node._node_type )
{
    for( const auto & child : node._children ) {
        this->appendChild( child->cloneNode( true ) );
    }
}

/**
 * Move-constructor
 * @param node Node to move
 */
Node::Node( Node &&node )  noexcept :
    _parent( node._parent ),
    _prev_sibling( node._prev_sibling ),
    _next_sibling( node._next_sibling ),
    _node_type( node._node_type )
{
    for( auto & ptr : node._children ) {
        _children.emplace_back( std::move( ptr ) );
    }

    node._parent       = nullptr;
    node._prev_sibling = nullptr;
    node._next_sibling = nullptr;
    node._node_type    = NodeType::UNDEFINED;
    node._children.clear();
}

/**
 * Destructor
 */
Node::~Node() {
    if( _prev_sibling ) {
        _prev_sibling->setNextSibling( _next_sibling );
    }

    if( _next_sibling ) {
        _next_sibling->setPrevSibling( _prev_sibling );
    }
}

/**
 * Copy-assignment operator
 * @param node Node to copy
 * @return Detached copied node
 */
Node & Node::operator =( const Node &node ) {
    if( &node != this ) {
        _parent       = nullptr;
        _prev_sibling = nullptr;
        _next_sibling = nullptr;
        _node_type    = node._node_type;

        for( const auto & child : node._children ) {
            this->appendChild( child->cloneNode( true ) );
        }
    }

    return *this;
}

/**
 * Move-operator
 * @return node Node to move
 */
Node & Node::operator =( Node &&node ) noexcept {
    if( &node != this ) {
        _parent            = node._parent;
        _prev_sibling      = node._prev_sibling;
        _next_sibling      = node._next_sibling;
        _node_type         = node._node_type;

        for( auto & ptr: node._children ) {
            _children.emplace_back( std::move( ptr ) );
        }

        node._parent       = nullptr;
        node._prev_sibling = nullptr;
        node._next_sibling = nullptr;
        node._node_type    = NodeType::UNDEFINED;
        node._children.clear();
    }

    return *this;
}

/**
 * Equality comparator
 * @param rhs Node to compare to
 * @return Is same
 */
bool Node::operator ==( const node::Node &rhs ) const {
    return this == &rhs;
}

/**
 * Not-equal comparator
 * @param rhs Node to compare to
 * @return Is not same
 */
bool Node::operator !=( const node::Node &rhs ) const {
    return !( *this == rhs );
}

/**
 * Gets the node's type
 * @return Type as an enum
 */
blogator::parser::dom::NodeType Node::nodeType() const {
    return _node_type;
}

/**
 * Gets the node's name
 * @return Name string
 */
blogator::parser::dom::DOMString_t Node::nodeName() const {
    //ref: https://dom.spec.whatwg.org/#dom-node-nodename
    return {};
}

/**
 * Gets the absolute base URL of the document containing the node
 * @return Base URI
 */
blogator::parser::dom::USVString_t Node::baseURI() const {
    const auto * parent_doc = const_cast<node::Node *>( this )->ownerDocument();

    return ( parent_doc
             ? parent_doc->documentURI()
             : USVString_t() );
}

/**
 * Checks whether the node is connected to/is a Document
 * @return Document connection state
 */
bool Node::isConnected() const {
    return ( const_cast<node::Node *>( this )->ownerDocument() != nullptr );
}

/**
 * Gets the owner Document
 * @return Pointer to owner Document
 */
blogator::parser::dom::node::Document * Node::ownerDocument() {
    if( this->_node_type == NodeType::DOCUMENT_NODE ) {
        return dynamic_cast<Document *>( this );

    } else {
        auto * ptr = this->parentNode();

        while( ptr && ptr->nodeType() != NodeType::DOCUMENT_NODE ) {
            ptr = ptr->parentNode();
        }

        return dynamic_cast<Document *>( ptr );
    }
}

/**
 * Gets the root node
 * @return Pointer to root node
 */
Node * Node::getRootNode() {
    auto * root = this;

    while( root->parentNode() ) {
        root = root->parentNode();
    }

    return root;
}

/**
 * Checks if node has children
 * @return Has child/children state
 */
bool Node::hasChildNodes() const {
    return !_children.empty();
}

/**
 * Gets the children of the Node
 * @return Reference to children collection
 */
blogator::parser::dom::Nodes_t & Node::childNodes() {
    return _children;
}

/**
 * Gets the children of the Node
 * @return Const reference to children collection
 */
const blogator::parser::dom::Nodes_t & Node::childNodes() const {
    return _children;
}

/**
 * Gets the parent node
 * @return parent node (or nullptr if orphaned)
 */
Node * Node::parentNode() {
    return _parent;
}

/**
 * Gets the parent node
 * @return parent node (or nullptr if orphaned)
 */
const Node * Node::parentNode() const {
    return _parent;
}

/**
 * Returns the node's parent Element
 * @return Parent Element (nullptr if no parent or not a parent of type Element)
 */
blogator::parser::dom::node::Element * Node::parentElement() {
    if( this->parentNode() && this->parentNode()->nodeType() == NodeType::ELEMENT_NODE ) {
        return dynamic_cast<Element *>( this->parentNode() );
    } else {
        return nullptr;
    }
}

/**
 * Returns the node's parent Element
 * @return Parent Element (nullptr if no parent or not a parent of type Element)
 */
const Element *Node::parentElement() const {
    if( this->parentNode() && this->parentNode()->nodeType() == NodeType::ELEMENT_NODE ) {
        return dynamic_cast<const Element *>( this->parentNode() );
    } else {
        return nullptr;
    }
}

/**
 * Gets the first child of the node
 * @return Pointer to first child (or nullptr when there are no children)
 */
Node * Node::firstChild() {
    return ( hasChildNodes() ? _children.front().get() : nullptr );
}

/**
 * Gets the first child of the node
 * @return Pointer to first child (or nullptr when there are no children)
 */
const Node *Node::firstChild() const {
    return ( hasChildNodes() ? _children.front().get() : nullptr );
}

/**
 * Gets the last child of the node
 * @return Reference to last child  (or nullptr when there are no children)
 */
Node * Node::lastChild() {
    return ( hasChildNodes() ? _children.back().get() : nullptr );
}

/**
 * Gets the last child of the node
 * @return Reference to last child  (or nullptr when there are no children)
 */
const Node *Node::lastChild() const {
    return nullptr;
}

/**
 * Gets the node's previous sibling
 * @return Previous sibling (or nullptr)
 */
Node * Node::previousSibling() {
    return _prev_sibling;
}

/**
 * Gets the node's previous sibling
 * @return Previous sibling (or nullptr)
 */
const Node * Node::previousSibling() const {
    return _prev_sibling;
}

/**
 * Gets the node's next sibling
 * @return Next sibling (or nullptr
 */
Node * Node::nextSibling() {
    return _next_sibling;
}

/**
 * Gets the node's next sibling
 * @return Next sibling (or nullptr
 */
const Node * Node::nextSibling() const {
    return _next_sibling;
}

/**
 * Gets the node's value
 * @return Node value string or empty string
 */
blogator::parser::dom::DOMString_t * Node::nodeValue() {
    return nullptr;
}

/**
 * Gets the text content
 * @return Text content string
 */
blogator::parser::dom::DOMString_t Node::textContent() const {
    //ref: https://dom.spec.whatwg.org/#dom-node-textcontent
    return {}; //default
}

/**
 * Removes empty and concatenates the data of remaining
 * exclusive Text nodes into the first of their nodes
 */
void Node::normalize() {
    //ref: https://dom.spec.whatwg.org/#dom-node-normalize
    auto curr_it = _children.begin();

    while( curr_it != _children.end() ) {
        const auto type = (*curr_it)->nodeType();

        if( type == NodeType::TEXT_NODE || type == NodeType::COMMENT_NODE ) {
            if( (*curr_it)->length() != 0 ) {
                auto * curr_data = (*curr_it)->nodeValue();
                auto   next_it   = std::next( curr_it );

                while( next_it != _children.end() && (*next_it)->nodeType() == type ) {

                    if( (*next_it)->length() != 0 ) {
                        curr_data->append( *(*next_it)->nodeValue() );
                    }

                    next_it = _children.erase( next_it );
                }

                curr_it = next_it;

            } else {
                curr_it = _children.erase( curr_it );
            }

        } else {
            (*curr_it)->normalize();
            curr_it = std::next( curr_it );
        }
    }
}

/**
 * Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t Node::cloneNode( bool deep ) const {
    auto clone = std::make_unique<Node>( _node_type );

    if( deep ) {
        for( const auto & child : _children ) {
            clone->appendChild( child->cloneNode( deep ) );
        }
    }

    return std::move( clone );
}

/**
 * Gets the length of the node
 * @return Number of children
 */
size_t Node::length() const {
    return _children.size();
}

/**
 * Checks node is equivalent to reference node
 * @param node Node to check
 * @return Are equivalent nodes
 */
bool Node::isEqualNode( const NodePtr_t &other ) const {
    if( other ) {
        return this->isEqualNode( *other );
    }

    return false;
}

/**
 * Checks node is equivalent to reference node
 * @param node Node to check
 * @return Are equivalent nodes
 */
bool Node::isEqualNode( const Node &other ) const {
    //ref: https://dom.spec.whatwg.org/#concept-node-equals

    if( this->nodeType()       == other.nodeType() &&
        this->_children.size() == other._children.size() )
    {
        for( size_t i = 0; i < this->_children.size(); ++i ) {
            if( !this->_children[i]->isEqualNode( *other._children[i] ) ) {
                return false;
            }
        }

        return true;
    }

    return false;
}

/**
 * Checks if node is the same (===)
 * @param other Pointer to Node to check against
 * @return Are same node
 */
bool Node::isSameNode( const blogator::parser::dom::NodePtr_t &other ) const {
    if( other ) {
        return this->isSameNode( *other );
    }

    return false;
}

/**
 * Checks if node is the same (===)
 * @param other Node to check against
 * @return Are same node
 */
bool Node::isSameNode( const Node &other ) const {
    return *this == other;
}

/**
 * Returns a bitmask indicating the position of other relative to node
 * @param other Node to compare to
 * @return Document position bit mask
 */
unsigned short Node::compareDocumentPosition( const Node &other ) {
    //ref: https://dom.spec.whatwg.org/#dom-node-comparedocumentposition

    if( *this == other ) {
        return 0;
    }

    node::Node * node1 = const_cast<Node *>( &other );
    node::Node * node2 = this;
    node::Attr * attr1 = nullptr;
    node::Attr * attr2 = nullptr;

    if( node1->nodeType() == NodeType::ATTRIBUTE_NODE ){
        attr1 = dynamic_cast<Attr *>( node1 );
        node1 = dynamic_cast<Node *>( attr1->ownerElement() );
    }

    if( node2->nodeType() == NodeType::ATTRIBUTE_NODE ) {
        attr2 = dynamic_cast<Attr *>( node2 );
        node2 = dynamic_cast<Node *>( attr2->ownerElement() );

        if( attr1 && node1 && node2 == node1 ) {
            for( const AttrPtr_t & attr : dynamic_cast<Element *>( node2 )->attributes() ) {
                if( *attr == *attr1 ) {
                    return DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC + DOCUMENT_POSITION_PRECEDING;
                }
                if( *attr == *attr2 ) {
                    return DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC + DOCUMENT_POSITION_FOLLOWING;
                }
            }
        }
    }

    if( node1 == nullptr || node2 == nullptr || node1->getRootNode() != node2->getRootNode() ) {
        return DOCUMENT_POSITION_DISCONNECTED + DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC + DOCUMENT_POSITION_PRECEDING;
    }

    if( ( node1 != node2 && node1->contains( *node2 ) /* && attr1 == nullptr */ ) || ( node1 == node2 && attr2 != nullptr ) ) { //*
        return DOCUMENT_POSITION_CONTAINS + DOCUMENT_POSITION_PRECEDING;
    }

    if( ( node2->contains( *node1 ) /* && attr2 == nullptr */ ) || ( node1 == node2 && attr1 != nullptr ) ) { //*
        return DOCUMENT_POSITION_CONTAINED_BY + DOCUMENT_POSITION_FOLLOWING;
    }

    // *Note: `attr1 == nullptr` and `attr2 == nullptr` have been commented out above to be able to
    //        identify when one attribute's parent is a descendant of the other's parent.

    for( auto * curr = node2; curr != nullptr; curr = curr->_prev_sibling ) {
        if( curr == node1 ) {
            return DOCUMENT_POSITION_PRECEDING;
        }
    }

    return DOCUMENT_POSITION_FOLLOWING;
}

/**
 * Checks if other is an inclusive descendant
 * @param other Node to search for
 * @return Is inclusive descendant
 */
bool Node::contains( const Node &other ) const {
    if( *this == other )
        return true; //EARLY RETURN

    for( const auto & child : _children ) {
        if( child->contains( other ) ) {
            return true; //EARLY RETURN
        }
    }

    return false;
}

blogator::parser::dom::DOMString_t Node::lookupPrefix( specs::html5::Namespace ns ) { //TODO
    return blogator::parser::dom::DOMString_t();
}

blogator::parser::dom::DOMString_t Node::lookupNamespaceURI( const DOMString_t &prefix ) { //TODO
    return blogator::parser::dom::DOMString_t();
}

bool Node::isDefaultNamespace( const DOMString_t &ns ) const { //TODO
    return false;
}

bool Node::isDefaultNamespace( specs::html5::Namespace ns ) const { //TODO
    return false;
}

/**
 * Inserts a child node before a specified child (auto-sets the parent/sibling pointers)
 * @param node Pointer to Node to insert
 * @param child Pointer to reference node for insertion placement
 * @return Pointer to inserted node
 * @throws DOMException when valid hierarchy is violated
 * @throws std::invalid_argument if node is null
 */
Node * Node::insertBefore( NodePtr_t & node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( !node ) {
        throw std::invalid_argument( "Node to insert is null." );
    }

    if( this->contains( *node ) ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "Node is already in this DOM tree." );
    }

    return insertNodeBefore( std::move( node ), child );
}

/**
 * Inserts a child node before a specified child (auto-sets the parent/sibling pointers)
 * @param node Pointer to Node to insert
 * @param child Pointer to reference node for insertion placement
 * @return Pointer to inserted node
 * @throws DOMException when valid hierarchy is violated
 * @throws std::invalid_argument if node is null
 */
Node * Node::insertBefore( NodePtr_t && node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( !node ) {
        throw std::invalid_argument( "Node to insert is null." );
    }

    return insertNodeBefore( std::move( node ), child );
}

/**
 * Appends a child node to the children list (auto-sets the parent/sibling pointers)
 * @param node_ptr Pointer to child node to append
 * @return Pointer to newly created child
 * @throws std::invalid_argument if node is null
 */
Node * Node::appendChild( NodePtr_t &node_ptr ) {
    if( !node_ptr ) {
        throw std::invalid_argument( "Node to append is null." );
    }

    return this->insertNodeBefore( std::move( node_ptr ), nullptr );
}

/**
 * Appends a child node to the children list (auto-sets the parent/sibling pointers)
 * @param node_ptr Pointer to child node to append
 * @return Pointer to newly created child
 * @throws std::invalid_argument if node is null
 */
Node * Node::appendChild( NodePtr_t &&node_ptr ) {
    if( !node_ptr ) {
        throw std::invalid_argument( "Node to append is null." );
    }

    return this->insertNodeBefore( std::move( node_ptr ), nullptr );
}

/**
 * Replaces a child node (including its tree)
 * @param node Node to replace with
 * @param child Child node to replace
 * @return Old child node
 * @throws DOMException when child cannot be found in child list
 * @throws std::invalid_argument when child is null
 */
blogator::parser::dom::NodePtr_t Node::replaceChild( NodePtr_t &node, NodePtr_t &child ) {
    if( !child ) {
        throw std::invalid_argument( "Node to replace with is null." );
    }

    try {
        auto it = this->getChildListIterator( child.get() ); //throws

        return this->replaceChildNode( node, child );

    } catch( blogator::exception::failed_expectation &e ) {
        using exception::DOMException;
        using exception::DOMExceptionType;

        throw DOMException( DOMExceptionType::NotFoundError, "Child cannot be found in node's child list." );
    }
}

/**
 * Replaces a child node (including its tree)
 * @param node Node to replace with
 * @param child Index child node
 * @return Old child node
 * @throws DOMException when index value outside the children range
 */
blogator::parser::dom::NodePtr_t Node::replaceChild( NodePtr_t &node, size_t index ) {
    if( !_children.empty() && index < _children.size() ) {
        auto & child = _children.at( index );

        return this->replaceChildNode( node, child );

    } else {
        using exception::DOMException;
        using exception::DOMExceptionType;

        throw DOMException( DOMExceptionType::NotFoundError, "Child index '" + std::to_string( index ) + "' out-of-range of child list." );
    }
}

/**
 * Removes a child node
 * @param child Child to remove
 * @return Removed child node
 * @throws DOMException when child cannot be found in child list
 */
blogator::parser::dom::NodePtr_t Node::removeChild( NodePtr_t &child ) { //TODO remove map reference of node from Document
    try {
        return Node::removeChildNode( this->getChildListIterator( child.get() ) );

    } catch( blogator::exception::failed_expectation &e ) {
        using exception::DOMException;
        using exception::DOMExceptionType;

        throw DOMException( DOMExceptionType::NotFoundError,
                            "Child cannot be found in node's child list." );
    }
}

/**
 * Removes a child node
 * @param index Children index to remove node from
 * @return Removed node
 * @throws DOMException when index value outside the children range
 */
blogator::parser::dom::NodePtr_t Node::removeChild( size_t index ) { //TODO remove map reference of node from Document
    if( !_children.empty() && index < _children.size() ) {
        return Node::removeChildNode( std::next( _children.begin(), index ) );

    } else {
        using exception::DOMException;
        using exception::DOMExceptionType;

        throw DOMException( DOMExceptionType::NotFoundError,
                            "index '" + std::to_string( index ) + "' out-of-range of child list." );
    }
}

/**
 * [PROTECTED] Gets the iterator pointing to child in this parent's children list (good for getting siblings)
 * @param node Pointer to child node
 * @return Iterator of child in parent's children list
 * @throws blogator::exception::failed_expectation when child is orphaned or not in this parent's children list
 */
std::vector<blogator::parser::dom::NodePtr_t>::iterator Node::getParentChildListIterator( const Node * node ) {
    if( _parent == nullptr ) {
        throw blogator::exception::failed_expectation(
            "[blogator::parser::dom::node::Node::getParentChildListIterator( const Node * )] "
            "Given child is an orphan."
        );
    }

    auto & child_list = _parent->childNodes();

    auto it = std::find_if( child_list.begin(),child_list.end(),
                            [&node]( const NodePtr_t &uptr ) { return uptr.get() == node; }
    );

    if( it == child_list.end() ) {
        throw blogator::exception::failed_expectation(
            "[blogator::parser::dom::node::Node::getParentChildListIterator( const Node * )] "
            "Child does not exist the parent's children list."
        );
    }

    return it;
}

/**
 * [PROTECTED] Gets the iterator pointing to the child in this node's children list
 * @param child Pointer to child node to search for
 * @return Iterator of the child node in the children list
 * @throws blogator::exception::failed_expectation when child is orphaned or not in this node's children list
 */
blogator::parser::dom::Nodes_t::iterator Node::getChildListIterator( const Node * child ) {
    if( child->_parent == nullptr ) {
        throw blogator::exception::failed_expectation(
            "[blogator::parser::dom::node::Node::getChildListIterator( const Node * )] "
            "Given child is an orphan."
        );
    }

    if( child->_parent != this ) {
        throw blogator::exception::failed_expectation(
            "[blogator::parser::dom::node::Node::getChildListIterator( const Node * )] "
            "Given child's parent is not this node."
        );
    }

    auto it = std::find_if( _children.begin(), _children.end(),
                            [&child]( const NodePtr_t &uptr ) { return uptr.get() == child; }
    );

    if( it == _children.end() ) {
        throw blogator::exception::failed_expectation(
            "[blogator::parser::dom::node::Node::getChildListIterator( const Node * )] "
            "Child does not exist in the children list."
        );
    }

    return it;
}

/**
 * Insert node before a child
 * @param node Node to insert
 * @param child Pointer to child (nullptr if append at end)
 * @return Pointer to inserted child
 * @throws DOMException when insertion breaks DOM tree validity
 */
Node * Node::insertNodeBefore( NodePtr_t node, Node * child ) { //TODO add reference to new node to this Document's maps
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( child ) {
        try {
            auto   child_it     = getChildListIterator( child ); //throws
            Node * prev_sibling = child->previousSibling();
            Node * inserted     = _children.insert( child_it, std::move( node ) )->get();

            child->setPrevSibling( inserted );

            if( prev_sibling ) {
                prev_sibling->setNextSibling( inserted );
            }

            inserted->setParent( this );
            inserted->setPrevSibling( prev_sibling );
            inserted->setNextSibling( nullptr );

            return inserted;

        } catch( blogator::exception::failed_expectation &e ) {
            throw DOMException( DOMExceptionType::NotFoundError, "Child specified does not belong to this tree." );
        }

    } else {
        Node * prev_sibling = ( this->hasChildNodes() ? _children.back().get() : nullptr );
        Node * inserted     = _children.emplace_back( std::move( node ) ).get();

        if( prev_sibling ) {
            prev_sibling->setNextSibling( inserted );
        }

        inserted->setParent( this );
        inserted->setPrevSibling( prev_sibling );
        inserted->setNextSibling( nullptr );

        return inserted;
    }
}

/**
 * [PROTECTED] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t Node::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( this->contains( *node ) ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "Replacement node already in DOM tree." );
    }

    if( target->parentNode() != this ) {
        throw DOMException( DOMExceptionType::NotFoundError, "Target node's parent different from this node." );
    }

    switch( node->nodeType() ) {
        case NodeType::UNDEFINED: {
            LOG_ERROR(
                "[parser::dom::node::Node::replaceChildNode( NodePtr_t &, NodePtr_t & )] "
                "Replacement for '", blogator::to_string( target->nodeType() ), "' node is '", blogator::to_string( node->nodeType() ),"' "
                " - ignore this if for testing purposes."
            );
        } break;

        case NodeType::TEXT_NODE:          [[fallthrough]];
        case NodeType::CDATA_SECTION_NODE: [[fallthrough]];
        case NodeType::COMMENT_NODE:       [[fallthrough]];
        case NodeType::DOCUMENT_TYPE_NODE: [[fallthrough]];
        case NodeType::ELEMENT_NODE:       break;

        case NodeType::ATTRIBUTE_NODE:     [[fallthrough]];
        case NodeType::DOCUMENT_NODE:      [[fallthrough]];
        default: {
            throw DOMException(
                DOMExceptionType::HierarchyRequestError,
                "Node type '" + blogator::to_string( node->nodeType() ) + "' is not valid as a replacement"
            );
        };
    }

    //TODO add reference to new node to and remove old one from this Document's maps
    std::swap( node, target );

    target->setParent( node->parentNode() );
    target->setPrevSibling( node->previousSibling() );
    target->setNextSibling( node->nextSibling() );

    node->setParent( nullptr );
    node->setPrevSibling( nullptr );
    node->setNextSibling( nullptr );

    return std::move( node );
}

/**
 * [PROTECTED] Removes a child node
 * @param it Child iterator to node to remove
 * @return Removed node
 */
blogator::parser::dom::NodePtr_t Node::removeChildNode( dom::Nodes_t::iterator it ) {
    auto node = std::move( *it );

    if( node->previousSibling() ) {
        node->previousSibling()->setNextSibling( node->nextSibling() );
    }

    if( node->nextSibling() ) {
        node->nextSibling()->setPrevSibling( node->previousSibling() );
    }

    node->setParent( nullptr );
    node->setPrevSibling( nullptr );
    node->setNextSibling( nullptr );

    _children.erase( it );

    return std::move( node );
}

/**
 * [PROTECTED] Sets the pointer to the parent
 * @param node Pointer to parent
 */
void Node::setParent( Node * node ) {
    _parent = node;
}

/**
 * [PROTECTED] Sets the pointer to the previous sibling
 * @param node Pointer to previous sibling
 */
void Node::setPrevSibling( Node * node ) {
    _prev_sibling = node;
}

/**
 * [PROTECTED] Sets the pointer to the next sibling
 * @param node Pointer to next sibling
 */
void Node::setNextSibling( Node * node ) {
    _next_sibling = node;
}
