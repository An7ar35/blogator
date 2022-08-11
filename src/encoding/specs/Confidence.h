#ifndef BLOGATOR_ENCODING_SPECS_CONFIDENCE_H
#define BLOGATOR_ENCODING_SPECS_CONFIDENCE_H

namespace blogator::encoding::specs {
    /**
     * Certainty of the encoding format
     */
    enum class Confidence {
        TENTATIVE = 0,
        CERTAIN,
        IRRELEVANT,
    };
}

#endif //BLOGATOR_ENCODING_SPECS_CONFIDENCE_H
