#include "XML.h"

#include "../exception/DOMException.h"
#include "../../../string/helpers.h"
#include "../../specs/infra/Namespace.h"
#include "../../../unicode/ascii.h"

/**
 * [LOCAL] Checks if character is a valid 'NameStartChar'
 * @param c Character
 * @return Validity
 */
static inline bool isNameStartChar( char32_t c ) {
    //ref: https://www.w3.org/TR/xml/#NT-Name
    return c == 0x3A                  //':'
        || ( 0x41 <= c && c <= 0x5A ) //[A-Z]
        || c == 0x5D                  //'_'
        || ( 0x61 <= c && c <= 0x7A ) //[a-z]
        || ( 0xC0 <= c && c <= 0xD6 )
        || ( 0xD8 <= c && c <= 0xF6 )
        || ( c >= 0xF8    && c <= 0x2FF )
        || ( c >= 0x370   && c <= 0x37D )
        || ( c >= 0x37F   && c <= 0x1FFF )
        || ( c >= 0x200C  && c <= 0x200D )
        || ( c >= 0x2070  && c <= 0x218F )
        || ( c >= 0x2C00  && c <= 0x2FEF )
        || ( c >= 0x3001  && c <= 0xD7FF )
        || ( c >= 0xF900  && c <= 0xFDCF )
        || ( c >= 0xFDF0  && c <= 0xFFFD )
        || ( c >= 0x10000 && c <= 0xEFFFF );
}

/**
 * [LOCAL] Checks if character is a valid 'NameChar'
 * @param c Character
 * @return Validity
 */
static inline bool isNameChar( char32_t c ) {
    //ref: https://www.w3.org/TR/xml/#NT-Name
    return isNameStartChar( c )
        || c == 0x2D //'-'
        || c == 0x2E //'.'
        || ( 0x30 <= c && c <= 0x39 ) //[0-9]
        || c == 0xB7
        || ( 0x300  <= c && c <= 0x36F )
        || ( 0x203F <= c && c <= 0x2040 );
}

/**
 * [LOCAL] Checks if character is a valid 'NameChar' minus the ':' char
 * @param c Character
 * @return Validity
 */
static inline bool isNCNameStartChar( char32_t c ) {
    return ( 0x41 <= c && c <= 0x5A ) //[A-Z]
        || c == 0x5D                  //'_'
        || ( 0x61 <= c && c <= 0x7A ) //[a-z]
        || ( 0xC0 <= c && c <= 0xD6 )
        || ( 0xD8 <= c && c <= 0xF6 )
        || ( c >= 0xF8    && c <= 0x2FF )
        || ( c >= 0x370   && c <= 0x37D )
        || ( c >= 0x37F   && c <= 0x1FFF )
        || ( c >= 0x200C  && c <= 0x200D )
        || ( c >= 0x2070  && c <= 0x218F )
        || ( c >= 0x2C00  && c <= 0x2FEF )
        || ( c >= 0x3001  && c <= 0xD7FF )
        || ( c >= 0xF900  && c <= 0xFDCF )
        || ( c >= 0xFDF0  && c <= 0xFFFD )
        || ( c >= 0x10000 && c <= 0xEFFFF );
}

/**
 * [LOCAL] Checks if character is a valid 'NCNameChar'
 * @param c Character
 * @return Validity
 */
static inline bool isNCNameChar( char32_t c ) {
    return isNCNameStartChar( c )
        || c == 0x2D //'-'
        || c == 0x2E //'.'
        || ( 0x30 <= c && c <= 0x39 ) //[0-9]
        || c == 0xB7
        || ( 0x300  <= c && c <= 0x36F )
        || ( 0x203F <= c && c <= 0x2040 );
}

/**
 * [LOCAL] Check if character is a valid 'Char'
 * @param c Character
 * @return Validity
 */
static inline bool isChar( char32_t c ) {
    //ref: https://www.w3.org/TR/REC-xml/#NT-Char
    return c == 0x09 //'\t'
        || c == 0x0A //'\n'
        || c == 0x0D //'\r'
        || ( c >= 0x20    && c <= 0xD7FF   )
        || ( c >= 0xE000  && c <= 0xFFFD   )
        || ( c >= 0x10000 && c <= 0x10FFFF );
}

/**
 * [LOCAL] Checks if a character is a non-character
 * @return Non-character state
 */
static inline bool isNonChar( char32_t c ) {
    //ref: https://infra.spec.whatwg.org/#noncharacter
    return ( c >= 0xDFF0 && c <= 0xFDEF )
        || c == 0xFFFE
        || c == 0xFFFF
        || c == 0x1FFFE
        || c == 0x1FFFF
        || c == 0x2FFFE
        || c == 0x2FFFF
        || c == 0x3FFFE
        || c == 0x3FFFF
        || c == 0x4FFFE
        || c == 0x4FFFF
        || c == 0x5FFFE
        || c == 0x5FFFF
        || c == 0x6FFFE
        || c == 0x6FFFF
        || c == 0x7FFFE
        || c == 0x7FFFF
        || c == 0x8FFFE
        || c == 0x8FFFF
        || c == 0x9FFFE
        || c == 0x9FFFF
        || c == 0xAFFFE
        || c == 0xAFFFF
        || c == 0xBFFFE
        || c == 0xBFFFF
        || c == 0xCFFFE
        || c == 0xCFFFF
        || c == 0xDFFFE
        || c == 0xDFFFF
        || c == 0xEFFFE
        || c == 0xEFFFF
        || c == 0xFFFFE
        || c == 0xFFFFF
        || c == 0x10FFFE
        || c == 0x10FFFF;
}

