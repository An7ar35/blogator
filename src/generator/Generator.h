#ifndef BLOGATOR_GENERATOR_H
#define BLOGATOR_GENERATOR_H

#include <unordered_map>

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/IndexPane.h"
#include "../dto/Options.h"

namespace blogator {
    class Generator {
      public:
        typedef std::map<std::string, std::vector<size_t>> TagIndexMap_t;

        explicit Generator( dto::Options global_options );

        void useMonthStrings( const std::unordered_map<unsigned, std::string> &month_map );
        bool init( const dto::Index &master_index );

      private:
        dto::Options                               _options;
        std::unique_ptr<dto::HTML>                 _header;
        std::unique_ptr<dto::HTML>                 _footer;
        std::unordered_map<unsigned, std::string>  _month_map;

        std::unique_ptr<TagIndexMap_t> createTagIndex( const dto::Index &master_index ) const;

        std::unique_ptr<dto::IndexPane> generateIndexPaneHTML( const dto::Index &master_index, const TagIndexMap_t &tag_index ) const;
        std::unique_ptr<dto::IndexPane> generateIndexPane_ByDate_HTML( const dto::Index &master_index,
                                                                       const TagIndexMap_t &tag_index,
                                                                       std::unique_ptr<dto::IndexPane> index_pane ) const;
        std::unique_ptr<dto::IndexPane> generateIndexPane_ByTags_HTML( const dto::Index &master_index,
                                                                       const TagIndexMap_t &tag_index,
                                                                       std::unique_ptr<dto::IndexPane> index_pane ) const;
    };
}

#endif //BLOGATOR_GENERATOR_H
