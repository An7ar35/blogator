#ifndef BLOGATOR_DOM_HTML5_KB_KB_TAGS_H
#define BLOGATOR_DOM_HTML5_KB_KB_TAGS_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../enums/Tag.h"
#include "../enums/TagStructure.h"
#include "../enums/NameSpace.h"
#include "../../dto/TagSpecs.h"

namespace blogator::dom::html5::kb::tags {
    std::unordered_map <html5::Tag, dto::TagSpecs> createTag2StrMap();
    std::unordered_map <std::u32string, Tag>       createStr2TagMap( const std::unordered_map <Tag, dto::TagSpecs> &tag2str );
}

#endif //BLOGATOR_DOM_HTML5_KB_KB_TAGS_H