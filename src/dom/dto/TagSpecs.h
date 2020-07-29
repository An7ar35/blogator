#ifndef BLOGATOR_DOM_DTO_TAGSPECS_H
#define BLOGATOR_DOM_DTO_TAGSPECS_H

#include <string>

#include "../html5/enums/TagStructure.h"
#include "../html5/enums/NameSpace.h"

namespace blogator::dom::dto {
    struct TagSpecs {
        html5::TagStructure           structure;
        std::vector<html5::NameSpace> ns;
        std::u32string                tag_str;
    };
}

#endif //BLOGATOR_DOM_DTO_TAGSPECS_H
