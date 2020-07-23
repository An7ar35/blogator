#ifndef BLOGATOR_DOM_DTO_GLOBALMAPS_H
#define BLOGATOR_DOM_DTO_GLOBALMAPS_H

#include <map>
#include <vector>
#include <string>

#include "../dot/DOTNode.h"

namespace blogator::dom::dto {
    struct GlobalMaps {
        std::map<std::u32string, const dom::DOTNode *>               id_map;
        std::map <std::u32string, std::vector<const dom::DOTNode *>> class_map;
    };
}

#endif //BLOGATOR_DOM_DTO_GLOBALMAPS_H