/**
 * Checks if a character is a valid in an Attribute name
 * @param c Character
 * @return Validity
 */
static inline bool isAttrNameChar( char32_t c ) {
    //ref: https://html.spec.whatwg.org/#attributes-2
    return !( blogator::unicode::ascii::iscntrl( c ) ||
              c == 0x20 /* space */                  ||
              c == 0x22 /* "     */                  ||
              c == 0x27 /* '     */                  ||
              c == 0x3E /* >     */                  ||
              c == 0x2F /* /     */                  ||
              c == 0x3D /* =     */                  ||
              isNonChar( c ) );
}

using namespace blogator::parser::dom::validation;

/**
 * Extracts and validates a namespaced qualified name
 * @param ns Namespace
 * @param qualified_name Qualified name
 * @return Collection containing either a local name or a prefix and local name
 */
std::vector<blogator::parser::dom::DOMString_t> XML::validateNS( const dom::DOMString_t &ns, const dom::DOMString_t &qualified_name ) {
    //ref: https://dom.spec.whatwg.org/#validate-and-extract

    using blogator::parser::dom::exception::DOMException;
    using blogator::parser::dom::exception::DOMExceptionType;
    using blogator::parser::specs::infra::Namespace;

    auto v = blogator::string::split<char32_t>( qualified_name, ':', false );

    if( v.size() == 1 && dom::validation::XML::checkName( v[ 0 ] ) ) {
        const auto & local_name = v[ 0 ];

        if( local_name == U"xmlns" && ns != specs::infra::to_namespaceURI( Namespace::XMLNS ) ) {
            throw DOMException(
                DOMExceptionType::NamespaceError,
                "Incorrect namespace specified for 'xlmns'."
            );

        } else if( ns == specs::infra::to_namespaceURI( Namespace::XMLNS ) && local_name != U"xmlns" ) {
            throw DOMException(
                DOMExceptionType::NamespaceError,
                "Incorrect prefix/local name specified for 'xlmns'."
            );
        }

    } else if( v.size() == 2 && dom::validation::XML::checkName( qualified_name ) ) {
        const auto & prefix = v[ 0 ];

        if( ns.empty() ) {
            throw DOMException(
                DOMExceptionType::NamespaceError,
                "Prefix specified without a namespace."
            );

        } else if( prefix == U"xml" && ns != specs::infra::to_namespaceURI( Namespace::XML ) ) {
            throw DOMException(
                DOMExceptionType::NamespaceError,
                "Incorrect namespace specified for 'xml'."
            );

        } else if( ( qualified_name == U"xmlns" || prefix == U"xmlns" ) && ns != specs::infra::to_namespaceURI( Namespace::XMLNS ) ) {
            throw DOMException(
                DOMExceptionType::NamespaceError,
                "Incorrect namespace specified for 'xlmns'."
            );

        } else if( ns == specs::infra::to_namespaceURI( Namespace::XMLNS ) && prefix != U"xmlns" ) {
            throw DOMException(
                DOMExceptionType::NamespaceError,
                "Incorrect prefix/local name specified for 'xlmns'."
            );
        }

    } else {
        throw DOMException(
            DOMExceptionType::InvalidCharacterError,
            "Invalid qualified name."
        );
    }

    return v;
}

/**
 * Checks if string is a valid 'Name'
 * @param name Name string
 * @return Validity
 */
bool XML::checkName( const DOMString_t &name ) {
    //ref: https://www.w3.org/TR/xml/#NT-Name
    if( name.empty() ) {
        return true;
    }

    if( isNameStartChar( name[0] ) ) {
        for( size_t i = 1; i < name.length(); ++i ) {
            if( !isNameChar( name[i] ) ) {
                return false;
            }
        }

        return true;
    }

    return false;
}

/**
 * Checks if string is a valid 'QName'
 * @param name Local part of the qualified name
 * @return Validity
 */
bool XML::checkQName( const DOMString_t &name ) {
    //ref: https://www.w3.org/TR/xml-names/#NT-QName
    return checkNCName( name );
}

/**
 * Checks if strings make up a valid QName
 * @param prefix Prefix of the qualified name (characters before ':')
 * @param name Local part of the qualified name (characters after ':')
 * @return Validity
 */
bool XML::checkQName( const DOMString_t & prefix, const DOMString_t & name ) {
    //ref: https://www.w3.org/TR/xml-names/#NT-QName
    return checkNCName( prefix ) && checkNCName( name );
}

/**
 * Checks if string is a valid Non-Colonized Name
 * @param name NCName string
 * @return Validity
 */
bool XML::checkNCName( const blogator::parser::dom::DOMString_t &name ) {
    //ref: https://www.w3.org/TR/xml-names/#NT-NCName
    if( name.empty() ) {
        return true;
    }

    if( isNCNameStartChar( name[0] ) ) {
        for( size_t i = 1; i < name.length(); ++i ) {
            if( !isNCNameChar( name[i] ) ) {
                return false;
            }
        }

        return true;
    }

    return false;
}

/**
 * Checks if string is a valid attribute name
 * @param name Attribute name string
 * @returnValidity
 */
bool XML::checkAttrName( const blogator::parser::dom::DOMString_t &name ) {
    if( name.empty() ) {
        return false;
    }

    for( auto c : name ) {
        if( !isAttrNameChar( c ) ) {
            return false;
        }
    }

    return true;
}