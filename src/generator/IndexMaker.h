#ifndef BLOGATOR_GENERATOR_INDEXMAKER_H
#define BLOGATOR_GENERATOR_INDEXMAKER_H

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/Options.h"
#include "../dto/Template.h"

namespace blogator::generator {
    class IndexMaker {
      public:
        explicit IndexMaker( dto::Options global_options );

        bool init( const dto::Index &master_index,
                   const dto::Template &templates,
                   const dto::Index::TagIndexMap_t &tag_index );

      private:
        typedef std::vector<std::filesystem::path> PagePaths_t;

        dto::Options                 _options;
        std::unique_ptr<PagePaths_t> _by_date_page_paths;

        bool generateDateIndexPages( const dto::Index &master_index,
                                     const dto::Template &templates,
                                     const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const;

//        bool generateTagIndexPages();

        void writeDateIndexPage( std::ofstream &page,
                                 const PagePaths_t::const_iterator &page_path,
                                 const dto::Template &templates,
                                 const dto::Index::Articles_t &articles,
                                 dto::Index::Articles_t::const_iterator &article_it,
                                 const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const;

//        void writeTagIndexPage();

        void writeIndexEntry( std::ofstream &page,
                              const std::string &fore_space,
                              const dto::Template::MonthStringLookup_t &months,
                              const dto::Article &article,
                              const std::filesystem::path &post_href_path ) const;

        void writePageNavDiv( std::ofstream &file,
                              const std::string &fore_space,
                              const PagePaths_t &page_paths,
                              const PagePaths_t::const_iterator &page_path_it ) const;
    };
}

#endif //BLOGATOR_GENERATOR_INDEXMAKER_H
