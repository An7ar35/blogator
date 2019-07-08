#ifndef BLOGATOR_FS_H
#define BLOGATOR_FS_H

#include <filesystem>
#include <unordered_map>

#include "../dto/Options.h"
#include "../dto/Template.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"

namespace blogator::fs {
    std::shared_ptr<dto::Options> importOptions( const std::filesystem::path &file_path );
    void setupEnvironment( const std::shared_ptr<dto::Options>& global_options );
    std::shared_ptr<dto::Template> importTemplates( const dto::Index &master_index );
    std::unique_ptr<dto::HTML> importHTML( const std::filesystem::path &file_path );
}

#endif //BLOGATOR_FS_H
