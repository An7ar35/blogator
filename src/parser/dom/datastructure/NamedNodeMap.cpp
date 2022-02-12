#include "NamedNodeMap.h"

#include <memory>
#include <utility>

#include "../node/Attr.h"
#include "../node/Document.h"
#include "../node/Element.h"
#include "../../../unicode/unicode.h"
#include "../../../logger/Logger.h"
#include "../validation/XML.h"

using namespace blogator::parser::dom;

/**
 * Constructor
 * @param parent
 */
NamedNodeMap::NamedNodeMap( node::Node * parent ) :
    _parent( parent )
{}


/**
 * Copy-constructor
 * @param nnp NameNodeMap to copy
 */
NamedNodeMap::NamedNodeMap( const NamedNodeMap &nnp ) :
    _parent( nnp._parent )
{
    for( const auto & attr : nnp._nodes ) {
        if( attr ) {
            if( attr->value() ) {
                this->appendAttribute( std::make_unique<node::Attr>(
                    attr->prefix(),
                    attr->name(),
                    *attr->value()
                ) );

            } else {
                this->appendAttribute( std::make_unique<node::Attr>(
                    attr->prefix(),
                    attr->name()
                ) );
            }
        }
    }
}

/**
 * Move-constructor
 * @param nnp NamedNodeMap to move
 */
NamedNodeMap::NamedNodeMap( NamedNodeMap &&nnp ) noexcept :
    _parent( nnp._parent )
{
    for( auto & attr : nnp._nodes ) {
        if( attr ) {
            this->appendAttribute( std::move( attr ) );
        }
    }
}

/**
 * Copy-assignment operator
 * @param nnp NameNodeMap to copy
 * @return Copy
 */
NamedNodeMap & NamedNodeMap::operator =( const NamedNodeMap &nnp ) {
    if( &nnp != this ) {
        this->_parent = nnp._parent;
        this->_map.clear();
        this->_nodes.clear();

        for( const auto & attr : nnp._nodes ) {
            if( attr ) {
                if( attr->value() ) {
                    this->appendAttribute( std::make_unique<node::Attr>(
                        attr->prefix(),
                        attr->name(),
                        *attr->value()
                    ) );

                } else {
                    this->appendAttribute( std::make_unique<node::Attr>(
                        attr->prefix(),
                        attr->name()
                    ) );
                }
            }
        }
    }

    return *this;
}

/**
 * Move-assignment operator
 * @param nnp NamedNodeMap to move
 * @return Moved object
 */
NamedNodeMap & NamedNodeMap::operator =( NamedNodeMap &&nnp ) noexcept {
    if( &nnp != this ) {
        this->_parent = nnp._parent;
        this->_map.clear();
        this->_nodes.clear();

        for( auto & attr : nnp._nodes ) {
            if( attr ) {
                this->appendAttribute( std::move( attr ) );
            }
        }
    }

    return *this;
}

/**
 * Swaps Element nodes (Note: all attributes will have their document owner and parent pointers reset)
 * @param rhs NamedNodeMap to swap with
 */
void NamedNodeMap::swap( NamedNodeMap &rhs ) {
    if( &rhs != this ) {
        std::swap( this->_nodes, rhs._nodes );
        std::swap( this->_map, rhs._map );

        this->setParent( _parent );
        rhs.setParent( rhs._parent );
    }
}

/**
 * Gets the underlining attribute list data-structure
 * @return AttributeList
 */
const Attributes_t & NamedNodeMap::list() const {
    return _nodes;
}

/**
 * Checks if an attribute exists
 * @param qualified_name Qualified name
 * @return Existence state
 */
bool NamedNodeMap::attributeExists( DOMString_t qualified_name ) const {
    if( this->ownerElement() && this->ownerElement()->isHtmlNative() ) {
        unicode::ascii::tolower( qualified_name );
    }

    return ( _map.contains( qualified_name ) );
}

/**
 * Gets the empty state of the attribute list
 * @return Empty state
 */
