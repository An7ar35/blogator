#ifndef BLOGATOR_DOM_HTML5_PROPERTIES_RELATIONSHIPS_H
#define BLOGATOR_DOM_HTML5_PROPERTIES_RELATIONSHIPS_H

#include <vector>

namespace blogator::dom::html5::properties::relationships {
    /**
     * Data for context/relationship based rules
     */
    std::vector<std::vector<bool>> createAttr2TagRelationshipTable();
    std::vector<std::vector<bool>> createTag2ContentModelRelationshipTable();
//        static std::vector<std::vector<bool>> createTagContextRelationshipTable();
//        TODO something storing allowable missing tags rules
}


#endif //BLOGATOR_DOM_HTML5_PROPERTIES_RELATIONSHIPS_H
