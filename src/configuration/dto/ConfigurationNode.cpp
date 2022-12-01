#include "ConfigurationNode.h"

#include "../../unicode/utf8.h"

using namespace blogator::configuration;

/**
 * Constructor
 * @param suffix Node suffix string
 */
ConfigurationNode::ConfigurationNode( std::u32string suffix ) :
    suffix( std::move( suffix ) )
{}

/**
 * Move-constructor
 * @param node Node to move
 */
ConfigurationNode::ConfigurationNode( ConfigurationNode &&node ) noexcept :
    children( std::move( node.children ) ),
    suffix( std::move( node.suffix ) ),
    is_key( node.is_key ),
    value( std::move( node.value ) )
{}

/**
 * Copy-constructor
 * @param node Node to copy
 */
ConfigurationNode::ConfigurationNode( const ConfigurationNode & node ) :
    suffix( node.suffix ),
    is_key( node.is_key ),
    value( node.value )
{
    for( const auto & child : node.children ) {
        this->children.emplace( child.first, child.second.copy() );
    }
}

/**
 * Move-operator
 * @param node Node to move
 * @return Moved node
 */
ConfigurationNode & ConfigurationNode::operator =( ConfigurationNode &&node ) noexcept {
    suffix   = std::move( node.suffix );
    is_key   = node.is_key;
    value    = std::move( node.value );
    children = std::move( node.children );
    return *this;
}

/**
 * Copy-operator
 * @param node Node to copy
 * @return Copied node
 */
ConfigurationNode & ConfigurationNode::operator =( const ConfigurationNode & node ) {
    suffix = node.suffix;
    is_key = node.is_key;
    value  = node.value;

    for( const auto & child : node.children ) {
        this->children.emplace( child.first, child.second.copy() );
    }

    return *this;
}

/**
 * Equality operator
 * @param rhs ConfigurationNode to compare with
 * @return Equality state of subtree
 */
bool ConfigurationNode::operator ==( const ConfigurationNode &rhs ) const {
    return this->suffix   == rhs.suffix
        && this->is_key   == rhs.is_key
        && this->value    == rhs.value
        && this->children == rhs.children;
}

/**
 * Inequality operator
 * @param rhs ConfigurationNode to compare with
 * @return Inequality state of subtree
 */
bool ConfigurationNode::operator !=( const ConfigurationNode &rhs ) const {
    return !( *this == rhs );
}

/**
 * Clears the suffix for the node
 */
void ConfigurationNode::clearSuffix() {
    suffix.clear();
}

/**
 * Gets the node's value
 * @return Reference to the value
 */
Value & ConfigurationNode::getValue() {
    return value;
}

/**
 * Sets the node's value
 * @param val Value
 * @return Reference to the value in the node
 */
Value & ConfigurationNode::setValue( Value &&val ) {
    if( !is_key ) {
        is_key = true;
    }

    return ( value = std::move( val ) );
}

/**
 * Appends a value to the node's value (convert into array if already has an atomic value)
 * @param val Value
 * @return Reference to the value added to the node
 */
Value & ConfigurationNode::appendValue( Value &&val ) { //TODO unit test
    if( value.type() == ValueType::ARRAY ) {
        auto & list = value.getArray();
        return list.emplace_back( std::move( val ) );

    } else if( isKey() && value.type() != ValueType::VOID ) { //i.e.: already has a value
        auto old_val = std::move( value );
        value = Value( Value::IS_ARRAY_VALUE );
        auto & list = value.getArray();
        list.emplace_back( std::move( old_val ) );
        return list.emplace_back( std::move( val ) );

    } else {
        return setValue( std::move( val ) );
    }
}


/**
 * Copy node recursively
 * @return Copy of node
 */
ConfigurationNode ConfigurationNode::copy() const {
    return { *this };
}

/**
 * Checks if node is a leaf (i.e.: a key)
 * @return Leaf state
 */
bool ConfigurationNode::isLeaf() const {
    return children.empty();
}

/**
 * Checks the key flag for the node
 * @return Key-value state
 */
bool ConfigurationNode::isKey() const {
    return is_key;
}

/**
 * Gets the number of keys in the subtree
 * @return Number of keys with values in subtree
 */
size_t ConfigurationNode::size() const {
    size_t size = ( isKey() ? 1 : 0 );

    for( const auto & [k, v] : children ) {
        size += v.size();
    }

    return size;
}

/**
 * Output stream operator
 * @param os Output stream
 * @param node configuration:ConfigurationNode to print out
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const ConfigurationNode &node ) {
    if( node.isKey() ) {
        os << blogator::unicode::utf8::convert( node.suffix ) << " = " << node.value;
    } else {
        os << blogator::unicode::utf8::convert( node.suffix );
    }

    return os;
}