bool NamedNodeMap::empty() const {
    return _nodes.empty();
}

/**
 * Gets the size of the attribute list
 * @return Number of attributes in list
 */
size_t NamedNodeMap::length() const {
    return _nodes.size();
}

/**
 * Gets an attribute at specified index
 * @param index Index of attribute in list
 * @return Pointer to attribute node (or nullptr if not in range)
 */
const node::Attr * NamedNodeMap::item( size_t index ) const {
    if( index < _nodes.size() ) {
        return _nodes.at( index ).get();
    }

    return nullptr;
}

/**
 * Gets an attribute by its qualified name
 * @param qualified_name Qualified name string
 * @return Pointer to attribute node (or nullptr)
 */
const node::Attr * NamedNodeMap::getNamedItem( DOMString_t qualified_name ) const {
    if( this->ownerElement() && this->ownerElement()->isHtmlNative() ) {
        unicode::ascii::tolower( qualified_name );
    }

    if( _map.contains( qualified_name ) ) {
        return _map.at( qualified_name );
    }

    return nullptr;
}

/**
 * Sets an attribute
 * @param attr Attribute node
 * @return Pointer to attribute node edited/added
 * @throws DOMException when qualified name of node is invalid
 */
const node::Attr * NamedNodeMap::setNamedItem( const node::Attr & attr ) {
    if( !dom::validation::XML::isValidName( attr.nodeName() ) ) {
        using blogator::unicode::utf8::convert;

        throw exception::DOMException(
            exception::DOMExceptionType::InvalidCharacterError,
            "[parser::dom::NamedNodeMap::setNamedItem( const node::Attr & )] Invalid qualified name ('" + convert( attr.nodeName() ) + "')."
        );
    }

    auto map_it = _map.find( attr.nodeName() );

    if( map_it != _map.end() ) {
        auto * old = map_it->second;

        if( old == nullptr ) {
            LOG_WARNING(
                "[parser::dom::NamedNodeMap::setNamedItem( node::Attr )] "
                "Found a null pointer in map ('", blogator::unicode::utf8::convert( attr.nodeName() ), "') "
                "- removing entry."
            );

            _map.erase( map_it );

        } else if( *old != attr ) {
            old->_prefix    = attr.prefix();
            old->_name      = attr.name();
            old->_value     = ( attr.value() ? *attr.value() : DOMString_t() );
            old->_has_value = attr.hasValue();

            return old; //EARLY RETURN
        }
    }

    return this->appendAttribute( std::make_unique<node::Attr>( attr ) );
}

/**
 * Sets an attribute
 * @param attr Attribute
 * @return Pointer to attribute node edited/added
 * @throws DOMException when qualified name of node is invalid
 */
const node::Attr * NamedNodeMap::setNode( AttrPtr_t attr ) {
    if( !attr ) {
        LOG_WARNING(
            "[parser::dom::NamedNodeMap::setNode( AttrPtr_t )] "
            "Pointer is null."
        );

        return nullptr; //EARLY RETURN
    }

    if( !dom::validation::XML::isValidName( attr->nodeName() ) ) {
        using blogator::unicode::utf8::convert;

        throw exception::DOMException(
            exception::DOMExceptionType::InvalidCharacterError,
            "[parser::dom::NamedNodeMap::setNode( AttrPtr_t )] Invalid qualified name ('" + convert( attr->nodeName() ) + "')."
        );
    }

    auto map_it = _map.find( attr->nodeName() );

    if( map_it != _map.end() ) {
        auto * old = map_it->second;

        if( old == nullptr ) {
            LOG_WARNING(
                "[parser::dom::NamedNodeMap::setNode( AttrPtr_t )] "
                "Found a null pointer in map ('", blogator::unicode::utf8::convert( attr->nodeName() ), "') "
                "- removing entry."
            );

            _map.erase( map_it );

        } else if( *old != *attr ) {
            std::swap( *attr, *old );
            return old; //EARLY RETURN
        }
    }

    return this->appendAttribute( std::move( attr ) );
}


