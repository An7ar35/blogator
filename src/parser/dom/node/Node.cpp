#include "Node.h"

#include <stdexcept>
#include <utility>

#include "Attr.h"
#include "CDATASection.h"
#include "Comment.h"
#include "Document.h"
#include "DocumentType.h"
#include "DocumentFragment.h"
#include "Element.h"
#include "Text.h"
#include "../../../unicode/unicode.h"
#include "../../../logger/Logger.h"
#include "../../../exception/failed_expectation.h"

using namespace blogator::parser::dom::node;

blogator::parser::dom::NamespaceMap Node::namespace_map = blogator::parser::dom::NamespaceMap();

/**
 * Constructor
 */
Node::Node() :
    _document( nullptr ),
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
    _document( nullptr ),
    _parent( nullptr ),
    _prev_sibling( nullptr ),
    _next_sibling( nullptr ),
    _node_type( type )
{}

/**
 * Constructor
 * @param document Owner document
 * @param type Node type
 */
Node::Node( Document *document, NodeType type ) :
    _document( document ),
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
    _document( ( parent ? parent->ownerDocument() : nullptr ) ),
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
    _document( ( parent ? parent->ownerDocument() : nullptr ) ),
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
    _document( nullptr ),
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
    _document( node._document ),
    _parent( node._parent ),
    _prev_sibling( node._prev_sibling ),
    _next_sibling( node._next_sibling ),
    _node_type( node._node_type )
{
    for( auto & ptr : node._children ) {
        auto & child = _children.emplace_back( std::move( ptr ) );

        if( child ) {
            child->setParent( this );
        }
    }

    node._document     = nullptr;
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
        _document     = nullptr;
        _parent       = nullptr;
        _prev_sibling = nullptr;
        _next_sibling = nullptr;
        _node_type    = node._node_type;
        _children.clear();

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
        _document          = node._document;
        _parent            = node._parent;
        _prev_sibling      = node._prev_sibling;
        _next_sibling      = node._next_sibling;
        _node_type         = node._node_type;
        _children.clear();

        for( auto & ptr: node._children ) {
            auto & child = _children.emplace_back( std::move( ptr ) );

            if( child ) {
                child->setParent( this );
            }
        }

        node._document     = nullptr;
        node._parent       = nullptr;
        node._prev_sibling = nullptr;
        node._next_sibling = nullptr;
        node._node_type    = NodeType::UNDEFINED;
        node._children.clear();
    }

    return *this;
}

/**
 * Output stream operator
 * @param os Output stream
 * @param node DOM Node
 * @return Output stream
 */
