#ifndef BLOGATOR_GENERATOR_RSS_H
#define BLOGATOR_GENERATOR_RSS_H

#include <memory>

#include "../dto/Index.h"
#include "../dto/Options.h"
#include "../dto/Template.h"
#include "../cli/MsgInterface.h"

namespace blogator::generator {
    class RSS {
      public:
        RSS( std::shared_ptr<const dto::Index> master_index,
             std::shared_ptr<const dto::Template> templates,
             std::shared_ptr<const dto::Options> global_options );

        bool init();

      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Template> _templates;
        std::shared_ptr<const dto::Options>  _options;
        size_t                               _feed_item_count;
        cli::MsgInterface                   &_display;

        void writeHead( std::ofstream &file ) const;
        void writeItems( std::ofstream &file ) const;
        void writeFoot( std::ofstream &file ) const;

    };
}

#endif //BLOGATOR_GENERATOR_RSS_H
