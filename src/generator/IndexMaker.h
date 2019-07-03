#ifndef BLOGATOR_GENERATOR_INDEXMAKER_H
#define BLOGATOR_GENERATOR_INDEXMAKER_H

#include <list>
#include <regex>

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/Options.h"
#include "../dto/Template.h"

namespace blogator::generator {
    class IndexMaker {
      public:
        IndexMaker( std::shared_ptr<const dto::Index>    master_index,
                    std::shared_ptr<const dto::Template> templates,
                    std::shared_ptr<const dto::Options>  global_options );

        bool init();

      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Template> _templates;
        std::shared_ptr<const dto::Options>  _options;

        bool generateDateIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                     const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        bool writeTagListPage( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                               const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        bool generateTagIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                    const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writeDateIndexPage( std::ofstream &page,
                                 const dto::Index::PagePaths_t::const_iterator &path_it,
                                 dto::Index::Articles_t::const_iterator &article_it,
                                 const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                 const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writeTagIndexPage( std::ofstream &page,
                                const dto::Index::TagIndexPaths_t::const_iterator &tag_it,
                                const dto::Index::PagePaths_t::const_iterator &page_path_it,
                                std::vector<size_t>::const_iterator &article_i_it,
                                const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writeIndexEntry( std::ofstream &page,
                              const std::string &indent,
                              const dto::Article &article,
                              const std::filesystem::path &post_href_path,
                              const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writeTemplateIndexEntry( std::ofstream &page,
                                      const std::string &indent,
                                      const dto::Article &article,
                                      const dto::HTML &src_html,
                                      const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writePageNavDiv( std::ofstream &page,
                              const std::string &indent,
                              const dto::Index::PagePaths_t &paths,
                              const dto::Index::PagePaths_t::const_iterator &path_it ) const;

        void writeTagList( std::ofstream &page, const std::string &indent ) const;
        void writeTagListHierarchy( std::ofstream &page, const std::string &indent ) const;

        void writeDateIndexBreadcrumb( std::ofstream &page,
                                       const std::string &indent,
                                       const size_t &page_number ) const;

        void writeTagIndexBreadcrumb( std::ofstream &page,
                                      const std::string &indent,
                                      const std::string &tag ) const;

        void writeTagListBreadcrumb( std::ofstream &page,
                                     const std::string &indent ) const;
    };
}

#endif //BLOGATOR_GENERATOR_INDEXMAKER_H
