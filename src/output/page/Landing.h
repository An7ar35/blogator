#ifndef BLOGATOR_OUTPUT_PAGE_LANDING_H
#define BLOGATOR_OUTPUT_PAGE_LANDING_H

#include <memory>

#include "../abstract/Page.h"
#include "../generic/EntryWriter.h"
#include "../../dto/Options.h"
#include "../../dto/Index.h"
#include "../../dto/Templates.h"
#include "../../dto/Page.h"
#include "../../dto/Line.h"

namespace blogator::output::page {
    class Landing : public abstract::Page {
      public:
        Landing( std::shared_ptr<const dto::Index>     master_index,
                 std::shared_ptr<const dto::Templates> templates,
                 std::shared_ptr<const dto::Options>   global_options );

        [[nodiscard]] bool init() const;

      private:
        generic::EntryWriter _entry_maker;
        BreadCrumb_t   _breadcrumb_parents;

        void generateLandingPage() const;

        void writeTemplateLine( dto::Page       &page,
                                const dto::Line &line,
                                dto::Template::WritePosIterators &insert_it ) const;

        void writeHtmlBlock( dto::Page &page,
                             const std::string &indent,
                             const std::string &block_class ) const;

        void writeTopTags( std::ofstream &page, const std::string &indent ) const;
        void writeTopAuthors( std::ofstream &page, const std::string &indent ) const;
        void writeNewestPosts( dto::Page &page, const std::string &indent ) const;
        void writeFeatured( dto::Page &page, const std::string &indent ) const;
        void writeEntry( dto::Page &page, const std::string &indent, const dto::Article &article ) const;
    };
}

#endif //BLOGATOR_OUTPUT_PAGE_LANDING_H