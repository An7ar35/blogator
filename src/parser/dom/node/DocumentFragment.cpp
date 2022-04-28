#include "DocumentFragment.h"

using namespace blogator::parser::dom::node;

/**
 * Constructor
 */
DocumentFragment::DocumentFragment() :
    node::Node( NodeType::DOCUMENT_FRAGMENT_NODE )
{}

/**
 * Gets the associated node
 * @return Element in different tree or nullptr
 */
Node * DocumentFragment::host() {
    return this->parentNode();
}

/**
 * [OVERRIDE] Lookup a namespace's prefix
 * @param prefix Prefix string (empty == null)
 * @return Namespace URI
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t DocumentFragment::lookupPrefix( const blogator::parser::dom::DOMString_t &ns ) const {
    return {};
}

/**
 * [OVERRIDE] Find the namespace URI of a given prefix
 * @param prefix Prefix string (or "null")
 * @return Namespace URI (or "null")
 * @throws blogator::exception::failed_expectation when NamespaceMap lookup with this node's ID failed
 */
blogator::parser::dom::DOMString_t DocumentFragment::lookupNamespaceURI( const DOMString_t &prefix ) const {
    return {};
}

/**
 * [OVERRIDE] Outputs the node as UTF-8 formatted html into a stream
 * @param os Output stream
 */
void DocumentFragment::toUTF8Stream( std::ostream &os ) const {
    for( const auto & child : this->childNodes() ) {
        os << *child;
    }
}