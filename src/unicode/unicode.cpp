#include "unicode.h"

#include <vector>
#include <sstream>
#include <iomanip>

/**
 * Converts a unicode integer value into its hexadecimal representation
 * @param val Integer value to convert
 * @param prefix Hex code prefix (default="\\u")
 * @return Unicode hex string
 */
std::string blogator::unicode::toxunicode( uint32_t val, std::string prefix ) {
    std::stringstream ss;

    ss << prefix
       << std::setfill( '0' )
       << std::setw( sizeof( uint32_t ) )
       << std::hex
       << std::uppercase
       << val;

    return ss.str();
}