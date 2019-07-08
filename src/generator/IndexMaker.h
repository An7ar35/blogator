#ifndef BLOGATOR_GENERATOR_INDEXMAKER_H
#define BLOGATOR_GENERATOR_INDEXMAKER_H

#include <list>
#include <regex>

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/Options.h"
#include "../dto/Template.h"
#include "../cli/MsgInterface.h"

namespace blogator::generator {
    class IndexMaker {
      public:
        IndexMaker( std::shared_ptr<const dto::Index>    master_index,
                    std::shared_ptr<const dto::Template> templates,
                    std::shared_ptr<const dto::Options>  global_options );

        bool init();

      private:
        struct ParentPage {
            std::string           _name;
            std::filesystem::path _path;
        };

        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Template> _templates;
        std::shared_ptr<const dto::Options>  _options;
        size_t                               _total_jobs;
        cli::MsgInterface                   &_display;

        void generateDateIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                     const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writeTagListPage( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                               const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writeAuthorListPage( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                  const dto::Template::ConsecutiveWritePositions_t &item_insert_points );

        void generateTagIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                    const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void generateAuthorIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                       const dto::Template::ConsecutiveWritePositions_t &item_insert_points );

        void writeDateIndexPage( std::ofstream &page,
                                 const dto::Index::PagePaths_t::const_iterator &path_it,
                                 dto::Index::Articles_t::const_iterator &article_it,
                                 const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                 const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const;

        void writeCategoryIndexPage( std::ofstream &page,
                                     const ParentPage &parent,
                                     const dto::Index::ListIndexPaths_t::const_iterator &cat_it,
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
        void writeAuthorList( std::ofstream &page, const std::string &indent ) const;

        void writeIndexPageBreadcrumb(  std::ofstream &page,
                                        const std::string &indent,
                                        const std::string &parent_name,
                                        const std::filesystem::path &parent_path,
                                        const std::string &page_name ) const;

        void writeIndexListBreadcrumb( std::ofstream &page,
                                       const std::string &indent,
                                       const std::string & list_name ) const;
    };
}

#endif //BLOGATOR_GENERATOR_INDEXMAKER_H
