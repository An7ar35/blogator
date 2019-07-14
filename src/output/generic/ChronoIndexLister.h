#ifndef BLOGATOR_OUTPUT_GENERIC_ENTRYLISTER_H
#define BLOGATOR_OUTPUT_GENERIC_ENTRYLISTER_H

#include "EntryWriter.h"
#include "../abstract/Lister.h"
#include "../../dto/Options.h"
#include "../../dto/Template.h"
#include "../../dto/Index.h"

namespace blogator::output::generic {
  class ChronoIndexLister : protected abstract::Lister {
    public:
      ChronoIndexLister( std::shared_ptr<const dto::Options> global_options,
                         const dto::Index::Articles_t        &articles,
                         const dto::Template                 &page_template,
                         generic::EntryWriter                entry_maker );

      void write( const dto::Index::Indices::ChronoIndex &chrono_index,
                  const abstract::Lister::BreadCrumb_t   &breadcrumb,
                  const std::filesystem::path            &dir_path );

    private:
      const generic::EntryWriter _entry_maker;

      BreadCrumb_t          _breadcrumb_parents;
      size_t                _page_num            { 0 };
      size_t                _entry_counter       { 0 };

      void writeTemplateLine( dto::Page       &page,
                              const dto::Line &line,
                              const dto::Index::PagePaths_t                  &page_paths,
                              const dto::Index::PagePaths_t::const_iterator  &page_path_it,
                              dto::Index::Articles_t::const_iterator         &article_it,
                              dto::Template::WritePosIterators               &insert_it );

      void writeHtmlBlock( dto::Page &page,
                           const std::string  &indent,
                           const std::string  &block_name,
                           const dto::Index::PagePaths_t                  &page_paths,
                           const dto::Index::PagePaths_t::const_iterator  &page_path_it,
                           dto::Index::Articles_t::const_iterator         &article_it );

      void writeIndexEntry( dto::Page &page,
                            const std::string &indent,
                            const dto::Article &article,
                            const std::filesystem::path &post_href_path ) const override;
    };
}

#endif //BLOGATOR_OUTPUT_GENERIC_ENTRYLISTER_H