#ifndef BLOGATOR_PARSER_ENCODING_FORMAT_H
#define BLOGATOR_PARSER_ENCODING_FORMAT_H

namespace blogator::parser::encoding {
    /**
     * Encoding formats
     */
    typedef enum class Format {
        UNKNOWN,
        UTF8,
        UTF16_BE,
        UTF16_LE,
        UTF32_BE,
        UTF32_LE,
    } Format_e;
}

#endif //BLOGATOR_PARSER_ENCODING_FORMAT_H
