#ifndef BLOGATOR_DOM_HTML5_KB_KB_ATTRIBUTES_H
#define BLOGATOR_DOM_HTML5_KB_KB_ATTRIBUTES_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../enums/Attribute.h"

/**
 * Create data for attribute conversion
 */
namespace blogator::dom::html5::kb::attributes {
    std::vector<std::u32string>                          createAttributeStrTable();
    std::unordered_map<std::u32string, html5::Attribute> createStrAttributeMap( const std::vector<std::u32string> &attr2str );
}

#endif //BLOGATOR_DOM_HTML5_KB_KB_ATTRIBUTES_H
