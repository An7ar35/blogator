#ifndef BLOGATOR_PARSER_ENCODING_FORMAT_H
#define BLOGATOR_PARSER_ENCODING_FORMAT_H

#include <string>
#include <ostream>

namespace blogator::parser::encoding {
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
    std::string to_string( blogator::parser::encoding::Format fmt );
    std::u32string to_u32string( blogator::parser::encoding::Format fmt );
}

#endif //BLOGATOR_PARSER_ENCODING_FORMAT_H
