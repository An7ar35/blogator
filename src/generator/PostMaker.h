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
        PostMaker( std::shared_ptr<const dto::Index>    master_index,
                   std::shared_ptr<const dto::Template> templates,
                   std::shared_ptr<const dto::Options>  global_options );

        bool init();

      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Template> _templates;
        std::shared_ptr<const dto::Options>  _options;

        std::unique_ptr<dto::IndexDateTree> generateIndexDateTreeHTML() const;

        std::unique_ptr<dto::IndexTagTree> generateIndexTagTreeHTML() const;

        void writeContentDiv( const std::filesystem::path &source_path,
                              const std::string &indent,
                              std::ofstream &out ) const;

        void writePageNavDiv( std::ofstream &file,
                              const std::string &indent,
                              const size_t &article_pos ) const;

        void writeIndexDateTree( std::ofstream &file,
                                 const std::string &indent,
                                 const dto::Article &article,
                                 const size_t &article_pos,
                                 const dto::IndexDateTree &tree ) const;

        void writeIndexTagTree( std::ofstream &file,
                                const std::string &indent,
                                const dto::Article &article,
                                const size_t &article_pos,
                                const dto::IndexTagTree &tree ) const;

        void writeBreadcrumb( std::ofstream &file,
                              const std::string &indent,
                              const dto::Article &article ) const;
    };
}

#endif //BLOGATOR_GENERATOR_POSTMAKER_H
