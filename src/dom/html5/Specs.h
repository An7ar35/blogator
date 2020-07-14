#ifndef BLOGATOR_DOM_HTML5_SPECS_H
#define BLOGATOR_DOM_HTML5_SPECS_H

#include <unordered_map>
#include <string>

#include "Attribute.h"
#include "Tag.h"

namespace blogator::dom::html5 {
    class Specs {
      public:
        static bool isPaired( html5::Tag tag );
        static std::u32string tagToStr( html5::Tag tag );
        static html5::Tag strToTag( std::u32string str );

      private:
        struct TagSpecs {
            TagStructure   structure;
            std::u32string tag_str;
        };

        static const std::unordered_map<Tag, TagSpecs>       _tag2str_map;
        static const std::unordered_map<std::u32string, Tag> _str2tag_map;
    };
}

#endif //BLOGATOR_DOM_HTML5_SPECS_H
