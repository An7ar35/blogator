#ifndef BLOGATOR_OUTPUT_GENERIC_ENTRYWRITER_H
#define BLOGATOR_OUTPUT_GENERIC_ENTRYWRITER_H

#include "../../dto/Templates.h"
#include "../../dto/Options.h"
#include "../../dto/Article.h"
#include "../../dto/Page.h"
#include "../../dto/Line.h"
#include "../../cli/MsgInterface.h"

namespace blogator::output::generic {
    class EntryWriter {
      public:
        EntryWriter( std::shared_ptr<const dto::Template> entry_template,
                     const dto::Options::MonthStringLookup_t &months );

        void write( dto::Page &page, const std::string &indent, const dto::Article &article ) const;

        dto::Template::Type getTemplateType() const;

      private:
        cli::MsgInterface                       &_display;
        std::shared_ptr<const dto::Template>    _template;
        const dto::Options::MonthStringLookup_t &_months;

        void writeTemplateLine( dto::Page            &page,
                                const std::string    &indent,
                                const dto::Line      &line,
                                const dto::Article   &article,
                                dto::Template::WritePosIterators &insert_its ) const;

        void writeHtmlBlock( dto::Page &page,
                             const std::string &indent,
                             const std::string &block_name,
                             const dto::Article &article ) const;
    };
}

#endif //BLOGATOR_OUTPUT_GENERIC_ENTRYWRITER_H