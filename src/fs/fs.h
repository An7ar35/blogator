#ifndef BLOGATOR_FS_H
#define BLOGATOR_FS_H

#include <filesystem>
#include <unordered_map>

#include "../dto/Options.h"
#include "../dto/HTML.h"

namespace blogator::fs {
    bool setupEnvironment( const dto::Options &global_options );

    std::unique_ptr<dto::HTML> importHTML( const std::filesystem::path & file_path );
    std::unordered_map<unsigned, std::string> importMonthNames( const std::filesystem::path & file_path );
}

#endif //BLOGATOR_FS_H
