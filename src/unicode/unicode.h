#ifndef BLOGATOR_UNICODE_H
#define BLOGATOR_UNICODE_H

#include "ascii.h"
#include "utf8.h"
#include "utf32.h"

namespace blogator::unicode {
    std::string toxunicode( uint32_t val, std::string prefix = "\\u" );
}

#endif //BLOGATOR_UNICODE_H
