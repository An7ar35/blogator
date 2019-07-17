#ifndef BLOGATOR_OUTPUT_ABSTRACT_INDEXLIST_H
#define BLOGATOR_OUTPUT_ABSTRACT_INDEXLIST_H

#include <functional>

#include "../../dto/Template.h"
#include "../../dto/Page.h"
#include "../../dto/Line.h"
#include "Page.h"

namespace blogator::output::abstract {
    class IndexList : Breadcrumb {
      public:
        typedef std::list<std::pair<std::string, std::filesystem::path>> BreadCrumb_t;

        IndexList( std::shared_ptr<const dto::Options> global_options,
                   const dto::Template &entry_template,
                   BreadCrumb_t        breadcrumb,
                   std::string         page_name );

        void write( dto::Page &page ) const;

      protected:
        cli::MsgInterface                  &_display;
        std::shared_ptr<const dto::Options> _options;

        const dto::Template &_template;
        BreadCrumb_t        _breadcrumb_parents;
        std::string         _breadcrumb_page_str;

        void writeTemplateLine( dto::Page       &page,
                                const dto::Line &line,
                                dto::Template::WritePosIterators &insert_it ) const;

        void writeHtmlBlock( dto::Page &page,
                             const std::string  &indent,
                             const std::string  &block_name ) const;

        virtual void writeFlatList( dto::Page &, const std::string & ) const = 0;
        virtual void writeHierarchyList( dto::Page &, const std::string & ) const = 0;
    };
}

#endif //BLOGATOR_OUTPUT_ABSTRACT_INDEXLIST_H