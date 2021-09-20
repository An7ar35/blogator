#ifndef BLOGATOR_PARSER_SPECS_BLOGATOR_ELEMENTS_H
#define BLOGATOR_PARSER_SPECS_BLOGATOR_ELEMENTS_H

namespace blogator::parser::specs::blogator {
    enum class Element {
        UNKNOWN = 0,
        TAG_OPEN,
        TAG_CLOSE,

        //=== Blogator elements ===
        PAGE_REF,       //BLOGATOR::PAGE_REF
        PAGE_REF_SELF,  //BLOGATOR::PAGE_REF_SELF
        AUTO_TOC,       //BLOGATOR::AUTO_TOC

        ENUM_END //used for iterating enums
    };
}

#endif //BLOGATOR_PARSER_SPECS_BLOGATOR_ELEMENTS_H
