#ifndef BLOGATOR_OUTPUT_GENERIC_CATEGORYLISTER_H
#define BLOGATOR_OUTPUT_GENERIC_CATEGORYLISTER_H

#include "EntryWriter.h"
#include "../abstract/Lister.h"
#include "../../dto/Index.h"
#include "../../dto/Template.h"
#include "../../dto/Options.h"
#include "../../dto/Page.h"

namespace blogator::output::generic {
    class CategoryLister : protected abstract::Lister {
      public:
        CategoryLister( std::shared_ptr<const dto::Options> global_options,
                        const dto::Index::Articles_t &articles,
                        const dto::Template          &page_template,
                        generic::EntryWriter         entry_maker );

        void write( const dto::Index::Category  &cat_index,
                    const BreadCrumb_t          &breadcrumb,
                    const std::filesystem::path &root_dir );

      private:
        const generic::EntryWriter _entry_maker;

        BreadCrumb_t          _breadcrumb_parents;
        size_t                _entry_counter { 0 };

        void writeTemplateLine( dto::Page       &page,
                                const dto::Line &line,
                                const dto::Index::ListIndexPaths_t::const_iterator &cat_it,
                                const dto::Index::PagePaths_t::const_iterator &page_path_it,
                                std::vector<size_t>::const_iterator &article_i_it,
                                dto::Template::WritePosIterators    &insert_it );

        bool writeHtmlBlock( dto::Page &page,
                             const std::string  &indent,
                             const std::string  &block_name,
                             const dto::Index::ListIndexPaths_t::const_iterator &cat_it,
                             const dto::Index::PagePaths_t::const_iterator &page_path_it,
                             std::vector<size_t>::const_iterator &article_i_it );

        void writeIndexEntry( dto::Page &page,
                              const std::string &indent,
                              const dto::Article &article,
                              const std::filesystem::path &post_href_path ) const override;
    };
}

#endif //BLOGATOR_OUTPUT_GENERIC_CATEGORYLISTER_H
