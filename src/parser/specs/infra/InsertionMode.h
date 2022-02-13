#ifndef BLOGATOR_PARSER_SPECS_HTML5_INSERTIONMODE_H
#define BLOGATOR_PARSER_SPECS_HTML5_INSERTIONMODE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::infra {
    enum class InsertionMode {
        INITIAL = 0,
        BEFORE_HTML,
        BEFORE_HEAD,
        IN_HEAD,
        IN_HEAD_NOSCRIPT,
        AFTER_HEAD,
        IN_BODY,
        TEXT,
        IN_TABLE,
        IN_TABLE_TEXT,
        IN_CAPTION,
        IN_COLUMN_GROUP,
        IN_TABLE_BODY,
        IN_ROW,
        IN_CELL,
        IN_SELECT,
        IN_SELECT_IN_TABLE,
        IN_TEMPLATE,
        AFTER_BODY,
        IN_FRAMESET,
        AFTER_FRAMESET,
        AFTER_AFTER_BODY,
        AFTER_AFTER_FRAMESET,

        ENUM_END = AFTER_AFTER_FRAMESET, //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, InsertionMode mode );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::infra::InsertionMode mode );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_INSERTIONMODE_H