std::ostream & blogator::parser::dom::node::operator <<( std::ostream & os, const Node & node ) {
    node.toUTF8Stream( os );
    return os;
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
 * Gets a begin iterator whose root is this node
 * @return Iterator
 */
Node::iterator Node::begin() {
    return { this, false };
}

/**
 * Gets a begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Iterator
 */
Node::iterator Node::begin( NodeFilter filter ) {
    return { this, std::make_shared<NodeFilter>( std::move( filter ) ) };
}


/**
 * Gets a begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Iterator
 */
Node::iterator Node::begin( std::shared_ptr<NodeFilter> filter ) {
    return { this, std::move( filter ) };
}

/**
 * Gets an end iterator whose root is this node
 * @return Iterator
 */
Node::iterator Node::end() {
    return { this, nullptr };
}

/**
 * Gets a begin iterator whose root is this node
 * @return Const iterator
 */
Node::const_iterator Node::cbegin() const {
    return { this, false };
}

/**
 * Gets a begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Const iterator
 */
Node::const_iterator Node::cbegin( blogator::parser::dom::NodeFilter filter ) const {
    return { this, std::make_shared<NodeFilter>( std::move( filter ) ) };
}

/**
 * Gets a begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Const iterator
 */
Node::const_iterator Node::cbegin( std::shared_ptr<NodeFilter> filter ) const {
    return { this, std::move( filter ) };
}

/**
 * Gets an end iterator whose root is this node
 * @return Const iterator
 */
Node::const_iterator Node::cend() const {
    return { this, nullptr };
}

/**
 * Gets a reverse begin iterator whose root is this node
 * @return Reverse iterator
 */
Node::iterator Node::rbegin() {
    return { this, true };
}

/**
 * Gets a reverse begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Reverse iterator
 */
Node::iterator Node::rbegin( blogator::parser::dom::NodeFilter filter ) {
    return { this, std::make_shared<NodeFilter>( std::move( filter ) ), true };
}

/**
 * Gets a reverse begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Reverse iterator
 */
Node::iterator Node::rbegin( std::shared_ptr<NodeFilter> filter ) {
    return { this, std::move( filter ), true };
}

/**
 * Gets a reverse end iterator whose root is this node
 * @return Reverse iterator
 */
Node::iterator Node::rend() {
    return { this, nullptr, true };
}

/**
 * Gets a reverse begin iterator whose root is this node
 * @return Const reverse iterator
 */
Node::const_iterator Node::crbegin() const {
    return { this, true };
}

/**
 * Gets a reverse begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Const reverse iterator
 */
Node::const_iterator Node::crbegin( blogator::parser::dom::NodeFilter filter ) const {
    return { this, std::make_shared<NodeFilter>( std::move( filter ) ), true };
}

/**
 * Gets a reverse begin iterator whose root is this node
 * @param filter Filter to use for the iteration
 * @return Const reverse iterator
 */
Node::const_iterator Node::crbegin( std::shared_ptr<NodeFilter> filter ) const {
    return { this, std::move( filter ), true };
}

/**
 * Gets a const reverse end iterator whose root is this node
 * @return Const reverse Iterator
 */
Node::const_iterator Node::crend() const {
    return { this, nullptr, true };
}

/**
 * Gets the tree depth of the node
 * @return Level in tree
 */
size_t Node::depth() const { //TODO unit test
    auto * curr = this;
    size_t lvl  = 0;

    while( curr->parentNode() ) {
        curr = curr->parentNode();
        ++lvl;
    }

    return lvl;
}

/**
 * Shallow swaps Nodes in-place
 * @param rhs Node to swap content with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void Node::swap( Node &rhs ) {
    if( &rhs != this ) {
        //---------------------------------------------------------------------------------------------
        // NOTE: Parent, siblings, children and document pointers are not swapped since the node's DOM
        //       placement in its hierarchy should be left unchanged.
        //       Only the content itself should be swapped.
        //---------------------------------------------------------------------------------------------

        if( this->nodeType() != rhs.nodeType() ) {
            using blogator::parser::dom::exception::DOMException;
            using blogator::parser::dom::exception::DOMExceptionType;

            throw DOMException(
                DOMExceptionType::HierarchyRequestError,
                "Trying to swap different node types "
                "(" + blogator::to_string( this->nodeType() ) + " <-> " + blogator::to_string( rhs.nodeType() ) + ")"
            );
        }
    }
}

/**
 * Detaches node tree from parent
 * @return Detached node
 * @throws parser::dom::exception::DOMException when there is no parent to detach from
 */
blogator::parser::dom::NodePtr_t Node::detach() { //TODO test
    try {
        if( _parent == nullptr ) {
            throw FAILED_EXPECTATION_EXCEPTION( "[parser::dom::node::Node::detach()] Parent is NULL." );
        }

        return _parent->removeChildNode( getParentChildListIterator( this ) );

    } catch( const blogator::exception::failed_expectation &e ) {
        using blogator::parser::dom::exception::DOMException;
        using blogator::parser::dom::exception::DOMExceptionType;

        throw DOMException(
            DOMExceptionType::HierarchyRequestError,
            "Trying to detach an orphaned node (no parent)."
        );
    }
}

/**
 * Gets the Namespace enum associated with this node's ID
 * @return specs::infra::Namespace enum associated with the ID
 * @throws blogator::exception::failed_expectation when mapping inconsistency is detected
 */
blogator::parser::specs::infra::Namespace Node::getNamespaceEnum() const {
    return blogator::parser::specs::infra::Namespace::NONE;
}

/**
 * Helper method to get a Namespace enum from an ID
 * @param id Namespace ID
 * @return specs::infra::Namespace enum associated with the ID
 * @throws blogator::exception::failed_expectation when mapping inconsistency is detected
 */
blogator::parser::specs::infra::Namespace Node::getNamespaceEnum( NamespaceMap::id_t id ) {
    try {
        return Node::namespace_map.getNamespaceEnum( id ); //throws

    } catch( const std::out_of_range &e ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::node::Node::getNamespaceEnum( " + std::to_string( id ) + " )] " + e.what()
        );
    }
}

