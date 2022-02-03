#include "NodeType.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param type NodeType enum
 * @return Output stream
 */
std::ostream & blogator::parser::dom::operator <<( std::ostream &os, blogator::parser::dom::NodeType type ) {
    switch( type ) {
        case NodeType::UNDEFINED:              { os << "undefined";             } break;
        case NodeType::ELEMENT_NODE:           { os << "Element";               } break;
        case NodeType::ATTRIBUTE_NODE:         { os << "Attr";                  } break;
        case NodeType::TEXT_NODE:              { os << "Text";                  } break;
        case NodeType::CDATA_SECTION_NODE:     { os << "CDATASection";          } break;
        case NodeType::COMMENT_NODE:           { os << "Comment";               } break;
        case NodeType::DOCUMENT_NODE:          { os << "Document";              } break;
        case NodeType::DOCUMENT_TYPE_NODE:     { os << "DocumentType";          } break;
        case NodeType::DOCUMENT_FRAGMENT_NODE: { os << "DocumentFragment";      } break;
        default:                               { os << "enum string not found"; } break;
    }

    return os;
}

/**
 * Converts a parser::dom::NodeType enum to a string representation
 * @param type NodeType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::dom::NodeType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}