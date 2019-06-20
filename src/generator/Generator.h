#ifndef BLOGATOR_GENERATOR_H
#define BLOGATOR_GENERATOR_H

#include <unordered_map>

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/Options.h"

namespace blogator {
    class Generator {
      public:
        typedef std::map<std::string, std::vector<size_t>> TagIndexMap_t;

        explicit Generator( dto::Options global_options );

        void useMonthStrings( const std::unordered_map<unsigned, std::string> &month_map );
        bool init( const dto::Index &index );

      private:
        dto::Options                               _options;
        std::unique_ptr<dto::HTML>                 _header;
        std::unique_ptr<dto::HTML>                 _footer;
        std::unordered_map<unsigned, std::string>  _month_map;

        std::unique_ptr<TagIndexMap_t> createTagIndex( const dto::Index &index ) const;
    };
}

#endif //BLOGATOR_GENERATOR_H
