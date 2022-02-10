#include "XML.h"

using namespace blogator::parser::dom::validation;

/**
 * Checks if string is a valid 'Name'
 * @param name Name string
 * @return Validity
 */
bool XML::isValidName( const blogator::parser::dom::DOMString_t &name ) { //TODO
    if( !name.empty() && isNameStartChar( name[0] ) ) {
        for( size_t i = 1; i < name.length(); ++i ) {
            if( !isNameChar( name[i] ) ) {
                return false;
            }
        }
    }

    return true;
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
