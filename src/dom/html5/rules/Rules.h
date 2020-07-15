#ifndef BLOGATOR_DOM_HTML5_RULES_H
#define BLOGATOR_DOM_HTML5_RULES_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../enums/Tag.h"
#include "../enums/Attribute.h"

namespace blogator::dom::html5 {
    namespace special_char {
        const char32_t QUOTATION_MARK { U'\"' }; //reserved
        const char32_t APOSTROPHE     { U'\'' }; //reserved
        const char32_t AMPERSAND      { U'&'  }; //reserved
        const char32_t LESS_THAN      { U'<'  }; //reserved
        const char32_t GREATER_THAN   { U'>'  }; //reserved
        const char32_t EQUALS_SIGN    { U'='  };
        const char32_t SOLIDUS        { U'/'  };
        const char32_t GRAVE_ACCENT   { U'`'  };
        const char32_t SPACE          { U' '  };
        const char32_t SEMICOLON      { U';'  };
    }

    class Rules {
      public:
        ~Rules() = default;

        /**
         * Instance getter for the singleton
         * @return Instance of Rules
         */
        static Rules & getInstance() {
            static Rules instance;
            return instance;
        }

        bool isPaired( html5::Tag tag );
        std::u32string tagToStr( html5::Tag tag );
        html5::Tag strToTag( std::u32string str );

        //TODO tag-attribute matrix
        std::u32string attributeToStr( html5::Attribute attribute );
        html5::Attribute strToAttribute( std::u32string str );

        //TODO Tag matrix (i.e.: what Tag is cool inside what Tags)

        size_t tagCount() const;
        size_t attributeCount() const;

      private:
        struct TagSpecs {
            TagStructure   structure;
            std::u32string tag_str;
        };

        Rules();

        const std::unordered_map<Tag, TagSpecs>             _tag2str_map;
        const std::unordered_map<std::u32string, Tag>       _str2tag_map;
        const std::unordered_map<Attribute, std::u32string> _attr2str_map;
        const std::unordered_map<std::u32string, Attribute> _str2attr_map;
        const std::vector<std::vector<bool>>                _attr2tag_table;

        struct Factories {
            static std::unordered_map<Tag, TagSpecs>             createTag2StrMap();
            static std::unordered_map<std::u32string, Tag>       createStr2TagMap( const std::unordered_map<Tag, TagSpecs> &tag2str );
            static std::unordered_map<Attribute, std::u32string> createAttr2StrMap();
            static std::unordered_map<std::u32string, Attribute> createStr2AttrMap( const std::unordered_map<Attribute, std::u32string> &str2attr );
            static std::vector<std::vector<bool>>                createAttr2TagLookupTable();
        };
    };
}

#endif //BLOGATOR_DOM_HTML5_RULES_H