/**
 * Removes an attribute
 * @param index Index of attribute
 * @return Removed attribute node (or null)
 */
AttrPtr_t NamedNodeMap::removeItem( size_t index ) {
    if( index >= _nodes.size() ) {
        return {}; //EARLY RETURN
    }

    auto & ptr = _nodes.at( index );
    auto   it  = std::next( _nodes.begin(), index );

    if( it == _nodes.end() ) {
        LOG_ERROR(
            "[parser::dom::NamedNodeMap::removeItem( ", std::to_string( index ), " )] "
            "Could not find the attribute iterator to be removed ('", blogator::unicode::utf8::convert( ptr->nodeName() ), "') "
            "- this should not happen."
        );
    }

    if( ptr ) {
        _map.erase( ptr->nodeName() );

        auto removed = std::move( ptr );

        if( it != _nodes.end() ) {
            _nodes.erase( it );
        }

        return std::move( removed ); //BRANCHED RETURN (1/2)

    } else {
        if( it != _nodes.end() ) {
            _nodes.erase( it );
        }

        LOG_WARNING(
            "[parser::dom::NamedNodeMap::removeItem( ", std::to_string( index ), " )] "
            "Attribute's `unique_ptr` was null - position was cleaned-up."
        );

        return {}; //BRANCHED RETURN (2/2)
    }
}

/**
 * Removes an attribute
 * @param node Pointer to attribute node
 * @return Removed attribute node
 */
AttrPtr_t NamedNodeMap::removeNode( const node::Attr * node ) {
    if( node == nullptr ) {
        LOG_WARNING(
            "[parser::dom::NamedNodeMap::removeNode( node::Attr * )] "
            "Pointer is null."
        );

        return {}; //EARLY RETURN
    }

    auto it = this->getIterator( node );

    if( it != _nodes.end() && *it ) {
        _map.erase( (*it)->nodeName() );

        auto removed = std::move( *it );

        _nodes.erase( it );

        return std::move( removed ); //EARLY RETURN
    }

    LOG_ERROR(
        "[parser::dom::NamedNodeMap::removeNode( node::Attr * )] "
        "Pointer not found in attribute list."
    );

    return {}; //EARLY RETURN
}

/**
 * Removes an attribute
 * @param qualified_name Qualified name of the attribute to remove
 * @return Removed attribute node
 */
AttrPtr_t NamedNodeMap::removeNamedItem( DOMString_t qualified_name ) {
    if( this->ownerElement() && this->ownerElement()->isHtmlNative() ) {
        unicode::ascii::tolower( qualified_name );
    }

    auto map_it = _map.find( qualified_name );

    if( map_it == _map.end() ) {
        return {}; //EARLY RETURN
    }

    const auto * node_ptr = map_it->second;
    auto         node_it  = std::find_if( _nodes.begin(),
                                          _nodes.end(),
                                          [&node_ptr]( const AttrPtr_t &ptr ) { return ( ptr && ptr.get() == node_ptr ); } );

    _map.erase( map_it );

    if( node_it != _nodes.end() ) {
        auto removed = std::move( *node_it );

        _nodes.erase( node_it );

        return std::move( removed ); //BRANCHED RETURN (1/2)

    } else {
        LOG_ERROR(
            "[parser::dom::NamedNodeMap::removeItem( ", blogator::unicode::utf8::convert( qualified_name ), " )] "
            "Could not find the attribute iterator to be removed despite finding it in the map "
            "('", ( node_ptr ? blogator::unicode::utf8::convert( node_ptr->nodeName() ) : "NULL" ), "')."
        );

        return {}; //BRANCHED RETURN (2/2)
    }
}

/**
 * Gets the current associated owning node
 * @return Pointer to parent node
 */
const node::Node * NamedNodeMap::ownerNode() const {
    return _parent;
}

