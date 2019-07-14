#ifndef BLOGATOR_OUTPUT_ABSTRACT_PAGE_H
#define BLOGATOR_OUTPUT_ABSTRACT_PAGE_H

#include "Breadcrumb.h"
#include "../../dto/Index.h"
#include "../../dto/Templates.h"
#include "../../dto/Options.h"

namespace blogator::output::abstract {
    class Page : public Breadcrumb {
      public:
        Page( std::shared_ptr<const dto::Index>     master_index,
              std::shared_ptr<const dto::Templates> templates,
              std::shared_ptr<const dto::Options>   global_options );

      protected:
        std::shared_ptr<const dto::Index>     _index;
        std::shared_ptr<const dto::Templates> _templates;
        std::shared_ptr<const dto::Options>   _options;
    };
}

#endif //BLOGATOR_OUTPUT_ABSTRACT_PAGE_H