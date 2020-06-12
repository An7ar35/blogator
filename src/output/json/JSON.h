#ifndef BLOGATOR_OUTPUT_JSON_JSON_H
#define BLOGATOR_OUTPUT_JSON_JSON_H

#include <string>

#include "../../cli/MsgInterface.h"
#include "../../dto/Index.h"
#include "../../dto/Options.h"

namespace blogator::output::json {

    std::string escapeStr( const std::string &str );

    class JSON {
      public:
        JSON( std::shared_ptr<const dto::Index>   master_index,
              std::shared_ptr<const dto::Options> global_options );

        bool init();

      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Options>  _options;
        size_t                               _json_item_processed;
        size_t                               _json_item_count;
        cli::MsgInterface                   &_display;

        void writeArticles( std::ofstream &file );
        void writeTags( std::ofstream &file );
        void writeAuthors( std::ofstream &file );
        void writeYears( std::ofstream &file );
        void appendJSON( const std::filesystem::path &src_path, std::ofstream &file );
    };
}

#endif //BLOGATOR_OUTPUT_JSON_JSON_H