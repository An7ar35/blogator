#include "DocumentType.h"

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
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( node );
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
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<node::Node &&>( node );
        this->_name      = std::move( node._name );
        this->_public_id = std::move( node._public_id );
        this->_system_id = std::move( node._system_id );
    }

    return *this;
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

    return std::move( clone );
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
 * [OVERRIDE] Inserts a child node before a specified child (auto-sets the parent/sibling pointers)
 * @param node Pointer to Node to insert
 * @param child Reference to reference node for insertion placement
 * @return Pointer to inserted node
 * @throws DOMException when valid hierarchy is violated
 */
Node * DocumentType::insertNodeBefore( NodePtr_t node, Node * child ) {
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
blogator::parser::dom::NodePtr_t DocumentType::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType nodes do not have children." );
}