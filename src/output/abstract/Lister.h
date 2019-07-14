#ifndef BLOGATOR_OUTPUT_ABSTRACT_LISTER_H
#define BLOGATOR_OUTPUT_ABSTRACT_LISTER_H

#include "Breadcrumb.h"
#include "../../dto/Index.h"
#include "../../dto/Template.h"
#include "../../dto/Options.h"

namespace blogator::output::abstract {
    class Lister : protected abstract::Breadcrumb {
      public:
        Lister( std::shared_ptr<const dto::Options> global_options,
                const dto::Index::Articles_t &articles,
                const dto::Template          &page_template );

//        virtual void write( const dto::Index::Category  &cat_index,
//                            const BreadCrumb_t          &breadcrumb,
//                            const std::filesystem::path &root_dir ) = 0;

      protected:
        std::shared_ptr<const dto::Options> _options;

        const dto::Index::Articles_t &_articles;
        const dto::Template          &_template;

        void writePageNavDiv( dto::Page &page,
                              const std::string &indent,
                              const dto::Index::PagePaths_t &paths,
                              const dto::Index::PagePaths_t::const_iterator &path_it ) const;

        virtual void writeIndexEntry( dto::Page &page,
                                      const std::string &indent,
                                      const dto::Article &article,
                                      const std::filesystem::path &post_href_path ) const = 0;
    };
}

#endif //BLOGATOR_OUTPUT_ABSTRACT_LISTER_H