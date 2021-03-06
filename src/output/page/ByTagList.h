#ifndef BLOGATOR_OUTPUT_PAGE_BYTAGLIST_H
#define BLOGATOR_OUTPUT_PAGE_BYTAGLIST_H

#include "../abstract/IndexList.h"

namespace blogator::output::page {
    class ByTagList : public abstract::IndexList {
      public:
        ByTagList( std::shared_ptr<const dto::Index>   master_index,
                   std::shared_ptr<const dto::Options> global_options,
                   const dto::Template &entry_template );

      private:
        std::shared_ptr<const dto::Index> _index;

        void writeFlatList( dto::Page &page, const std::string &indent ) const override;
        void writeHierarchyList( dto::Page &page, const std::string &indent ) const override;
    };
}

#endif //BLOGATOR_OUTPUT_PAGE_BYTAGLIST_H