#ifndef BLOGATOR_DTO_TEMPLATES_H
#define BLOGATOR_DTO_TEMPLATES_H

#include <map>
#include <memory>
#include <functional>
#include <regex>
#include <filesystem>

#include "Template.h"

namespace blogator::dto {
    struct Templates {
        Templates();

        std::unique_ptr<Template> _post;          //Post/Article page template
        std::shared_ptr<Template> _index;         //Generic index page template
        std::shared_ptr<Template> _index_list;    //Generic category index list page templates
        std::shared_ptr<Template> _year_list;     //Year list (dates.html) page template
        std::shared_ptr<Template> _year_index;    //Year index page template
        std::shared_ptr<Template> _tag_list;      //Tag list (tags.html) page template
        std::shared_ptr<Template> _tag_index;     //Tag index page template
        std::shared_ptr<Template> _author_list;   //Author list (authors.html) page template
        std::shared_ptr<Template> _author_index;  //Author index page template
        std::shared_ptr<Template> _index_entry;   //Index entry template
        std::unique_ptr<Template> _landing;       //Blog landing page (start) template
        std::shared_ptr<Template> _landing_entry; //Landing page newest/featured posts entry template

        static ConsecutivePathPositions_t extractRelativePaths( HTML &source );
    };
}

#endif //BLOGATOR_DTO_TEMPLATES_H
