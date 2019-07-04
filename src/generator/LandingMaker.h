#ifndef BLOGATOR_GENERATOR_LANDINGMAKER_H
#define BLOGATOR_GENERATOR_LANDINGMAKER_H

#include <memory>

#include "../dto/Options.h"
#include "../dto/Index.h"
#include "../dto/Template.h"

namespace blogator::generator {
    class LandingMaker {
      public:
        LandingMaker( std::shared_ptr<const dto::Index>    master_index,
                      std::shared_ptr<const dto::Template> templates,
                      std::shared_ptr<const dto::Options>  global_options );

        bool init() const;

      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Template> _templates;
        std::shared_ptr<const dto::Options>  _options;

        void generateLandingPage( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                  const dto::Template::ConsecutiveWritePositions_t &entry_insert_points ) const;

        void writeBreadcrumb( std::ofstream &page, const std::string &indent ) const;

        void writeTopTags( std::ofstream &page, const std::string &indent ) const;

        void writeNewestPosts( std::ofstream &page,
                               const std::string &indent,
                               const dto::Template::ConsecutiveWritePositions_t &entry_insert_points ) const;

        void writeFeatured( std::ofstream &page,
                            const std::string &indent,
                            const dto::Template::ConsecutiveWritePositions_t &entry_insert_points ) const;

        void writeArticleEntry( std::ofstream &page,
                                const std::string &indent,
                                const dto::Article &article,
                                const dto::HTML &src_html,
                                const dto::Template::ConsecutiveWritePositions_t &insert_points ) const;
    };
}

#endif //BLOGATOR_GENERATOR_LANDINGMAKER_H