#ifndef BLOGATOR_DOM_HTML5_HTML5RELATIONSHIPS_H
#define BLOGATOR_DOM_HTML5_HTML5RELATIONSHIPS_H

#include <vector>

#include "enums/Attribute.h"
#include "enums/Tag.h"
#include "enums/InsertMode.h"
#include "../dot/DOTNode.h"

namespace blogator::dom::html5 {
    struct Html5Relationships {
        static bool areAffiliated( Attribute attr, Tag tag );
        static bool areAffiliated( Tag tag, ContentModel model );
//        static bool isContextValid( Tag child_tag, Tag parent_tag );//TODO data matrix (i.e.: what Tag is cool inside what Tags)?
        static bool validateNode( const DOTNode &node );
        static bool validateNode( const DOTNode &node, InsertMode &insert_mode );
        // ^^^ master method for checking context of a node -> will need other methods to deal with each tag and rules
        // outlined in the HTML5.3 docs

      private:
        //relationships/context
        static const std::vector<std::vector<bool>> _tag_context_table;
        static const std::vector<std::vector<bool>> _attr2tag_table;
        static const std::vector<std::vector<bool>> _tag2cm_membership_table;

        static bool hasDescendant( const DOTNode &root, Tag tag );
        static bool hasDescendant_( const DOTNode &root, Tag tag );
        static bool hasDescendant( const DOTNode &root, ContentModel model );
        static bool hasDescendant_( const DOTNode &root, ContentModel model );
        static bool hasDescendant( const DOTNode &root, const std::vector<ContentModel>& models );
        static bool hasDescendant_( const DOTNode &root, const std::vector<ContentModel>& models );
    };
}

#endif //BLOGATOR_DOM_HTML5_HTML5RELATIONSHIPS_H