#ifndef BLOGATOR_FS_H
#define BLOGATOR_FS_H

#include <filesystem>
#include <unordered_map>

#include "../dto/Options.h"
#include "../dto/Templates.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"

namespace blogator::fs {
    std::shared_ptr<dto::Options> importOptions( const std::filesystem::path &file_path, const dto::BlogatorInfo &info );
    void setupEnvironment( const std::shared_ptr<dto::Options>& global_options );

    uintmax_t purge( const dto::Options &options );
    std::shared_ptr<dto::Templates> importTemplates( const dto::Index &master_index, const dto::Options &options );
    void importTemplateHTML( const dto::Index &master_index, dto::Templates &templates );
    void extractTemplateRelPaths( const dto::Index &master_index, dto::Templates &templates );
    void getConsecutiveWritePositions( const dto::Index &master_index, dto::Templates &templates );

    std::string readFileContent( const std::filesystem::path &path );
    std::unique_ptr<dto::HTML> importHTML( const std::filesystem::path &file_path );
    dto::HTML importHTML( const std::filesystem::path &file_path, const std::list<dto::Article::SeekRange> &positions );
    std::filesystem::path adaptRelPath( const std::filesystem::path &source_file, const std::filesystem::path &target_file, const std::filesystem::path &rel_path );
    void   checkTemplateRelPaths( const dto::Index &master_index, const dto::Templates &templates, const dto::Options &options );
    size_t checkTemplateRelPaths( const dto::Template &src_template );
}

#endif //BLOGATOR_FS_H