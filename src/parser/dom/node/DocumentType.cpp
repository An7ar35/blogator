#include "DocumentType.h"

#include "../../../unicode/utf8.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param name Name string
 * @param public_id Public ID string
 * @param system_id System ID string
 */
DocumentType::DocumentType( DOMString_t name, DOMString_t public_id, DOMString_t system_id ) :
    node::Node( NodeType::DOCUMENT_TYPE_NODE ),
    _name( std::move( name ) ),
    _public_id( std::move( public_id ) ),
    _system_id( std::move( system_id ) )
{}


/**
 * Constructor
 * @param name Name string
 * @param public_id Public ID string
 * @param system_id System ID string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 */
DocumentType::DocumentType( DOMString_t name, DOMString_t public_id, DOMString_t system_id, Node *parent, Node *prev_sibling ) :
    node::Node( NodeType::DOCUMENT_TYPE_NODE, parent, prev_sibling ),
    _name( std::move( name ) ),
    _public_id( std::move( public_id ) ),
    _system_id( std::move( system_id ) )
{}

/**
 * Constructor
 * @param name Name string
 * @param public_id Public ID string
 * @param system_id System ID string
 * @param parent Pointer to parent
 * @param prev_sibling Pointer to previous sibling
 * @param next_sibling Pointer to next sibling
 */
DocumentType::DocumentType( DOMString_t name, DOMString_t public_id, DOMString_t system_id, Node *parent, Node *prev_sibling, Node *next_sibling ) :
    node::Node( NodeType::DOCUMENT_TYPE_NODE, parent, prev_sibling, next_sibling ),
    _name( std::move( name ) ),
    _public_id( std::move( public_id ) ),
    _system_id( std::move( system_id ) )
{}

/**
 * Copy-constructor
 * @param node Node to copy
 */
DocumentType::DocumentType( const DocumentType &node ) :
    node::Node( node ),
    _name( node._name ),
    _public_id( node._public_id ),
    _system_id( node._system_id )
{}

/**
 * Move-constructor
 * @param node Node to move
 */
DocumentType::DocumentType( DocumentType &&node ) noexcept :
    node::Node( std::move( node ) ),
    _name( std::move( node._name ) ),
    _public_id( std::move( node._public_id ) ),
    _system_id( std::move( node._system_id ) )
{}

/**
 * Copy-operator
 * @param node Node to copy
 * @return Copy
 */
DocumentType & DocumentType::operator =( const DocumentType &node ) {
    if( &node != this ) {
        Node::operator =( dynamic_cast<const node::Node &>( node ) );
        this->_name      = node._name;
        this->_public_id = node._public_id;
        this->_system_id = node._system_id;
    }

    return *this;
}

/**
 * Move-operator
 * @param node Node to move
 * @return Node
 */
DocumentType & DocumentType::operator =( DocumentType &&node ) noexcept {
    if( &node != this ) {
        Node::operator =( static_cast<node::Node &&>( node ) );
        this->_name      = std::move( node._name );
        this->_public_id = std::move( node._public_id );
        this->_system_id = std::move( node._system_id );
    }

    return *this;
}

/**
 * [OVERRRIDE] Shallow swaps nodes
 * @param rhs node to swap with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void DocumentType::swap( Node &rhs ) {
    Node::swap( rhs );
    this->swap( dynamic_cast<DocumentType &>( rhs ) );
}

/**
 * Shallow swaps DocumentType nodes
 * @param rhs node::DocumentType to swap with
 */
void DocumentType::swap( DocumentType &rhs ) {
    if( &rhs != this ) {
        std::swap( this->_name, rhs._name );
        std::swap( this->_public_id, rhs._public_id );
        std::swap( this->_system_id, rhs._system_id );
    }
}

/**
 * Gets the name
 * @return Name string
 */
const blogator::parser::dom::DOMString_t &DocumentType::name() const {
    return _name;
}

