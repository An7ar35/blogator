#ifndef BLOGATOR_UNICODE_H
#define BLOGATOR_UNICODE_H

#include "ascii.h"
#include "utf8.h"
#include "utf32.h"

namespace blogator::unicode {
    std::string normalize( const std::u32string & u32str );
    std::ostream & normalize( std::ostream & os, const std::u32string & u32str );
    std::ostream & normalize( std::ostream & os, const std::string & u8str );
}

#endif //BLOGATOR_UNICODE_H
