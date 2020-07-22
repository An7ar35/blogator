#ifndef BLOGATOR_DOM_HTML5_VALIDATOR_H
#define BLOGATOR_DOM_HTML5_VALIDATOR_H

#include <string>
#include <vector>

#include "../Html5Properties.h"

namespace blogator::dom::html5::validator {
    enum class AttrBoundaryChar { NONE, APOSTROPHE, QUOTATION_MARK };

    /* Helper methods */
    bool isTagEnd( char32_t c );
    bool isValidAttributeNameChar( char32_t c, bool &err_flag );
    bool isValidAttributeValueChar( char32_t c, bool &err_flag );
    bool isValidAttributeValueChar( char32_t c, char32_t boundary_char, bool &err_flag );
    bool isValidCharReference( const std::u32string &str );
    std::u32string escapeReservedChar( char32_t c );

    /* Validation methods */
    html5::Tag validateOpeningTag( const std::u32string &str, std::vector<std::pair<std::u32string, std::u32string>> &attributes );
    void       validateAttributes( const std::u32string &str, std::vector<std::pair<std::u32string, std::u32string>> &attributes );
    html5::Tag validateClosingTag( const std::u32string &str );
}

#endif //BLOGATOR_DOM_HTML5_VALIDATOR_H