/**
 * Helper method to get a Namespace URI string from an ID
 * @param id Namespace ID
 * @return Namespace URI string associated with the ID
 * @throws blogator::exception::failed_expectation when mapping inconsistency is detected
 */
const blogator::parser::dom::DOMString_t & Node::getNamespaceURI( NamespaceMap::id_t id ) {
    try {
        return Node::namespace_map.getNamespaceURI( id ); //throws

    } catch( const std::out_of_range &e ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::dom::node::Node::getNamespaceURI( " + std::to_string( id ) + " )] " + e.what()
        );
    }
}

/**
 * Gets the global namespace mapper
 * @return Global NamespaceMap
 */
blogator::parser::dom::NamespaceMap & Node::namespaceMap() {
    return Node::namespace_map;
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
 * Gets the ownerElement Document
 * @return Pointer to ownerElement Document
 */
blogator::parser::dom::node::Document * Node::ownerDocument() {
    if( this->_node_type == NodeType::DOCUMENT_NODE ) {
        return dynamic_cast<Document *>( this );

    } else {
        return _document;
    }
}

/**
 * Gets the ownerElement Document
 * @return Pointer to ownerElement Document
 */
const blogator::parser::dom::node::Document * Node::ownerDocument() const {
    return const_cast<Document *>( const_cast<Node *>( this )->ownerDocument() );
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
    return const_cast<Nodes_t &>( const_cast<Node *>( this )->childNodes() );
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
    return const_cast<Node *>( const_cast<Node *>( this )->parentNode() );
}

/**
 * Returns the node's parent Element
 * @return Parent Element (nullptr if no parent or not a parent of type Element)
 */
blogator::parser::dom::node::Element * Node::parentElement() {
    if( _parent && _parent->nodeType() == NodeType::ELEMENT_NODE ) {
        return dynamic_cast<Element *>( _parent );
    } else {
        return nullptr;
    }
}

/**
 * Returns the node's parent Element
 * @return Parent Element (nullptr if no parent or not a parent of type Element)
 */
const Element * Node::parentElement() const {
    return const_cast<Element *>( const_cast<Node *>( this )->parentElement() );
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
const Node * Node::firstChild() const {
    return const_cast<Node *>( const_cast<Node *>( this )->firstChild() );
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
const Node * Node::lastChild() const {
    return const_cast<Node *>( const_cast<Node *>( this )->lastChild() );
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
    return const_cast<Node *>( const_cast<Node *>( this )->previousSibling() );
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
    return const_cast<Node *>( const_cast<Node *>( this )->nextSibling() );
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
            for( const AttrPtr_t & attr : dynamic_cast<Element *>( node2 )->attributes().list() ) {
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

/**
 * Lookup a namespace's prefix
 * @param ns Namespace to find prefix for
 * @return Prefix found (empty string == null)
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t Node::lookupPrefix( const DOMString_t & ns ) const {
    //ref: https://dom.spec.whatwg.org/#dom-node-lookupprefix
    if( this->parentElement() ) {
        return this->parentElement()->lookupPrefix( ns );
    }

    return {};
}

/**
 * Find the namespace URI of a given prefix
 * @param prefix Prefix string (or "null")
 * @return Namespace URI (or "null")
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t Node::lookupNamespaceURI( const DOMString_t &prefix ) const {
    //ref: https://dom.spec.whatwg.org/#locate-a-namespace (non-null prefix)
    if( this->parentElement() ) {
        return this->parentElement()->lookupNamespaceURI( prefix );
    }

    return {};
}

/**
 * Checks if given namespace is default
 * @param ns Namespace string
 * @return Default state
 */
bool Node::isDefaultNamespace( const DOMString_t &ns ) const {
    //ref: https://dom.spec.whatwg.org/#dom-node-isdefaultnamespace
    return ( this->lookupNamespaceURI( U"" ) == ns );
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
        //Note: the check for host-including inclusive ancestor of a parent is skipped
        //      as DocumentFragments will never have hosts in this implementation
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
blogator::parser::dom::NodePtr_t Node::removeChild( NodePtr_t &child ) {
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
blogator::parser::dom::NodePtr_t Node::removeChild( size_t index ) {
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
        throw FAILED_EXPECTATION_EXCEPTION(
            "[blogator::parser::dom::node::Node::getParentChildListIterator( const Node * )] "
            "Given child is an orphan."
        );
    }

    auto & child_list = _parent->childNodes();

    auto it = std::find_if( child_list.begin(),child_list.end(),
                            [&node]( const NodePtr_t &uptr ) { return uptr.get() == node; }
    );

    if( it == child_list.end() ) {
        throw FAILED_EXPECTATION_EXCEPTION(
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
        throw FAILED_EXPECTATION_EXCEPTION(
            "[blogator::parser::dom::node::Node::getChildListIterator( const Node * )] "
            "Given child is an orphan."
        );
    }

    if( child->_parent != this ) {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[blogator::parser::dom::node::Node::getChildListIterator( const Node * )] "
            "Given child's parent is not this node."
        );
    }

    auto it = std::find_if( _children.begin(), _children.end(),
                            [&child]( const NodePtr_t &uptr ) { return uptr.get() == child; }
    );

    if( it == _children.end() ) {
        throw FAILED_EXPECTATION_EXCEPTION(
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
Node * Node::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    /**
     * [LAMBDA] Checks if this node is a 'template' element (edge case)
     */
    const auto isTemplateElement = [this]() {
        return this->nodeType()                               == NodeType::ELEMENT_NODE
            && dynamic_cast<Element *>( this )->elementType() == specs::infra::Element::HTML5_TEMPLATE;
    };


    if( child ) {
        try {
            auto child_it = getChildListIterator( child ); //throws

            if( node->nodeType() == NodeType::DOCUMENT_FRAGMENT_NODE && !isTemplateElement() ) {
                return insertNodesBefore( node->childNodes(), child_it );
            } else {
                return insertNodeBefore( node, child_it );
            }

        } catch( blogator::exception::failed_expectation &e ) {
            throw DOMException(
                DOMExceptionType::NotFoundError,
                "Child specified does not belong to this tree."
            );
        }

    } else {
        if( node->nodeType() == NodeType::DOCUMENT_FRAGMENT_NODE && !isTemplateElement() ) { //edge case
            return appendNodes( node->childNodes() );
        } else {
            return appendNode( node );
        }
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

    if( this->contains( *node ) ) { //TODO re-implement as this is now wrong!
        //Note: the check for host-including inclusive ancestor of a parent is skipped
        //      as DocumentFragments will never have hosts in this implementation
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

    std::swap( node, target );

    target->setOwnerDocument( node->ownerDocument() );
    target->setParent( node->parentNode() );
    target->setPrevSibling( node->previousSibling() );
    target->setNextSibling( node->nextSibling() );

    node->setOwnerDocument( nullptr );
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

    node->setOwnerDocument( nullptr );
    node->setParent( nullptr );
    node->setPrevSibling( nullptr );
    node->setNextSibling( nullptr );

    _children.erase( it );

    return std::move( node );
}

/**
 * [PROTECTED] Recursively sets the pointer to the owning Document
 * @param node Pointer to new document
 */
void Node::setOwnerDocument( Document * document ) {
    _document = document;

    for( auto & child : _children ) {
        child->setOwnerDocument( document );
    }
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

/**
 * Outputs the node as UTF-8 formatted html into a stream
 * @param os Output stream
 */
void Node::toUTF8Stream( std::ostream &os ) const { /* do nothing / use overrides */ }

/**
 * [PRIVATE] Appends a single node to the end of the children list
 * @param node Node to append
 * @return Pointer to appended node
 */
Node * Node::appendNode( NodePtr_t &node ) {
    Node * prev_sibling = ( this->hasChildNodes() ? _children.back().get() : nullptr );
    Node * inserted     = _children.emplace_back( std::move( node ) ).get();

    if( prev_sibling ) {
        prev_sibling->setNextSibling( inserted );
    }

    inserted->setOwnerDocument( this->ownerDocument() );
    inserted->setParent( this );
    inserted->setPrevSibling( prev_sibling );
    inserted->setNextSibling( nullptr );

    return inserted;
}

/**
 * [PRIVATE] Appends a list of nodes to the end of the children list
 * @param nodes List of nodes to append
 * @return Pointer to first appended node (or nullptr when the list is empty)
 */
Node * Node::appendNodes( Nodes_t &nodes ) {
    auto it = nodes.begin();

    if( it == nodes.end() ) {
        return nullptr; //EARLY RETURN
    }

    Node * first_inserted = appendNode( *(it++) );

    while( it != nodes.end() ) {
        appendNode( *(it++) );
    }

    nodes.clear(); //clear all NULL unique_ptr

    return first_inserted;
}

/**
 * [PRIVATE] Insert a single node before another in the children list
 * @param node Node to insert
 * @param it Iterator to the child node to insert before
 * @return Pointer to inserted node
 */
Node * Node::insertNodeBefore( NodePtr_t &node, Nodes_t::iterator it ) {
    Node * prev_sibling = (*it)->previousSibling();
    Node * next_sibling = (*it).get();
    Node * inserted     = _children.insert( it, std::move( node ) )->get();

    next_sibling->setPrevSibling( inserted );

    if( prev_sibling ) {
        prev_sibling->setNextSibling( inserted );
    }

    inserted->setOwnerDocument( this->ownerDocument() );
    inserted->setParent( this );
    inserted->setPrevSibling( prev_sibling );
    inserted->setNextSibling( next_sibling );

    return inserted;

}

/**
 * [PRIVATE] Insert a list of nodes before a child node in the children list
 * @param nodes List of nodes to insert
 * @param it Iterator to the child node to insert before
 * @return Pointer to first inserted node
 */
Node * Node::insertNodesBefore( Nodes_t &nodes, Nodes_t::iterator it ) {
    Node * prev_sibling    = (*it)->previousSibling();
    Node * next_sibling    = (*it).get();
    auto   last_new_child  = nodes.back().get();
    auto   first_new_child = _children.insert( it,
                                               std::make_move_iterator( nodes.begin() ),
                                               std::make_move_iterator( nodes.end() ) )->get();


    first_new_child->setPrevSibling( prev_sibling );
    last_new_child->setNextSibling( next_sibling );

    if( prev_sibling ) {
        prev_sibling->setNextSibling( first_new_child );
    }

    if( next_sibling ) {
        next_sibling->setPrevSibling( last_new_child );
    }

    for( auto * curr = first_new_child; curr != next_sibling; curr = curr->nextSibling() ) {
        curr->setOwnerDocument( this->ownerDocument() );
        curr->setParent( this );
    }

    nodes.clear();

    return first_new_child;
}

/**
 * Shallow swaps Nodes
 * @param lhs Node
 * @param rhs Node
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void blogator::parser::dom::node::swap( Node &lhs, Node &rhs ) {
    lhs.swap( rhs );
}