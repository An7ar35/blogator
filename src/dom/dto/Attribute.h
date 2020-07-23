#ifndef BLOGATOR_DOM_DTO_ATTRIBUTE_H
#define BLOGATOR_DOM_DTO_ATTRIBUTE_H

#include <string>
#include "../html5/enums/AttrBoundaryChar.h"

namespace blogator::dom::dto {
    struct Attribute {
        std::u32string value;
        html5::AttrBoundaryChar boundary;

        bool operator ==( const Attribute &rhs ) const;
        bool operator !=( const Attribute &rhs ) const;
    };
}

#endif //BLOGATOR_DOM_DTO_ATTRIBUTE_H
