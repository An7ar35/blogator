#ifndef BLOGATOR_PARSER_ENCODING_CONFIDENCE_H
#define BLOGATOR_PARSER_ENCODING_CONFIDENCE_H

namespace blogator::parser::encoding {
    /**
     * Certainty of the encoding format
     */
    typedef enum class Confidence {
        TENTATIVE = 0,
        CERTAIN,
        IRRELEVANT,

    } Confidence_e;
}

#endif //BLOGATOR_PARSER_ENCODING_CONFIDENCE_H
