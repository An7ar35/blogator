#ifndef BLOGATOR_OUTPUT_FEED_RSS_H
#define BLOGATOR_OUTPUT_FEED_RSS_H

#include <memory>

#include "../../dto/Index.h"
#include "../../dto/Options.h"
#include "../../dto/Templates.h"
#include "../../cli/MsgInterface.h"

namespace blogator::output::feed {
    class RSS {
      public:
        RSS( std::shared_ptr<const dto::Index>     master_index,
             std::shared_ptr<const dto::Templates> templates,
             std::shared_ptr<const dto::Options>   global_options );

        bool init();

      private:
        std::shared_ptr<const dto::Index>     _master_index;
        std::shared_ptr<const dto::Templates> _templates;
        std::shared_ptr<const dto::Options>   _options;
        size_t                                _feed_item_count;
        cli::MsgInterface                    &_display;

        void writeHead( std::ofstream &file ) const;
        void writeItems( std::ofstream &file ) const;
        void writeFoot( std::ofstream &file ) const;

    };
}

#endif //BLOGATOR_OUTPUT_FEED_RSS_H
