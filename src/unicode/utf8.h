#ifndef BLOGATOR_PARSER_UNICODE_UTF8_H
#define BLOGATOR_PARSER_UNICODE_UTF8_H

#include <iostream>

namespace blogator::unicode::utf8 {
    std::ostream & convert( std::ostream & stream, const std::u32string & str );
    std::string convert( const std::u32string & str );
}

#endif //BLOGATOR_PARSER_UNICODE_UTF8_H