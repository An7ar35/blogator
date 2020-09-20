#ifndef BLOGATOR_DOM_HTML5_KB_KNOWLEDGEBASE_H
#define BLOGATOR_DOM_HTML5_KB_KNOWLEDGEBASE_H

#include <unordered_map>
#include <vector>

#include "../enums/Attribute.h"
#include "../enums/ContentModel.h"
#include "../enums/NameSpace.h"
#include "../enums/Tag.h"
#include "../enums/TagStructure.h"
#include "../../dto/TagSpecs.h"

namespace blogator::dom::html5::kb {
    struct KnowledgeBase {
        static std::u32string tagToStr( html5::Tag tag );
        static html5::Tag strToTag( std::u32string str );

        static std::u32string attributeToStr( html5::Attribute attribute );
        static html5::Attribute strToAttribute( std::u32string str );
        static bool isValidAttribute( std::u32string str );

        static bool isPaired( html5::Tag tag );

        static std::vector<NameSpace> getNamespaces();
        static std::vector<NameSpace> getNamespace( html5::Tag tag );
        static bool belongsToNamespace( html5::Tag tag, NameSpace ns );
        static bool isNamespaceAgnostic( html5::Tag tag );

        static size_t tagCount();
        static size_t attributeCount();

        static bool areAffiliated( Attribute attr, Tag tag );

      private:
        //conversion
        static const std::unordered_map <Tag, dto::TagSpecs>        _tag2str_map;
        static const std::unordered_map <std::u32string, Tag>       _str2tag_map;
        static const std::vector <std::u32string>                   _attr2str_table;
        static const std::unordered_map <std::u32string, Attribute> _str2attr_map;

        //relationships/context
        static const std::vector<std::vector<bool>>                 _attr2tag_table;
    };
}

#endif //BLOGATOR_DOM_HTML5_KB_KNOWLEDGEBASE_H