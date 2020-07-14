#ifndef BLOGATOR_DOM_HTML5_RULES_H
#define BLOGATOR_DOM_HTML5_RULES_H

#include <string>
#include <vector>

#include "Tag.h"

namespace blogator::dom::html5 {
    namespace special_char {
        const char32_t QUOTATION_MARK { U'\"' }; //reserved
        const char32_t APOSTROPHE     { U'\'' }; //reserved
        const char32_t AMPERSAND      { U'&'  }; //reserved
        const char32_t LESS_THAN      { U'<'  }; //reserved
        const char32_t GREATER_THAN   { U'>'  }; //reserved
        const char32_t EQUALS_SIGN    { U'='  };
        const char32_t SOLIDUS        { U'/'  };
        const char32_t GRAVE_ACCENT   { U'`'  };
        const char32_t SPACE          { U' '  };
        const char32_t SEMICOLON      { U';'  };
    }

    //TODO attribute matrix

    namespace rules {
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
}


#endif //BLOGATOR_DOM_HTML5_RULES_H