#ifndef BLOGATOR_ENCODING_SPECS_FORMAT_H
#define BLOGATOR_ENCODING_SPECS_FORMAT_H

#include <string>
#include <ostream>

namespace blogator::encoding::specs {
    /**
     * Encoding formats
     */
    enum class Format {
        UNKNOWN,
        UTF8,
        UTF16_BE,
        UTF16_LE,
        UTF32_BE,
        UTF32_LE,
    };

    std::ostream & operator <<( std::ostream &os, Format fmt );
}

namespace blogator {
    std::string to_string( blogator::encoding::specs::Format fmt );
    std::u32string to_u32string( blogator::encoding::specs::Format fmt );
}

#endif //BLOGATOR_ENCODING_SPECS_FORMAT_H
