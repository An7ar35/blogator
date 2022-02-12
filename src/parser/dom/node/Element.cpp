#include "Element.h"

#include "CDATASection.h"
#include "Comment.h"
#include "Document.h"
#include "Text.h"
#include "../../../unicode/unicode.h"
#include "../validation/XML.h"
#include "../../../string/helpers.h"
#include "../../../logger/Logger.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 * @param element
 */
Element::Element( specs::html5::Element element ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _namespace_id( Node::namespace_map.setNamespace( specs::html5::getNamespace( element ) ) ),
    _local_name( blogator::to_u32string( element ) ),
    _attributes( this )
{}

/**
 * Constructor
 * @param ns Element namespace
 * @param name Name string
 */
Element::Element( const DOMString_t &ns, DOMString_t name ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _namespace_id( Node::namespace_map.setNamespace( ns ) ),
    _local_name( std::move( name ) ),
    _attributes( this )
{}

/**
 * Constructor
 * @param ns_id Element NamespaceMap id
 * @param name Name string
 */
Element::Element( NamespaceMap::id_t ns_id, DOMString_t name ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _namespace_id( ns_id ),
    _local_name( std::move( name ) ),
    _attributes( this )
{}

/**
 * Constructor
 * @param ns Namespace
 * @param prefix Prefix namespace
 * @param name Local name
 */
Element::Element( const DOMString_t &ns, DOMString_t prefix, DOMString_t name ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _namespace_id( Node::namespace_map.setNamespace( ns ) ),
    _prefix( std::move( prefix ) ),
    _local_name( std::move( name ) ),
    _attributes( this )
{}

/**
 * Constructor
 * @param ns_id Element NamespaceMap id
 * @param prefix Prefix namespace
 * @param name Local name
 */
Element::Element( NamespaceMap::id_t ns_id, DOMString_t prefix, DOMString_t name ) :
    node::Node( NodeType::ELEMENT_NODE ),
    _namespace_id( ns_id ),
    _prefix( std::move( prefix ) ),
    _local_name( std::move( name ) ),
    _attributes( this )
{}

/**
 * Copy-constructor
 * @param node Node to copy
 */
Element::Element( const Element &node ) :
    node::Node( node ),
    _namespace_id( node._namespace_id ),
    _prefix( node._prefix ),
    _local_name( node._local_name ),
    _attributes( ( node._attributes ) )
{
    _attributes.setParent( this );
}

/**
 * Move-constructor
 * @param node Node to move
 */
Element::Element( Element &&node ) noexcept :
    node::Node( std::move( node ) ),
    _namespace_id( node._namespace_id ),
    _prefix( std::move( node._prefix ) ),
    _local_name( std::move( node._local_name ) ),
    _attributes( std::move( node._attributes ) )
{
    _attributes.setParent( this );
}

/**
 * Copy-assignment operator
 * @param node Node to copy
 * @return Copy
 */
Element & Element::operator =( const Element &node ) {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( node );
        this->_namespace_id = node._namespace_id;
        this->_prefix       = node._prefix;
        this->_local_name   = node._local_name;
        this->_attributes   = node._attributes;
        this->_attributes.setParent( this );
    }

    return *this;
}

/**
 * Move-assignment operator
 * @param node Node to move
 * @return Node
 */
Element & Element::operator =( Element &&node ) noexcept {
    if( &node != this ) {
        dynamic_cast<node::Node &>( *this ) = dynamic_cast<const node::Node &>( node );
        this->_namespace_id = node._namespace_id;
        this->_prefix       = std::move( node._prefix );
        this->_local_name   = std::move( node._local_name );
        this->_attributes   = std::move( node._attributes );
        this->_attributes.setParent( this );
    }

    return *this;
}

/**
 * Shallow swaps Element nodes inclusive of their attributes
 * @param rhs node::Element to swap with
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void Element::swap( Element &rhs ) {
    if( &rhs != this ) {
        dynamic_cast<Node &>( *this ).swap( dynamic_cast<Node &>( rhs ) ); //throws
        std::swap( this->_namespace_id, rhs._namespace_id );
        std::swap( this->_prefix, rhs._prefix );
        std::swap( this->_local_name, rhs._local_name );
        dom::swap( this->_attributes, rhs._attributes );
    }
}

/**
 * Gets the element's matching HTML type
 * @return Matching specs::html5::Element enum (UNKNOWN if string not in Element list)
 */
blogator::parser::specs::html5::Element Element::elementType() const {
    return specs::html5::getElementType( _local_name );
}

/**
 * Gets the element's qualified name
 * @return Qualified name
 */
blogator::parser::dom::DOMString_t Element::qualifiedName() const {
    if( _prefix.empty() ) {
        return _local_name;
    } else {
        return _prefix + U":" + _local_name;
    }
}

/**
 * Gets the namespace NamespaceMap ID
 * @return NamespaceMap ID for the Element's namespace
 */
blogator::parser::dom::NamespaceMap::id_t Element::namespaceID() const {
    return _namespace_id;
}

/**
 * Checks if Element is in the HTML namespace and also in a HTML Document
 * @return HTML doc+namespace state of element
 */
bool Element::isHtmlNative() const {
    return this->ownerDocument()
        && this->ownerDocument()->type() == node::Document::Type::HTML
        && node::Node::getNamespaceEnum( this->namespaceID() ) == specs::html5::Namespace::HTML5;
}

/**
 * Creates and appends an Attr(ibute) Node
 * @param name Name string (unchecked)
 * @return Pointer to newly created Attr(ibute) node
 */
blogator::parser::dom::node::Attr * Element::createAttribute( const DOMString_t & name ) {
    return _attributes.appendAttribute( std::make_unique<node::Attr>( DOMString_t(), name ) );
}

/**
 * Creates and appends an Attr(ibute) Node
 * @param prefix Prefix string (unchecked)
 * @param name Name string (unchecked)
 * @return Pointer to newly created Attr(ibute) node
 */
blogator::parser::dom::node::Attr * Element::createAttribute( const DOMString_t & prefix, const DOMString_t & name ) {
    return _attributes.appendAttribute( std::make_unique<node::Attr>( prefix, name ) );
}

/**
 * Creates and appends an Attr(ibute) Node
 * @param prefix Prefix string (unchecked)
 * @param name Name string (unchecked)
 * @param value Value string (unchecked)
 * @return Pointer to newly created Attr(ibute) node
 */
blogator::parser::dom::node::Attr * Element::createAttribute( const DOMString_t &prefix, const DOMString_t &name, const DOMString_t &value ) {
    return _attributes.appendAttribute( std::make_unique<node::Attr>( prefix, name, value ) );
}

/**
 * Creates and appends a child CDATASection node
 * @param str Content string
 * @return Pointer to created CDATASection node
 */
blogator::parser::dom::node::CDATASection * Element::createCDATASection( const DOMString_t & str ) {
    return dynamic_cast<node::CDATASection *>(
        this->appendChild(
            std::make_unique<node::CDATASection>( str, this, this->lastChild() )
        )
    );
}

/**
 * Creates and append a child Comment node
 * @param comment Comment string
 * @return Pointer to created Comment node
 */
blogator::parser::dom::node::Comment * Element::createComment( const DOMString_t & comment ) {
    return dynamic_cast<node::Comment *>(
        this->appendChild(
            std::make_unique<node::Comment>( comment, this, this->lastChild() )
        )
    );
}

/**
 * Creates and append a child Element node (auto-namespace)
 * @param type HTML5/SVG/MathML element (enum)
 * @return Pointer to created Element node
 */
blogator::parser::dom::node::Element * Element::createElement( specs::html5::Element type ) {
    const auto u8name = blogator::to_string( type );

    return dynamic_cast<node::Element *>(
        this->appendChild(
            std::make_unique<node::Element>(
                this->_namespace_id,
                DOMString_t( u8name.cbegin(), u8name.cend() )
            )
        )
    );
}

/**
 * Creates and append a child Element node
 * @param ns Element namespace
 * @param name Element name string
 * @return Pointer to created Element node
 */
Element * Element::createElement( DOMString_t ns, DOMString_t name ) {
    return dynamic_cast<node::Element *>(
        this->appendChild(
            std::make_unique<node::Element>(
                std::move( ns ),
                std::move( name )
            )
        )
    );
}

/**
 * Creates and append a child Text node
 * @param txt Text string
 * @return Pointer to created Text node
 */
blogator::parser::dom::node::Text * Element::createText( const DOMString_t & txt ) {
    return dynamic_cast<Text *>(
        this->appendChild(
            std::make_unique<Text>( txt, this, this->lastChild() )
        )
    );
}

/**
 * Gets the namespace URI
 * @return Namespace URI
 * @throws blogator::exception::failed_expectation when namespace ID doesn't exist in global NamespaceMap
 */
blogator::parser::dom::DOMString_t Element::namespaceURI() const {
    try {
        return Node::namespace_map.getNamespaceURI( _namespace_id );

    } catch( const std::out_of_range &e ) {
        using blogator::unicode::utf8::convert;

        throw blogator::exception::failed_expectation(
            "[parser::dom::node::Element::namespaceURI()] "
            "Failed to get URI for node '" + convert( this->nodeName() ) + "'."
        );
    }
}

/**
 * Gets the element's prefix namespace
 * @return Prefix namespace
 */
blogator::parser::dom::DOMString_t Element::prefix() const {
    return _prefix;
}

/**
 * Gets the element's local name
 * @return Local name
 */
blogator::parser::dom::DOMString_t Element::localName() const {
    return _local_name;
}

/**
 * Gets the element's tag name
 * @return Tag name
 */
blogator::parser::dom::DOMString_t Element::tagName() const {
    return unicode::ascii::toupper( qualifiedName() );
}

/**
 * Gets the element's ID value
 * @return ID string (empty if none/not set)
 */
blogator::parser::dom::DOMString_t Element::id() const {
    const auto * attr = _attributes.getNamedItem( U"id" );
    return ( attr && attr->value() ? *attr->value() : DOMString_t() );
}

/**
 * Gets the element's class name
 * @return Class name(s) string
 */
blogator::parser::dom::DOMString_t Element::className() const {
    const auto * attr = _attributes.getNamedItem( U"class" );
    return ( attr && attr->value() ? *attr->value() : DOMString_t() );
}

/**
 * Gets the element's classes
 * @return List of class values
 */
blogator::parser::dom::DOMTokenList_t Element::classList() const {
    const auto * attr = _attributes.getNamedItem( U"class" );

    if( attr && attr->value() ) {
        auto list = DOMTokenList_t();
        auto v    = string::split<char32_t>( *attr->value(), U' ' );

        std::copy_if(v.begin(),
                     v.end(),
                     std::back_inserter(list),
                     []( const auto & val ){ return !val.empty(); }
        );

        return std::move( list );
    }

    return {};
}

/**
 * Gets the element's slot value
 * @return Slot value
 */
blogator::parser::dom::DOMString_t Element::slot() const {
    const auto * attr = _attributes.getNamedItem( U"slot" );
    return ( attr && attr->value() ? *attr->value() : DOMString_t() );
}

/**
 * Checks Element had attribute(s)
 * @return Has attributes
 */
bool Element::hasAttributes() const {
    return !_attributes.empty();
}

/**
 * Gets the element's attributes
 * @return Attributes
 */
blogator::parser::dom::NamedNodeMap & Element::attributes() {
    return _attributes;
}

/**
 * Gets the element's attributes
 * @return Attributes
 */
const blogator::parser::dom::NamedNodeMap & Element::attributes() const {
    return _attributes;
}

/**
 * Gets a list of names for this Element's attributes
 * @return Element's attribute names
 */
blogator::parser::dom::Sequence_t<blogator::parser::dom::DOMString_t> Element::getAttributeNames() {
    auto sequence = Sequence_t<DOMString_t>();

    for( const auto & attr : _attributes.list() ) {
        if( attr ) {
            sequence.emplace_back( attr->nodeName() );
        }
    }

    return std::move( sequence );
}

/**
 * Gets an attribute's value
 * @param qualified_name Qualified name
 * @return Pointer to value string or nullptr if attribute not found or has no value
 */
const blogator::parser::dom::DOMString_t * Element::getAttribute( const DOMString_t &qualified_name ) const {
    auto * attr = this->_attributes.getNamedItem( qualified_name );
    return ( attr ? attr->value() : nullptr );
}

/**
 * Sets an attribute
 * @param qualified_name Qualified name
 * @return Success in adding/updating attribute
 * @throws DOMException when qualified name is invalid
 */
bool Element::setAttribute( const DOMString_t &qualified_name ) {
    if( dom::validation::XML::isValidName( qualified_name ) ) {
        auto v = blogator::string::split<char32_t>( qualified_name, ':' );

        if( v.size() == 1 ) {
            return ( this->_attributes.setNamedItem( Attr( DOMString_t(), v[0] ) ) != nullptr ); //EARLY RETURN

        } else if( v.size() == 2 ) {
            return ( this->_attributes.setNamedItem( Attr( v[0], v[1] ) ) != nullptr ); //EARLY RETURN
        }
    }

    using blogator::unicode::utf8::convert;

    throw exception::DOMException(
        exception::DOMExceptionType::InvalidCharacterError,
        "[parser::dom::node::Element::setAttribute( \"" + convert( qualified_name ) + "\" )] Invalid qualified name."
    );
}

/**
 * Sets a attribute
 * @param qualified_name Qualified name
 * @param value Value
 * @return Success in adding/updating attribute
 * @throws DOMException when qualified name is invalid
 */
bool Element::setAttribute( const DOMString_t &qualified_name, const DOMString_t &value ) {
    if( dom::validation::XML::isValidName( qualified_name ) ) {
        auto v = blogator::string::split<char32_t>( qualified_name, ':' );

        if( v.size() == 1 ) {
            return ( this->_attributes.setNamedItem( Attr( DOMString_t(), v[0], value ) ) != nullptr ); //EARLY RETURN

        } else if( v.size() == 2 ) {
            return ( this->_attributes.setNamedItem( Attr( v[0], v[1], value ) ) != nullptr ); //EARLY RETURN
        }
    }

    using blogator::unicode::utf8::convert;

    throw exception::DOMException(
        exception::DOMExceptionType::InvalidCharacterError,
        "[parser::dom::node::Element::setAttribute( \"" + convert( qualified_name ) + "\", \"" + convert( value ) + "\" )] Invalid qualified name."
    );
}

/**
 * Removes an attribute
 * @param qualified_name Qualified name
 * @return Success
 */
bool Element::removeAttribute( const DOMString_t & qualified_name ) {
    return ( this->_attributes.removeNamedItem( qualified_name ) ).operator bool();
}

/**
 * Toggles attribute
 * @param qualified_name Qualified name of attribute
 * @return Current state of qualified name (existence)
 * @throws DOMException when qualified name is invalid
 */
bool Element::toggleAttribute( DOMString_t qualified_name ) {
    if( this->isHtmlNative() ) {
        unicode::ascii::tolower( qualified_name );
    }

    try {
        if( this->_attributes.attributeExists( qualified_name ) ) {
            return !( this->_attributes.removeNamedItem( qualified_name ).operator bool() );
        } else {
            return this->setAttribute( qualified_name );
        }

    } catch( const exception::DOMException &e ) {
        throw exception::DOMException(
            exception::DOMExceptionType::InvalidCharacterError,
            "[parser::dom::node::Element::toggleAttribute( \"" + blogator::unicode::utf8::convert( qualified_name ) + "\" )] Invalid qualified name."
        );
    }
}

/**
 * Toggles attribute
 * @param qualified_name Qualified name of attribute
 * @param force Force flag (if attribute exists; true: adds it, false: removes it)
 * @return Current state of qualified name (existence)
 */
bool Element::toggleAttribute( DOMString_t qualified_name, bool force ) {
    if( this->isHtmlNative() ) {
        unicode::ascii::tolower( qualified_name );
    }

    try {
        auto *attr = this->_attributes.getNamedItem( qualified_name );

        if( attr ) {
            if( force ) {
                return true;
            } else {
                return !( this->_attributes.removeNamedItem( qualified_name ) ).operator bool();
            }
        } else { //!attr
            if( force ) {
                return this->setAttribute( qualified_name );
            } else {
                return false;
            }
        }

    } catch( const exception::DOMException &e ) {
        throw exception::DOMException(
            exception::DOMExceptionType::InvalidCharacterError,
            "[parser::dom::node::Element::toggleAttribute( \"" + blogator::unicode::utf8::convert( qualified_name ) + "\", force=" + ( force ? "true" : "false" ) + " )] ""Invalid qualified name."
        );
    }
}

/**
 * Checks attribute exists
 * @param qualified_name Qualified name
 * @return Existence state
 */
bool Element::hasAttribute( const DOMString_t &qualified_name ) const {
    return this->_attributes.attributeExists( qualified_name );
}

/**
 * Gets an attribute
 * @param qualified_name Qualified name
 * @return Pointer to attribute node
 */
const Attr * Element::getAttributeNode( const DOMString_t &qualified_name ) {
    return this->_attributes.getNamedItem( qualified_name );
}

/**
 * Sets/updates an attribute
 * @param attr Attribute
 * @return Pointer to added/updated attribute node
 */
const Attr * Element::setAttributeNode( const Attr &attr ) {
    return this->_attributes.setNamedItem( attr );
}

/**
 * Removes an attribute node
 * @param node Pointer to attribute
 * @return Removed attribute
 */
blogator::parser::dom::NodePtr_t Element::removeAttributeNode( const Attr * node ) {
    return this->_attributes.removeNode( node );
}

/**
 * Get Elements by their tag names
 * @param qualified_name Qualified name
 * @return Collection of pointers to elements whose tag names match the qualified name specified
 */
blogator::parser::dom::HTMLCollection_t Element::getElementsByTagName( DOMString_t qualified_name ) {
    //ref: https://dom.spec.whatwg.org/#concept-getelementsbytagname

    auto list = HTMLCollection_t();

    if( qualified_name == U"*" /* U+002A */ ) {
        auto it = std::next( this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT ) ) );

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }

    } else if( this->isHtmlNative() ) {
        const auto ns_id = this->namespaceID();

        const auto filter_fn = [&qualified_name, &ns_id]( const node::Node &node ) {
            const auto * element = dynamic_cast<const Element *>( &node );

            if( element->namespaceID() == ns_id ) {
                blogator::unicode::ascii::tolower( qualified_name );
            }

            return ( element->qualifiedName() == qualified_name ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
        };

        auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

        if( it.node() == this ) {
            ++it;
        }

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }

    } else {
        const auto filter_fn = [&qualified_name]( const node::Node &node ) {
            const auto * element = dynamic_cast<const Element *>( &node );
            return ( element->qualifiedName() == qualified_name ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
        };

        auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

        if( it.node() == this ) {
            ++it;
        }

        while( it != this->end() ) {
            list.emplace_back( dynamic_cast<Element *>( it.node() ) );
            ++it;
        }
    }

    return std::move( list );
}

/**
 * Get Elements by their class names
 * @param class_names Set of class names
 * @return Collection of pointers to elements whose class match the class name(s) specified
 */
blogator::parser::dom::HTMLCollection_t Element::getElementsByClassName( std::set<DOMString_t> class_names ) {
    //ref: https://dom.spec.whatwg.org/#concept-getelementsbyclassname

    if( class_names.empty() ) {
        return {}; //EARLY RETURN
    }

    std::set<DOMString_t>                      class_names_lowercase;
    HTMLCollection_t                           list;
    std::function<bool( const DOMString_t & )> compare_fn;

    if( _document && _document->quirksMode() != QuirksMode::NO_QUIRKS ) { //i.e.: 'quirks' mode
        compare_fn = [&class_names_lowercase]( auto name ) {
            return class_names_lowercase.contains( blogator::unicode::ascii::tolower( name ) );
        };

        for( auto & name : class_names ) {
            class_names_lowercase.emplace( blogator::unicode::ascii::tolower( name ) );
        }

    } else {
        compare_fn = [&class_names]( const auto & name ) {
            return class_names.contains( name );
        };
    }

    const auto filter_fn = [&compare_fn]( const node::Node &node ) {
        const auto * element    = dynamic_cast<const Element *>( &node );
        const auto   class_list = element->classList();
        const auto   it         = std::find_if( class_list.cbegin(), class_list.cend(), compare_fn );

        return ( it != class_list.cend() ? NodeFilter::Result::FILTER_ACCEPT : NodeFilter::Result::FILTER_SKIP );
    };

    auto it = this->begin( std::make_shared<NodeFilter>( NodeFilter::SHOW_ELEMENT, filter_fn ) );

    while( it != this->end() ) {
        list.emplace_back( dynamic_cast<Element *>( it.node() ) );
        ++it;
    }

    return std::move( list );
}

/**
 * [OVERRIDE] Gets the text content
 * @return Text content string
 */
blogator::parser::dom::DOMString_t Element::textContent() const {
    auto concatenation = DOMString_t();

    for( const auto &node : _children ) {
        if( node->nodeType() == NodeType::TEXT_NODE ) {
            if( node->nodeValue() ) {
                concatenation.append( *node->nodeValue() );
            }

        } else if( node->nodeType() == NodeType::ELEMENT_NODE ) {
            concatenation.append( node->textContent() );
        }
    }

    return std::move( concatenation );
}

/**
 * Gets the node's name
 * @return HTML-upper-cased qualified name string
 */
blogator::parser::dom::DOMString_t Element::nodeName() const {
    using specs::html5::Namespace;

    const node::Document * doc = const_cast<Element *>( this )->ownerDocument();
    const auto             el  = this->elementType();

    if( doc && doc->type() == Document::Type::HTML && specs::html5::getNamespace( el ) == Namespace::HTML5 ) {
        return unicode::ascii::toupper( _local_name );

    } else {
        return qualifiedName();
    }
}

/**
 * [OVERRIDE] Clones the node
 * @param deep Deep copy flag
 * @return Pointer to clone
 */
blogator::parser::dom::NodePtr_t Element::cloneNode( bool deep ) const {
    auto clone = std::make_unique<Element>( _namespace_id, _prefix, _local_name );

    clone->_attributes = this->_attributes;
    clone->_attributes.setParent( clone.get() );

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
bool Element::isEqualNode( const Node &other ) const {
    if( this->nodeType()          == other.nodeType() &&
        this->childNodes().size() == other.childNodes().size() )
    {
        const auto * rhs = dynamic_cast<const Element *>( &other );

        bool equal = this->namespaceURI()        == rhs->namespaceURI()
                  && this->nodeName()            == rhs->nodeName()
                  && this->attributes().length() == rhs->attributes().length();

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
 * [OVERRIDE] Insert node before a child
 * @param node Node to insert
 * @param child Pointer to child (nullptr if append at end)
 * @return Pointer to inserted child
 * @throws DOMException when insertion breaks DOM tree validity
 */
Node * Element::insertNodeBefore( NodePtr_t node, Node * child ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( node->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType must be a Document child." );

    } else if( node->nodeType() == NodeType::ATTRIBUTE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "Attr must be added using the dedicated API." );

    } else {
        return Node::insertNodeBefore( std::move( node ), child ); //EARLY RETURN
    }
}

/**
 * [OVERRIDE] Replace a child node
 * @param node Node to replace with
 * @param target Target child node to replace
 * @return Replaced child node
 * @throw DOMException when replacement breaks DOM tree validity
 */
blogator::parser::dom::NodePtr_t Element::replaceChildNode( NodePtr_t &node, NodePtr_t &target ) {
    using exception::DOMException;
    using exception::DOMExceptionType;

    if( node->nodeType() == NodeType::DOCUMENT_TYPE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "DocumentType must be a Document child." );

    } else if( node->nodeType() == NodeType::ATTRIBUTE_NODE ) {
        throw DOMException( DOMExceptionType::HierarchyRequestError, "Attr must be added using the dedicated API." );

    } else {
        return Node::replaceChildNode( node, target );
    }
}

/**
 * Shallow swaps Element nodes
 * @param lhs Element
 * @param rhs Element
 * @throws parser::dom::exception::DOMException when nodes being swapped are not the same type
 */
void blogator::parser::dom::node::swap( Element &lhs, Element &rhs ) {
    lhs.swap( rhs );
}