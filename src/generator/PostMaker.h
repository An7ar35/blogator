#ifndef BLOGATOR_GENERATOR_POSTMAKER_H
#define BLOGATOR_GENERATOR_POSTMAKER_H

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/IndexDateTree.h"
#include "../dto/IndexTagTree.h"
#include "../dto/Options.h"
#include "../dto/Template.h"

namespace blogator::generator {
    class PostMaker {
      public:
        explicit PostMaker( dto::Options global_options );

        bool init( const dto::Index &master_index,
                   const dto::Template &templates,
                   const dto::Index::TagIndexMap_t &tag_index );

      private:
        dto::Options _options;

        std::unique_ptr<dto::IndexDateTree> generateIndexDateTreeHTML( const dto::Index    &master_index,
                                                                       const dto::Template &master_template ) const;

        std::unique_ptr<dto::IndexTagTree> generateIndexTagTreeHTML( const dto::Index    &master_index,
                                                                     const dto::Index::TagIndexMap_t &tag_index ) const;

        void writeContentDiv( const std::filesystem::path &source_path,
                              const std::string &fore_space,
                              std::ofstream &out ) const;

        void writePageNavDiv( std::ofstream &file,
                              const std::string &fore_space,
                              const dto::Index &index,
                              const size_t &article_pos ) const;

        void writeIndexDateTree( std::ofstream &file,
                                 const std::string &fore_space,
                                 const dto::Article &article,
                                 const size_t &article_pos,
                                 const dto::IndexDateTree &tree ) const;

        void writeIndexTagTree( std::ofstream &file,
                                const std::string &fore_space,
                                const dto::Article &article,
                                const size_t &article_pos,
                                const dto::IndexTagTree &tree ) const;
    };
}

#endif //BLOGATOR_GENERATOR_POSTMAKER_H
