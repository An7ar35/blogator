#ifndef BLOGATOR_OUTPUT_PAGE_POST_H
#define BLOGATOR_OUTPUT_PAGE_POST_H

#include "../abstract/Page.h"
#include "../../dto/Article.h"
#include "../../dto/Index.h"
#include "../../dto/HTML.h"
#include "../../dto/IndexDateTree.h"
#include "../../dto/IndexTagTree.h"
#include "../../dto/Options.h"
#include "../../dto/Templates.h"
#include "../../dto/Line.h"
#include "../../cli/MsgInterface.h"

namespace blogator::output::page {
    class Posts : abstract::Page {
      public:
        Posts( std::shared_ptr<const dto::Index>     master_index,
               std::shared_ptr<const dto::Templates> templates,
               std::shared_ptr<const dto::Options>   global_options );

        bool init() const;

      private:
        struct PageInfo {
            const dto::Article          &article;
            const size_t                article_i;
            const size_t                css_line;
            const std::filesystem::path css_path;
        };

        std::unique_ptr<const dto::IndexDateTree> _html_date_tree;
        std::unique_ptr<const dto::IndexTagTree>  _html_tag_tree;
        Breadcrumb::BreadCrumb_t                  _breadcrumb_parents;

        std::filesystem::path copyStylesheet( const dto::Article &article ) const;

        void writeTemplateLine( dto::Page       &page,
                                const dto::Line &line,
                                const PageInfo  &page_info,
                                dto::Template::WritePosIterators &insert_it ) const;

        void writeHtmlBlock( dto::Page &page,
                             const std::string &indent,
                             const PageInfo    &page_info,
                             const std::string &block_class ) const;

        void writeContentDiv( const std::filesystem::path &source_path,
                              const std::string &indent,
                              std::ofstream &out ) const;

        void writePageNavDiv( dto::Page &page,
                              const std::string &indent,
                              const size_t &article_pos ) const;

        void writeIndexDateTree( dto::Page &page,
                                 const std::string &indent,
                                 const dto::Article &article,
                                 const size_t &article_pos ) const;

        void writeIndexTagTree( dto::Page &page,
                                const std::string &indent,
                                const dto::Article &article,
                                const size_t &article_pos ) const;
    };
}

#endif //BLOGATOR_OUTPUT_PAGE_POST_H