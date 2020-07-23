#ifndef BLOGATOR_DOM_HTML5_SPECIAL_CHAR_H
#define BLOGATOR_DOM_HTML5_SPECIAL_CHAR_H

namespace blogator::dom::html5::special_char {
    const char32_t QUOTATION_MARK   { U'\"' }; //reserved
    const char32_t APOSTROPHE       { U'\'' }; //reserved
    const char32_t AMPERSAND        { U'&'  }; //reserved
    const char32_t LESS_THAN        { U'<'  }; //reserved
    const char32_t GREATER_THAN     { U'>'  }; //reserved
    const char32_t EQUALS_SIGN      { U'='  };
    const char32_t SOLIDUS          { U'/'  };
    const char32_t GRAVE_ACCENT     { U'`'  };
    const char32_t SPACE            { U' '  };
    const char32_t SEMICOLON        { U';'  };
    const char32_t EXCLAMATION_MARK { U'!'  };
    const char32_t HYPHEN_MINUS     { U'-'  };
}

#endif //BLOGATOR_DOM_HTML5_SPECIAL_CHAR_H
