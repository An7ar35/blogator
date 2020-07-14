#ifndef BLOGATOR_CHECKER_CHECKER_H
#define BLOGATOR_CHECKER_CHECKER_H

#include "../cli/MsgInterface.h"
#include "../dto/Index.h"
#include "../dto/Options.h"

namespace blogator::checker {
    class Checker {
      public:
        Checker( std::shared_ptr<const dto::Index>   master_index,
                 std::shared_ptr<const dto::Options> global_options );

        //TODO src check -> html validator functionality?
        //TODO src check -> rel links resolution in post sources?
        //TODO external html checks (paths given in options) - probably run post everything else as an atomic module

        [[nodiscard]] bool init() const;

      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Options>  _options;
        cli::MsgInterface                   &_display;

        [[nodiscard]] bool checkRelHref( const std::filesystem::path &file_path ) const;
    };
}

#endif //BLOGATOR_CHECKER_CHECKER_H
