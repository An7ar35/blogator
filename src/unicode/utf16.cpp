#include "utf16.h"

constexpr uint32_t UTF16_SURROGATE_OFFSET = 0x10000 - ( 0x0D800 << 10 ) - 0xDC00;

/**
 * Joins 2 bytes into a UTF16 code unit
 * @param byte1 First byte (most significant bits `XXXX XXXX 0000 0000`)
 * @param byte2 Second byte (least significant bits `0000 0000 XXXX XXXX`)
 * @return U16 code unit
 */
uint16_t blogator::unicode::utf16::join( uint8_t byte1, uint8_t byte2 ) noexcept {
    return (uint16_t) ( byte1 << 8 ) + byte2;
}

/**
 * Checks if a high surrogate code unit requires a low surrogate to complete the code point
 * @param hi_surrogate High surrogate
 * @return number of code-units for the UTF-16 sequence (0 when in low-surrogate range)
 */
size_t blogator::unicode::utf16::codeunits( uint16_t hi_surrogate ) noexcept {
    if( utf16::ishighsurrogate( hi_surrogate ) ) {
        return 2; //in high surrogate range
    } else if( utf16::islowsurrogate( hi_surrogate ) ) {
        return 0; //in low surrogate range
    } else {
        return 1;
    }
}

/**
 * Checks if code unit is within the range of a low surrogate
 * @param c UTF-16 code unit
 * @return Is valid low surrogate
 */
bool blogator::unicode::utf16::islowsurrogate( uint16_t cu ) noexcept {
    return ( cu >= 0xDC00 && cu <= 0xDFFF );
}

/**
 * Checks if code unit is within the range of a high surrogate from a pair
 * @param c UTF-16 code unit
 * @return Is valid high surrogate in pair
 */
bool blogator::unicode::utf16::ishighsurrogate( uint16_t cu ) noexcept {
    return ( cu >= 0xD800 && cu <= 0xDBFF );
}

/**
 * Checks if code unit is scalar (i.e. not a low surrogate)
 * @param c UTF-16 code unit
 * @return Is scalar
 */
bool blogator::unicode::utf16::isscalar( uint16_t cu ) noexcept {
    return !islowsurrogate( cu );
}


/**
 * [UNCHECKED] Converts a UTF-16 sequence int a UTF-32 code point
 * @param hi_surrogate High surrogate
 * @return UTF-32 code point
 */
uint32_t blogator::unicode::utf16::toU32( uint16_t hi_surrogate ) noexcept {
    return (uint32_t) hi_surrogate;
}

/**
 * [UNCHECKED] Converts a UTF-16 sequence int a UTF-32 code point
 * @param hi_surrogate High surrogate
 * @param lo_surrogate Low surrogate
 * @return UTF-32 code point
 */
uint32_t blogator::unicode::utf16::toU32( uint16_t hi_surrogate, uint16_t lo_surrogate ) noexcept {
    return (uint32_t) ( hi_surrogate << 10 ) + lo_surrogate + UTF16_SURROGATE_OFFSET;
}