/**
 * Gets the current associated owning Element node
 * @return Pointer to parent Element node
 */
const node::Element * NamedNodeMap::ownerElement() const {
    return ( _parent && _parent->nodeType() == NodeType::ELEMENT_NODE
             ? dynamic_cast<const node::Element *>( _parent )
             : nullptr );
}

/**
 * [PRIVATE] Sets the owner document (only) for all the Attr(ibute) nodes
 * @param document Pointer to new owner document
 */
void NamedNodeMap::setOwnerDocument( node::Document * document ) {
    for( auto & attr : _nodes ) {
        if( attr ) {
            attr->setOwnerDocument( document );
        }
    }
}

/**
 * [PRIVATE] Sets the parent Element and owner Document (from the element) for all the Attr(ibute) nodes
 * @param parent Pointer to parent Element node
 */
void NamedNodeMap::setParent( node::Node * parent ) {
    _parent = parent;

    for( auto & attr : _nodes ) {
        if( attr ) {
            attr->setParent( parent );

            if( parent ) {
                attr->setOwnerDocument( parent->ownerDocument() );
            }
        }
    }
}

/**
 * [PRIVATE] Gets the iterator in the attribute list that match a pointer
 * @param ptr Attr node pointer
 * @return Iterator pointing to matching node
 */
Attributes_t::iterator NamedNodeMap::getIterator( const node::Attr * ptr ) {
    return std::find_if( _nodes.begin(),
                         _nodes.end(),
                         [&ptr]( const auto & attr ) { return attr.get() == ptr; } );
}

/**
 * [PRIVATE] Appends an Attr(ibute) node to the list of attributes
 * @param attr Pointer to Attr(ibute) node
 * @return Pointer to appended node (or nullptr when failed)
 */
node::Attr * NamedNodeMap::appendAttribute( AttrPtr_t &&attr ) {
    node::Attr * node = nullptr;

    if( !attr ) {
        LOG_WARNING(
            "[parser::dom::NamedNodeMap::appendAttribute( nullptr )] "
            "Trying to add empty AttrPtr_t."
        );

        return nullptr; //EARLY RETURN
    }

    if( _map.contains( attr->nodeName() ) ) {
        auto   map_it       = _map.find( attr->nodeName() );
        auto   attr_it      = getIterator( map_it->second );
        auto * prev_sibling = (*attr_it)->previousSibling();
        auto * next_sibling = (*attr_it)->nextSibling();

        (*attr_it) = std::move( attr );
        (*attr_it)->setParent( _parent );
        (*attr_it)->setPrevSibling( prev_sibling );
        (*attr_it)->setNextSibling( next_sibling );

        if( _parent ) {
            (*attr_it)->setOwnerDocument( _parent->ownerDocument() );
        }

        node = (*attr_it).get();

    } else {
        auto * last     = ( _nodes.empty() ? nullptr : _nodes.back().get() );
        auto & inserted = _nodes.emplace_back( std::move( attr ) );

        node = dynamic_cast<node::Attr *>( inserted.get() );

        if( last ) {
            last->setNextSibling( node );
        }

        node->setParent( _parent );
        node->setPrevSibling( last );
        node->setNextSibling( nullptr );

        if( _parent ) {
            node->setOwnerDocument( _parent->ownerDocument() );
        }

        auto [ pair, success ] = _map.emplace( std::pair<DOMString_t, node::Attr *>( inserted->nodeName(), node ) );

        if( !success ) {
            LOG_ERROR(
                "[parser::dom::NamedNodeMap::appendAttribute( ", blogator::unicode::utf8::convert( inserted->nodeName() ), " )] "
                "Failed to insert mapping for attribute."
            );
        }
    }

    return node;
}

/**
 * Swaps NamedNodeMap
 * @param lhs NamedNodeMap
 * @param rhs NamedNodeMap
 */
void blogator::parser::dom::swap( NamedNodeMap &lhs, NamedNodeMap &rhs ) {
    lhs.swap( rhs );
}