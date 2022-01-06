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
 * Gets the length of the node
 * @return 0
 */
size_t DocumentType::length() const {
    return 0;
}

/**
 * Gets the node's name
 * @return Name string
 */
blogator::parser::dom::DOMString_t DocumentType::nodeName() const {
    return _name;
}

/**
 * Clones the node
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