/**
 * Gets the public ID
 * @return Public ID string
 */
const blogator::parser::dom::DOMString_t &DocumentType::publicId() const {
    return _public_id;
}

/**
 * Gets the system ID
 * @return System ID string
 */
const blogator::parser::dom::DOMString_t &DocumentType::systemId() const {
    return _system_id;
}

/**
 * [OVERRIDE] Gets the length of the node
 * @return 0
 */
size_t DocumentType::length() const {
    return 0;
}

/**
 * [OVERRIDE] Gets the node's name
 * @return Name string
 */
blogator::parser::dom::DOMString_t DocumentType::nodeName() const {
    return _name;
}

/**
 * [OVERRIDE] Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t DocumentType::cloneNode( bool deep ) const {
    auto clone = std::make_unique<DocumentType>( this->_name, this->_public_id, this->_system_id );

    if( deep ) {
        for( const auto & child : _children ) {
            clone->appendChild( child->cloneNode( deep ) );
        }
    }

    return clone;
}

/**
 * [OVERRIDE] Checks node is equivalent to reference node
 * @param node Node to check
 * @return Are equivalent nodes
 */
bool DocumentType::isEqualNode( const Node &other ) const {
    if( this->nodeType()          == other.nodeType() &&
        this->childNodes().size() == other.childNodes().size() )
    {
        const auto * rhs = dynamic_cast<const DocumentType *>( &other );

        bool equal = this->name()     == rhs->name()
                  && this->publicId() == rhs->publicId()
                  && this->systemId() == rhs->systemId();

        if( equal ) {
            for( size_t i = 0; i < this->childNodes().size(); ++i ) {
                if( !this->childNodes()[i]->isEqualNode( *other.childNodes()[i] ) ) {
                    return false;
                }
            }

            return true;
        }
    }

    return false;
}

/**
 * [OVERRIDE] Lookup a namespace's prefix
 * @param prefix Prefix string (or "null")
 * @return Namespace URI (or "null")
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t DocumentType::lookupPrefix( [[maybe_unused]] const blogator::parser::dom::DOMString_t &ns ) const {
    return {};
}

/**
 * [OVERRIDE] Find the namespace URI of a given prefix
 * @param prefix Prefix string (empty == null)
 * @return Namespace URI
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t DocumentType::lookupNamespaceURI( [[maybe_unused]] const DOMString_t &prefix ) const {
    return {};
}

/**
 * [OVERRIDE] Inserts a child node before a specified child (auto-sets the parent/sibling pointers)
 * @param node Pointer to Node to insert
 * @param child Reference to reference node for insertion placement
 * @return Pointer to inserted node
 * @throws DOMException when valid hierarchy is violated
 */
Node * DocumentType::insertNodeBefore( [[maybe_unused]] NodePtr_t node, [[maybe_unused]] Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType nodes cannot have children." );
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t DocumentType::replaceChildNode( [[maybe_unused]] NodePtr_t &node, [[maybe_unused]] NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType nodes do not have children." );
}

/**
 * [OVERRIDE] Outputs the node as UTF-8 formatted html into a stream
 * @param os Output stream
 */
void DocumentType::toUTF8Stream( std::ostream &os ) const {
    os << "<!DOCTYPE ";
    blogator::unicode::utf8::convert( os, _name );

    if( !_public_id.empty() ) {
        os << " PUBLIC \"";
        blogator::unicode::utf8::convert( os, _public_id );
        os << "\"";
    }
    if( !_system_id.empty() ) {
        os << " SYSTEM \"";
        blogator::unicode::utf8::convert( os, _system_id );
        os << "\"";
    }

    os << ">";
}

/**
 * Shallow swaps DocumentType nodes
 * @param lhs DocumentType
 * @param rhs DocumentType
 */
void blogator::parser::dom::node::swap( DocumentType &lhs, DocumentType &rhs ) {
    lhs.swap( rhs );
}
