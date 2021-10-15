#ifndef BLOGATOR_PARSER_UNICODE_UTF8_H
#define BLOGATOR_PARSER_UNICODE_UTF8_H

#include <iostream>

namespace blogator::unicode::utf8 {
    size_t bytes( uint8_t first );
    std::string toxunicode( uint32_t val, const std::string & prefix = "\\u" );
    std::ostream & convert( std::ostream & stream, const std::u32string & str );
    std::ostream & convert( std::ostream & stream, char32_t c );
    std::string convert( const std::u32string & str );
}

#endif //BLOGATOR_PARSER_UNICODE_UTF8_H