#include "XML.h"

#include "../exception/DOMException.h"
#include "../../../string/helpers.h"
#include "../../specs/infra/Namespace.h"

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

    auto v = blogator::string::split<char32_t>( qualified_name, ':', true );

    if( v.size() == 1 && dom::validation::XML::checkQName( v[ 0 ] ) ) {
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

    } else if( v.size() == 2 && dom::validation::XML::checkQName( v[ 0 ], v[ 1 ] )  ) {
        const auto & prefix     = v[ 0 ];
        const auto & local_name = v[ 1 ];

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

    return std::move( v );
}

/**
 * Checks if string is a valid 'Name'
 * @param name Name string
 * @return Validity
 */
bool XML::checkName( const blogator::parser::dom::DOMString_t &name ) {
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
inline bool XML::checkNCName( const blogator::parser::dom::DOMString_t &name ) {
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
 * [PRIVATE] Checks if character is a valid 'NameStartChar'
 * @param c Character
 * @return Validity
 */
inline bool XML::isNameStartChar( char32_t c ) {
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
 * [PRIVATE] Checks if character is a valid 'NameChar'
 * @param c Character
 * @return Validity
 */
inline bool XML::isNameChar( char32_t c ) {
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
 * [PRIVATE] Checks if character is a valid 'NameChar' minus the ':' char
 * @param c Character
 * @return Validity
 */
inline bool XML::isNCNameStartChar( char32_t c ) {
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
 * [PRIVATE] Checks if character is a valid 'NCNameChar'
 * @param c Character
 * @return Validity
 */
inline bool XML::isNCNameChar( char32_t c ) {
    return isNCNameStartChar( c )
        || c == 0x2D //'-'
        || c == 0x2E //'.'
        || ( 0x30 <= c && c <= 0x39 ) //[0-9]
        || c == 0xB7
        || ( 0x300  <= c && c <= 0x36F )
        || ( 0x203F <= c && c <= 0x2040 );
}

/**
 * [PRIVATE] Check if character is a valid 'Char'
 * @param c Character
 * @return Validity
 */
inline bool XML::isChar( char32_t c ) {
    //ref: https://www.w3.org/TR/REC-xml/#NT-Char
    return c == 0x09 //'\t'
        || c == 0x0A //'\n'
        || c == 0x0D //'\r'
        || ( c >= 0x20    && c <= 0xD7FF   )
        || ( c >= 0xE000  && c <= 0xFFFD   )
        || ( c >= 0x10000 && c <= 0x10FFFF );
}