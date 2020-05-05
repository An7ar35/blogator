#ifndef BLOGATOR_OUTPUT_PAGE_INDICES_H
#define BLOGATOR_OUTPUT_PAGE_INDICES_H

#include <list>
#include <regex>

#include "../abstract/Page.h"
#include "../generic/EntryWriter.h"
#include "../generic/CategoryLister.h"
#include "../../dto/Article.h"
#include "../../dto/Index.h"
#include "../../dto/HTML.h"
#include "../../dto/Options.h"
#include "../../dto/Templates.h"
#include "../../cli/MsgInterface.h"

namespace blogator::output::page {
    class Indices : abstract::Page {
      public:
        Indices( std::shared_ptr<const dto::Index>     master_index,
                 std::shared_ptr<const dto::Templates> templates,
                 std::shared_ptr<const dto::Options>   global_options );

        [[nodiscard]] bool init() const;

      private:
        generic::EntryWriter _entry_maker;
        BreadCrumb_t         _breadcrumb_parents;
        size_t               _total_jobs;
    };
}

#endif //BLOGATOR_OUTPUT_PAGE_INDICES_H
